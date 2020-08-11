#include "stdafx.h"
#include "CPubFunc.h"

using namespace SURGICALPLAN;

CPubFunc::CPubFunc()
{
}


CPubFunc::~CPubFunc()
{
}

int CPubFunc::Round(double x)
{
	if (x >= 0.0)
	{
		return (int)(x + 0.5);
	}
	else
	{
		return (int)(x - 0.5);
	}
}

