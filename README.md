*This project has been created as part of the 42 curriculum by yel-moha and fedegugl.*

# libunit_What-the-fork-

## Description

**libunit** is a micro-framework for unit testing in C, developed as part of the
42 curriculum. It provides a simple yet powerful system for organizing, running,
and validating test suites with comprehensive signal handling and result
reporting.

The framework allows you to:
- Execute a series of tests sequentially without interruption
- Run each test in a separate process to prevent cascading failures
- Capture test results and system signals (SIGSEGV, SIGBUS, SIGABRT, SIGFPE,
  SIGPIPE, SIGILL)
- Format output in a structured manner for easy parsing and analysis
- Organize tests by function in a modular directory structure

The framework is compiled as a static library (`libunit.a`) that you can include
in your test routines. It includes bonus features such as colored output, support
for multiple signal types, and flexible test organization.

This project focuses on:
- Process management (fork, wait, exit)
- Signal handling and system-level error detection
- Modular test organization and execution
- Dynamic data structures (linked lists)
- Clean and norm-compliant C code

---

## Instructions

### Compilation

To compile the entire project (framework, libft, tests, and real-tests), run:

```bash
make
```

This will generate:
- `framework/libunit.a` - The core testing framework
- `libft/libft.a` - Utility library
- `unit_test` - The main executable (combining all tests)

To compile only the framework:

```bash
make -C framework
```

### Execution

Run the complete test suite:

```bash
make test
```

This compiles and executes all tests (both basic tests and real-tests on libft
functions).

Run tests from a specific directory:

```bash
make -C tests test
make -C real-tests test
```

### Test Output Format

Each test result follows this format:

```
[function_name]:[test_name]:[status]
```

Where `status` is one of:
- `OK` - Test passed
- `KO` - Test failed
- `SIGSEGV` - Segmentation fault detected
- `SIGBUS` - Bus error detected
- `SIGABRT` - Abort signal detected
- `SIGFPE` - Floating point exception detected
- `SIGPIPE` - Pipe broken signal detected
- `SIGILL` - Illegal instruction signal detected

Example output:

```
[FT_STRLEN]:[Basic test]:[OK]
[FT_STRLEN]:[NULL test]:[SIGSEGV]
[FT_STRLEN]:[Long string]:[KO]

---------------------------------------------
             12/21 tests checked
```

---

## Project Structure

### Directory Organization

```
libunit/
├── framework/              - Core testing framework library
│   ├── includes/
│   │   └── libunit.h      - Framework header with struct definitions
│   ├── src/
│   │   ├── launch_tests.c - Execute test list
│   │   ├── load_test.c    - Register tests in linked list
│   │   ├── run_one_test.c - Fork and run single test
│   │   ├── handle_result.c - Process test results
│   │   ├── free_list.c    - Memory cleanup
│   │   ├── signal_name.c  - Signal to name mapping
│   │   ├── print_result.c - Format output
│   │   ├── print_summary.c - Summary statistics
│   │   ├── putstr_color.c - Colored output (bonus)
│   │   └── num_len.c      - Utility function
│   └── Makefile
│
├── libft/                  - Utility functions library
│   ├── ft_strlen.c
│   ├── ft_strdup.c
│   ├── ft_putnbr_fd.c
│   ├── libft.h
│   └── Makefile
│
├── tests/                  - Basic test suite
│   ├── Launcher.c         - Main entry point for tests
│   ├── strlen/
│   │   ├── 00_launcher.c  - Test launcher for strlen
│   │   ├── 01_basics_test.c
│   │   ├── 02_null_test.c
│   │   ├── 03_segv_test.c
│   │   ├── 04_bus_test.c
│   │   └── tests_strlen.h
│   └── Makefile
│
├── real-tests/            - Comprehensive test suite (21+ tests)
│   ├── Launcher.c         - Main entry point for real-tests
│   ├── ft_strlen/
│   │   ├── 00_launcher.c  - Test launcher
│   │   ├── 01_basics_test.c through 21_sigill_test.c
│   │   └── tests_ft_strlen.h
│   └── Makefile
│
├── Launchers.c            - Root launcher (calls all test suites)
├── Makefile               - Main build configuration
└── README.md              - This file
```

### Key Features

- **Process Isolation**: Each test runs in a separate process using `fork()`,
  preventing one failure from affecting others
- **Signal Handling**: Comprehensive signal detection including SIGSEGV, SIGBUS,
  SIGABRT, SIGFPE, SIGPIPE, SIGILL
- **Modular Design**: Tests organized by function in separate directories
- **Dynamic Test Registration**: Tests registered at runtime using a linked list
  structure
- **Colored Output**: Terminal color support for better readability (bonus)
- **Flexible Configuration**: Tests can be commented out or conditionally loaded

### File Naming Convention

Test files follow a specific naming pattern for proper execution order:

```
00_launcher.c          - Always loaded first, contains launcher function
01_*.c                 - Test executed first
02_*.c                 - Test executed second
...
NN_*.c                 - Test executed in numerical order
```

---

## Test Writing Guide

### Basic Test Function

```c
int my_test(void)
{
	if (/* test condition */)
		return (0);        // Test passes
	else
		return (-1);       // Test fails
}
```

### Registering Tests

In your launcher file:

```c
int strlen_launcher(void)
{
	t_unit_test *testlist = NULL;

	load_test(&testlist, "Basic test", &basic_test);
	load_test(&testlist, "NULL test", &null_test);
	// load_test(&testlist, "Segv test", &segv_test);  // Commented out

	return (launch_tests(&testlist));
}
```

---

## Makefile Targets

### Main Makefile

| Target | Description |
|--------|-------------|
| `make` or `make all` | Compile framework library and libft library |
| `make test` | Compile complete binary with tests and run test suite |
| `make clean` | Remove object files |
| `make fclean` | Remove objects and executables |
| `make re` | Clean and rebuild |

### Subdirectory Makefiles

Each subdirectory (framework, libft, tests, real-tests) supports the same
targets.

---

## Resources

### Theory & References

- Dining Philosophers Problem (test case example)
- Process management (fork, wait, exit)
- Signal handling in C (POSIX signals)
- Linked list data structures
- Unit testing best practices

### Documentation

- `man 2 fork` - Process creation
- `man 2 wait` - Process termination
- `man 2 signal` - Signal handling
- `man 7 signal` - Signal overview

---

## Compliance

This project fully complies with:
- **42 Norm**: All source files pass norminette style checker (47/47 files OK)
- **Memory Management**: All dynamically allocated memory is properly freed
- **Signal Handling**: Comprehensive coverage of system signals
- **Build System**: Proper Makefile structure with required targets

---

## Bonus Features Implemented

- ✅ **Colored Output**: Test results displayed with ANSI color codes
- ✅ **Extended Signal Support**: SIGABRT, SIGFPE, SIGPIPE, SIGILL detection
- ✅ **Comprehensive Testing**: 21+ real-world tests on libft functions
- ✅ **Clean Architecture**: Well-organized modular code structure

---

## Author

**yel-moha** – 42 Student (42 Firenze)

---

## License

This project is provided as-is for educational purposes as part of the 42
curriculum.
