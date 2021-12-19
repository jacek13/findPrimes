#pragma once

#include<fstream>
#include<string>
#include<vector>

 /**
  * Interfejs dla obiektów fileStstem.
  */
class fileSystem
{
private:
	std::string outputPath;
	std::string fileName;
	std::size_t begin;
	std::size_t end;
	std::string threadInfo;


public:
	fileSystem(const char * _outputPath, size_t _begin, size_t _end, const char * _threadInfo) :
		outputPath(_outputPath), begin(_begin), end(_end), threadInfo(_threadInfo)
	{
		fileName = "Interval_from_";
		fileName += std::to_string(_begin);
		fileName += "_to_";
		fileName += std::to_string(_end);
		fileName += "_task_from_thread_";
		fileName += threadInfo;
	}

	//bool loadTriggers(std::vector<std::string> _args);

	/**
	 * Zapisuje/dopisuje informacje o zakoñczonej rozgrywce do pliku o rozszerzeniu CSV.
	 *
	 * \param[in] _peimes liczby ktore nalezy zapisac do pliku
	 * \param[in] _separator separator dla pliku CSV
	 * \return true je¿eli procedura zapisu siê powiod³a, w przeciwnym wypadku falase
	 */
	bool saveCSV(	std::vector<size_t> & _primes,
					char _separator = ',');
};