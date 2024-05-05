#pragma once
#include "CTextureManager.h"
#include <iostream>

class CMap {
    const size_t horTiles = 32;
    const size_t vertTiles = 32;
    const size_t tileSize = 32;

    CTextureManager m_tm;
    std::string mapText;

public:
    CMap();
    CMap(const CTextureManager& tm, const std::string& fileName);
    CMap(const CMap& src);

    bool drawMap();

    CMap & operator = (const CMap& src);
};