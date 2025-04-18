#pragma once
#include "Database.h"
#include <boost/asio.hpp>
#include <queue>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

/**
 * @brief Класс сессии клиента. Обрабатывает команды по одному TCP-соединению.
 */
class Session : public std::enable_shared_from_this<Session>
{
public:
	/**
 * @brief Конструктор сессии.
 * @param socket TCP-сокет клиента.
 * @param db Ссылка на общую базу данных.
 */
	Session(tcp::socket socket, Database& db);
	/**
 * @brief Запускает сессию (чтение входящих данных).
 */
	void start();

private:
	/**
	 * @brief Асинхронно читает команду от клиента.
	 */
	void do_read();

	/**
	 * @brief Обрабатывает полученную строку команды.
	 * @param command Команда от клиента.
	 */
	void handle_command(const std::string& command);

	/**
	 * @brief Помещает ответ в очередь отправки.
	 * @param msg Ответ клиенту.
	 */
	void queue_response(const std::string& msg);

	/**
	 * @brief Асинхронно отправляет сообщения клиенту.
	 */
	void do_write();

	tcp::socket socket_; ///< TCP-сокет клиента.
	boost::asio::streambuf buffer_; ///< Буфер для чтения входящих данных.
	Database& db_; ///< Ссылка на базу данных.
	std::queue<std::string> response_queue_; ///< Очередь ответов клиенту.
};
