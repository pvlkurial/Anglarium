#include "CTextureManager.h"
#include <string>
#include <raylib.h>
#include <map>
#include "CTextureManager.h"

bool CTextureManager::addTexture(const std::string& key, const char* filePath) {
	m_textures.insert({ key, LoadTexture(filePath) });
	return 0;
}

Texture2D& CTextureManager::getTexture(const std::string& key){
	return m_textures[key];
}

bool CTextureManager::unloadTextures() {
    for (const auto& item : m_textures) {
        UnloadTexture(item.second);
    }
    return true;
}

CTextureManager CTextureManager::operator = (const CTextureManager& src) {

    for (auto it = m_textures.begin(); it != m_textures.end(); ++it ) {
        m_textures.erase(it);
    }

    for (auto it = src.m_textures.begin(); it != src.m_textures.end(); ++it) {
        m_textures.insert(*it);
    }

    return *this;
}

bool CSoundManager::addSound(const char* filePath) {
    sounds.push_back(LoadSound(filePath));
    return true;
}
Sound& CSoundManager::getSound(int pos) {
    return sounds[pos];
}
bool CSoundManager::unloadSounds() {
    for (const auto& item : sounds) {
        UnloadSound(item);
    }
    return true;
}