#include "filesystem.hpp"

void filesystem::Parser::initReader(std::string path, Filetype mode) {
	if (filestream.is_open()) {
		filestream.close();
	}
	filestream.open(path);

	switch (mode) {
	case JSON:
		try {
			jsonData = json::parse(filestream);
		} catch (json::parse_error& e) {
			std::cerr << "filesystem::Parser::JSON error loading " << path << ": " << LOG_RED << e.what() << LOG_RESET << std::endl;
			break;
		}
		std::cout << "filesystem::Parser loaded " + path + " : " << LOG_GREEN << "GREEN" << LOG_RESET << std::endl;
		break;
	case INI:
		break;
	case TXT:
		break;
	case CPP:
		break;
	case HPP:
		break;
	case OTHER:
		break;
	}
}

filesystem::Parser::Parser(std::string file, Filetype type) {
	initReader(file,type);
}

void filesystem::Parser::printContents() {
	if (filestream.is_open()) {
		std::cout << filestream.rdbuf() << std::endl;
	}
}

std::string filesystem::Parser::getContents() {
	std::string returnStr;
	filestream >> returnStr;
	return returnStr;
}
/*
template <typename T>
std::vector<T> filesystem::Parser::getJsonArrayFromKey(std::vector<std::string> key) {
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
}*/

size_t filesystem::Parser::getJsonArraySize(std::vector<std::string> key) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}

	return arr.size();
}

template <typename T>
T filesystem::Parser::getJsonArrayFromIndex(std::vector<std::string> key, size_t index) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}

	return arr[index];
}

sf::Vector2f filesystem::Parser::getVector2fFromKey(std::vector<std::string> key) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}

	sf::Vector2f returnVector = sf::Vector2f(arr[0], arr[1]); // seperated for debugging
	return returnVector;
}

sf::Color filesystem::Parser::getColorFromKey(std::vector<std::string> key) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}

	return sf::Color(arr[0],arr[1],arr[2],arr[3]);
}

std::vector<sf::Vector2f> filesystem::Parser::getVector2fArrayFromKey(std::vector<std::string> key) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}
	
	json vertex;
	std::vector<sf::Vector2f> returnVector;
	for (auto& el : arr.items())
	{
		vertex = el.value();
		returnVector.push_back(sf::Vector2f(vertex[0],vertex[1]));
	}

	return returnVector;
}

std::vector<sf::Color> filesystem::Parser::getColorArrayFromKey(std::vector<std::string> key) {
	json arr = jsonData;
	for (std::string str : key) {
		arr = arr[str];
	}

	json color;
	std::vector<sf::Color> returnVector;
	for (auto& el : arr.items())
	{
		color = el.value();
		returnVector.push_back(sf::Color(color[0], color[1],color[2],color[3]));
	}

	return returnVector;
}

bool filesystem::Parser::isKeyNull(std::vector<std::string> key) {
	json val = jsonData;
	for (std::string str : key) {
		val = val[str];
	}

	return val.is_null();
}