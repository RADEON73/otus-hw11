#include "Database.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

bool Database::insert(const std::string_view& table, int id, const std::string& name)
{
	std::scoped_lock<std::mutex> lock(mutex_);
	if (table == "A") {
		if (tableA_.contains(id))
			return false;
		tableA_[id] = { id, name };
	}
	else if (table == "B") {
		if (tableB_.contains(id))
			return false;
		tableB_[id] = { id, name };
	}
	else {
		return false;
	}
	return true;
}

bool Database::truncate(const std::string_view& table)
{
	std::scoped_lock<std::mutex> lock(mutex_);
	if (table == "A") {
		tableA_.clear();
	}
	else if (table == "B") {
		tableB_.clear();
	}
	else {
		return false;
	}
	return true;
}

std::vector<std::tuple<int, std::string, std::string>> Database::intersection()
{
	std::scoped_lock<std::mutex> lock(mutex_);
	std::vector<std::tuple<int, std::string, std::string>> result;

	for (const auto& [id, recordA] : tableA_) {
		if (tableB_.contains(id)) {
			result.emplace_back(id, recordA.name, tableB_.at(id).name);
		}
	}

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
		return std::get<0>(a) < std::get<0>(b);
		});

	return result;
}

std::vector<std::tuple<int, std::string, std::string>> Database::symmetric_difference()
{
	std::scoped_lock<std::mutex> lock(mutex_);
	std::vector<std::tuple<int, std::string, std::string>> result;

	// Records in A but not in B
	for (const auto& [id, recordA] : tableA_) {
		if (!tableB_.contains(id)) {
			result.emplace_back(id, recordA.name, "");
		}
	}

	// Records in B but not in A
	for (const auto& [id, recordB] : tableB_) {
		if (!tableA_.contains(id)) {
			result.emplace_back(id, "", recordB.name);
		}
	}

	std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
		return std::get<0>(a) < std::get<0>(b);
		});

	return result;
}