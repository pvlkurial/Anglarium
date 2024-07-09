#pragma once
#include "raylib.h"
#include <string>

class CFishingRod {
public:
	std::string name;
	int luck; // increases all parameters
	int strenght; // increases the max weight a fish can be caught
	int level_needed;
	int price;
	Texture2D m_tex;
	bool selected;

	CFishingRod(const std::string & name, int luck, int strength, int level_needed, int price, const Texture2D & tex);
	CFishingRod();

	~CFishingRod();

	bool operator<(const CFishingRod& a) const;
	bool operator==(const CFishingRod& a) const;
	enum ToolRarity { Common, Uncommon, Rare };
};