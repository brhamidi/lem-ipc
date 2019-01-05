CC	= gcc
NAME	= lemipc
CFLAGS	= -Werror -Wextra -Wall -O2
CFLAGS	+= -lrt
CFLAGS	+= -pthread
CFLAGS	+= -lncurses

SRC_PATH	= src/
INCLUDE_PATH	= include/

OBJ		= main.o get_number.o run.o game.o display.o play.o can_play.o move.o

INCLUDE	= lemipc.h

OBJS=$(addprefix $(SRC_PATH), $(OBJ))
INCLUDES=$(addprefix $(INCLUDE_PATH), $(INCLUDE))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INCLUDES) Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE_PATH) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
