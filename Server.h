#pragma once
#include <boost/asio.hpp>
#include "Database.h"

using boost::asio::ip::tcp;

/**
 * @brief Класс TCP-сервера, обрабатывающего входящие соединения.
 */
class Server
{
public:
	/**
	 * @brief Конструктор сервера.
	 * @param io_context Контекст ввода-вывода Boost Asio.
	 * @param port Порт, на котором будет слушать сервер.
	 */
	Server(boost::asio::io_context& io_context, unsigned short port);

private:
	/**
	 * @brief Начинает асинхронный процесс приёма входящих соединений.
	 */
	void do_accept();

	tcp::acceptor acceptor_; ///< Объект для приёма входящих соединений.
	Database db_; ///< Общая база данных для всех сессий.
};