#include<iostream>
#include<fstream>
#include<iterator>
#include<algorithm>
#include<vector>
#include<string>
#include<utility>

#include"fileSystem.h"
#include"myException.h"

bool fileSystem::saveCSV(std::vector<size_t>& _primes, char _separator)
{
	std::string tmpPath(fileSystem::outputPath);
	tmpPath += fileSystem::fileName;
	tmpPath += ".csv";

	bool success = true;
	bool notExist = false;

	// W celu sprawdzenia istnienia danego pliku tworze strumien wejsciowy dla plikow
	std::ifstream check(tmpPath.c_str());

	if (!check)
		notExist = true;
	check.close();

	std::fstream file(tmpPath.c_str(), std::ios::out /*| std::ios::app*/);

	if (file.good())
	{
		//if (notExist)
			file << "Index" << _separator << "Number" << std::endl;
		for(size_t i = 0; i < _primes.size(); i++)
			file << i + 1 << _separator << _primes.at(i) << std::endl;
	}
	else
	{
		success = false;
		std::string error("Cannot open file from path: " + tmpPath);
		throw myException_fileSystem(error.c_str());
	}
	file.close();

	return success;
}