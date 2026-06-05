NAME        = librealtests.a
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
AR          = ar rcs
RM          = rm -f

SRCS        = Launcher.c \
              ft_strlen/00_launcher.c \
              ft_strlen/01_basics_test.c \
              ft_strlen/02_null_test.c \
              ft_strlen/03_segv_test.c \
              ft_strlen/04_bus_test.c \
              ft_strlen/05_one_char.c \
              ft_strlen/06_long_string.c \
              ft_strlen/07_spaces.c \
              ft_strlen/08_special_chars.c \
              ft_strlen/09_multibyte.c \
              ft_strlen/10_very_long.c \
              ft_strlen/11_alloc_test.c \
              ft_strlen/12_boundary.c \
              ft_strlen/13_compare.c \
              ft_strlen/14_overflow.c \
              ft_strlen/15_random_test.c \
              ft_strlen/16_null_pointer.c \
              ft_strlen/17_empty_string.c \
              ft_strlen/18_sigabrt_test.c \
              ft_strlen/19_sigfpe_test.c \
              ft_strlen/20_sigpipe_test.c \
              ft_strlen/21_sigill_test.c \
              ft_strlen/17_empty_string.c

OBJS        = $(SRCS:.c=.o)

INCLUDES    = -I ft_strlen \
              -I ../libft \
              -I ../framework/includes

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all test clean fclean re