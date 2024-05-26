#pragma once
#include "raylib.h"
#include <iostream>
#include <map>
#include <vector>

class CTextureManager {

    std::map<std::string, Texture2D> m_textures;

public:

    CTextureManager() = default;

    bool addTexture(const std::string& key, const std::string& filePath);
    Texture2D& getTexture(const std::string& key) ;

    bool unloadTextures();

    CTextureManager operator = (const CTextureManager& src);


};

class CSoundManager {
    std::vector<Sound> sounds;
public:
    bool addSound(const char* filePath);
    Sound& getSound(int pos);
    bool unloadSounds();

};