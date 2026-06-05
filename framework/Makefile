NAME		= libunit.a
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
AR			= ar rcs
RM			= rm -f

SRC_DIR		= src
INC_DIR		= includes

SRCS		= $(SRC_DIR)/free_list.c \
			  $(SRC_DIR)/handle_result.c \
			  $(SRC_DIR)/launch_tests.c \
			  $(SRC_DIR)/load_test.c \
			  $(SRC_DIR)/num_len.c \
			  $(SRC_DIR)/print_result.c \
			  $(SRC_DIR)/print_summary.c \
			  $(SRC_DIR)/putstr_color.c \
			  $(SRC_DIR)/run_one_test.c \
			  $(SRC_DIR)/signal_name.c 

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re