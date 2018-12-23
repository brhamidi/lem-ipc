
CC	= gcc
NAME	= lemipc
COFLAGS	= -Werror -Wextra -Wall -O2

SRC_PATH	= src/
INCLUDE_PATH	= include/

OBJ		= main.o

INCLUDE	= lemipc.h

OBJS=$(addprefix $(SRC_PATH), $(OBJ))
INCLUDES=$(addprefix $(INCLUDE_PATH), $(INCLUDE))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INCLUDES) Makefile
	$(CC) $(COFLAGS) -I $(INCLUDE_PATH) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
