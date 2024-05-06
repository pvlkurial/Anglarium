#pragma once
#include "raylib.h"
#include <iostream>
#include "CCharacter.h"

class CPopups {
    Texture2D m_popImage;
public:

    CPopups(Texture2D& img);
    void pop(const std::string& msg);
    void xpPop(const std::string& xp, CCharacter& player);
};