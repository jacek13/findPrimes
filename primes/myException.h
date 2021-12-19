#pragma once

#include<iostream>
#include<exception>

 /**
  * Klasa myException pe�ni rol� interfejsu.
  */
class myException : public std::exception
{
protected:
	std::string message; ///< informacja o zaistnia�ym wyj�tku
public:
	/**
	 * Konstruktor.
	 *
	 * \param[in] _message tre�� jak� obiekt b�dzie przechowywa�
	 */
	myException(const char* _message) :
		message(_message) {}

	/**
	 * Umo�liwia zwr�cenie zawarto�ci przechowywanej przez wyj�tek.
	 *
	 * \return tre�� wyj�tku
	 */
	virtual const char* what() const noexcept override;
};

/// Klasa wyj�tku dla obiektu fileSystem
class myException_fileSystem : public myException
{
public:
	myException_fileSystem(const char* _message) :
		myException(_message) {}
	virtual const char* what() const noexcept override;
};