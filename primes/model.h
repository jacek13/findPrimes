#pragma once
#include <string>
#include <vector>
#include <thread>
#include <exception>

#include "primesCppLib.h"
#include "../primesAsmPrim/primesASM.h"

class Model
{
private:
    size_t maximumNumberOfThreads;
    std::vector<std::thread>threads;
    bool _x64;

    std::vector<size_t> findPrimesInRangeCallASM(size_t _a, size_t _b);
public:
	Model();
    void findPrimeNumbersParallel(  size_t _a,
                                    size_t _b,
                                    size_t numberOfThreads = std::thread::hardware_concurrency(),
                                    std::string _s = "C++");

    bool limitTest(size_t _A, size_t _B);
// settery
    void setASMType(bool _type);
// gettery
    size_t getMaximumNumberOfThreads();
};