#pragma once

#ifndef ENG_FILESYSTEM
#define ENG_FILESYSTEM

#include <string> // Strings
#include <sstream> // Path strings for parsetree
#include <fstream> // Manual file reading
#include <boost/property_tree/ptree.hpp> // Parsing
#include <boost/property_tree/ini_parser.hpp> // For INI file reading
#include <boost/property_tree/json_parser.hpp> // For JSON file reading

namespace filesystem {
	enum Filetype {
		JSON,
		INI,
		TXT,
		CPP,
		HPP,
		OTHER
	};

	class Reader {
	private:
		boost::property_tree::ptree pt;
		std::stringstream path;
		Filetype targetType = OTHER;
		std::fstream manualStream;

		void initReader();
	public:
		Reader(std::string file, Filetype type);

		void setPath(std::string newPath) { path = std::stringstream(newPath); initReader(); }
		std::string getPath() { return path.str(); initReader(); }
	};

	// https://stackoverflow.com/questions/15206705/reading-json-file-with-boost
	class Parser : Reader {
	private:

	public:
	};
}

#endif