#pragma once

#ifndef ENG_FILESYSTEM
#define ENG_FILESYSTEM

#include "MACROS.hpp" // Log color
#include <string> // Strings
#include <sstream> // Path strings for parsetree
#include <fstream> // Manual file reading
#include <iostream> // std::cout
#include <vector> // for arrays
#include <nlohmann/json.hpp> // run vs as admin
#include <SFML/Graphics.hpp>

using json = nlohmann::json;

namespace filesystem {
	enum Filetype {
		JSON,
		INI,
		TXT,
		CPP,
		HPP,
		OTHER
	};

	class Parser {
	private:
		std::fstream filestream;
		json jsonData;
	public:
		Parser() {};
		Parser(std::string file, Filetype type);

		void initReader(std::string path, Filetype mode);

		void printContents();
		std::string getContents();

		/* JSON parsing functions */
		template <typename T>
		T getJsonFromKey(std::vector<std::string> key) {
			json data = jsonData;
			for (std::string str : key) {
				data = data[str];
			}
			return data;
		}

		template <typename T>
		std::vector<T> getJsonArrayFromKey(std::vector<std::string> key) {
			json arr = jsonData;
			for (std::string str : key) {
				arr = arr[str];
			}

			std::vector<T> returnVector = {};
			for (auto& el : arr.items())
			{
				returnVector.push_back(el.value());
			}
			return returnVector;
		}

		size_t getJsonArraySize(std::vector<std::string> key);

		template <typename T>
		T getJsonArrayFromIndex(std::vector<std::string> key, size_t index);

		sf::Vector2f getVector2fFromKey(std::vector<std::string> key);
		sf::Color getColorFromKey(std::vector<std::string> key);

		std::vector<sf::Vector2f> getVector2fArrayFromKey(std::vector<std::string> key);
		std::vector<sf::Color> getColorArrayFromKey(std::vector<std::string> key);

		bool isKeyNull(std::vector<std::string> key);
	};
}

#endif