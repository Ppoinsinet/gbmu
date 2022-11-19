SRC = main.cpp \
	cartridge.cpp \
	cpu.cpp \
	ram.cpp \
	ui.cpp \
	bus.cpp \
	dbg.cpp \
	interrupts.cpp \
	io.cpp \
	timer.cpp \
	instructions/instructions.cpp \
	instructions/ld.cpp \
	instructions/instructions_proc.cpp \
	instructions/cb_instructions.cpp

OBJS = $(addprefix src/, $(SRC:.cpp=.o))

INCLUDE = include/

NAME = mygbmu

FLAGS =

CC = clang++

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) -lSDL2 -lpthread $(OBJS) -std=c++11

.cpp.o:
	$(CC) -c $< -o $(<:.cpp=.o) -I$(INCLUDE) $(FLAGS) -std=c++11

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(OBJS)

re: fclean all