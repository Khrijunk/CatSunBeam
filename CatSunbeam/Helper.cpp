#include "Helper.h"

Helper::Helper()
{
	timer = 0;
	lastCount = GetTickCount();
}

//useful functions
string Helper::toString(float num)
{
	stringstream sstream;
	sstream.str("");
	sstream << num;
	return sstream.str();
}

void Helper::IncreaseTimer()
{
	if(lastCount != GetTickCount())
	{
		lastCount = GetTickCount();
		timer++;		
	}
}

long Helper::GetTime()
{
	return timer;
}