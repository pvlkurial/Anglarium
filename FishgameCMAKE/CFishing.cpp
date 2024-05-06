#include "CFishing.h"
#include "raylib.h"
#include <iostream>
#include "CTextureManager.h"

CFishingSpot::CFishingSpot(const size_t w, const size_t h) : m_width(w), m_height(h) {}



void CFishingSpot::addFishingSpot(int posX, int posY, int width, int height) {
    Rectangle r = { posX, posY, width, height };
    fishingSpots.push_back(r);
}


void CFishingSpot::drawFishingSpots(CTextureManager & texman) const {
    for(const auto & item : fishingSpots){
        DrawRectangleLines(item.x, item.y, item.width, item.height, RED);
    }
}

void CFishingSpot::drawObjects(CTextureManager& texman) const {
    for (const auto& item : fishingSpots) {
        DrawTextureEx(texman.getTexture("fishhut"), { item.x-20, item.y-20 }, 0, 2.5, WHITE);
    }
}

CFishingSpot::CFishingSpot() : m_width(0), m_height(0) {}

CFishingSpot::~CFishingSpot() {
}

CShop::CShop() {}

CShop::~CShop() {}

void CShop::addShopSpot(int posX, int posY, int width, int height) {
    Rectangle r = { posX, posY, width, height };
    shopSpots.push_back(r);
}
void CShop::drawShopSpots() const {
    for (const auto& item : shopSpots) {
        DrawRectangleLines(item.x, item.y, item.width, item.height, RAYWHITE);
    }
}