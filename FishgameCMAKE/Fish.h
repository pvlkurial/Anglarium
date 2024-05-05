#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <iostream>
#include "CTextureManager.h"

class Fish {
public:
    Fish(int rodLuck, int playerLuck, int playerSkill, int playerLevel, int rodStrength, CTextureManager& texman);
    int getRarity(int rodLuck, int playerLuck, int playerSkill) const;

    std::string m_name;
    std::string prefix = "prefix";
    std::string m_description = "fishes fish";
    Texture2D m_tex;
    int rarity;
    double weight;


};