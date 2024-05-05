#pragma once


class CFishingRod {
public:

	int luck; // increases all parameters
	int strenght; // increases the max weight a fish can be caught

	CFishingRod();

	~CFishingRod();

	enum ToolRarity { Common, Uncommon, Rare };
};