# CMakeList.txt: Projekt CMake pro FishgameCMAKE, sem přidejte logiku zdrojového
# kódu a definic specifickou pro projekt.
#
cmake_minimum_required (VERSION 3.8)

# Přidejte zdrojový kód ke spustitelnému souboru tohoto projektu.
add_executable (FishgameCMAKE "FishgameCMAKE.cpp" "FishgameCMAKE.h" "CCharacter.h" "CCharacter.cpp" "CTools.cpp"  "App.cpp" "CMap.h" "CMap.cpp" "CTexureManager.cpp" "CFishing.h" "CFishing.cpp" "CPopups.h" "CPopups.cpp" "App.h" "Fish.h" "Fish.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FishgameCMAKE PROPERTY CXX_STANDARD 20)
endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

# TODO: V případě potřeby přidejte testy a cíle instalace.
