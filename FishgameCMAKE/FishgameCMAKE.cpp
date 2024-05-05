// FishgameCMAKE.cpp: Definuje vstupní bod pro aplikaci.
//

#include "FishgameCMAKE.h"
#include "raylib.h"
#include "App.h"
#include <iostream>

using namespace std;

int main()
{
	App app;
	app.RunGame();
	app.Init();
	while (!WindowShouldClose()) {
		
		app.OnRender();

	}
	app.unloadTex();
	CloseWindow();
	return 0;
}
