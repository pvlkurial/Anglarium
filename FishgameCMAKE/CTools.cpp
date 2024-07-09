#include "CTools.h"

CFishingRod::CFishingRod(const std::string& name, int luck, int strength, int level_needed, int price, const Texture2D& tex)
	: name(name), luck(luck), strenght(strenght), level_needed(level_needed), price(price), m_tex(tex) {
	selected = false;
}

CFishingRod::CFishingRod() : name("Basic Rod"), luck(1), strenght(1), level_needed(0), price(0), m_tex() {
	selected = false;
}

CFishingRod::~CFishingRod() = default;

bool CFishingRod::operator<(const CFishingRod& a) const {
	return name < a.name;
}
bool CFishingRod::operator==(const CFishingRod& a) const {
	return name == a.name;
}

