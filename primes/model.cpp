#include "model.h"

Model::Model()
{
    maximumNumberOfThreads = std::thread::hardware_concurrency();
}

std::vector<size_t> Model::findPrimesInRangeCallASM(size_t _a, size_t _b)
{
    std::vector<size_t> r;
    for (size_t i = _a; i <= _b; i++)
        if (isPrimeASM(i))
            r.push_back(i);
    return r;
}

void Model::findPrimeNumbersParallel(   size_t _a,
                                        size_t _b,
                                        size_t numberOfThreads,
                                        std::string _s)
{
    if (numberOfThreads > maximumNumberOfThreads)
        numberOfThreads = maximumNumberOfThreads;
    //std::vector<size_t>r;
    // Problem jest z dost�pem do jednego vectora przez kilka w�tk�w naraz, TODO spr�buj zapisywania do pliku
    //std::vector<std::vector<size_t>>tmp; // je�li b�dzie si� wywala�o to mo�na jeszcze spr�bowa� mutexa
    if (_s == "C++")
    {
        //for (size_t i = 0; i < numberOfThreads; i++)
        //    tmp.push_back(std::vector<size_t>());
        for (size_t i = 0; i < numberOfThreads; i++)
            threads.push_back(std::thread([&]() {
            /*tmp.at(i) = */findPrimesInRange(_a + i * (_b - _a) / numberOfThreads,
                _a + (i + 1) * (_b - _a) / numberOfThreads);
            //r.insert(r.end(), tmp.at(i).begin(), tmp.at(i).end());
        }));
        for (auto& t : threads)
            t.join();
    }
    else if (_s == "ASM")
    {
        //for (size_t i = 0; i < numberOfThreads; i++)
        //    tmp.push_back(std::vector<size_t>());
        for (size_t i = 0; i < numberOfThreads; i++)
            threads.push_back(std::thread([&]() {
            /*tmp.at(i) = */findPrimesInRangeCallASM(_a + i * (_b - _a) / numberOfThreads,
                _a + (i + 1) * (_b - _a) / numberOfThreads);
            //r.insert(r.end(), tmp.at(i).begin(), tmp.at(i).end());
        }));
        for (auto& t : threads)
            t.join();
    }
    else
        throw std::exception("Unknown library!");

    for (size_t i = 0; i < numberOfThreads; i++)
        threads.pop_back();

    //return r;
}

size_t Model::getMaximumNumberOfThreads()
{
    return maximumNumberOfThreads;
}

bool Model::limitTest(size_t _A, size_t _B)
{
    return _A < _B ? true : false;
}