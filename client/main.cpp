#include <iostream>
#include <exception>
#include "client.h"

int main(int argc, const char* argv[]) {
	try {
		if (argc != 3) {
			std::cerr << "Bad program call. Expected " << argv[0] << " <hostname> <servname>.\n";
			return ERROR;
		}

		const std::string hostname = argv[1];
		const std::string servname = argv[2];

		std::cout << "Levanto cliente en hostname: " << hostname << " y servername: " << servname << "\n";

		Client cliente(hostname, servname);
     	cliente.start();

		return SUCCESS;

	} catch (const std::exception& err) {
		std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
		return ERROR;
	} catch (...) {
		std::cerr << "Something went wrong and an unknown exception was caught.\n";
		return ERROR;
	}
}
