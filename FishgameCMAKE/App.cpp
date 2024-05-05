#include "raylib.h"
#include "App.h"
#include "CTextureManager.h"
#include "CMap.h"
#include "CCharacter.h"
#include "CFishing.h"
#include <sstream>

template<typename T>
std::string toString(const T& src) {
	std::stringstream is;
	is << src;
	return is.str();
}

App::App() : textures(CTextureManager()), camera({0}) {}

App::~App() = default;
void App::Init() {
	player = CCharacter(400, 400); // Two SpawnPoint Coords

	currentScreen = TITLE;
	
	currentMenu = NONE;

	currentShopMenu = NO;

	textures.addTexture("nature", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/Resources/Textures/texture.png");
	textures.addTexture("bark", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/bark_large.png");
	textures.addTexture("popup_bg", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/popup_bg.png");
	textures.addTexture("grass", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/grass.png");
	textures.addTexture("water", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/water.png");
	textures.addTexture("sand", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/sand.png");
	textures.addTexture("char", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/char.png");
	textures.addTexture("fish1", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/Lfish1.png");
	textures.addTexture("sand32", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/sand32.png");
	textures.addTexture("grass32", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/grass32.png");
	textures.addTexture("coin", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/coin.png");
	textures.addTexture("pufferfish", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/fish2.png");
	textures.addTexture("bluefish", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/fish3.png");
	textures.addTexture("clownfish", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/clownfish.png");

	map = CMap(textures, "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/maps/map1.fshmap");

	player.loadTexture(textures.getTexture("char"));

	fishSpot.addFishingSpot(50, 50, 50, 50);
	fishSpot.addFishingSpot(200, 200, 50, 50);

	shops.addShopSpot(0, 100, 50, 50);

	soundsMan.addSound("C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/sounds/footsteps_grass.wav");
}


void App::RunGame()
{
	
	InitWindow(screenWidth, screenHeight, "Anglarium");
	SetWindowIcon(LoadImageFromTexture(textures.getTexture("fish1")));
	SetTargetFPS(60);
	SetExitKey(KEY_ESCAPE);
	InitAudioDevice();
	Vector2 trgt = { player.m_posX + 10.0f, player.m_posY + 10.0f };
	camera.target = trgt;
	Vector2 ofs = { screenWidth / 2.0f, screenHeight / 2.0f };
	camera.offset = ofs;
	camera.rotation = 0.0f;
	camera.zoom = 3.0f;

}

void App::OnRender() {



	int mouseX = GetMouseX();
	int mouseY = GetMouseY();

	BeginDrawing();
	ClearBackground(BLACK);
	Rectangle button = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2, 200, 100 };
	int inventoryPage = 1;
	int screenCornerX = player.m_posX - GetScreenWidth() / 2 / camera.zoom + 10;
	int screenCornerY = player.m_posY - GetScreenHeight() / 2 / camera.zoom + 10;
	Rectangle mouseRec;
	Rectangle shop_inv_rec;
	Rectangle shop_talk_rec;
	Vector2 trgt;

	switch (currentScreen) {

	case TITLE:
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = GAMEPLAY;
		}
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
		DrawText("ANGLARIUM", GetScreenWidth() / 2 - 150, 100, 50, BLACK);

		DrawRectangleRoundedLines(button, 0.5f, 4, 2.0f, BLACK);
		DrawRectangleRounded(button, 0.5f, 4, WHITE);
		DrawText("PLAY", button.x + 60, button.y + 30, 30, BLACK);
		if (mouseX >= button.x && mouseX <= button.x + button.width && mouseY >= button.y && mouseY <= button.y + button.height) {
			DrawRectangleRounded(button, 0.5f, 4, LIGHTGRAY);
			DrawText("PLAY", button.x + 60, button.y + 30, 30, RAYWHITE);
			DrawRectangleRoundedLines(button, 0.5f, 4, 2.0f, BLACK);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				currentScreen = GAMEPLAY;
			}
		}

		break;
	case PAUSE:
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = GAMEPLAY;
		}
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
		DrawText("ANGLARIUM", GetScreenWidth() / 2 - 150, 100, 50, BLACK);
		DrawText("PAUSED", GetScreenWidth() / 2 - 150, 200, 30, BLACK);

		DrawRectangleRoundedLines(button, 0.5f, 4, 2.0f, BLACK);
		DrawRectangleRounded(button, 0.5f, 4, WHITE);
		DrawText("PLAY", button.x + 60, button.y + 30, 30, BLACK);
		if (mouseX >= button.x && mouseX <= button.x + button.width && mouseY >= button.y && mouseY <= button.y + button.height) {
			DrawRectangleRounded(button, 0.5f, 4, LIGHTGRAY);
			DrawText("PLAY", button.x + 60, button.y + 30, 30, RAYWHITE);
			DrawRectangleRoundedLines(button, 0.5f, 4, 2.0f, BLACK);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				currentScreen = GAMEPLAY;
			}
		}


		break;

	case GAMEPLAY:

		if (IsKeyPressed(KEY_Q)) {
			currentScreen = PAUSE;
		}
		if (IsKeyPressed(KEY_E) && player.checkShopCollision(shops)){
			currentScreen = SHOP;
		}

		BeginMode2D(camera);

		map.drawMap();
		trgt = { player.m_posX + 10.0f, player.m_posY + 10.0f };
		camera.target = trgt;
		player.checkCollision(fishSpot, textures.getTexture("popup_bg"));
		player.checkShopCollision(shops);
		player.charUpdate();
		screenCornerX = player.m_posX - GetScreenWidth() / 2 / camera.zoom + 10;
		screenCornerY = player.m_posY - GetScreenHeight() / 2 / camera.zoom + 10;

		switch (currentMenu) {
		case NONE:
			if (IsKeyPressed(KEY_E) && !player.checkShopCollision(shops)) {
				currentMenu = INVENTORY;
			}
			player.kbListen(fishSpot, textures.getTexture("popup_bg"), shops, soundsMan, camera, textures);
			camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
			if (camera.zoom > 4.5f) camera.zoom = 4.5f;
			if (camera.zoom < 0.2f) camera.zoom = 0.2f;
			break;

		case INVENTORY:
			if (IsKeyPressed(KEY_E)) {
				currentMenu = NONE;
			}
			DrawText("INVENTORY", screenCornerX + 1, screenCornerY + 1, 100 / camera.zoom, BLACK);
			DrawText("INVENTORY", screenCornerX, screenCornerY, 100 / camera.zoom, RAYWHITE);
			inventoryPage = 1;
			for (size_t i = 0; i < player.fishInventory.size(); i++) {
				DrawTexture(textures.getTexture("fish1"), screenCornerX + 100, screenCornerY + 100 + i * 128, WHITE);

				std::stringstream ss;
				ss << mouseY;

				DrawText(ss.str().c_str(), player.m_posX, player.m_posY, 20, WHITE);

			}
			if ((mouseX > 0) && (128 > mouseX) &&
				(mouseY > 0) && (mouseY < 128) && player.fishInventory.size() > 0) {
				// check which box is mouse in
				DrawTexture(textures.getTexture("fish1"), screenCornerX, screenCornerY, WHITE);
				DrawText(toString<double>(player.fishInventory[0].weight).c_str(), screenCornerX, screenCornerY, 50, RAYWHITE);
			}
			if ((mouseX > 0) && (128 > mouseX) &&
				(mouseY > 128) && (mouseY < 128 * 2) && player.fishInventory.size() > 1) {
				// check which box is mouse in
				DrawText(toString<double>(player.fishInventory[1].weight).c_str(), screenCornerX, screenCornerY, 50, RAYWHITE);
			}
			if ((mouseX > 0) && (128 > mouseX) &&
				(mouseY > 128 * 2) && (mouseY < 128 * 3) && player.fishInventory.size() > 2) {
				// check which box is mouse in
				DrawText(toString<double>(player.fishInventory[2].weight).c_str(), screenCornerX, screenCornerY, 50, RAYWHITE);
			}
			if ((mouseX > 0) && (128 > mouseX) &&
				(mouseY > 128 * 3) && (mouseY < 128 * 4) && player.fishInventory.size() > 3) {
				// check which box is mouse in
				DrawText(toString<double>(player.fishInventory[3].weight).c_str(), screenCornerX, screenCornerY, 50, RAYWHITE);
			}
			if ((mouseX > 0) && (128 > mouseX) &&
				(mouseY > 128 * 4) && (mouseY < 128 * 5) && player.fishInventory.size() > 4) {
				// check which box is mouse in
				DrawText(toString<double>(player.fishInventory[4].weight).c_str(), screenCornerX, screenCornerY, 50, RAYWHITE);
			}
			break;
		}
		break;
	case SHOP:
		if (IsKeyPressed(KEY_E)) {
			currentScreen = GAMEPLAY;
		}
		DrawText("FISH SHOP", 0 + 1, 0 + 1, 100, BLACK);
		DrawText("FISH SHOP", 0 + 1, 0 + 1, 100, RAYWHITE);
		DrawRectangle(0, 0 + GetScreenHeight()  / 3 * 2, GetScreenWidth(), GetScreenHeight() / 2, RAYWHITE);
		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 }, 0, 2.2, WHITE);
		player.gold = 290;
		DrawText(toString(player.gold).c_str(), 0 + 64, 0 + GetScreenHeight() / 3 * 2 + 12, 45, BLACK);
		switch (currentShopMenu) {
		case NO:
			shop_inv_rec = { (float)GetScreenWidth() / 4, (float)GetScreenHeight() / 3 * 2 + 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };
			DrawRectangleRounded(shop_inv_rec, 0.5, 4, LIGHTGRAY);
			if (CheckCollisionPointRec(GetMousePosition(), shop_inv_rec)) {
				DrawRectangleRounded(shop_inv_rec, 0.5, 4, DARKGRAY);
				if (IsMouseButtonPressed(0)) {
					currentShopMenu = SELL;
				}
			}
		break;
		case SELL:
			if(IsKeyPressed(KEY_BACKSPACE)){
				currentShopMenu = NO;
			}
			DrawTextureEx(player.fishInventory[0].m_tex, { (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 3*2 + 20 }, 0, 1, WHITE);
			DrawTextureEx(player.fishInventory[1].m_tex, { (float)GetScreenWidth() / 5 * 2, (float)GetScreenHeight() / 3*2 + 20 }, 0, 1, WHITE);
			DrawTextureEx(player.fishInventory[2].m_tex, { (float)GetScreenWidth() / 5 * 3, (float)GetScreenHeight() / 3*2 + 20 }, 0, 1, WHITE);
			DrawTextureEx(player.fishInventory[3].m_tex, { (float)GetScreenWidth()/5 * 4, (float)GetScreenHeight()/3*2 + 20}, 0, 1, WHITE);
		break;
		default:
		break;
		
		}


		break;
	}

		EndMode2D();
		EndDrawing();

}

	void App::unloadTex() {
		textures.unloadTextures();
		soundsMan.unloadSounds();
		CloseAudioDevice();
	}
