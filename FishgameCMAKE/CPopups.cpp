#include "CPopups.h"
#include "string"
#include "raylib.h"
#include "CCharacter.h"
#include "utils.h"

CPopups::CPopups(Texture2D& img) : m_popImage(img) {}

void CPopups::pop(const std::string& msg) {
    DrawTexture(m_popImage, GetScreenWidth() - m_popImage.width, GetScreenHeight() - m_popImage.height, WHITE);
    DrawTexture(m_popImage, GetRenderWidth() - m_popImage.width, GetRenderHeight() - m_popImage.height, WHITE);

    DrawText(msg.c_str(), GetScreenWidth() - m_popImage.width + 26, GetScreenHeight() - m_popImage.height + 17, 15, BLACK);
    DrawText(msg.c_str(), GetScreenWidth() - m_popImage.width + 25, GetScreenHeight() - m_popImage.height + 15, 15, WHITE);
}
void CPopups::xpPop(const std::string& xp, CCharacter & player) {
    

}