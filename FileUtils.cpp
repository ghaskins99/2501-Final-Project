#include "FileUtils.h"

std::string FileUtils::LoadTextFile(const char *filename) {
	// Open file
	std::ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
	}

	// Read file
	std::string content;
	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	// Close file
	f.close();

	return content;
}
