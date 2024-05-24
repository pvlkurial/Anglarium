#pragma once
#include "raylib.h"
#include <vector>
#include "CTextureManager.h"

class CFishingSpot {
public:
    size_t m_width;
    size_t m_height;


    CFishingSpot();

    ~CFishingSpot();

    CFishingSpot(const size_t w, const size_t h);

    std::vector<Rectangle> fishingSpots;

    void addFishingSpot(int posX, int posY, int width, int height);

    void drawFishingSpots(CTextureManager& texman) const;

    void drawObjects(CTextureManager& texman) const;
};

class CShop {
public:
    CShop();

    ~CShop();

    std::vector<Rectangle> shopSpots;
    void addShopSpot(int posX, int posY, int width, int height);
    void drawShopSpots() const;
    void drawShopObjects(CTextureManager & texman) const;

};