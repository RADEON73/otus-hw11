#include "Session.h"
#include <format>
#include <vector>

Session::Session(tcp::socket socket, Database& db) : socket_(std::move(socket)), db_(db)
{}

void Session::start()
{
	do_read();
}

void Session::do_read()
{
	auto self(shared_from_this());
	boost::asio::async_read_until(socket_, buffer_, '\n',
		[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				std::istream is(&buffer_);
				std::string command;
				std::getline(is, command);
				if (!command.empty() && command.back() == '\r') {
					command.pop_back();
				}
				handle_command(command);
			}
		});
}

void Session::handle_command(const std::string& command)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = command.find(' ');

	while (end != std::string::npos) {
		tokens.push_back(command.substr(start, end - start));
		start = end + 1;
		end = command.find(' ', start);
	}
	tokens.push_back(command.substr(start));

	if (tokens.empty()) {
		queue_response("ERR invalid command\n");
		return;
	}

	if (tokens[0] == "INSERT" && tokens.size() == 4) {
		try {
			int id = std::stoi(tokens[2]);
			if (db_.insert(tokens[1], id, tokens[3])) {
				queue_response("< OK\n");
			}
			else {
				queue_response("ERR duplicate " + tokens[2] + "\n");
			}
		}
		catch (...) {
			queue_response("ERR invalid id\n");
		}
	}
	else if (tokens[0] == "TRUNCATE" && tokens.size() == 2) {
		if (db_.truncate(tokens[1])) {
			queue_response("< OK\n");
		}
		else {
			queue_response("ERR invalid table\n");
		}
	}
	else if (tokens[0] == "INTERSECTION" && tokens.size() == 1) {
		auto result = db_.intersection();
		for (const auto& [id, a, b] : result) {
			queue_response(std::format("{},{},{}\n", id, a, b));
		}
		queue_response("< OK\n");
	}
	else if (tokens[0] == "SYMMETRIC_DIFFERENCE" && tokens.size() == 1) {
		auto result = db_.symmetric_difference();
		for (const auto& [id, a, b] : result) {
			queue_response(std::format("{},{},{}\n", id, a, b));
		}
		queue_response("< OK\n");
	}
	else {
		queue_response("ERR invalid command\n");
	}

	do_read();
}

void Session::queue_response(const std::string& msg)
{
	bool write_in_progress = !response_queue_.empty();
	response_queue_.push(msg);

	if (!write_in_progress) {
		do_write();
	}
}

void Session::do_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_,
		boost::asio::buffer(response_queue_.front()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				response_queue_.pop();
				if (!response_queue_.empty()) {
					do_write();
				}
			}
		});
}