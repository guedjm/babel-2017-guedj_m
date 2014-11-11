#include <sstream>
#include "BabelServer.h"
#include "constant.h"

int	main(int ac, char *argv[])
{
	int port = DEFAULT_SERVER_PORT;
	if (ac > 1)
	{
		std::stringstream ss(argv[1]);
		ss >> port;
	}
	BabelServer babel(port);
	babel.start();
}
