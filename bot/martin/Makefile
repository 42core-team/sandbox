SRCDIR = src
OBJDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC = gcc
CFLAGS = -Wall -Wextra -Werror #-ggdb -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
CONNECTIONDIR = ../connection
LIBS = $(CONNECTIONDIR)/con_lib.a
INC = $(CONNECTIONDIR)/inc
EXEC = martin
.PHONY: all run clean fclean re $(EXEC)

all: build $(EXEC)

run: all
	./$(EXEC)

$(EXEC): $(OBJS)
	make -C $(CONNECTIONDIR)
	$(CC) $(CFLAGS) $^ -o $@ -L $(CONNECTIONDIR) -l:con_lib.a -I $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC)

build:
	mkdir -p $(OBJDIR)
	make -C $(CONNECTIONDIR)

clean:
	make -C $(CONNECTIONDIR) clean
	rm -rf $(OBJDIR)

fclean: clean
	make -C $(CONNECTIONDIR) fclean
	rm -rf $(EXEC)

re: fclean all
