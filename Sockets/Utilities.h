#ifndef UTILITIES_H
#define UTILITIES_H
//////////////////////////////////////////////////////////////////
// Utilities.h      - small, generally useful, helper classes	//
// Version          - 1.1                                       //
// Last Modified    - 05/05/2016                                //
// Language         - Visual C++, Visual Studio 2015            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////
/*
 * INTRODUCTION
 * ------------
 * This package provides classes StringHelper and Converter and a global
 * function putline(). This class will be developed continuously to provide 
 * convenience functions for general C++ applications.
 *
 *
 * REQUIRED FILES
 * --------------
 * N/A
 *
 *
 * CHANGELOG
 * ---------
 * ver 1.0 : 02/08/2016
 * - first release.
 *
 * ver 1.1 : 05/05/2016
 * - Proper commenting of code.
 * - Updated Test Stub to be easier to understand.
 * - Removed functions : title & test stub display helpers.
 *	> From now on use Title instead of title function.
 *	> Test Stub helper functions are now member functions
 *	  (This has no impact on Utilities class)
 *
 */
#include <ctime>
#include <string>
#include <vector>  
#include <cctype>
#include <locale>
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

/// <summary>
/// Namespace Containing Methods which are Commonly used 
/// Throughout whole Solution in Multiple Packages.
/// </summary>
namespace Utilities
{
	/// <summary>
	/// Class Containing Static Functions to Perform operations on Strings.
	/// </summary>
	class StringHelper {
	public:
		static std::vector<std::string> split(const std::string& src);
		static void Title(const std::string& src, char underline = '-');
		static std::string ltrim(std::string &s);
		static std::string rtrim(std::string &s);
		static std::string lrtrim(std::string &s);
		static char * stringToCharArr(std::string str);
	};

	void putline();

	/// <summary>
	/// Class Which Helps in Timing Operations. Useful when benchmarking
	/// methods. 
	/// </summary>
	class Timer {
	public:
		bool StartClock();
		int  StopClock();
	private:
		int time = 0;
	};

	class TimeHelper {
	private:
		static std::string leftpadZero(int val);
	public:
		static long long int getCurrentTimestamp();
		static std::string timestamptoStrimg(long long int ts);
	};

	/// <summary>
	/// Template Class To Convert Objects To and From String.
	/// </summary>
	template <typename T> 
	class Converter {
	public:
		static std::string toString(const T& t);
		static T toValue(const std::string& src);
	};

	/// <summary>
	/// Function to Convert an Object to String.
	/// </summary>
	/// <param name="t">Object to be Converted to String</param>
	/// <returns></returns>
	template <typename T>
	std::string Converter<T>::toString(const T& t)
	{
		std::ostringstream out;
		out << t;
		return out.str();
	}

	/// <summary>
	/// Function to Convert a String to other Data Type.
	/// </summary>
	/// <param name="src">String which has to be castead to Specified Data Type</param>
	/// <returns>Object with Specified Data Type Converted From String</returns>
	template<typename T> 
	T Converter<T>::toValue(const std::string& src)
	{
		std::istringstream in(src);
		T t;
		in >> t;
		return t;
	}
}
#endif