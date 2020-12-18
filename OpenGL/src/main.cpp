#include <iostream>
#include "previewer/Previewer.h"


using namespace Preview;

int main(void)
{
	int resolutionX = 1280, resolutionY = 900;
	Previewer app("Previewer", resolutionX, resolutionY);
	app.Init();
	app.SetMaxFps(60);
	app.MainLoop();

    return 0;
}
