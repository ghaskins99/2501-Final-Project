#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

class FileUtils {
public:
	static std::string LoadTextFile(const char *filename);
};