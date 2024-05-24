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

bool CCharacter::isWithinBounds(int m_posX, int m_posY){
    if (m_posX < 0 || m_posX > 800 || m_posY < 0 || m_posY > 800) {
        return false;
    }
    return true;
}

CCharacter::CCharacter(int posX, int posY) : m_posX(posX), m_posY(posY), skill(1), luck(1), level(1), xp(0), qtime(0), gold(0), silver(0) {
	m_colRec = { static_cast<float>(m_posX), static_cast<float>(m_posY), 20, 20 };
}

CCharacter::CCharacter(): m_posX(GetScreenWidth() / 2), m_posY(GetScreenHeight() / 2), skill(1), luck(1), level(1), xp(0), qtime(0), gold(0), silver(0) {}

void CCharacter::charUpdate() {
	m_colRec = { static_cast<float>(m_posX), static_cast<float>(m_posY), 20, 20 };
    DrawRectangle(m_posX, m_posY, 10, 10, RED); //Character model
    //DrawTexture(char_tex, m_posX, m_posY, WHITE);
}

void CCharacter::fish(CFishingSpot& fishSpot, Texture2D& pop_tex, CTextureManager & texman) {
    if (checkCollision(fishSpot, pop_tex)) {
        if (fishInventory.size() > 4) {
            std::cout << "Inventory Full" << std::endl;
            if (popupQ.size() < 7) {
                popupQ.push_back("Full Inventory!");
            }
            return;
        }
        Fish fish(rod.luck, luck, skill, level, rod.strenght, texman);
	    fishInventory.push_back(fish);
        std::cout << xp << " + " << 1.246 * level * ((fish.rarity ^ 2) * 0.267) * fish.weight * 100 << std::endl;
        xp += 1.246 * level * ((fish.rarity ^ 2) * 0.267) * fish.weight * 100;
        std::cout << "Caught a fish" << std::endl;
        std::string temp;
        temp.append("+").append(toString((int)(1.246 * level * ((fish.rarity ^ 2) * 0.267) * fish.weight * 100))).append("xp");
        if (popupQ.size() < 7) {
            popupQ.push_back(temp);
        }
        update();
        return;
    }
    std::cout << "Not in a fishZone" << std::endl;

}

void CCharacter::kbListen(CFishingSpot& fishSpot, Texture2D& pop_tex, CShop & shops , CSoundManager& soundMan, Camera2D & camera, CTextureManager & texman) {
    if (IsKeyDown(KEY_UP)) {
        if (isWithinBounds(m_posX, m_posY-1)) {

            if (IsKeyDown(KEY_LEFT_SHIFT)) {
            m_posY--;
            }
            m_posY--;
            if (!IsSoundPlaying(soundMan.getSound(0))) {
                PlaySound(soundMan.getSound(0));
            }
        }
    }
    if (IsKeyDown(KEY_DOWN)) {
        if (isWithinBounds(m_posX, m_posY+1)) {

            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                m_posY++;
            }
            m_posY++;
            if (!IsSoundPlaying(soundMan.getSound(0))) {
                PlaySound(soundMan.getSound(0));
            }
        }
    }
    if (IsKeyDown(KEY_LEFT)) {
        if (isWithinBounds(m_posX-1, m_posY)) {

            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                m_posX--;
            }
            m_posX--;
            if (!IsSoundPlaying(soundMan.getSound(0))) {
                PlaySound(soundMan.getSound(0));
            }
        }
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (isWithinBounds(m_posX + 1, m_posY)) {

            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                m_posX++;
            }
            m_posX++;
            if (!IsSoundPlaying(soundMan.getSound(0))) {
                PlaySound(soundMan.getSound(0));
            }
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
            //DrawRectangleLines(item.x, item.y, item.width, item.height, YELLOW); // DEBUG
           
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
    if (xp < 151) {
        level = 1;
    }
    if (xp > 150 && xp < 750) {
        if (level < 2) {
            skill_points += 3;
            popupQ.push_back("LEVEL UP!");
        }
        level = 2;
    }
        if (xp > 750 && xp < 3126) {
            if (level < 3) {
                popupQ.push_back("LEVEL UP!");
                skill_points += 3;
            }
            level = 3;
        }
        if (xp > 3125 && xp < 13259) {
            if (level < 4) {
                popupQ.push_back("LEVEL UP!");
                skill_points += 3;
            }
            level = 4;
        }
        if (xp > 13258 && xp < 38906) {
            if (level < 5) {
                popupQ.push_back("LEVEL UP!");
                skill_points += 3;
            }
            level = 5;
        }
        if (xp > 38905) {
            if (level < 6) {
                popupQ.push_back("LEVEL UP!");
                skill_points += 3;
            }
            level = 6;
        }
    std::cout << "LEVEL: " << level << std::endl;
    
}



bool CCharacter::checkShopCollision(CShop & shop) const {
    for (const auto& item : shop.shopSpots) {
        if (CheckCollisionRecs(m_colRec, item)) {
            //DrawRectangleLines(item.x, item.y, item.width, item.height, DARKBLUE); // DEBUG
            return true;
        }
    }
    return false;
}

int CCharacter::getGold(Fish& fish) {
    return (fish.rarity * 0.4 + 1) * fish.weight * (luck*0.25 + 1) * 0.522 * 25 * (level*0.6 + 1);
}

int CCharacter::getSilver(Fish& fish) {
    int temp = (fish.rarity * 0.4 + 1) * fish.weight * (luck * 0.25 + 1) * 0.522 * 25 * (level * 0.6 + 1);
    return ((fish.rarity * 0.4 + 1) * fish.weight * (luck * 0.25 + 1) * 0.522 * 25 * (level * 0.6 + 1) - temp) * 100;
}

void CCharacter::sellFish(Fish& fish) {
    gold += getGold(fish);
    silver += getSilver(fish);
    std::cout << (fish.rarity * 0.4 + 1) * fish.weight * (luck * 0.25 + 1) * 0.522 * 25 * (level * 0.6 + 1) << std::endl;
    auto it = std::find(fishInventory.begin(), fishInventory.end(), fish);
    if (it != fishInventory.end()) {
        fishInventory.erase(it);
    }
}

void CCharacter::displayQ() {
    //every 60 frames pop back
    int opacity = 100;
    qtime++;
    if (qtime > 120) {
        qtime = 0;
        if (!popupQ.empty()) {
            popupQ.pop_front();
        }
    }
    int i = 1;
    for (size_t i = 0; i < popupQ.size(); ++i) {
        DrawTextEx(GetFontDefault(), popupQ[i].c_str(), { (float)m_posX - 15, (float)m_posY - qtime / 15 - 30 + 10*i }, 15, 2, { 255,255,255, 180 });
    }


}
