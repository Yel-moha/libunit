# Documentazione Esaustiva del Framework LibUnit

## Indice
1. [Introduzione](#introduzione)
2. [Architettura Generale](#architettura-generale)
3. [Strutture Dati](#strutture-dati)
4. [Descrizione Dettagliata delle Funzioni](#descrizione-dettagliata-delle-funzioni)
5. [Gestione dei Segnali](#gestione-dei-segnali)
6. [Gestione dei Fork e Processi](#gestione-dei-fork-e-processi)
7. [Flusso di Esecuzione](#flusso-di-esecuzione)
8. [Output e Formattazione](#output-e-formattazione)

---

## Introduzione

LibUnit è un **framework di unit testing leggero per C** progettato per testare funzioni C in un ambiente controllato e isolato. L'elemento chiave che lo distingue è la capacità di eseguire ogni test in un **processo separato (fork)**, permettendo di isolare eventuali crash, segmentation fault o altri segnali fatali senza influenzare l'esecuzione del processo principale.

### Caratteristiche Principali:
- **Isolamento dei test**: ogni test viene eseguito in un processo figlio indipendente
- **Rilevamento dei segnali**: cattura automaticamente SIGSEGV, SIGBUS, SIGABRT, SIGFPE, SIGPIPE, SIGILL
- **Gestione degli errori**: distingue tra:
  - Test che hanno completato l'esecuzione con codice di uscita
  - Test che si sono interrotti a causa di un segnale
- **Output formattato**: risultati colorati e riepilogo statistico
- **Gestione della memoria**: pulizia automatica delle risorse allocate

---

## Architettura Generale

Il framework è organizzato in diversi componenti funzionali:

```
┌─────────────────────────────────────────┐
│   API Pubblica (libunit.h)              │
│  - load_test()                          │
│  - launch_tests()                       │
│  - run_one_test()                       │
└──────────────┬──────────────────────────┘
               │
       ┌───────┴───────────────────────────┐
       │                                   │
       v                                   v
┌──────────────────────┐      ┌───────────────────────┐
│  Gestione Lista      │      │  Esecuzione Test      │
│  (load_test, free)   │      │  (run_one_test)       │
└──────────────────────┘      └──────────┬────────────┘
                                         │
                    ┌────────────────────┼────────────────────┐
                    │                    │                    │
                    v                    v                    v
            ┌──────────────┐    ┌─────────────────┐   ┌──────────────┐
            │ Fork Processo│    │ wait() & Status │   │ handle_result│
            │              │    │  Analysis       │   │              │
            └──────────────┘    └─────────────────┘   └──────────────┘
                    │                    │                    │
                    └────────────────────┼────────────────────┘
                                         │
                    ┌────────────────────┴────────────────────┐
                    │                                         │
                    v                                         v
            ┌──────────────────┐                   ┌─────────────────┐
            │  Output & Print  │                   │  Utility Helper │
            │ (print_result,   │                   │ (signal_name,   │
            │  print_summary)  │                   │  num_len, etc)  │
            └──────────────────┘                   └─────────────────┘
```

---

## Strutture Dati

### 1. **Struttura `t_unit_test`** (definita in [libunit.h](framework/includes/libunit.h))

```c
typedef struct s_unit_test
{
    char                *name;      // Nome descrittivo del test
    int                 (*f)(void); // Puntatore a funzione test (ritorna int)
    struct s_unit_test  *next;      // Puntatore al test successivo (lista concatenata)
}   t_unit_test;
```

**Spiegazione:**
- **`name`**: stringa allocata dinamicamente che identifica univocamente il test
- **`f`**: funzione test che non accetta parametri e ritorna un intero (codice di uscita)
- **`next`**: implementa una lista concatenata semplice per mantenere l'ordine dei test

**Rappresentazione in memoria (per una lista di 3 test):**
```
┌────────────────────────────┐
│  name: "01_basics_test"    │
│  f: pointer to test_func1  │
│  next: ────────────┐       │
└────────────────────────────┘
                     │
                     v
┌────────────────────────────┐
│  name: "02_null_test"      │
│  f: pointer to test_func2  │
│  next: ────────────┐       │
└────────────────────────────┘
                     │
                     v
┌────────────────────────────┐
│  name: "03_segv_test"      │
│  f: pointer to test_func3  │
│  next: NULL                │
└────────────────────────────┘
```

### 2. **Struttura Interna `s_sig_map`** (definita in [signal_name.c](framework/src/signal_name.c))

```c
static const struct s_sig_map
{
    int         signum;     // Numero del segnale (es: SIGSEGV = 11)
    const char  *name;      // Nome simbolico del segnale
}   g_sigtab[];
```

Tabella di mappatura statica che associa i numeri di segnale Unix ai loro nomi simbolici.

---

## Descrizione Dettagliata delle Funzioni

### 1. **`load_test()` - Caricamento dei Test nella Lista**

**File:** [framework/src/load_test.c](framework/src/load_test.c)

```c
void load_test(t_unit_test **list, const char *name, int (*f)(void))
```

**Parametri:**
- `**list`: puntatore al puntatore della lista (permette di modificare il puntatore iniziale)
- `*name`: nome del test (stringa, es: "01_basics_test")
- `(*f)(void)`: puntatore a funzione test

**Funzionamento:**

```
INIZIO
   │
   ├─> Allocare memoria per nuovo nodo (sizeof(t_unit_test))
   │   └─> Se fallisce malloc: return (perdita di memoria!)
   │
   ├─> Duplicare stringa del nome con ft_strdup()
   │   └─> Se fallisce: free(new_node), return
   │
   ├─> Inizializzare il nuovo nodo:
   │   │  new_node->name = duplicated_string
   │   │  new_node->f = f
   │   └─> new_node->next = NULL
   │
   └─> Inserire il nodo nella lista:
       ├─> Se lista vuota (*list == NULL):
       │   └─> *list = new_node
       │
       └─> Se lista non vuota:
           ├─> Trovare l'ultimo nodo iterando la lista
           ├─> temp = *list
           ├─> while (temp->next) temp = temp->next
           └─> temp->next = new_node
```

**Complessità Temporale:** O(n) dove n è il numero di test già presenti

**Implicazioni sulla Memoria:**
- Alloca: sizeof(t_unit_test) + strlen(name) + 1
- Errore silenzioso: se malloc fallisce, il test non viene aggiunto ma il programma non segnala l'errore

**Flusso di Esecuzione nel Context del Framework:**
```
main()
  │
  └─> launcher_function()
        │
        ├─> load_test(&tests, "01_basics", test_func1)
        ├─> load_test(&tests, "02_null", test_func2)
        ├─> load_test(&tests, "03_segv", test_func3)
        └─> launch_tests(&tests, "strlen")
```

---

### 2. **`launch_tests()` - Orchestrazione dell'Esecuzione**

**File:** [framework/src/launch_tests.c](framework/src/launch_tests.c)

```c
int launch_tests(t_unit_test **list, const char *func_name)
```

**Parametri:**
- `**list`: puntatore al puntatore della lista di test
- `*func_name`: nome della funzione testata (es: "strlen")

**Ritorno:**
- `0`: tutti i test sono passati (ok == tot)
- `-1`: almeno un test è fallito o errore fatale di fork/wait

**Funzionamento:**

```
INIZIO
   │
   ├─> Inizializzare contatori:
   │   │  tot = 0    (numero totale test)
   │   └─> ok = 0    (numero test passati)
   │
   ├─> Puntatore al primo test:
   │   └─> curr = *list
   │
   ├─> Iterare su TUTTI i test:
   │   │
   │   ├─> while (curr != NULL):
   │   │   │
   │   │   ├─> Eseguire il test:
   │   │   │   └─> run_one_test(curr, func_name, &tot, &ok)
   │   │   │
   │   │   ├─> Se run_one_test ritorna -1:
   │   │   │   ├─> Errore critico (fork fallito)
   │   │   │   ├─> free_list(&list)
   │   │   │   └─> return (-1) [STOP ESECUZIONE]
   │   │   │
   │   │   └─> Avanzare al test successivo:
   │   │       └─> curr = curr->next
   │   │
   │   └─> Fine loop: tutti i test elaborati
   │
   ├─> Stampare riepilogo:
   │   └─> print_summary(ok, tot)
   │
   ├─> Liberare memoria:
   │   └─> free_list(&list)
   │
   └─> Ritornare esito:
       ├─> Se ok == tot: return (0) [SUCCESSO]
       └─> Se ok < tot: return (-1) [FALLIMENTO]
```

**Complessità Temporale:** O(n) + O(n*T) dove n = numero test, T = tempo esecuzione medio test

**Gestione Errori Critica:**
```c
if (run_one_test(curr, func_name, &tot, &ok) == -1)
{
    free_list(list);  // IMPORTANTE: evita memory leak
    return (-1);      // Esecuzione interrotta
}
```

**Flusso Completo dal Main:**
```
main()
  │
  └─> strlen_launcher()
        │
        ├─> Caricamento test via load_test()
        │   ├─> lista: test1 → test2 → test3 → NULL
        │
        └─> launch_tests(&lista, "strlen")
              │
              ├─> Iterazione 1: run_one_test(test1, ...)
              │   ├─> fork()
              │   ├─> Esecuzione in child
              │   └─> wait() nel parent
              │
              ├─> Iterazione 2: run_one_test(test2, ...)
              │
              ├─> Iterazione 3: run_one_test(test3, ...)
              │
              ├─> print_summary(ok, tot)
              └─> return (ok == tot ? 0 : -1)
```

---

### 3. **`run_one_test()` - Core del Framework (Gestione Fork)**

**File:** [framework/src/run_one_test.c](framework/src/run_one_test.c)

```c
int run_one_test(t_unit_test *test, const char *func_name,
                 int *tot, int *ok)
```

**Questo è il CUORE del framework. Eseguire ogni test isolato.**

**Parametri:**
- `*test`: puntatore al test da eseguire
- `*func_name`: nome della funzione testata
- `*tot`: puntatore al contatore totale test
- `*ok`: puntatore al contatore test passati

**Funzionamento Dettagliato:**

```
ESECUZIONE: run_one_test(test, "strlen", &tot, &ok)

   STEP 1: Fork
   ┌──────────────────────────────────────┐
   │ pid = fork()                         │
   │ if (pid == -1):                      │
   │   └─> Errore fork                    │
   │       ├─> write(2, "fork error\n")   │
   │       └─> return (-1)                │
   └──────────────────────────────────────┘

   STEP 2: Processo Figlio
   ┌──────────────────────────────────────┐
   │ if (pid == 0):  // Esecuzione FIGLIO │
   │   │                                  │
   │   ├─> Eseguire il test:              │
   │   │   test->f()  (es: test_strlen()) │
   │   │                                  │
   │   └─> exit(ritorno_test)             │
   │       └─> Exit code: 0=OK, 1=KO      │
   │       └─> Questo è CRITICO!          │
   │                                      │
   └──────────────────────────────────────┘
   
   STEP 3: Processo Padre
   ┌──────────────────────────────────────┐
   │ if (pid > 0):  // Esecuzione PADRE   │
   │   │                                  │
   │   ├─> Attendere fine figlio:         │
   │   │   if (wait(&status) == -1):      │
   │   │     ├─> write(2, "wait error")   │
   │   │     └─> return (-1)              │
   │   │                                  │
   │   ├─> status contiene info exit del  │
   │   │   figlio (macros WIFEXITED, etc) │
   │   │                                  │
   │   ├─> Incrementare contatore:        │
   │   │   (*tot)++                       │
   │   │                                  │
   │   └─> Analizzare e registrare:       │
   │       handle_result(status, ...)     │
   │                                      │
   └──────────────────────────────────────┘

   STEP 4: Return
   └─> return (0)  // Successo
```

**Diagramma Temporale Fork/Wait:**

```
Tempo
  │
  ├─ PARENT: pid = fork() ──────────┐
  │                                  │
  ├─ PARENT: wait(&status) ◄────────┼───┬─ CHILD: pid == 0
  │  (bloccato in attesa)            │   │
  │                                  │   ├─ CHILD: test->f()
  │                                  │   │  (esecuzione test)
  │                                  │   │
  │                                  │   ├─ CHILD: exit(result)
  │  ◄─────────────────────────────┬─┘   │
  ├─ PARENT: ritorno wait()         │
  ├─ PARENT: (*tot)++               │
  └─ PARENT: handle_result()        │
```

**Implicazioni Critiche:**

1. **Isolamento**: ogni test è un processo separato
   - Crash in un test NON influenza altri test
   - Memory leak nel test NON persiste nel padre

2. **Wait e Status**:
   - `wait(&status)` blocca il padre fino a fine figlio
   - `status` contiene info di uscita (non è il codice di uscita diretto!)
   - Deve essere interpretato con macro POSIX

---

### 4. **`handle_result()` - Analisi dello Status del Test**

**File:** [framework/src/handle_result.c](framework/src/handle_result.c)

```c
void handle_result(int status, const char *func_name,
                   const char *test_name, int *ok)
```

**Funzionamento:**

Questa funzione analizza il valore `status` ritornato da `wait()` usando macro POSIX:

```
INIZIO
   │
   ├─> if (WIFEXITED(status))  // Il processo è uscito normalmente
   │   │  ✓ Il figlio ha chiamato exit() o return
   │   │
   │   ├─> code = WEXITSTATUS(status)  // Estrae il codice di uscita (0-255)
   │   │
   │   ├─> if (code == 0):
   │   │   │  ✓ Test PASSATO
   │   │   ├─> print_result(..., "OK", ANSI_GREEN)
   │   │   └─> (*ok)++  // Incrementa contatore successi
   │   │
   │   └─> else:
   │       │  ✗ Test FALLITO
   │       └─> print_result(..., "KO", ANSI_RED)
   │
   └─> else if (WIFSIGNALED(status))  // Processo terminato da segnale
       │  ✗ Il figlio è stato terminato da un segnale
       │
       ├─> sig = WTERMSIG(status)  // Estrae numero segnale
       │
       ├─> sig_name = signal_name(sig)  // Mappa numero → nome
       │   │  Esempi:
       │   │  - sig 11 → "SIGSEGV"
       │   │  - sig 5  → "SIGBUS"
       │   │  - sig 6  → "SIGABRT"
       │   │
       └─> print_result(..., sig_name, ANSI_YELLOW)
           └─> NON incrementa (*ok) // Test fallito per segnale
```

**Tabella dei Segnali Riconosciuti:**

| Segnale | Numero | Causa Tipica | Colore |
|---------|--------|--------------|--------|
| SIGSEGV | 11 | Segmentation Fault (accesso memoria illegale) | YELLOW |
| SIGBUS | 5 | Bus Error (accesso memoria non allineato) | YELLOW |
| SIGABRT | 6 | Abort signal (assert fallito) | YELLOW |
| SIGFPE | 8 | Floating Point Exception (div per 0) | YELLOW |
| SIGPIPE | 13 | Broken pipe | YELLOW |
| SIGILL | 4 | Illegal Instruction | YELLOW |

**Macro POSIX Standard (sys/wait.h):**

```c
// Verificare COME è terminato il processo
WIFEXITED(status)      // Ritorna true se uscita normale
WIFSIGNALED(status)    // Ritorna true se terminato da segnale
WIFSTOPPED(status)     // Ritorna true se fermato da segnale di stop
WIFCONTINUED(status)   // Ritorna true se ripreso da SIGCONT

// Estrarre informazioni
WEXITSTATUS(status)    // Codice di uscita (0-255) se WIFEXITED
WTERMSIG(status)       // Numero segnale se WIFSIGNALED
WSTOPSIG(status)       // Numero segnale se WIFSTOPPED
```

**Flusso Esecutivo Completo:**

```
Test Scenario 1: Exit Code 0 (Successo)
─────────────────────────────────────
Padre                           Figlio
│                               │
├─ fork()                        │
│                               ├─ test_strlen() eseguito
│                               ├─ return 0 (o exit(0))
│                               └─ exit(0)
│
├─ wait(&status)
│ status = 0 (WIFEXITED + exit code 0)
│
├─ WIFEXITED(status) = true
├─ WEXITSTATUS(status) = 0
├─ print_result(..., "OK", GREEN)
└─ (*ok)++


Test Scenario 2: Exit Code 1 (Fallimento Logico)
──────────────────────────────────────────────────
Padre                           Figlio
│                               │
├─ fork()                        │
│                               ├─ test_strlen() eseguito
│                               ├─ return 1 (valore atteso != reale)
│                               └─ exit(1)
│
├─ wait(&status)
│ status = (1 << 8) = 256 (WIFEXITED + exit code 1)
│
├─ WIFEXITED(status) = true
├─ WEXITSTATUS(status) = 1
└─ print_result(..., "KO", RED)


Test Scenario 3: Segmentation Fault
─────────────────────────────────────
Padre                           Figlio
│                               │
├─ fork()                        │
│                               ├─ test accede memoria illegale
│                               ├─ Kernel manda SIGSEGV (signal 11)
│                               └─ Processo terminato dal kernel
│
├─ wait(&status)
│ status = 11 (WIFSIGNALED + sig 11)
│
├─ WIFEXITED(status) = false
├─ WIFSIGNALED(status) = true
├─ WTERMSIG(status) = 11
├─ signal_name(11) = "SIGSEGV"
└─ print_result(..., "SIGSEGV", YELLOW)
```

---

### 5. **`signal_name()` - Mappatura Segnali**

**File:** [framework/src/signal_name.c](framework/src/signal_name.c)

```c
const char *signal_name(int sig)
```

**Funzionamento:**

```
INIZIO
   │
   ├─> Tabella statica g_sigtab:
   │   ┌─────────────────────────────┐
   │   │ {SIGSEGV, "SIGSEGV"}   11   │
   │   │ {SIGBUS, "SIGBUS"}     5    │
   │   │ {SIGABRT, "SIGABRT"}   6    │
   │   │ {SIGFPE, "SIGFPE"}     8    │
   │   │ {SIGPIPE, "SIGPIPE"}   13   │
   │   │ {SIGILL, "SIGILL"}     4    │
   │   │ {0, NULL}  // Terminatore   │
   │   └─────────────────────────────┘
   │
   ├─> Iterare tabella:
   │   while (g_sigtab[i].name != NULL):
   │       if (g_sigtab[i].signum == sig):
   │           return g_sigtab[i].name  // ✓ Trovato
   │       i++
   │
   └─> Se non trovato:
       return "CRASH"  // Fallback per segnali sconosciuti
```

**Complessità:** O(1) per segnali comuni (massimo 6 comparazioni)

---

### 6. **`print_result()` - Output Formattato**

**File:** [framework/src/print_result.c](framework/src/print_result.c)

```c
void print_result(const char *func, const char *test,
                  const char *status, const char *color)
```

**Output Format:**
```
[func]:[test]:[status]
```

**Esempio di Output Reale:**
```
[strlen]:[01_basics_test]:[OK]
[strlen]:[02_null_test]:[KO]
[strlen]:[03_segv_test]:[SIGSEGV]
```

**Implementazione:**

```c
void print_result(const char *func, const char *test,
                  const char *status, const char *color)
{
    write(1, "[", 1);                      // "["
    write(1, func, ft_strlen(func));       // nome funzione
    write(1, "]:[", 3);                    // "]:"
    write(1, test, ft_strlen(test));       // nome test
    write(1, "]:[", 3);                    // "]:"
    putstr_color(status, color, 1);        // status con colore
    write(1, "]\n", 2);                    // "]\n"
}
```

**Gestione Colori:**
- `ANSI_GREEN` ("\033[32m"): test OK
- `ANSI_RED` ("\033[31m"): test KO
- `ANSI_YELLOW` ("\033[33m"): test con segnale
- `ANSI_RESET` ("\033[0m"): reset colore

---

### 7. **`print_summary()` - Riepilogo Finale**

**File:** [framework/src/print_summary.c](framework/src/print_summary.c)

```c
void print_summary(int ok, int tot)
```

**Output Esempio:**
```
---------------------------------------------

          3/5 tests checked
```

**Funzionamento:**
```
1. Calcolare lunghezza cifre di ok e tot
2. Calcolare padding per centrare il testo
3. Stampare righe decorative
4. Stampare "ok/tot tests checked" centrato
```

---

### 8. **`free_list()` - Pulizia Memoria**

**File:** [framework/src/free_list.c](framework/src/free_list.c)

```c
void free_list(t_unit_test **list)
```

**Funzionamento:**

```
INIZIO
   │
   ├─> Salvare puntatore corrente: curr = *list
   │
   ├─> Iterare lista fino a NULL:
   │   while (curr != NULL):
   │       │
   │       ├─> Salvare next: next = curr->next
   │       │   (necessario: curr sarà liberato!)
   │       │
   │       ├─> Liberare stringa nome: free(curr->name)
   │       │
   │       ├─> Liberare nodo: free(curr)
   │       │
   │       └─> Avanzare: curr = next
   │
   └─> Nullare pointer lista: *list = NULL
       (evita dangling pointer)
```

**Importanza:**
- Previene memory leak
- Essenziale per test multipli
- Evita doppia liberazione (double free)

---

### 9. **Funzioni Utility**

#### 9.1 **`putstr_color()`** - Stampa Colorata

```c
void putstr_color(const char *str, const char *color, int fd)
```

**Funzionamento:**
```
if (color):
    write(fd, color_code)      // Es: "\033[32m"
    write(fd, str)             // Testo
    write(fd, ANSI_RESET)      // Reset colore

else:
    write(fd, str)             // Testo senza colore
```

#### 9.2 **`num_len()`** - Lunghezza Numero in Cifre

```c
int num_len(int n)
```

Usata per formattare correttamente il riepilogo (calcolare padding).

---

## Gestione dei Segnali

### 1. **Cosa sono i Segnali POSIX?**

I segnali POSIX sono meccanismi di comunicazione **asincrona** nel sistema operativo Unix/Linux. Quando un processo compie un'azione illegale (es: accesso memoria non autorizzato), il **kernel invia un segnale** al processo che lo **termina**.

### 2. **Segnali Riconosciuti dal Framework**

| Segnale | Num | Causa | Scenario Tipico |
|---------|-----|-------|-----------------|
| **SIGSEGV** | 11 | Segmentation Violation | Dereference NULL, buffer overflow |
| **SIGBUS** | 5 | Bus Error | Accesso misallineato |
| **SIGABRT** | 6 | Abort | Assert fallito, free() corrotto |
| **SIGFPE** | 8 | Floating Point Exception | Divisione per zero |
| **SIGPIPE** | 13 | Pipe rotta | Scrittura pipe chiusa |
| **SIGILL** | 4 | Illegal Instruction | Esecuzione codice non valido |

### 3. **Come il Framework Cattura i Segnali**

**Il framework NON cattura i segnali nel senso tradizionale (signal handlers). Invece:**

1. **Ogni test corre in un processo figlio**
2. **Se il test causa un segnale, solo il figlio termina**
3. **Il padre (`wait()`) riceve il numero di segnale via `status`**
4. **Si analizza con `WIFSIGNALED(status)` e `WTERMSIG(status)`**

### 4. **Flusso Completo: Test che Causa SIGSEGV**

```
┌─────────────────────────────────────────────────────────────┐
│                    PROCESSO PADRE                            │
│                                                              │
│  1. run_one_test(test, func, &tot, &ok)                    │
│     ├─> pid = fork()                                        │
│     ├─> if (pid == 0) SALTA A SEZIONE FIGLIO               │
│     │                                                        │
│     └─> wait(&status) // Bloccato qui                      │
│         status variable pronta ricevere info                │
└─────────────────────────────────────────────────────────────┘
                         │
        ┌────────────────┴────────────────┐
        │                                 │
        v                                 v
┌──────────────────────┐       ┌─────────────────────────────┐
│  PROCESSO FIGLIO     │       │  KERNEL (Sistema Operativo) │
│                      │       │                             │
│  2. if (pid == 0):   │       │  - Monitora accessi memoria │
│     exit(test->f())  │       │  - Rileva violazione        │
│     ├─> Chiama:      │       │  - Invia SIGSEGV al figlio  │
│     │   test_func()  │       │                             │
│     │                │       │  SIGTERM ◄─────────────────┤
│     ├─> Accede ptr   │◄──────┤  (signal 11)               │
│     │   NULL         │       │                             │
│     └─> Errore!      │       │                             │
│        Processo      │       │                             │
│        terminato     │       │                             │
│        dal kernel    │       │                             │
└──────────────────────┘       └─────────────────────────────┘
        │                                 │
        └─────────────────┬───────────────┘
                          │
        ┌─────────────────v─────────────────┐
        │  Processo figlio = TERMINATO      │
        │  (kernel ha messo fine a tutto)   │
        │  return status = SIGSEGV (11)     │
        └────────────────┬────────────────┘
                         │
        ┌────────────────v────────────────┐
        │  PROCESSO PADRE                 │
        │  3. wait() ritorna               │
        │     status = SIGSEGV encoding    │
        │                                 │
        │  4. handle_result(status, ...):  │
        │     if WIFSIGNALED(status):      │
        │        sig = WTERMSIG(status)    │
        │        sig == 11                 │
        │        → signal_name(11)         │
        │        → "SIGSEGV"               │
        │        → print_result(...        │
        │           "SIGSEGV", YELLOW)     │
        │                                 │
        │  5. NON incrementa ok            │
        │     (test fallito)               │
        └─────────────────────────────────┘
```

### 5. **Rappresentazione Binaria dello Status**

Quando `wait()` ritorna:

```
Caso A: Exit Normale (code 42)
┌─────────────────────────┐
│ status = 0x2A00         │
│ High Byte: 42 (exit code)
│ Low Byte: 0             │
└─────────────────────────┘
WIFEXITED(status) = true
WEXITSTATUS(status) = 42


Caso B: Terminato da Segnale (SIGSEGV = 11)
┌─────────────────────────┐
│ status = 0x000B         │
│ Low Byte: 11 (signal)   │
│ High Byte: 0            │
└─────────────────────────┘
WIFEXITED(status) = false
WIFSIGNALED(status) = true
WTERMSIG(status) = 11
```

---

## Gestione dei Fork e Processi

### 1. **Principi Fondamentali del Fork**

`fork()` crea un **duplicato esatto** del processo chiamante:

```
Processo Padre (PID: 1000)
├─ Codice: text segment
├─ Memoria: data segment
├─ Stack: locali
└─ File descriptors: aperti


fork()
│
├─ Processo Padre (PID: 1000) ← fork() ritorna 1001
│  │
│  └─ Continua esecuzione da fork()
│
└─ Processo Figlio (PID: 1001) ← fork() ritorna 0
   │
   ├─ Copia memoria padre
   ├─ Continua esecuzione da fork()
   └─ Può divergere dal padre
```

### 2. **Ciclo di Vita del Processo nel Framework**

```
MAIN
  │
  ├─> load_test(&tests, "test1", func1)
  ├─> load_test(&tests, "test2", func2)
  ├─> load_test(&tests, "test3", func3)
  │
  └─> launch_tests(&tests, "strlen")
        │
        ├─> PRIMO TEST ─────────────────────────────────┐
        │   launch_tests:                                │
        │   ├─> run_one_test(test1, ...)                │
        │   │   ├─> pid = fork()                        │
        │   │   │   │                                   │
        │   │   │   ├─ PADRE (pid = 2001)               │
        │   │   │   │  wait(&status)                    │
        │   │   │   │                                   │
        │   │   │   └─ FIGLIO (pid = 0)                 │
        │   │   │      test1->f()                       │
        │   │   │      exit(result)                     │
        │   │   │      [FIGLIO TERMINA]                 │
        │   │   │                                       │
        │   │   ├─> wait() ritorna status              │
        │   │   ├─> (*tot)++ = 1                       │
        │   │   └─> handle_result(status, ...)         │
        │   │                                          │
        │   ├─> SECONDO TEST                           │
        │   │   (Stessa sequenza)                       │
        │   │                                          │
        │   └─> TERZO TEST                             │
        │       (Stessa sequenza)                       │
        │                                              │
        ├─> print_summary(ok, tot)                     │
        ├─> free_list(&tests)                          │
        └─> return (ok == tot ? 0 : -1)               │
```

### 3. **Isolamento tra Test**

```
Test 1                      Test 2
└─ Figlio 1                 └─ Figlio 2
   ├─ Memoria indipendente     ├─ Memoria indipendente
   ├─ Stack proprio            ├─ Stack proprio
   └─ File descriptors         └─ File descriptors
      propri                       propri

Conseguenza:
- Memory leak in test1 NON affetra test2
- Buffer overflow in test1 NON crash test2
- Crash in test1 NON crash padre
```

### 4. **Gestione degli Errori di Fork/Wait**

```
run_one_test():
    ├─> pid = fork()
    │   if (pid == -1):  // Errore fork
    │       write(2, "fork error\n")
    │       return (-1)
    │
    └─> if (wait(&status) == -1):  // Errore wait
        write(2, "wait error\n")
        return (-1)

launch_tests():
    └─> if (run_one_test(...) == -1):
        ├─> free_list(list)  // Pulizia
        └─> return (-1)      // Propagare errore
```

### 5. **Zombie Processes (Processi Zombie)**

```
Senza wait():
Figlio termina → Diventa ZOMBIE
    ├─ Risorsa (PID) NON liberata
    ├─ `ps aux` mostra: <defunct>
    └─ Accumulo zombie dopo molti test

Con wait():  ← Framework usa questo
Figlio termina → wait() lo raccoglie
    ├─ Risorsa (PID) liberata
    ├─ Niente zombie
    └─ Pulizia corretta
```

---

## Flusso di Esecuzione

### 1. **Flusso Principale Completo**

```
START: main()
│
├─ Inclusione headers
│  ├─ libunit.h
│  ├─ sys/wait.h (fork, wait, macros)
│  └─ signal.h (segnali)
│
├─ Allocazione lista: t_unit_test *tests = NULL
│
├─ CARICAMENTO TEST (load_test)
│  ├─> load_test(&tests, "01_basics", test_basics)
│  │   └─> malloc + strdup + insert in lista
│  ├─> load_test(&tests, "02_null", test_null)
│  ├─> load_test(&tests, "03_segv", test_segv)
│  └─> Lista: [test1] → [test2] → [test3] → NULL
│
├─ ESECUZIONE TEST (launch_tests)
│  │
│  ├─ LOOP ITERAZIONE 1
│  │  └─> run_one_test(&test1, "strlen", &tot, &ok)
│  │      ├─ fork()
│  │      │  ├─ FIGLIO (pid=0):
│  │      │  │  ├─ test_basics() eseguito
│  │      │  │  ├─ return 0 / 1
│  │      │  │  └─ exit(return_value)
│  │      │  │  [FIGLIO TERMINA]
│  │      │  │
│  │      │  └─ PADRE (pid>0):
│  │      │     wait(&status)
│  │      │     ├─ status = exit_code (0 o 1)
│  │      │     ├─ tot = 1
│  │      │     └─ handle_result(status)
│  │      │        ├─ WIFEXITED = true
│  │      │        ├─ WEXITSTATUS = 0 → OK, ok++
│  │      │        └─ print_result(..., "OK", GREEN)
│  │
│  ├─ LOOP ITERAZIONE 2
│  │  └─> run_one_test(&test2, "strlen", &tot, &ok)
│  │      ├─ fork()
│  │      │  ├─ FIGLIO:
│  │      │  │  test_null() con NULL pointer
│  │      │  │  → Kernel manda SIGSEGV
│  │      │  │  [FIGLIO TERMINATO DA SEGNALE]
│  │      │  │
│  │      │  └─ PADRE:
│  │      │     wait(&status)
│  │      │     ├─ status = segnale encoding
│  │      │     ├─ tot = 2
│  │      │     └─ handle_result(status)
│  │      │        ├─ WIFEXITED = false
│  │      │        ├─ WIFSIGNALED = true
│  │      │        ├─ WTERMSIG = 11
│  │      │        ├─ signal_name(11) = "SIGSEGV"
│  │      │        └─ print_result(..., "SIGSEGV", YELLOW)
│  │
│  ├─ LOOP ITERAZIONE 3
│  │  └─> run_one_test(&test3, "strlen", &tot, &ok)
│  │      └─ ... (simile iterazione 1-2)
│  │
│  ├─ DOPO LOOP
│  │  ├─ print_summary(ok=1, tot=3)
│  │  │  └─ "1/3 tests checked"
│  │  ├─ free_list(&tests)
│  │  │  └─ free(test1->name), free(test1)
│  │  │     free(test2->name), free(test2)
│  │  │     free(test3->name), free(test3)
│  │  └─ return (ok == tot ? 0 : -1)
│  │     └─ return (-1) perché ok < tot
│  │
│  └─ Ritorno al main()
│
└─ END: main() exits
```

### 2. **Output Traccia Esecuzione**

```
[strlen]:[01_basics]:[OK]      ← Verde
[strlen]:[02_null]:[SIGSEGV]   ← Giallo
[strlen]:[03_segv]:[KO]        ← Rosso

---------------------------------------------

          1/3 tests checked
```

---

## Output e Formattazione

### 1. **Codici ANSI di Colore**

```c
#define ANSI_GREEN  "\033[32m"   // OK
#define ANSI_RED    "\033[31m"   // KO
#define ANSI_YELLOW "\033[33m"   // Segnale
#define ANSI_RESET  "\033[0m"    // Reset
```

### 2. **Sequenza Escape ANSI**

```
"\033[32m" = ESC + "[" + "32" + "m"
│           │    │   │   │
│           │    │   └───┴─ Color code
│           │    └──────────── Command
│           └───────────────── Escape
└──────────────────────────────── ANSI Code
```

### 3. **Output Format**

```
Linea singolo test:
[func_name]:[test_name]:[status]

Colore applicato SOLO allo status:
[strlen]:[01_basics]:[<GREEN>OK<RESET>]
                       ^^^^          ^^^^
                   Colore         Reset


Riepilogo:
─────────────────────────────────────────
            ok_count/total tests checked

Linee decorative: 45 caratteri "-"
Testo centrato: padding = (45 - lunghezza_testo) / 2
```

---

## Flusso Dettagliato: Casi d'Uso Reali

### Caso 1: Test Che Passa

```
launcher.c:
    load_test(&tests, "01_basics", test_strlen_basics);

test_strlen_basics.c:
    int test_strlen_basics(void)
    {
        char *str = "hello";
        if (ft_strlen(str) == 5)
            return (0);  // ← EXIT CODE 0
        return (1);
    }

Esecuzione:
├─ fork()
├─ FIGLIO:
│  ├─ test_strlen_basics()
│  ├─ strlen("hello") = 5 ✓
│  ├─ return (0)
│  └─ exit(0)
│
├─ PADRE:
│  ├─ wait(&status) = 0
│  ├─ WIFEXITED(0) = true
│  ├─ WEXITSTATUS(0) = 0
│  ├─ handle_result():
│  │  if (WEXITSTATUS(status) == 0):
│  │    print_result("strlen", "01_basics", "OK", GREEN)
│  │    ok++
│  │
│  └─> Output: [strlen]:[01_basics]:[OK]  ← Verde
```

### Caso 2: Test Che Fallisce (Logicamente)

```
launcher.c:
    load_test(&tests, "02_return", test_strlen_return);

test_strlen_return.c:
    int test_strlen_return(void)
    {
        char *str = "test";
        if (ft_strlen(str) == 999)  // Condizione falsa
            return (0);
        return (1);  // ← EXIT CODE 1
    }

Esecuzione:
├─ fork()
├─ FIGLIO:
│  ├─ test_strlen_return()
│  ├─ strlen("test") = 4 ≠ 999 ✗
│  ├─ return (1)
│  └─ exit(1)
│
├─ PADRE:
│  ├─ wait(&status)
│  ├─ status = (1 << 8) = 256
│  ├─ WIFEXITED(256) = true
│  ├─ WEXITSTATUS(256) = 1
│  ├─ handle_result():
│  │  if (WEXITSTATUS(status) == 0):
│  │    // false, non entrare
│  │  else:
│  │    print_result("strlen", "02_return", "KO", RED)
│  │
│  └─> Output: [strlen]:[02_return]:[KO]  ← Rosso
```

### Caso 3: Test Che Crasha (Segnale)

```
launcher.c:
    load_test(&tests, "03_segv", test_strlen_null);

test_strlen_null.c:
    int test_strlen_null(void)
    {
        char *null_ptr = NULL;
        return (ft_strlen(null_ptr));  // ← Undefined behavior!
    }

Esecuzione:
├─ fork()
├─ FIGLIO:
│  ├─ test_strlen_null()
│  ├─ ft_strlen(NULL)
│  │  ├─ Accesso: *ptr (dove ptr = NULL)
│  │  └─ Kernel rileva violazione memoria
│  │
│  ├─ Kernel manda SIGSEGV (signal 11) al figlio
│  └─ FIGLIO TERMINATO DAL KERNEL
│
├─ PADRE:
│  ├─ wait(&status)
│  ├─ status encoding contiene info segnale
│  ├─ WIFEXITED(status) = false
│  ├─ WIFSIGNALED(status) = true
│  ├─ WTERMSIG(status) = 11
│  ├─ handle_result():
│  │  if (WIFSIGNALED(status)):
│  │    sig = WTERMSIG(status) = 11
│  │    sig_str = signal_name(11) = "SIGSEGV"
│  │    print_result("strlen", "03_segv", "SIGSEGV", YELLOW)
│  │    // NON incrementa ok
│  │
│  └─> Output: [strlen]:[03_segv]:[SIGSEGV]  ← Giallo
```

### Caso 4: Errore System Call (Fork/Wait)

```
Scenario: Sistema non ha risorse per fork

run_one_test():
    pid = fork()
    if (pid == -1):  // ← fork() fallito
        write(2, "fork error\n", 11)
        return (-1)

launch_tests():
    if (run_one_test(curr, func_name, &tot, &ok) == -1):
        free_list(list)  // Pulizia
        return (-1)      // Propagare errore

Output:
  fork error
  [programma termina con -1]
```

---

## Architettura Completa: Diagramma Finale

```
┌──────────────────────────────────────────────────────────────┐
│                    LIBUNIT FRAMEWORK                          │
└──────────────────────────────────────────────────────────────┘

API PUBLIC
────────
 load_test()          ← Carica test nella lista
 launch_tests()       ← Orchestracore esecuzione
 run_one_test()       ← Esegue singolo test con fork/wait
 free_list()          ← Libera memoria

CORE LOGIC
──────────
┌──────────────────────────────────────────────────────────────┐
│ 1. Load Phase          │ 2. Launch Phase        │ 3. Cleanup  │
├────────────────────────┼────────────────────────┼─────────────┤
│                        │                        │             │
│ load_test():           │ launch_tests():        │ free_list():│
│ - malloc nodo          │ - Itera lista test     │ - free name │
│ - strdup(name)         │ - Chiama run_one_test  │ - free nodo │
│ - insert in lista      │ - Incrementa tot/ok    │ - NULLify   │
│                        │ - Gestisce errori      │             │
│                        │                        │             │
└────────────────────────┴────────────────────────┴─────────────┘

PROCESS MANAGEMENT
──────────────────
┌────────────────────────────────────────────────────────────────┐
│ run_one_test()                                                 │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│ fork()                                                         │
│ ├─ CHILD (pid==0)          │ PARENT (pid>0)                  │
│ │  test->f()               │ wait(&status)                    │
│ │  exit(result)            │ handle_result(status)            │
│ │  [TERMINATE]             │ [CONTINUE]                       │
│ │                          │                                  │
│ └─────────────────────────┴──────────────────────────────────│
│                                                                │
└────────────────────────────────────────────────────────────────┘

SIGNAL HANDLING
───────────────
┌────────────────────────────────────────────────────────────────┐
│ handle_result(status, func_name, test_name, &ok)              │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│  WIFEXITED(status)         WIFSIGNALED(status)                │
│  │                         │                                  │
│  ├─ WEXITSTATUS==0         ├─ WTERMSIG→signal_name()        │
│  │  └─ "OK" (GREEN)        │  └─ "SIGSEGV" etc (YELLOW)    │
│  │     ok++                │     (NO ok++)                  │
│  │                         │                                  │
│  └─ WEXITSTATUS!=0         └─ CRASH (YELLOW)               │
│     └─ "KO" (RED)                                           │
│        (NO ok++)                                            │
│                                                                │
└────────────────────────────────────────────────────────────────┘

OUTPUT
──────
print_result()    → [func]:[test]:[status with color]
print_summary()   → "ok/tot tests checked"
signal_name()     → Map signal num to string
```

---

## Riassunto Funzionale

| Funzione | Scopo | Tipo |
|----------|-------|------|
| `load_test()` | Caricamento test in lista | Gestione memoria |
| `launch_tests()` | Orchestrazione esecuzione test | Controllo flusso |
| `run_one_test()` | Fork + wait singolo test | **Core (isolamento)** |
| `handle_result()` | Analisi status exit/segnale | **Core (diagnostica)** |
| `signal_name()` | Mapping signal number→name | Diagnostica |
| `print_result()` | Output singolo test | I/O |
| `print_summary()` | Riepilogo risultati | I/O |
| `free_list()` | Liberazione memoria | Gestione memoria |
| `putstr_color()` | Stampa con colore ANSI | I/O |
| `num_len()` | Calcolo cifre numero | Utility |

---

## Conclusione

LibUnit è un **framework di testing robusto che sfrutta l'isolamento dei processi** per garantire che ogni test sia indipendente dagli altri. La gestione dei fork e dei segnali è il **cuore del framework**, permettendo di:

1. **Eseguire test in processo separato** → Non influenzano il padre
2. **Catturare crash e segnali** → Informazioni diagnostiche precise
3. **Mantiene testo leggibile** → Output colorato e formattato
4. **Gestisce memoria** → Pulizia automatica risorse

Il framework è particolarmente utile per **testare funzioni C potenzialmente instabili** senza rischiare che il test harness stesso vada in crash.

