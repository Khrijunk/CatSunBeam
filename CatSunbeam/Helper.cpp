#include "Helper.h"

//useful functions
string Helper::toString(float num)
{
	stringstream sstream;
	sstream.str("");
	sstream << num;
	return sstream.str();
}