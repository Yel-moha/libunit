NAME        = libtests.a
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
AR          = ar rcs
RM          = rm -f

SRCS        = Launcher.c \
              strlen/00_launcher.c \
              strlen/01_basics_test.c \
              strlen/02_null_test.c \
              strlen/03_segv_test.c \
              strlen/04_bus_test.c

OBJS        = $(SRCS:.c=.o)

INCLUDES    = -I strlen \
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