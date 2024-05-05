#include "CMap.h"
#include "iostream"
#include "raylib.h"
#include <sstream>
#include "CTextureManager.h"

bool CMap::drawMap() {

    std::istringstream is(mapText);
    for (size_t i = 0; i < vertTiles; i++) {
        for (size_t j = 0; j < horTiles; j++) {
            std::string parsedString;
            is >> parsedString;
            DrawTexture(m_tm.getTexture(parsedString), j * tileSize, i * tileSize, WHITE);
        }
    }
    return true;
}

CMap::CMap(const CTextureManager& tm, const std::string& fileName) : m_tm(tm) {
        mapText = LoadFileText(fileName.c_str());
}

CMap::CMap() = default;

CMap::CMap(const CMap& src) {
    *this = src;
}

CMap & CMap::operator = (const CMap& src) {
    this->m_tm = src.m_tm;
    mapText = src.mapText;
    return *this; 
}