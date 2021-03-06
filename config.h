/**
  * @file config.h
  * @brief 配置类, 操作配置文件
  * @author Jack.wen hengfengyaoren@gmail.com
  * @version 0.0.1
  * @date 2021/11/01
  */

#pragma once

#include "exception.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace config {

class CConfig {
	// Method
public:
	CConfig(void);
	CConfig(const std::string &filename, const std::string &delimiter = "=", const std::string &comment = "#");

	bool init(const std::string &filename, const std::string &delimiter = "=", const std::string &comment = "#");
	// 兼容TT服务器的接口
	void loadfile(const std::string &filename, const std::string &delimiter = "=", const std::string &comment = "#") {
			init(filename, delimiter, comment);
		}

	void readto(const char*, std::vector<std::string>& );

	template<typename T> T read(const std::string &in_key) const;	//<! Searchfor key and read value or optional default value, call as read<T> 

	template<typename T> T read(const std::string &in_key, const T &in_value) const;
	// 兼容TT服务器的接口
	template<typename T> T getValue(const std::string &in_key) const;	//<! Searchfor key and read value or optional default value, call as read<T> 
	template<typename T> void getValue(const std::string &key, T &value) const;

	template<typename T> bool readInto(T &out_var, const std::string &in_key) const;
	template<typename T> bool readInto(T &out_var, const std::string &in_key, const T &in_value) const;
	bool fileExist(std::string &filename);
	void readFile(std::string &filename, std::string delimiter="=", std::string comment = "#");

	// Check whether key exists in configuration
	bool keyExists(const std::string &in_key) const;

	// Modify keys and values
	template<class T> void add(const std::string &in_key, const T& in_value);
	void remove(const std::string &in_key);

	// Check or change configuration syntax
	std::string getDelimiter(void) const { return m_Delimiter; }
	std::string getComment(void) const { return m_Comment; }
	std::string setDelimiter(const std::string &in_s) { 
			std::string old = m_Delimiter; m_Delimiter = in_s; return old;
		}

	std::string setComment(const std::string &in_s) {
			std::string old = m_Comment; m_Comment = in_s; return old;
		}

	// Write or read configuration 
	friend std::ostream& operator<<(std::ostream& os, const CConfig& cf);
	friend std::istream& operator>>(std::istream& is, CConfig& cf);
	// Data
protected:
	std::string m_Delimiter;	//<! separator between key and value 
	std::string m_Comment;		//<! separator between value and comments
	std::map<std::string, std::string> m_Contents;	//!<extracted keys and values 

	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapci;

	template<typename T> static std::string T_as_string(const T &t);
	template<typename T> static T string_as_T(const std::string &s);
	static void Trim(std::string &inout_s);
	static std::string& replace_all_distinct(std::string& str, const std::string& old_value,const std::string& new_value);
	static void ignoreComment(std::string &str, const std::string &comm, bool isfilter);
};  // class CConfig

/* Static */
template<typename T>
std::string CConfig::T_as_string(const T &t) {
	// Conver from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}

/* Static */
template<typename T>
T CConfig::string_as_T(const std::string &s) {
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}

/* Static */
template<>
inline std::string CConfig::string_as_T<std::string>(const std::string &s) {
	// Conver from a T to a string
	// In other words, do nothing
	return s;
}

/* Static */
template<> 
inline bool CConfig::string_as_T<bool>(const std::string& s) {
	// Convert from a string to a bool
	// Interpret "false", "F", "no", "n", "0" as false 
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;
	std::string sup = s;
	for (std::string::iterator p = sup.begin(); p != sup.end(); ++p) {
			*p = toupper(*p); // make string all caps
		}
	if (sup == std::string("FALSE") || sup == std::string("F")
					|| sup == std::string("NO") || sup == std::string("N")
								|| sup == std::string("0") || sup == std::string("NONE")) {
			b = false;
		}
	return b;
}

template<typename T>
T CConfig::read(const std::string& key) const {
	// Read the value corresponding to key
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) throw ::rayman::Exception("Key_not_found: " + key);
	return string_as_T<T>( p->second);
}

template<typename T>
T CConfig::read(const std::string &key, const T &value) const {
	// Return the value corresponding to key or given default value
	// if key is not found 
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) {
			printf("CConfig::%s, Not found, key is '%s'.\n", __func__, key.c_str());
			return value;
		}
	return string_as_T<T>( p->second);
}

template<typename T>
T CConfig::getValue(const std::string& key) const {
	// Read the value corresponding to key
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) throw ::rayman::Exception("Key_not_found: " + key);
	return string_as_T<T>( p->second);
}

template<typename T>
void CConfig::getValue(const std::string &key, T &value) const {
	// Return the value corresponding to key or given default value
	// if key is not found 
	mapci p = m_Contents.find(key);
	if (p == m_Contents.end()) {
			printf("CConfig::%s, Not found, key is '%s'.\n", __func__, key.c_str());
			return;
		}
	value = string_as_T<T>(p->second);
}

template<typename T>
bool CConfig::readInto(T &var, const std::string& key) const {
	// Get the value corresponding to key and store in var  
	// Return true if key is found  
	// Otherwise leave var untouched  
	mapci p = m_Contents.find(key);
	bool found = (p != m_Contents.end());
	if (found) {
			var = string_as_T<T>( p->second );
		}
	return found;
}

template<typename T> 
bool CConfig::readInto(T &var, const std::string &key, const T &value) const {
	// Get the value corresponding to key and store in var 
	// Return true if key is found
	// Otherwise set var to given default
	mapci p = m_Contents.find(key);
	bool found = (p != m_Contents.end());
	if (found) {
			var = string_as_T<T>( p->second );
		} else {
				var = value;
			}
	return found;
}

template<typename T>
void CConfig::add(const std::string &in_key, const T &value) {
	// Add a key with given value  
	std::string v = T_as_string(value);
	std::string key = in_key;
	Trim(key);
	Trim(v);
	m_Contents[key] = v;
	return;
}

  // namespace config
}

