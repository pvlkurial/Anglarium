#include "Fish.h"
#include "raylib.h"
#include "raymath.h"
#include "CCharacter.h"
#include <string>
#include <iostream>
#include <vector>
    
    int Fish::getRarity(int rodLuck, int playerLuck, int playerSkill) const {
        float temp = GetRandomValue(0, 50000/(rodLuck * ((playerLuck + playerSkill) * 0.2) * 1.000));
        if (temp < 20000) {
            if (temp < 5000) {
                if (temp < 2500) {
                    if (temp < 1000) {
                        if (temp < 500) {
                            if (temp < 100) {
                                return 6;
                            }
                            return 5;
                        }
                        return 4;
                    }
                    return 3;
                }
                return 2;
            }
            return 1;
        }
        return 0;
    }

    Fish::Fish(int rodLuck, int playerLuck, int playerSkill, int playerLevel, int rodStrength, CTextureManager & texman)  {
        weight = (GetRandomValue(500, 5000) / 1000 * rodStrength * (playerLevel / 5.000) + 0.673*playerSkill*playerLuck) / 10.000;
        rarity = getRarity(rodLuck, playerLuck, playerSkill);
        selected = false;
        switch (GetRandomValue(0, 5)) {
        case 0:
            m_name = "Joel";
            m_tex = texman.getTexture("cod");
        break;
        case 1:
            m_name = "Damien";
            m_tex = texman.getTexture("bluefish");

        break;
        case 2:
            m_name = "Puff";
            m_tex = texman.getTexture("pufferfish");

        break;
        case 3:
            m_name = "Rodger";
            m_tex = texman.getTexture("clownfish");

        break;
        case 4:
            m_name = "Bingus";
            m_tex = texman.getTexture("cod");

        break;
        case 5:
            m_name = "Ben";
            m_tex = texman.getTexture("cod");

        break;
        }

    }

    bool Fish::operator == (const Fish& src) {
        if (m_name == src.m_name && rarity == src.rarity && weight == src.weight) return true;
    }