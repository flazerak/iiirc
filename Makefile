C			:=	c++
CFLAGS	:= -std=c++98 -Wall -Wextra -Werror


SRCDIR		:=	src
INCDIR		:=	inc

SRCS		:=	$(SRCDIR)/main.cpp\
				$(SRCDIR)/Server.cpp \
				$(SRCDIR)/Message.cpp \
				$(SRCDIR)/Client.cpp\
				$(SRCDIR)/Channel.cpp\
				$(SRCDIR)/Command.cpp\


INCS		:=	$(INCDIR)/Server.hpp\
				$(INCDIR)/Message.hpp\
				$(INCDIR)/Client.hpp\
				$(INCDIR)/Channel.hpp\
				$(INCDIR)/Command.hpp\

OBJS		:=	$(SRCS:.cpp=.o)

NAME		:= ircserv

.SUFFIXES : .cpp .o
.cpp.o :
	$(C) $(FLAGS) -c $< -o $@

all	: $(NAME)

$(NAME)	:	$(OBJS)
	$(C)  $(OBJS) -o $@

clean :
	rm -f $(OBJS)

fclean :
	make clean
	rm -f $(NAME)

re :
	make fclean
	make $(NAME)

.PHONY : all clean fclean re
