#pragma once
#include <vector>

struct Settings {
	size_t width;
	size_t height;
	std::string startingPosition;
	void load(const std::string &filename);
	void save(const std::string &filename);
	inline bool isFileExist(const std::string& filename);
};