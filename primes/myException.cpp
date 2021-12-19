#include<iostream>

#include"myException.h"

const char* myException::what() const noexcept
{
	return message.empty() ? "Unknown ERROR!" : message.c_str();
}

const char* myException_fileSystem::what() const noexcept
{
	return message.empty() ? "Unknown fileSystem ERROR!" : message.c_str();
}