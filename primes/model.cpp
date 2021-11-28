#include "model.h"

Model::Model()
{
    maximumNumberOfThreads = std::thread::hardware_concurrency();
}

std::vector<size_t> Model::findPrimesInRangeCallASM(size_t _a, size_t _b)
{
    std::vector<size_t> r;
    if (this->_x64)
    {
        for (size_t i = _a; i <= _b; i += 2)
            if (primeTestASM64(i, i + 1) == 1)
                r.push_back(i);
            else if (primeTestASM64(i, i + 1) == 2)
                r.push_back(i + 1);
            else if (primeTestASM64(i, i + 1) == 3)
            {
                r.push_back(i);
                r.push_back(i + 1);
            }
    }
    else
    {
        for (size_t i = _a; i <= _b; i++)
            if (isPrimeASM86(i))
                r.push_back(i);
    }
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
    // Problem jest z dostêpem do jednego vectora przez kilka w¹tków naraz, TODO spróbuj zapisywania do pliku
    //std::vector<std::vector<size_t>>tmp; // jeœli bêdzie siê wywala³o to mo¿na jeszcze spróbowaæ mutexa
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

void Model::setASMType(bool _type)
{
    this->_x64 = _type;
}

size_t Model::getMaximumNumberOfThreads()
{
    return maximumNumberOfThreads;
}

bool Model::limitTest(size_t _A, size_t _B)
{
    return _A < _B ? true : false;
}