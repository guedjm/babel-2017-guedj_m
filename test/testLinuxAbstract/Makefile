CC=			g++

RM=			rm -f

NAME_SERV= 		testServer

NAME_CLIENT=		testClient

SRC_DIRECTORY=		../../src/common/

SRCS_SERV=		$(SRC_DIRECTORY)LinuxPipe.cpp			\
			$(SRC_DIRECTORY)LinuxSelect.cpp			\
			$(SRC_DIRECTORY)LinuxTCPClient.cpp		\
			$(SRC_DIRECTORY)LinuxTCPRemoteClient.cpp	\
			$(SRC_DIRECTORY)LinuxTCPServer.cpp		\
			$(SRC_DIRECTORY)LinuxTCPSocket.cpp		\
			$(SRC_DIRECTORY)LinuxUDPSocket.cpp		\
			$(SRC_DIRECTORY)LinuxUDPServer.cpp		\
            $(SRC_DIRECTORY)LinuxUDPClient.cpp		\
            $(SRC_DIRECTORY)LinuxMutex.cpp          \
			main_server.cpp

OBJS_SERV=	$(SRCS_SERV:.cpp=.o)


SRCS_CLIENT=		$(SRC_DIRECTORY)LinuxPipe.cpp			\
			$(SRC_DIRECTORY)LinuxSelect.cpp			\
			$(SRC_DIRECTORY)LinuxTCPClient.cpp		\
			$(SRC_DIRECTORY)LinuxTCPRemoteClient.cpp	\
			$(SRC_DIRECTORY)LinuxTCPServer.cpp		\
			$(SRC_DIRECTORY)LinuxTCPSocket.cpp		\
			$(SRC_DIRECTORY)LinuxUDPSocket.cpp		\
			$(SRC_DIRECTORY)LinuxUDPServer.cpp		\
            $(SRC_DIRECTORY)LinuxUDPClient.cpp		\
            $(SRC_DIRECTORY)LinuxMutex.cpp          \
			main_client.cpp

OBJS_CLIENT=	$(SRCS_CLIENT:.cpp=.o)

CXXFLAGS=	-Wall -Werror -Wextra

$(NAME_SERV):	$(OBJS_SERV)
		$(CC) $(OBJS_SERV) -o $(NAME_SERV) -lpthread

$(NAME_CLIENT):	$(NAME_SERV) $(OBJS_CLIENT)
		$(CC) $(OBJS_CLIENT) -o $(NAME_CLIENT) -lpthread

all:		$(NAME_CLIENT)

clean:
		$(RM) $(OBJS_SERV)
		$(RM) $(OBJS_CLIENT)

fclean:		clean
		$(RM) $(NAME_SERV)
		$(RM) $(NAME_CLIENT)

re:		fclean all
