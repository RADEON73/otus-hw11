#include "Server.h"
#include <exception>
#include <iostream>
#include <string>

/**
 * @brief Точка входа в приложение сервера.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения.
 */
int main(int argc, char* argv[])
{
	try {
		if (argc != 2) {
			std::cerr << "Usage: join_server <port>\n";
			return -1;
		}
		boost::asio::io_context io_context;
		Server server(io_context, (unsigned short)std::stoi(argv[1]));
		std::cout << "Server is Running\n";
		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

/* ТЕСТОВЫЕ ДАННЫЕ

ncat localhost 7777

INSERT A 0 lean
INSERT A 1 sweater
INSERT A 2 frank
INSERT A 3 violation
INSERT A 4 quality
INSERT A 5 precision
INSERT B 3 proposal
INSERT B 4 example
INSERT B 5 lake
INSERT B 6 flour
INSERT B 7 wonder
INSERT B 8 selection
INTERSECTION
SYMMETRIC_DIFFERENCE
TRUNCATE A
*/