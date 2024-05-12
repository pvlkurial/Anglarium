#pragma once
#include "raylib.h"
#include <iostream>
#include "CCharacter.h"

class CPopups {
    Texture2D m_popImage;
    int TTL;
public:

    CPopups(Texture2D& img);
    ~CPopups();
    CPopups();
    void pop(const std::string& msg);
    void xpPop(const std::string& xp, CCharacter& player);
};