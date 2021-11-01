/**
  * @file exception.h
  * @brief 异常类
  * @author Jack.wen hengfengyaoren@gmail.com
  * @version 0.0.1
  * @date 2021/11/1
  */
#pragma once

#include <exception>
#include <string>

namespace rayman {

class Exception : public std::exception {

public:
	explicit Exception(const char* what);
	explicit Exception(const std::string& what);
	virtual ~Exception() throw();
	virtual const char* what() const throw();
	const char* stackTrace() const throw();

private:
	void fillStackTrace();

	std::string message_;
	std::string stack_;
};

}

