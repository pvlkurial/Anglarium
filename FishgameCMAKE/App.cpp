#include "raylib.h"
#include "App.h"
#include "CTextureManager.h"
#include "CMap.h"
#include "CCharacter.h"
#include "CFishing.h"
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

static void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);   // Draw text using font inside rectangle limits
static void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);    // Draw text using font inside rectangle limits with support for text selection

std::string getRarityText(int rarity) {
	switch (rarity) {
	case 0:
		return "Common";
		break;
	case 1:
		return "Uncommon";
		break;
	case 2:
		return "Rare";
		break;
	case 3:
		return "Epic";
		break;
	case 4:
		return "Legendary";
		break;
	case 5:
		return "Ancient";
		break;
	case 6:
		return "Mythic";
		break;
	}
}

template<typename T>
std::string toString(const T& src) {
	std::stringstream is;
	is << src;
	return is.str();
}

void fish_in_shop_inventory(CCharacter & player, int pos, float scale_offset){
	DrawTextureEx(player.fishInventory[pos].m_tex, { (float)GetScreenWidth() / 6 * (pos+1) - 20, (float)GetScreenHeight() / 3 * 2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
	Rectangle fish1rec = { (float)GetScreenWidth() / 6 * (pos+1) - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
	if (!player.fishInventory[pos].selected) {
		if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
			std::string temp;
			temp.append(toString(player.fishInventory[pos].m_name)).append("\n").append(toString(player.fishInventory[pos].weight)).append("\n")
				.append(getRarityText(player.fishInventory[pos].rarity));
			DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
			DrawRectangleRec(fish1rec, { 0,0,0,100 });
			if (IsMouseButtonPressed(0)) {
				player.fishInventory[pos].selected = !player.fishInventory[pos].selected;
			}
		}
	}
	else {
		Rectangle rec = { (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
		DrawRectangleRec(rec, { 0,0,0,100 });
		std::string temp;
		temp.append("Sell ").append(player.fishInventory[pos].m_name).append(" for ").append(toString(player.getGold(player.fishInventory[pos]))).append(" gold and ")
			.append(toString(player.getSilver(player.fishInventory[pos]))).append(" silver?");
		DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
		Rectangle confirmRec{ (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 12 * 11 , player.fishInventory[pos].m_tex.width * scale_offset, (float)GetScreenHeight() / 10 };
		DrawRectangleRec(confirmRec, { 0,0,0, 150 });
		DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 * (pos + 1) - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
		if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
			std::cout << player.fishInventory[pos].m_name << std::endl;
			player.sellFish(player.fishInventory[pos]);
		}
		if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
			if (player.fishInventory.size() > pos) {
				player.fishInventory[pos].selected = !player.fishInventory[pos].selected;
			}
		}

	}
}

void fish_in_inventory(CCharacter& player, int pos, float scale_offset) {
	DrawTextureEx(player.fishInventory[pos].m_tex, { (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 + (20 * scale_offset) }, 0, 1 * scale_offset, WHITE);
	Rectangle fish1rec = { (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 , player.fishInventory[0].m_tex.width * scale_offset, (float)GetScreenHeight() / 3 };
		if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
			std::string temp;
			temp.append(toString(player.fishInventory[pos].m_name)).append("\n").append(toString(player.fishInventory[pos].weight)).append("\n")
				.append(getRarityText(player.fishInventory[pos].rarity));
			DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
			DrawRectangleRec(fish1rec, { 0,0,0,100 });
			if (IsMouseButtonPressed(0)) {
				player.fishInventory[pos].selected = !player.fishInventory[pos].selected;
			}
		}
}

void fishrod_in_shop_inventory(CCharacter& player, int pos, float scale_offset, CShop & shop, CTextureManager & textures) {

	DrawTextureEx(shop.shopRods[0].m_tex, {(float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 + (20 * scale_offset)}, 0, scale_offset, WHITE);
	Rectangle fish1rec = { (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 , shop.shopRods[0].m_tex.width * 3 * scale_offset, (float)GetScreenHeight() / 3 };
	if (!shop.shopRods[pos].selected) {
		if (CheckCollisionPointRec(GetMousePosition(), fish1rec)) {
			std::string temp;
			temp.append(toString(shop.shopRods[pos].name)).append("\n").append(toString(shop.shopRods[pos].level_needed)).append("\n")
				.append(toString(shop.shopRods[pos].price));
			DrawTextBoxed(GetFontDefault(), temp.c_str(), fish1rec, 30 * scale_offset, 1, true, BLACK);
			DrawRectangleRec(fish1rec, { 0,0,0,100 });
			if (IsMouseButtonPressed(0)) {
				shop.shopRods[pos].selected = !shop.shopRods[pos].selected;
			}
		}
	}
	else {
		Rectangle rec = { (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 3 * 2 , shop.shopRods[pos].m_tex.width * 3 * scale_offset, (float)GetScreenHeight() / 3 };
		DrawRectangleRec(rec, { 0,0,0,100 });
		std::string temp;
		temp.append("Buy ").append(shop.shopRods[pos].name).append(" for ").append(toString(shop.shopRods[pos].price)).append(" gold?");
		DrawTextBoxed(GetFontDefault(), temp.c_str(), rec, 25, 2, true, WHITE);
		Rectangle confirmRec{ (float)GetScreenWidth() / 6 * (pos + 1) - 20, (float)GetScreenHeight() / 12 * 11 , shop.shopRods[pos].m_tex.width * 3 * scale_offset, (float)GetScreenHeight() / 10 };
		DrawRectangleRec(confirmRec, { 0,0,0, 150 });
		DrawTextEx(GetFontDefault(), "CONFIRM", { (float)GetScreenWidth() / 6 * (pos + 1) - 20 + 5, (float)GetScreenHeight() / 12 * 11 + 5 }, 25, 2, RAYWHITE);
		if (CheckCollisionPointRec(GetMousePosition(), confirmRec) && IsMouseButtonPressed(0)) {
			if (player.gold >= shop.shopRods[pos].price) {
				player.gold += -shop.shopRods[pos].price;
				std::cout << shop.shopRods[pos].name << std::endl;
				player.ownedRods.push_back(shop.shopRods[pos]);
				auto it = std::lower_bound(shop.shopRods.begin(), shop.shopRods.end(), shop.shopRods[pos]);
				if (it != shop.shopRods.end()) {
					shop.shopRods.erase(it);
				}
			}
		}
		if (IsMouseButtonPressed(0) && !CheckCollisionPointRec(GetMousePosition(), confirmRec)) {
			if (shop.shopRods.size() > pos) {
				shop.shopRods[pos].selected = !shop.shopRods[pos].selected;
			}
		}

	}
}

App::InitEventListeners(){

	Events().AddListener("LEVEL_UP", [this](const std::unordered_map<std::string, std::string>& params) {
        int newLevel = std::stoi(params.at("level"));
        
        // Example: Display level up message
        std::string message = "Reached Level " + std::to_string(newLevel) + "!";
        player.popupQ.push_back(message);
	});
}



App::App() : textures(CTextureManager()), camera({0}) {}

App::~App() = default;
void App::Init() {
	player = CCharacter(400, 400); // Two SpawnPoint Coords

	currentScreen = TITLE;
	
	currentMenu = NONE;

	currentShopMenu = NO;
	appDir = GetApplicationDirectory();
	std::cout << appDir;

	textures.addTexture("cod", assetPath.string().append("textures/cod.png"));
	textures.addTexture("sand32", assetPath.string().append("textures/sand32.png"));
	textures.addTexture("grass32", assetPath.string().append("textures/grass32.png"));
	textures.addTexture("coin", assetPath.string().append("textures/coin.png"));
	textures.addTexture("pufferfish", assetPath.string().append("textures/fish2.png"));
	textures.addTexture("bluefish", assetPath.string().append("textures/fish3.png"));
	textures.addTexture("clownfish", assetPath.string().append("textures/clownfish.png"));
	textures.addTexture("water", assetPath.string().append("textures/water32.png"));
	textures.addTexture("fishhut", assetPath.string().append("textures/fishhut.png"));
	textures.addTexture("shop", assetPath.string().append("textures/shop.png"));
	textures.addTexture("path", assetPath.string().append("textures/path.png"));
	textures.addTexture("fishGameIcon", assetPath.string().append("textures/cod.png"));
	textures.addTexture("shopkeeper", assetPath.string().append("textures/shopkeeper.png"));
	textures.addTexture("basic_rod", assetPath.string().append("textures/basic_rod.png"));
	textures.addTexture("wooden_rod", assetPath.string().append("textures/wooden_rod.png"));
	textures.addTexture("stone_rod", assetPath.string().append("textures/stone_rod.png"));
	textures.addTexture("bone_rod", assetPath.string().append("textures/bone_rod.png"));
	textures.addTexture("shop_bg", assetPath.string().append("textures/shop_bg.png"));

	map = CMap(textures, assetPath.string().append("maps/map1.fshmap"));

	//player.loadTexture(textures.getTexture("char"));

	fishSpot.addFishingSpot(50, 50, 50, 50);
	fishSpot.addFishingSpot(200, 200, 50, 50);
	fishSpot.addFishingSpot(16 * 32, 16 * 32, 50, 50);

	shops.addShopSpot(0, 100, 50, 50);
	shops.addShopSpot(10 * 32, 10 * 32, 50, 50);

	soundsMan.addSound(assetPath.string().append("sounds/footsteps_grass.wav").c_str());

	std::ifstream f(assetPath.string().append("lib/fishrods.json"));
	if (f.is_open()) {
		nlohmann::json data = nlohmann::json::parse(f);
		std::cout << data["rods"][0] << std::endl;

		for (const auto& it : data["rods"]) {
			std::stringstream is;
			is << it["texture"];
			std::string temp = is.str();
			temp.erase(0,1);
			temp.erase(temp.size()-1,1);

			std::cout << temp << std::endl;
			player.fishingRods.push_back(CFishingRod(it["name"], it["luck"], it["skill"], it["level_needed"], it["price"], textures.getTexture(temp)));
		}

		std::cout << "Fishing Rods Loaded Succesfully" << std::endl;
	}
	f.close();


}


void App::RunGame()
{
	
	InitWindow(screenWidth, screenHeight, "Anglarium");
	SetWindowIcon(LoadImageFromTexture(textures.getTexture("fishGameIcon")));
	SetTargetFPS(60);
	SetExitKey(NONE);
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

	int frametime = GetTime();
	if (frametime > 60) frametime = 0;

	BeginDrawing();
	ClearBackground(BLACK);
	Rectangle button = { (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2, 200, 100 };
	int inventoryPage = 1;
	int screenCornerX = player.m_posX - GetScreenWidth() / 2 / camera.zoom + 10;
	int screenCornerY = player.m_posY - GetScreenHeight() / 2 / camera.zoom + 10;
	Rectangle mouseRec;
	Rectangle shop_inv_rec;
	Rectangle shop_talk_rec;
	Rectangle shop_buy_rec;


	Rectangle inv_skill_skill;
	Rectangle inv_skill_luck;
	Vector2 trgt;
	float scale_offset = sqrt((GetScreenHeight() * GetScreenHeight()) / (540 * 540));
	float currentTime = GetTime();

	if (player.silver >= 100)
	{
		player.gold += player.silver / 100;
		player.silver = player.silver % 100;
	}

	for (const auto& iter : player.fishingRods) {
		//std::cout << iter.level_needed << std::endl;
		bool temp = false;
		if (iter.level_needed <= player.level) {
			for (size_t i = 0; i < shops.shopRods.size(); ++i) {
				if (iter.name == shops.shopRods[i].name) {
					//std::cout << "GOTEM" << std::endl;
					temp = true;
					break;

				}
			}
			if (temp) break;
			shops.shopRods.push_back(iter);
		}
	}


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

	case DEATH:
		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
			currentScreen = GAMEPLAY;
		}
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RAYWHITE);
		DrawText("ANGLARIUM", GetScreenWidth() / 2 - 150, 100, 50, BLACK);
		DrawText("YOU DIED", GetScreenWidth() / 2 - 150, 200, 30, BLACK);
		DrawText("and lost all your money ... and fish", GetScreenWidth() / 2 - 150, 230, 20, BLACK);

		player.fishInventory.clear();
		player.gold = 0;
		player.silver = 0;

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
		time = 5;


		break;
	case PAUSE:
		if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
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

		if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
			currentScreen = PAUSE;
		}
		if (IsKeyPressed(KEY_E) && player.checkShopCollision(shops)) {
			currentScreen = SHOP;
		}

		time += GetFrameTime();

		BeginMode2D(camera);

		map.drawMap();
		trgt = { player.m_posX + 10.0f, player.m_posY + 10.0f };
		camera.target = trgt;
		player.checkCollision(fishSpot, textures.getTexture("popup_bg"));
		player.checkShopCollision(shops);
		player.charUpdate();
		screenCornerX = player.m_posX - GetScreenWidth() / 2 / camera.zoom + 10;
		screenCornerY = player.m_posY - GetScreenHeight() / 2 / camera.zoom + 10;
		fishSpot.drawObjects(textures);
		shops.drawShopObjects(textures);
		// Day and night cycle management

		DrawRectangle(screenCornerX - 5, screenCornerY - 5, GetScreenWidth() + 6, GetScreenHeight() + 6, { 0,0,0,(unsigned char)(cos(time / 10)*100 - 150)});
		DrawRectangle(screenCornerX, screenCornerY, 10, 50, BLACK);
		DrawRectanglePro({ (float)screenCornerX + 250, (float)screenCornerY + 20, 5, 30 }, {  0, 0}, 8.0f * time, WHITE);
		//std::cout << currentTime << '\n';
		if (time > 65) currentScreen = DEATH;

		player.displayQ();
		player.kbListen(fishSpot, textures.getTexture("popup_bg"), shops, soundsMan, camera, textures);

		if (IsKeyPressed(KEY_E) && !player.checkShopCollision(shops)) {
			currentScreen = INV;
		}
		camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
		if (camera.zoom > 4.5f) camera.zoom = 4.5f;
		if (camera.zoom < 0.2f) camera.zoom = 0.2f;


	break;
	case SHOP:
		if (IsKeyPressed(KEY_E)) {
			currentScreen = GAMEPLAY;
			player.popupQ.erase(player.popupQ.begin(), player.popupQ.end());
		}

		DrawTextureEx(textures.getTexture("shop_bg"), { 0, 0 }, 0, 1.5 * scale_offset, WHITE);


		DrawText("FISH SHOP", 0 + 1, 0 + 1, 75, BLACK);
		DrawText("FISH SHOP", 0 + 1, 0 + 1, 75, RAYWHITE);
		//DrawRectangle(0, 0 + GetScreenHeight()  / 3 * 2, GetScreenWidth(), GetScreenHeight() / 2, RAYWHITE);
		
		DrawTextureEx(textures.getTexture("shopkeeper"), { GetScreenWidth() / 2.0f, 50}, 0, 5 * scale_offset, WHITE);

		DrawRectangleGradientV(0, 0 + GetScreenHeight() / 3 * 2, GetScreenWidth(), GetScreenHeight() / 2, RAYWHITE, DARKGRAY);
		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 }, 0, 2.2 * scale_offset, WHITE);
		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 + 50}, 0, 2.2 * scale_offset, GRAY);
		DrawText(toString(player.gold).c_str(), 0 + (64 * scale_offset), 0 + GetScreenHeight() / 3 * 2 + (12 * scale_offset), 45 * scale_offset, BLACK);
		DrawText(toString(player.silver).c_str(), 0 + (64 * scale_offset), 0 + GetScreenHeight() / 3 * 2 + (12 * scale_offset) + 50, 45 * scale_offset, BLACK);
		switch (currentShopMenu) {
		case NO:
			shop_inv_rec = { (float)GetScreenWidth() / 4, (float)GetScreenHeight() / 3 * 2 + 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };
			shop_buy_rec = { (float)GetScreenWidth() / 4 * 2, (float)GetScreenHeight() / 3 * 2 + 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };
			shop_talk_rec = { (float)GetScreenWidth() / 4 * 3, (float)GetScreenHeight() / 3 * 2 + 8, (float)GetScreenWidth() / 5, (float)GetScreenHeight() / 10 };
			DrawRectangleRounded(shop_inv_rec, 0.5, 4, LIGHTGRAY);
			DrawRectangleRounded(shop_buy_rec, 0.5, 4, LIGHTGRAY);
			DrawRectangleRounded(shop_talk_rec, 0.5, 4, LIGHTGRAY);
			DrawTextEx(GetFontDefault(), "INVENTORY", { shop_inv_rec.x + 20, shop_inv_rec.y + 15 }, 25 * scale_offset, 2, BLACK);
			DrawTextEx(GetFontDefault(), "BUY ITEMS", { shop_buy_rec.x + 20, shop_buy_rec.y + 15 }, 25 * scale_offset, 2, BLACK);
			DrawTextEx(GetFontDefault(), "TALK", { shop_talk_rec.x + 20, shop_talk_rec.y + 15 }, 25 * scale_offset, 2, BLACK);
			if (CheckCollisionPointRec(GetMousePosition(), shop_inv_rec)) {
				DrawRectangleRounded(shop_inv_rec, 0.5, 4, DARKGRAY);
				DrawTextEx(GetFontDefault(), "INVENTORY", { shop_inv_rec.x + 20, shop_inv_rec.y + 15 }, 25 * scale_offset, 2, RAYWHITE);
				if (IsMouseButtonPressed(0)) {
					currentShopMenu = SELL;
				}
			}
			if (CheckCollisionPointRec(GetMousePosition(), shop_buy_rec)) {
				DrawRectangleRounded(shop_buy_rec, 0.5, 4, DARKGRAY);
				DrawTextEx(GetFontDefault(), "BUY ITEMS", { shop_buy_rec.x + 20, shop_buy_rec.y + 15 }, 25 * scale_offset, 2, RAYWHITE);
				if (IsMouseButtonPressed(0)) {
					currentShopMenu = BUY;
				}
			}

			if (CheckCollisionPointRec(GetMousePosition(), shop_talk_rec)) {
				DrawRectangleRounded(shop_talk_rec, 0.5, 4, DARKGRAY);
				DrawTextEx(GetFontDefault(), "TALK", { shop_talk_rec.x + 20, shop_talk_rec.y + 15 }, 25 * scale_offset, 2, RAYWHITE);
				if (IsMouseButtonPressed(0)) {
					currentShopMenu = TALK;
				}
			}

			break;
		case SELL:
			if (IsKeyPressed(KEY_BACKSPACE)) {
				currentShopMenu = NO;
			}
			if (!player.fishInventory.empty()) {
				fish_in_shop_inventory(player, 0, scale_offset);

				if (player.fishInventory.size() > 1) {
					fish_in_shop_inventory(player, 1, scale_offset);

					if (player.fishInventory.size() > 2) {
						fish_in_shop_inventory(player, 2, scale_offset);

						if (player.fishInventory.size() > 3) {
							fish_in_shop_inventory(player, 3, scale_offset);

							if (player.fishInventory.size() > 4) {
								fish_in_shop_inventory(player, 4, scale_offset);

							}
						}
					}

				}
			}
			else {
				DrawText("INVENTORY IS EMPTY", (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, 35 * scale_offset, BLACK);
			}

			break;
		case BUY:
			if (IsKeyPressed(KEY_BACKSPACE)) {
				currentShopMenu = NO;
			}
			if (!player.fishingRods.empty()) {
				fishrod_in_shop_inventory(player, 0, scale_offset, shops, textures);
			}


			break;
		case TALK:

			if (IsKeyPressed(KEY_BACKSPACE)) {
				currentShopMenu = NO;
			}

			if (!player.shopTalkQ.empty()) {
				
				if (!player.shopTalkQ.empty() && currentShopTalk < player.shopTalkQ.size()) {
					//DrawText(shopTalkQ[currentShopTalk].c_str(), (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, 35 * scale_offset, BLACK);
					DrawTextBoxed(GetFontDefault(), player.shopTalkQ[currentShopTalk].c_str(), { (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, GetScreenWidth() / 2.0f, GetScreenHeight() / 3.0f }, 35 * scale_offset, 5, 5, BLACK);

					if (IsMouseButtonPressed(0) || IsKeyPressed(KEY_ENTER)) {
						currentShopTalk++;
					}
				}
				else {
					DrawText("NO MORE DIALOGUE", (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, 35 * scale_offset, BLACK);

				}
			}
			else {
				DrawText("NO MORE DIALOGUE", (float)GetScreenWidth() / 3, (float)GetScreenHeight() / 3 * 2 + 20, 35 * scale_offset, BLACK);
			}
				

			break;
		

		}
	break;
	case INV:

		if (IsKeyPressed(KEY_E)) {
			currentScreen = GAMEPLAY;
			currentMenu = NONE;
		}
		DrawText("INVENTORY", 1, 1, 50, BLACK);
		DrawText("INVENTORY", 0, 0, 50, RAYWHITE);

		DrawText("LEVEL", 1, 76, 30, BLACK);
		DrawText("LEVEL", 0, 75, 30, RAYWHITE);

		DrawText("XP", 1, 120, 30, BLACK);
		DrawText("XP", 0, 120, 30, RAYWHITE);

		DrawText(toString(player.level).c_str(), 120, 70, 50, WHITE);


		switch (player.level) {
		case 1:
			DrawText(toString(player.xp).append(" / 150").c_str(), 50, 120, 30, WHITE);
			break;
		case 2:
			DrawText(toString(player.xp).append(" / 751").c_str(), 50, 120, 30, WHITE);
			break;
		case 3:
			DrawText(toString(player.xp).append(" / 3126").c_str(), 50, 120, 30, WHITE);
			break;
		case 4:
			DrawText(toString(player.xp).append(" / 13259").c_str(), 50, 120, 30, WHITE);
			break;
		case 5:
			DrawText(toString(player.xp).append(" / 38906").c_str(), 50, 120, 30, WHITE);
			break;
		case 6:
			DrawText(toString(player.xp).append(" / INF").c_str(), 50, 120, 30, WHITE);
			break;
		default:
			DrawText(toString(player.xp).append(" / INF").c_str(), 50, 120, 30, WHITE);
			break;
		}



		DrawText("SKILL POINTS", GetScreenWidth() / 8 * 4, 0, 50 * scale_offset, RAYWHITE);
		DrawText(toString(player.skill_points).c_str(), GetScreenWidth() / 8 * 7 + 50, 0, 50*scale_offset, WHITE);

		DrawRectangleGradientV(0, 0 + GetScreenHeight() / 3 * 2, GetScreenWidth(), GetScreenHeight() / 2, RAYWHITE, DARKGRAY);

		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 }, 0, 2.2 * scale_offset, WHITE);
		DrawTextureEx(textures.getTexture("coin"), { 0, (float)GetScreenHeight() / 3 * 2 + 50 }, 0, 2.2 * scale_offset, GRAY);
		DrawText(toString(player.gold).c_str(), 0 + (64 * scale_offset), 0 + GetScreenHeight() / 3 * 2 + (12 * scale_offset), 45 * scale_offset, BLACK);
		DrawText(toString(player.silver).c_str(), 0 + (64 * scale_offset), 0 + GetScreenHeight() / 3 * 2 + (12 * scale_offset) + 50, 45 * scale_offset, BLACK);

		if (!player.fishInventory.empty()) {
			fish_in_inventory(player, 0, scale_offset);

			if (player.fishInventory.size() > 1) {
				fish_in_inventory(player, 1, scale_offset);

				if (player.fishInventory.size() > 2) {
					fish_in_inventory(player, 2, scale_offset);

					if (player.fishInventory.size() > 3) {
						fish_in_inventory(player, 3, scale_offset);

						if (player.fishInventory.size() > 4) {
							fish_in_inventory(player, 4, scale_offset);

						}
					}
				}

			}
		}

		inv_skill_skill = { (float)GetScreenWidth() - 100 , 100 , 100 , 50 };
		inv_skill_luck = { (float)GetScreenWidth() - 100 ,  150 , 100 , 50 };
		DrawRectangleRounded(inv_skill_skill, 0.5, 4, DARKGRAY);
		DrawRectangleRounded(inv_skill_luck, 0.5, 4, DARKGRAY);
		DrawText("Skill", inv_skill_skill.x - 75, inv_skill_skill.y+25, 25 * scale_offset, RAYWHITE);
		DrawText("Luck", inv_skill_luck.x - 75, inv_skill_luck.y+25, 25 * scale_offset, RAYWHITE);
		DrawTextEx(GetFontDefault(), "+", { inv_skill_skill.x + 10, inv_skill_skill.y + 5 }, 20 * scale_offset, 2, RAYWHITE);
		DrawTextEx(GetFontDefault(), "+", { inv_skill_luck.x + 10, inv_skill_luck.y + 5 }, 20 * scale_offset, 2, RAYWHITE);

		if (CheckCollisionPointRec(GetMousePosition(), inv_skill_luck)) {

			DrawRectangleRounded(inv_skill_luck, 0.5, 4, LIGHTGRAY);
			DrawTextEx(GetFontDefault(), "+", { inv_skill_luck.x + 10, inv_skill_luck.y + 5 }, 20, 2, RAYWHITE);
			if (IsMouseButtonPressed(0)) {
				if (player.skill_points > 0) {
					player.luck++;
					player.skill_points--;
				}
			}
		}
		if (CheckCollisionPointRec(GetMousePosition(), inv_skill_skill)) {
			DrawRectangleRounded(inv_skill_skill, 0.5, 4, LIGHTGRAY);
			DrawTextEx(GetFontDefault(), "+", { inv_skill_skill.x + 10, inv_skill_skill.y + 5 }, 20, 2, RAYWHITE);
			if (IsMouseButtonPressed(0)) {
				if (player.skill_points > 0) {
					player.skill++;
					player.skill_points--;
				}
			}
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

	