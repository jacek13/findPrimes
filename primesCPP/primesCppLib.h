#pragma once

#ifdef PRIMESCPPLIB_EXPORTS
#define PRIMESCPPLIB_API __declspec(dllexport)
#else
#define PRIMESCPPLIB_API __declspec(dllimport)
#endif // PRIMESCPPLIB_EXPORTS

#include<vector>

extern "C" PRIMESCPPLIB_API bool isPrime(size_t _a);

extern "C" PRIMESCPPLIB_API size_t * findPrimesInArray(size_t _array[], size_t _n, size_t & _numberOfPrimes);

extern "C" PRIMESCPPLIB_API size_t * findPrimesInRange(size_t _a, size_t _b, size_t & _numberOfPrimes);

extern "C++" PRIMESCPPLIB_API std::vector<size_t> findPrimesInArray(size_t _array[], size_t _n);

extern "C++" PRIMESCPPLIB_API std::vector<size_t> findPrimesInRange(size_t _a, size_t _b);