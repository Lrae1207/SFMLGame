#include "filesystem.hpp"

void filesystem::Reader::initReader() {
	if (manualStream.is_open()) {
		manualStream.close();
	}

	switch (targetType) {
	case JSON:
		boost::property_tree::read_json(path, pt);
		break;
	case INI:
		boost::property_tree::read_ini(path, pt);
		break;
	case TXT:
		manualStream.open(path.str());
		break;
	case CPP:
		break;
	case HPP:
		break;
	case OTHER:
		manualStream.open(path.str());
		break;
	}
}

filesystem::Reader::Reader(std::string file, Filetype type) {
	setPath(file);
	targetType = type;
	initReader();
}