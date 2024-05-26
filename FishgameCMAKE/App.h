#pragma once
#include "CMap.h"
#include "CCharacter.h"
#include "CFishing.h"
#include "CTextureManager.h"
#include <iostream>
#include <filesystem>

class App {
	CMap map;
	CCharacter player;
	CFishingSpot fishSpot;
	CTextureManager textures;
	Camera2D camera;
	CShop shops;
	CSoundManager soundsMan;
	typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, SHOP, INV, PAUSE, ENDING} GameScreen;
	typedef enum ActiveMenu { NONE = 0, INVENTORY } ActiveMenu;
	typedef enum ShopMenu { NO = 0 , SELL, BUY, TALK} ShopMenu;

	GameScreen currentScreen;
	ActiveMenu currentMenu;
	ShopMenu currentShopMenu;

	const float screenHeight = 1080 / 2;
	const float screenWidth = 1920 / 2;

	std::filesystem::path assetPath = "./resources/";

	std::string appDir = GetApplicationDirectory();
public:

	App();
	~App();

	void Init();


	void RunGame();

	void OnRender();

	void unloadTex();

};