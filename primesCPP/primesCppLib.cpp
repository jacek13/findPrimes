#include"pch.h"
#include"primesCppLib.h"

bool isPrime(size_t _a)
{
	if (_a == 2 || _a == 3)
		return true;
	else if (_a <= 1 || !(_a % 2) || !(_a % 3))
		return false;
	else
		for (size_t i = 5; i * i <= _a; i += 6)
			if (!(_a % i) || !(_a % (i + 2)))
				return false;
	return true;
}

size_t* findPrimesInArray(size_t _array[], size_t _n, size_t& _numberOfPrimes)
{
	size_t j = 0;
	size_t numberOfPrimes = 0;
	size_t* tmpArray = new size_t[_n];

	for (size_t i = 0; i < _n; i++)
		if (isPrime(_array[i]))
		{
			tmpArray[j++] = _array[i];
			++numberOfPrimes;
		}
	_numberOfPrimes = numberOfPrimes;

	size_t* rArray = new size_t[numberOfPrimes];

	for (size_t i = 0; i < numberOfPrimes; i++)
		rArray[i] = tmpArray[i];

	delete[]tmpArray;
	return rArray;
}

size_t* findPrimesInRange(size_t _a, size_t _b, size_t& _numberOfPrimes)
{
	size_t j = 0;
	size_t numberOfPrimes = 0;
	size_t* tmpArray = new size_t[_b - _a];

	for (size_t i = _a; i <= _b; i++)
		if (isPrime(i))
		{
			tmpArray[j++] = i;
			++numberOfPrimes;
		}
	_numberOfPrimes = numberOfPrimes;

	size_t* rArray = new size_t[numberOfPrimes];

	for (size_t i = 0; i < numberOfPrimes; i++)
		rArray[i] = tmpArray[i];

	delete[]tmpArray;
	return rArray;
}

std::vector<size_t> findPrimesInArray(size_t _array[], size_t _n)
{
	std::vector<size_t> r;
	for (size_t i = 0; i < _n; i++)
		if (isPrime(_array[i]))
			r.push_back(_array[i]);
	return r;
}

std::vector<size_t> findPrimesInRange(size_t _a, size_t _b)
{
	std::vector<size_t> r;
	for (size_t i = _a; i <= _b; i++)
		if (isPrime(i))
			r.push_back(i);
	return r;
}
