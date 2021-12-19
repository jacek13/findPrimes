#pragma once

#include<iostream>
#include<exception>

 /**
  * Klasa myException pe³ni rolê interfejsu.
  */
class myException : public std::exception
{
protected:
	std::string message; ///< informacja o zaistnia³ym wyj¹tku
public:
	/**
	 * Konstruktor.
	 *
	 * \param[in] _message treœæ jak¹ obiekt bêdzie przechowywa³
	 */
	myException(const char* _message) :
		message(_message) {}

	/**
	 * Umo¿liwia zwrócenie zawartoœci przechowywanej przez wyj¹tek.
	 *
	 * \return treœæ wyj¹tku
	 */
	virtual const char* what() const noexcept override;
};

/// Klasa wyj¹tku dla obiektu fileSystem
class myException_fileSystem : public myException
{
public:
	myException_fileSystem(const char* _message) :
		myException(_message) {}
	virtual const char* what() const noexcept override;
};