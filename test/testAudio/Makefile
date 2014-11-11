CC =	g++

RM=	rm -f

NAME= 	testAudio

SRCS=	main.cpp	\
	CWMutex.cpp	\
	CUMutex.cpp	\
	IOControl.cpp	\
	Codec.cpp

OBJS=	$(SRCS:.cpp=.o)

LXXFLAGS=	-lpthread				\
		-L../../lib/opus-1.1/.libs/		\
		-lopus					\
		-L../../lib/portaudio/lib/.libs/	\
		-lportaudio

CXXFLAGS=	-Wall -Werror -W			\
		-I../../lib/opus-1.1/include/ 		\
		-I../../lib/portaudio/include/		


$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LXXFLAGS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all
