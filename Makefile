SRC = main.c cartridge.c cpu.c instructions.c ram.c instructions_proc.c ui.c bus.c dbg.c interrupts.c io.c timer.c

OBJS = $(addprefix src/, $(SRC:.c=.o))

INCLUDE = include/

NAME = mygbmu

FLAGS = 

CC = gcc

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) -lSDL2 -lpthread $(OBJS)

.c.o:
	$(CC) -c $< -o $(<:.c=.o) -I$(INCLUDE)

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(OBJS)

re: fclean all