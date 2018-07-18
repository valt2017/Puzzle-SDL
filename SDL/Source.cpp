#include <iostream>
#include <fstream>
#include <SDL.h>
#include <map>
#include <vector> //for random shuffle only
#include <algorithm> // for random shuffle only
#include <ctime> // for random shuffle only

// Ported by @valt to SDL from https://www.youtube.com/user/FamTrinli puzzle tutorial in SFML

//figures coordinates
std::map<int, SDL_Rect> Figures;
const int FigureSize = 64;
const int PuzzleSize = 4;
const int EmptyField = 16; // puzzle is from 1..15
// Puzzle board
int puzzle[PuzzleSize][PuzzleSize];

std::map<const char, SDL_Texture *> Textures;
std::map<const char, std::string> Bitmaps;
const int posX = 100, posY = 100;
const int sizeX = 256, sizeY = 256;

int main(int argc, char ** argv) {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	// Error checks
	std::cout << "SDL_Init\n";
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Puzzle", posX, posY, sizeX, sizeY, 0);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow error\n";
	}
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "SDL_CreateRenderer error\n";
	}
	// Load bitmaps
	Bitmaps[0] = "img/15.bmp";
	// Create textures from bitmaps
	for (auto bitmap : Bitmaps) {
		SDL_Surface * bitmapSurface = SDL_LoadBMP(bitmap.second.c_str());
		// create texture 
		Textures[bitmap.first] = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
		if (Textures[bitmap.first] == NULL) {
			std::cout << bitmap.second.c_str() <<" SDL_CreateTextureFromSurface error\n";
		}
		else
			std::cout << bitmap.second.c_str() << " SDL_CreateTextureFromSurface OK\n";
		SDL_FreeSurface(bitmapSurface);
	}
	// Random shuffle
	srand(time(0));
	std::vector<int> Items = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
	std::random_shuffle(Items.begin(), Items.end());
	// Load random puzzle, coordinates for figures from bitmap
	int Index = 1;
	for (int i = 0; i < PuzzleSize; i++)
		for (int j = 0; j < PuzzleSize; j++) {
			puzzle[i][j] = Items[Index-1];
			Figures[Index++] = SDL_Rect{ j*FigureSize, i*FigureSize, FigureSize, FigureSize };
		}
	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
			/* Mouse button down */
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_Point mousePos;
				mousePos.x = e.motion.x / FigureSize;
				mousePos.y = e.motion.y / FigureSize;
				if (e.button.button == SDL_BUTTON_LEFT) {
					std::cout << mousePos.x << "," << mousePos.y << "->" << puzzle[mousePos.y][mousePos.x] << std::endl;
					if (mousePos.y + 1 < PuzzleSize && puzzle[mousePos.y + 1][mousePos.x] == EmptyField) {
						puzzle[mousePos.y + 1][mousePos.x] = puzzle[mousePos.y][mousePos.x];
						puzzle[mousePos.y][mousePos.x] = EmptyField;
					}
					if (mousePos.x + 1 < PuzzleSize && puzzle[mousePos.y][mousePos.x + 1] == EmptyField) {
						puzzle[mousePos.y][mousePos.x + 1] = puzzle[mousePos.y][mousePos.x];
						puzzle[mousePos.y][mousePos.x] = EmptyField;
					}
					if (mousePos.y != 0 && puzzle[mousePos.y - 1][mousePos.x] == EmptyField) {
						puzzle[mousePos.y - 1][mousePos.x] = puzzle[mousePos.y][mousePos.x];
						puzzle[mousePos.y][mousePos.x] = EmptyField;
					}
					if (mousePos.x != 0 && puzzle[mousePos.y][mousePos.x - 1] == EmptyField) {
						puzzle[mousePos.y][mousePos.x - 1] = puzzle[mousePos.y][mousePos.x];
						puzzle[mousePos.y][mousePos.x] = EmptyField;
					}
				}
			}
		}
		SDL_RenderClear(renderer);
		// figures texture
		auto texture = Textures.find(0);
		if (texture != Textures.end()) {
			for (int i = 0; i < PuzzleSize; i++)
				for (int j = 0; j < PuzzleSize; j++) {
					auto it = Figures.find(puzzle[j][i]);
					if (it != Figures.end()) {
						SDL_Rect DestR;
						DestR.x = i*FigureSize;
						DestR.y = j*FigureSize;
						DestR.h = FigureSize;
						DestR.w = FigureSize;
						SDL_RenderCopy(renderer, texture->second, &it->second, &DestR);
					}
				}
		}
		SDL_RenderPresent(renderer);
	}
	for (auto texture : Textures) {
		SDL_DestroyTexture(texture.second);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	std::cout << "SDL_Quit\n";
	return 0;
}