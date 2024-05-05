#pragma once
#include "raylib.h"
#include <iostream>

class CPopups {
    Texture2D m_popImage;
public:

    CPopups(Texture2D& img);
    void pop(const std::string& msg);
};