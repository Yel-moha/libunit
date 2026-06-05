NAME        = unit_test
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
RM          = rm -f

FRAMEWORK_DIR = framework
LIBFT_DIR     = libft
TESTS_DIR     = tests
REAL_TESTS_DIR = real-tests

LIBUNIT = $(FRAMEWORK_DIR)/libunit.a
LIBFT   = $(LIBFT_DIR)/libft.a
TESTS   = $(TESTS_DIR)/libtests.a
REAL    = $(REAL_TESTS_DIR)/librealtests.a

MAIN_SRC = Launchers.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

all: $(LIBUNIT) $(LIBFT)

test: $(NAME)
	@./$(NAME) || true

# ORDINE CORRETTO: prima gli oggetti/librerie che USANO i simboli,
# poi le librerie che li FORNISCONO
$(NAME): $(MAIN_OBJ) $(TESTS) $(REAL) $(LIBUNIT) $(LIBFT)
	$(CC) $(CFLAGS) $(MAIN_OBJ) $(TESTS) $(REAL) $(LIBUNIT) $(LIBFT) -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CFLAGS) -I $(FRAMEWORK_DIR)/includes -c $< -o $@

$(LIBUNIT):
	$(MAKE) -C $(FRAMEWORK_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(TESTS):
	$(MAKE) -C $(TESTS_DIR) test

$(REAL):
	$(MAKE) -C $(REAL_TESTS_DIR) test

clean:
	$(RM) $(MAIN_OBJ)
	$(MAKE) -C $(FRAMEWORK_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(TESTS_DIR) clean
	$(MAKE) -C $(REAL_TESTS_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(FRAMEWORK_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(TESTS_DIR) fclean
	$(MAKE) -C $(REAL_TESTS_DIR) fclean

re: fclean all

.PHONY: all test clean fclean re