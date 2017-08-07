//////////////////////////////////////////////////////////////
// Utilities.h      - small, generally useful, helper       //
//                    classes and static functions.         //
// Version          - 1.2                                   //
// Last Modified    - 08/05/2017                            //
// Language         - Visual C++, Visual Studio 2015        //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10     //
// Author           - Venkata Bharani Krishna Chekuri       //
// e-mail           - bharanikrishna7@gmail.com             //
//////////////////////////////////////////////////////////////

#include "Utilities.h"

using namespace Utilities;

/////////////////////////////////////////////////////////////////////////
/* Display Title with a character underline (default character is '_') */
void StringHelper::Title(const std::string& src, char underline)
{
	std::cout << "\n " << src;
	std::cout << "\n " << std::string(src.size() + 1, underline);
}

////////////////////////////
/* Function for Left Trim */
std::string StringHelper::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

/////////////////////////////
/* Function for Right Trim */
std::string StringHelper::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

//////////////////////////////////
/* Function for Left Right Trim */
std::string StringHelper::lrtrim(std::string &s) {
	return ltrim(rtrim(s));
}

////////////////////////////////////////////////////////////////////
/* Split a string at ',' and return these sub-strings as a vector */
std::vector<std::string> StringHelper::split(const std::string& src)
{
	std::vector<std::string> accum;
	std::string temp;
	size_t index = 0;
	do
	{
		while ((isspace(src[index]) || src[index] == ',') && src[index] != '\n')
		{
			++index;
			if (temp.size() > 0)
			{
				accum.push_back(temp);
				temp.clear();
			}
		}
		if (src[index] != '\0')
			temp += src[index];
	} while (index++ < src.size());
	if (temp.size() > 0)
		accum.push_back(temp);
	return accum;
}

//////////////////////////////////////////////
/* Print newline to console (pointless TBH) */
void Utilities::putline() {
	std::cout << "\n";
}

/////////////////////
/* Start the timer */
bool Timer::StartClock() {
	time = clock();
	return true;
}

//////////////////////////////////////////////////
/* Stop Timer and Return the time elapsed in ms */
int Timer::StopClock() {
	time = clock() - time;
	return (int)(time / double(CLOCKS_PER_SEC) * 1000);
}

//////////////////////////////////////////////////////////////////
/* Function to left pad zero when the parameter is less than 10 */
std::string TimeHelper::leftpadZero(int val) {
	if (val / 10 == 0)
		return "0" + std::to_string(val);
	return std::to_string(val);
}

#pragma warning(push)
#pragma warning(disable : 4996)
////////////////////////////////////////////////////////////////
/* Function to get current timestamp in yyyyMMddhhmmss format */
long long int TimeHelper::getCurrentTimestamp() {
	time_t t = time(0);				// get current time
	struct tm * now = now = localtime(&t);
	long long int ts = 1;
	ts = (now->tm_year + 1900);
	ts = ts * 100 + now->tm_mon + 1;
	ts = ts * 100 + now->tm_mday;
	ts = ts * 100 + now->tm_hour;
	ts = ts * 100 + now->tm_min;
	ts = ts * 100 + now->tm_sec;
	return ts;
}
#pragma warning(pop)

///////////////////////////////////////////////////////////////////////////////
/* Function to convet long long int timestamp to user readable string format */
std::string TimeHelper::timestamptoStrimg(long long int ts) {
	if (ts / 10000000000000 == 0)
		return "Invalid Timestamp";
	std::string ss = leftpadZero(ts % 100);
	ts = ts / 100;
	std::string mm = leftpadZero(ts % 100);
	ts = ts / 100; 
	std::string hh = leftpadZero(ts % 100);
	ts = ts / 100; 
	std::string dd = leftpadZero(ts % 100);
	ts = ts / 100; 
	std::string MM = leftpadZero(ts % 100);
	ts = ts / 100;
	std::string yy = std::to_string(ts);
	return std::string(yy + "/" + MM + "/" + dd + " " + hh + ":" + mm + ":" + ss);
}

#pragma warning(push)
#pragma warning(disable : 4996)
char * StringHelper::stringToCharArr(std::string str) {
	return &str[0u];
}
#pragma warning(pop)

/* Test Stub */
#ifdef TEST_UTILITIES
/////////////////////////
/* Test Trim Functions */
void test_trim() {
	StringHelper::Title("Test Trim Operations", '-');
	// The trim operation is returns reference so the object passed to it is modified
	// for testing purpose we are again ang again assigning values to the same string (trim)
	std::cout << "\n ^ is begining of string";
	std::cout << "\n $ is end of string\n";
	std::string trim = "\t\t This is the \ttrim test string\t\t";

	std::cout << "\n Original String is\t : ^" << trim << "$";
	std::cout << "\n Left Trim	\t : ^" << StringHelper::ltrim(trim) << "$";

	trim = "\t\t This is the \toriginal string\t\t";
	std::cout << "\n Right Trim\t\t : ^" << StringHelper::rtrim(trim) << "$";

	trim = "\t\t This is the \toriginal string\t\t";
	std::cout << "\n LR Trim\t\t : ^" << StringHelper::lrtrim(trim) << "$";

	std::cout << "\n\n ";
}

//////////////////////////////
/* Test Converter Functions */
void test_converter() {
	StringHelper::Title("Test std::string Converter<T>::toString(T)", '-');

	std::string conv1 = Converter<double>::toString(3.1415927);
	std::string conv2 = Converter<int>::toString(73);
	std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

	std::cout << "\n Converting from values to strings: ";
	std::cout << conv1 << ", " << conv2 << ", " << conv3;
	putline();

	StringHelper::Title("Test T Converter<T>::toValue(std::string)", '-');

	std::cout << "\n Converting from strings to values: ";
	std::cout << Converter<double>::toValue(conv1) << ", ";
	std::cout << Converter<int>::toValue(conv2) << ", ";
	std::cout << Converter<std::string>::toValue(conv3);

	std::cout << "\n\n ";
}

/////////////////////////
/* Test Split Function */
void test_split() {
	StringHelper::Title("Test StringHelper::split(std::string)", '-');

	std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
	std::cout << "\n  test string = " << test;

	std::vector<std::string> result = StringHelper::split(test);

	std::cout << "\n";
	for (auto item : result)
	{
		if (item == "\n")
			std::cout << "\n " << "newline";
		else
			std::cout << "\n " << item;
	}

	std::cout << "\n\n ";
}

//////////////////////////////
/* Test Timestamp Functions */
void test_timestamp() {
	StringHelper::Title("Test getCurrentTimestamp", '-');
	std::cout << "\n Current Timestamp in yyyyMMddhhmmss fomat : " + std::to_string(TimeHelper::getCurrentTimestamp());
	std::cout << "\n Current Timestamp in user readable string : " + TimeHelper::timestamptoStrimg(TimeHelper::getCurrentTimestamp());
	std::cout << "\n\n ";
}

////////////////////////
/* Main Test Function */
int main()
{
	Timer time;
	time.StartClock();

	StringHelper::Title("Testing Utilities Package", '=');
	putline();

	// Test string split
	test_split();
	// Test string converter
	test_converter();
	// Test string trim
	test_trim();
	// Test timestamp
	test_timestamp();

	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	return 0;
}
#endif