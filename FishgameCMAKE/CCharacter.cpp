#include "CCharacter.h"
#include <vector>
#include "raylib.h"
#include "CTools.h"
#include "CFishing.h"
#include "iostream"
#include "CPopups.h"
#include "Fish.h"
#include <algorithm>
#include <sstream>

template<typename T>
std::string toString(const T& src) {
    std::stringstream is;
    is << src;
    return is.str();
}

CCharacter::CCharacter(int posX, int posY) : m_posX(posX), m_posY(posY), skill(1), luck(1), level(1), xp(0), qtime(0) {
	m_colRec = { static_cast<float>(m_posX), static_cast<float>(m_posY), 20, 20 };
}

CCharacter::CCharacter(): m_posX(GetScreenWidth() / 2), m_posY(GetScreenHeight() / 2), skill(1), luck(1), level(1), xp(0), qtime(0) {}

void CCharacter::charUpdate() {
	m_colRec = { static_cast<float>(m_posX), static_cast<float>(m_posY), 20, 20 };
    DrawRectangle(m_posX, m_posY, 10, 10, RED); //Character model
    //DrawTexture(char_tex, m_posX, m_posY, WHITE);
}

void CCharacter::fish(CFishingSpot& fishSpot, Texture2D& pop_tex, CTextureManager & texman) {
    if (checkCollision(fishSpot, pop_tex)) {
        if (fishInventory.size() > 4) {
            std::cout << "Inventory Full" << std::endl;
            return;
        }
        Fish fish(rod.luck, luck, skill, level, rod.strenght, texman);
	    fishInventory.push_back(fish);
        std::cout << xp << " + " << 1.246 * level * ((fish.rarity ^ 2) * 0.267) * fish.weight * 100 << std::endl;
        xp += 1.246 * level * ((fish.rarity ^ 2) * 0.267) * fish.weight * 100;
        std::cout << "Caught a fish" << std::endl;
        popupQ.push_back("xp");
        update();
        return;
    }
    std::cout << "Not in a fishZone" << std::endl;

}

void CCharacter::kbListen(CFishingSpot& fishSpot, Texture2D& pop_tex, CShop & shops , CSoundManager& soundMan, Camera2D & camera, CTextureManager & texman) {

    if (IsKeyDown(KEY_UP)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_posY--;
        }
        m_posY--;
        if (!IsSoundPlaying(soundMan.getSound(0))) {
            PlaySound(soundMan.getSound(0));
        }
    }
    if (IsKeyDown(KEY_DOWN)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_posY++;
        }
        m_posY++;
        if (!IsSoundPlaying(soundMan.getSound(0))) {
            PlaySound(soundMan.getSound(0));
        }
    }
    if (IsKeyDown(KEY_LEFT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_posX--;
        }
        m_posX--;
        if (!IsSoundPlaying(soundMan.getSound(0))) {
            PlaySound(soundMan.getSound(0));
        }
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_posX++;
        }
        m_posX++;
        if (!IsSoundPlaying(soundMan.getSound(0))) {
            PlaySound(soundMan.getSound(0));
        }
    }
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
        StopSound(soundMan.getSound(0));
    }
    if (IsKeyPressed(KEY_F)) {
        fish(fishSpot, pop_tex, texman);
    }
    if (IsKeyPressed(KEY_O)) {
        std::cout << fishInventory.size() << std::endl;
        for (const auto& item : fishInventory) {
            std::cout << item.prefix << " | " << item.m_name << " | " << item.rarity << " | " << item.weight << std::endl;
        }
    }
    if (IsKeyDown(KEY_B)) {
        shops.drawShopSpots();
        fishSpot.drawFishingSpots(texman);
        
    }
    if (IsKeyPressed(KEY_F11)) {
        SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
        camera.offset.x = GetMonitorWidth(0) / 2;
        camera.offset.y = GetMonitorHeight(0) / 2;
        camera.target.x = this->m_posX;
        camera.target.y = this->m_posY;
        ToggleFullscreen();
    }
    if (IsKeyPressed(KEY_F10)) {
        SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
        camera.offset.x = GetMonitorWidth(0) / 2;
        camera.offset.y = GetMonitorHeight(0) / 2;
        camera.target.x = this->m_posX;
        camera.target.y = this->m_posY;
        ToggleBorderlessWindowed();
    }

}

bool CCharacter::checkCollision(CFishingSpot& fishSpot, Texture2D& pop_tex) const {
    for (const auto& item : fishSpot.fishingSpots) {
        if (CheckCollisionRecs(m_colRec, item)) {
            DrawRectangleLines(item.x, item.y, item.width, item.height, YELLOW); // DEBUG
            CPopups popup(pop_tex);
            popup.pop("TEST POPUP MESSAGE");
            return true;
        }
    }
    return false;
}

CCharacter::~CCharacter() = default;
void CCharacter::loadTexture(const Texture2D& tex){
    char_tex = tex;
}

void CCharacter::update() {
    if (xp > 150) {
        level = 2;
        if (xp > 750) {
            level = 3;
            if(xp > 3125){
                level = 4;
                if (xp > 13258) {
                    level = 5;
                    if (xp > 38905) {
                        level = 6;
                    }
                }
            }
        }
    }
    std::cout << "LEVEL: " << level << std::endl;
}

bool CCharacter::checkShopCollision(CShop & shop) const {
    for (const auto& item : shop.shopSpots) {
        if (CheckCollisionRecs(m_colRec, item)) {
            DrawRectangleLines(item.x, item.y, item.width, item.height, DARKBLUE); // DEBUG
            return true;
        }
    }
    return false;
}

int CCharacter::getGold(Fish& fish) {
    return (fish.rarity + 1.5) * fish.weight * luck * 0.522 * 25 * (level * 1.267);
}

void CCharacter::sellFish(Fish& fish) {
    gold += getGold(fish);
    std::cout << (fish.rarity + 1.5) * fish.weight * luck * 0.522 * 25 * (level*1.267) << std::endl;
    auto it = std::find(fishInventory.begin(), fishInventory.end(), fish);
    if (it != fishInventory.end()) {
        fishInventory.erase(it);
    }
}

void CCharacter::displayQ() {
    //every 60 frames pop back
    int opacity = 100;
    qtime++;
    if (qtime > 360) {
        qtime = 0;
        if (!popupQ.empty()) {
            popupQ.pop_back();
        }
    }
    int i = 1;
    for (const auto& item : popupQ) {
        DrawTextEx(GetFontDefault(), item.c_str(), { (float)m_posX - 30, (float)m_posY - qtime/60 - 30 }, 25, 2, { 255,255,255, 255 });
        i++;
    }

}
