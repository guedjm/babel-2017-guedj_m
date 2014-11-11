CC=			g++

RM=			rm -f

NAME= 		server

SRCS=	main.cpp

OBJS=	$(SRCS:.cpp=.o)

CXXFLAGS=		-Wall -Werror -Wextra

$(NAME):	$(OBJS)
		$(CC) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
