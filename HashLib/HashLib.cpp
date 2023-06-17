#include <string>
#include <unordered_map>
#include <iostream>

extern "C" __declspec(dllexport) std::string hashing(std::string passw) {
	std::hash<std::string> hasher;
	std::string hash = std::to_string(hasher(passw));

	return hash;
}