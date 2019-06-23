#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\ini_parser.hpp>
#include "boost/optional/optional_fwd.hpp"
#include <vector>
#include <experimental/filesystem>
#include "settings.h"



namespace pt = boost::property_tree;

void Settings::load(const std::string &filename) {
	pt::ptree tree;
	pt::read_ini(filename, tree);

	width = std::stoi(tree.get_value("board.width"));
	height = std::stoi(tree.get_value("board.height"));
	startingPosition = tree.get_value("board.position");
}

void Settings::save(const std::string &filename) {
	pt::ptree tree;

	tree.put("board.width", width);
	tree.put("board.height", height);
	tree.put("board.position", startingPosition);

	pt::write_ini(filename, tree);
}

inline bool Settings::isFileExist(const std::string &filename) {
	return std::experimental::filesystem::exists("settings.ini");
}