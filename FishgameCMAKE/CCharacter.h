#pragma once
#include "Fish.h"
#include "CTools.h"
#include "CFishing.h"
#include "raylib.h"
#include "CTextureManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

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
	int silver;
	int qtime;
	int skill_points;

	std::deque<std::string> popupQ;

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
	void sellFish(Fish & fish);
	int getGold(Fish & fish);
	void displayQ();
	int getSilver(Fish& fish);
};