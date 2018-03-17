#pragma once

#include "tealTek.h"
#include "header.h"

int main(int argc, char *argv[])
{
	tealTek tealApp;

	if (tealApp.init("map1.ttm"))
	{
		tealApp.run();
	}
	else
	{
		return false;
	}

	tealApp.shutdown();

	return true;
}
