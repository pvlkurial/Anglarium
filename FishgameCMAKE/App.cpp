#include "raylib.h"
#include "App.h"
#include "CTextureManager.h"
#include "CMap.h"
#include "CCharacter.h"
#include "CFishing.h"
#include <sstream>

static void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);   // Draw text using font inside rectangle limits
static void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);    // Draw text using font inside rectangle limits with support for text selection

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
	textures.addTexture("cod", "C:/Users/pavel/source/repos/FishgameCMAKE/FishgameCMAKE/resources/textures/cod.png");
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
	player.gold = 290;

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
	float scale_offset = sqrt((GetScreenHeight() * GetScreenHeight()) / (540 * 540));


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
				DrawTexture(textures.getTexture("cod"), screenCornerX, screenCornerY, WHITE);
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
		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 }, 0, 2.2 * scale_offset, WHITE);
		DrawText(toString(player.gold).c_str(), 0 + (64 * scale_offset), 0 + GetScreenHeight() / 3 * 2 + (12 * scale_offset), 45 * scale_offset, BLACK);
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
			if (!player.fishInventory.empty()) {
				DrawTextureEx(player.fishInventory[0].m_tex, { (float)GetScreenWidth() / 6 - 20, (float)GetScreenHeight() / 3 * 2 + (20*scale_offset) }, 0, 1 * scale_offset, WHITE);
				Rectangle fish1rec = { (float)GetScreenWidth() / 6 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight()/3};
				if (!player.fishInventory[0].selected) {
					if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
						std::string temp;
						temp.append(toString(player.fishInventory[0].m_name)).append("\n").append(toString(player.fishInventory[0].weight)).append("\n")
							.append(toString(player.fishInventory[0].rarity));
						DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
						DrawRectangleRec(fish1rec, { 0,0,0,100 });
						if (IsMouseButtonPressed(0)) {
							if (!player.fishInventory.empty()) {
								player.fishInventory[0].selected = !player.fishInventory[0].selected;
							}
						}
					}

				}else {
						Rectangle rec = { (float)GetScreenWidth() / 6 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
						DrawRectangleRec(rec, { 0,0,0,100 });
						std::string temp;
						temp.append("Sell ").append(player.fishInventory[0].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[0]))).append(" gold?");
						DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
						Rectangle confirmRec{ (float)GetScreenWidth() / 6 - 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
						DrawRectangleRec(confirmRec, {0,0,0, 150});
						DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
						if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
							std::cout << player.fishInventory[0].m_name << std::endl;
							player.sellFish(player.fishInventory[0]);
						}
						if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
							if (!player.fishInventory.empty()) {
								player.fishInventory[0].selected = !player.fishInventory[0].selected;
							}
						}
					}
				
				if (player.fishInventory.size() > 1) {
					DrawTextureEx(player.fishInventory[1].m_tex, { (float)GetScreenWidth() / 6 * 2 - 20, (float)GetScreenHeight() / 3*2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
					Rectangle fish1rec = { (float)GetScreenWidth() / 6 * 2 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
					if (!player.fishInventory[1].selected) {
					
						if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
							std::string temp;
							temp.append(toString(player.fishInventory[1].m_name)).append("\n").append(toString(player.fishInventory[1].weight)).append("\n")
								.append(toString(player.fishInventory[1].rarity));
							DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
							DrawRectangleRec(fish1rec, { 0,0,0,100 });
							if (IsMouseButtonPressed(0)) {
								player.fishInventory[1].selected = !player.fishInventory[1].selected;
							}
						}
					}
					else {
						Rectangle rec = { (float)GetScreenWidth() / 6 * 2 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
						DrawRectangleRec(rec, { 0,0,0,100 });
						std::string temp;
						temp.append("Sell ").append(player.fishInventory[1].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[1]))).append(" gold?");
						DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
						Rectangle confirmRec{ (float)GetScreenWidth() / 6 * 2- 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[1].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
						DrawRectangleRec(confirmRec, { 0,0,0, 150 });
						DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6  * 2 - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
						if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
							std::cout << player.fishInventory[1].m_name << std::endl;
							player.sellFish(player.fishInventory[1]);
						}
						if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
							if (!player.fishInventory.empty()) {
								player.fishInventory[1].selected = !player.fishInventory[1].selected;
							}
						}
					
					}

					if (player.fishInventory.size() > 2) {
						DrawTextureEx(player.fishInventory[2].m_tex, { (float)GetScreenWidth() / 6 * 3 - 20, (float)GetScreenHeight() / 3*2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
						Rectangle fish1rec = { (float)GetScreenWidth() / 6 * 3 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
						if (!player.fishInventory[2].selected) {
							if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
								std::string temp;
								temp.append(toString(player.fishInventory[2].m_name)).append("\n").append(toString(player.fishInventory[2].weight)).append("\n")
									.append(toString(player.fishInventory[2].rarity));
								DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
								DrawRectangleRec(fish1rec, { 0,0,0,100 });
								if (IsMouseButtonPressed(0)) {
									player.fishInventory[2].selected = !player.fishInventory[2].selected;
								}
							}
							else {
								Rectangle rec = { (float)GetScreenWidth() / 6 * 3 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
								DrawRectangleRec(rec, { 0,0,0,100 });
								std::string temp;
								temp.append("Sell ").append(player.fishInventory[2].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[2]))).append(" gold?");
								DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
								Rectangle confirmRec{ (float)GetScreenWidth() / 6 * 3 - 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[2].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
								DrawRectangleRec(confirmRec, { 0,0,0, 150 });
								DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 * 3 - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
								if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
									std::cout << player.fishInventory[2].m_name << std::endl;
									player.sellFish(player.fishInventory[2]);
								}
								if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
									if (player.fishInventory.size() > 2) {
										player.fishInventory[2].selected = !player.fishInventory[2].selected;
									}
								}
							
							
							}
						}
						if (player.fishInventory.size() > 3) {
							DrawTextureEx(player.fishInventory[3].m_tex, { (float)GetScreenWidth() / 6 * 4 - 20, (float)GetScreenHeight() / 3 * 2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
							Rectangle fish1rec = { (float)GetScreenWidth() / 6 * 4 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
							
							if (!player.fishInventory[3].selected) {
								if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
									std::string temp;
									temp.append(toString(player.fishInventory[3].m_name)).append("\n").append(toString(player.fishInventory[3].weight)).append("\n")
										.append(toString(player.fishInventory[3].rarity));
									DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
									DrawRectangleRec(fish1rec, { 0,0,0,100 });
									if (IsMouseButtonPressed(0) ) {
										player.fishInventory[3].selected = !player.fishInventory[3].selected;
									}
								}
							}
							else {
								Rectangle rec = { (float)GetScreenWidth() / 6 * 4 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
								DrawRectangleRec(rec, { 0,0,0,100 });
								std::string temp;
								temp.append("Sell ").append(player.fishInventory[3].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[3]))).append(" gold?");
								DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
								Rectangle confirmRec{ (float)GetScreenWidth() / 6 * 4 - 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[3].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
								DrawRectangleRec(confirmRec, { 0,0,0, 150 });
								DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 * 4 - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
								if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
									std::cout << player.fishInventory[3].m_name << std::endl;
									player.sellFish(player.fishInventory[3]);
								}
								if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
									if (player.fishInventory.size() > 3) {
										player.fishInventory[3].selected = !player.fishInventory[3].selected;
									}
								}
						
							}
							if (player.fishInventory.size() > 4) {
								DrawTextureEx(player.fishInventory[4].m_tex, { (float)GetScreenWidth()/ 6 * 5 - 20, (float)GetScreenHeight()/3*2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
								Rectangle fish1rec = { (float)GetScreenWidth() / 6 * 5- 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
								if (!player.fishInventory[4].selected) {
									if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
										std::string temp;
										temp.append(toString(player.fishInventory[4].m_name)).append("\n").append(toString(player.fishInventory[4].weight)).append("\n")
											.append(toString(player.fishInventory[4].rarity));
										DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
										DrawRectangleRec(fish1rec, { 0,0,0,100 });
										if (IsMouseButtonPressed(0)) {
											player.fishInventory[4].selected = !player.fishInventory[4].selected;
										}
									}
								}
								else {
									Rectangle rec = { (float)GetScreenWidth() / 6 * 5 - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
									DrawRectangleRec(rec, { 0,0,0,100 });
									std::string temp;
									temp.append("Sell ").append(player.fishInventory[4].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[4]))).append(" gold?");
									DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
									Rectangle confirmRec{ (float)GetScreenWidth() / 6 * 5 - 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[4].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
									DrawRectangleRec(confirmRec, { 0,0,0, 150 });
									DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 * 5 - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
									if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
										std::cout << player.fishInventory[4].m_name << std::endl;
										player.sellFish(player.fishInventory[4]);
									}
									if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
										if (player.fishInventory.size() > 4) {
											player.fishInventory[4].selected = !player.fishInventory[4].selected;
										}
									}
								
								}
							}
						}
					}
				
				}
			}
			else {
				DrawText("INVENTORY IS EMPTY", (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, 35*scale_offset, BLACK);
			}

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

	static void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
	{
		DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
	}



	static void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
	{
		int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

		float textOffsetY = 0;          // Offset between lines (on line break '\n')
		float textOffsetX = 0.0f;       // Offset X to next character to draw

		float scaleFactor = fontSize / (float)font.baseSize;     // Character rectangle scaling factor

		// Word/character wrapping mechanism variables
		enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
		int state = wordWrap ? MEASURE_STATE : DRAW_STATE;

		int startLine = -1;         // Index where to begin drawing (where a line begins)
		int endLine = -1;           // Index where to stop drawing (where a line ends)
		int lastk = -1;             // Holds last value of the character position

		for (int i = 0, k = 0; i < length; i++, k++)
		{
			// Get next codepoint from byte string and glyph index in font
			int codepointByteCount = 0;
			int codepoint = GetCodepoint(&text[i], &codepointByteCount);
			int index = GetGlyphIndex(font, codepoint);

			// NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
			// but we need to draw all of the bad bytes using the '?' symbol moving one byte
			if (codepoint == 0x3f) codepointByteCount = 1;
			i += (codepointByteCount - 1);

			float glyphWidth = 0;
			if (codepoint != '\n')
			{
				glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width * scaleFactor : font.glyphs[index].advanceX * scaleFactor;

				if (i + 1 < length) glyphWidth = glyphWidth + spacing;
			}

			// NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
			// We store this info in startLine and endLine, then we change states, draw the text between those two variables
			// and change states again and again recursively until the end of the text (or until we get outside of the container).
			// When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
			// and begin drawing on the next line before we can get outside the container.
			if (state == MEASURE_STATE)
			{
				// TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
				// Ref: http://jkorpela.fi/chars/spaces.html
				if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

				if ((textOffsetX + glyphWidth) > rec.width)
				{
					endLine = (endLine < 1) ? i : endLine;
					if (i == endLine) endLine -= codepointByteCount;
					if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

					state = !state;
				}
				else if ((i + 1) == length)
				{
					endLine = i;
					state = !state;
				}
				else if (codepoint == '\n') state = !state;

				if (state == DRAW_STATE)
				{
					textOffsetX = 0;
					i = startLine;
					glyphWidth = 0;

					// Save character position when we switch states
					int tmp = lastk;
					lastk = k - 1;
					k = tmp;
				}
			}
			else
			{
				if (codepoint == '\n')
				{
					if (!wordWrap)
					{
						textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
						textOffsetX = 0;
					}
				}
				else
				{
					if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
					{
						textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
						textOffsetX = 0;
					}

					// When text overflows rectangle height limit, just stop drawing
					if ((textOffsetY + font.baseSize * scaleFactor) > rec.height) break;

					// Draw selection background
					bool isGlyphSelected = false;
					if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
					{
						DrawRectangleRec({ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize* scaleFactor }, selectBackTint);
						isGlyphSelected = true;
					}

					// Draw current character glyph
					if ((codepoint != ' ') && (codepoint != '\t'))
					{
						DrawTextCodepoint(font, codepoint, { rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected ? selectTint : tint);
					}
				}

				if (wordWrap && (i == endLine))
				{
					textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
					textOffsetX = 0;
					startLine = endLine;
					endLine = -1;
					glyphWidth = 0;
					selectStart += lastk - k;
					k = lastk;

					state = !state;
				}
			}

			if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
		}
	}