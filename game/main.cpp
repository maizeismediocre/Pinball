#include "stdafx.h"
#include "MyGame.h"

CMyGame game;
CGameApp app;

int main(int argc, char* argv[])
{
	//app.DebugMemoryLeaks();
	app.OpenConsole();
	app.OpenWindow(800, 600, "Pinball");
	app.OpenFullScreen(1920, 1080, 32);
	
	app.SetClearColor(CColor::Black());
	cout << "FPS: " << app.GetFPS() << endl;
	app.Run(&game);
	return(0);
}
