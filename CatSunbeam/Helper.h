#ifndef HELPER_H
#define HELPER_H

#include "Includes.h"

class Helper
{
public:
	Helper();
	string toString(float);
	void IncreaseTimer();
	long GetTime();
private:
		unsigned long timer;
		unsigned long lastCount;
};

#endif