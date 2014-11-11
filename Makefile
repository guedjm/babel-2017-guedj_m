
NAME =  babel

$(NAME):	call


call:
		$(MAKE) -C ./src/server/
		$(MAKE) -C ./src/client/
		$(MAKE) -C ./test/testAudio/
install:
		sh ./install_script

all:		$(NAME)

clean:
		$(MAKE) clean -C ./src/server/
		$(MAKE) clean -C ./src/client/
		$(MAKE) clean -C ./test/testAudio/

fclean:		clean
		$(MAKE) fclean -C ./src/server/
		$(MAKE) fclean -C ./src/client/
		$(MAKE) fclean -C ./test/testAudio/

re:		fclean all



