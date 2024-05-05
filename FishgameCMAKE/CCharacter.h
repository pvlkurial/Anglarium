#pragma once
#include "Fish.h"
#include "CTools.h"
#include "CFishing.h"
#include "raylib.h"
#include <iostream>
#include <vector>
#include "CTextureManager.h"

class CCharacter {
	Texture2D char_tex;

	Rectangle m_colRec;

public:
	std::vector<Fish> fishInventory;
	double luck;
	double skill;
	long xp;
	int level;
	CFishingRod rod;
	long gold;


	int m_posX;
	int m_posY;
	CCharacter(int posX, int posY);
	CCharacter();
	~CCharacter();
	void loadTexture(const Texture2D& tex);
	void fish(CFishingSpot& fishSpot, Texture2D& pop_tex, CTextureManager& texman);
	void kbListen(CFishingSpot& fishSpot, Texture2D& pop_tex, CShop& shops, CSoundManager & soundMan, Camera2D & camera, CTextureManager& texman);
	void charUpdate();
	bool checkCollision(CFishingSpot& fishSpot, Texture2D& pop_tex) const;
	bool checkShopCollision(CShop& shops) const;
	void update();
};