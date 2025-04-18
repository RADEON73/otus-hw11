#pragma once
#include <unordered_map>
#include <mutex>

/**
 * @brief Структура записи таблицы.
 */
struct Record
{
	int id = 0;                ///< Уникальный идентификатор записи.
	std::string name;          ///< Строковое имя записи.
};

/**
 * @brief Класс базы данных, содержащей две таблицы и операции над ними.
 */
class Database
{
private:
	std::unordered_map<int, Record> tableA_; ///< Таблица A.
	std::unordered_map<int, Record> tableB_; ///< Таблица B.
	std::mutex mutex_; ///< Мьютекс для обеспечения потокобезопасности.

public:
	/**
	 * @brief Вставляет запись в таблицу A или B.
	 * @param table Название таблицы ("A" или "B").
	 * @param id Уникальный идентификатор записи.
	 * @param name Имя записи.
	 * @return true, если вставка успешна; false, если дублирующий id или ошибка таблицы.
	 */
	bool insert(const std::string_view& table, int id, const std::string& name);

	/**
	 * @brief Очищает указанную таблицу.
	 * @param table Название таблицы ("A" или "B").
	 * @return true, если очистка успешна; false, если таблица некорректна.
	 */
	bool truncate(const std::string_view& table);

	/**
	 * @brief Выполняет пересечение таблиц A и B.
	 * @return Вектор кортежей с id, name из A и name из B.
	 */
	std::vector<std::tuple<int, std::string, std::string>> intersection();

	/**
	 * @brief Выполняет симметрическую разность таблиц A и B.
	 * @return Вектор кортежей с id, name из A (если есть) и name из B (если есть).
	 */
	std::vector<std::tuple<int, std::string, std::string>> symmetric_difference();
};