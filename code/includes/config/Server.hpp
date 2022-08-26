#include <conig/Composite.hpp>

class Server : public Composite<std::string>
{
	public:
		Server();
		Server(const Server &Server);
		~Server();
		Server &operator=(const Server &Server);
};
