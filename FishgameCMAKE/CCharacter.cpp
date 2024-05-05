#include "CCharacter.h"
#include <vector>
#include "raylib.h"
#include "CTools.h"
#include "CFishing.h"
#include "iostream"
#include "CPopups.h"
#include "Fish.h"

CCharacter::CCharacter(int posX, int posY) : m_posX(posX), m_posY(posY), skill(1), luck(1), level(1), xp(0) {
	m_colRec = { static_cast<float>(m_posX), static_cast<float>(m_posY), 20, 20 };
}

CCharacter::CCharacter(): m_posX(GetScreenWidth() / 2), m_posY(GetScreenHeight() / 2), skill(1), luck(1), level(1), xp(0) {}

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
	    fishInventory.push_back(Fish(rod.luck, luck, skill, level, rod.strenght, texman));
        std::cout << "Caught a fish" << std::endl;
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
        fishSpot.drawFishingSpots();
        
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
    std::cout << "update player" << std::endl;
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