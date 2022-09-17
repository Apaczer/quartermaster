//
// QUARTERMASTER: Collect the falling food cans!
//
// Copyleft <--(C)--> Copyright 2019 Hi-Ban
//
// Original concept and graphics by wargaming.net
// Game code, sound and part of the graphic assets by Hi-Ban. 
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <string.h>
#include <string>
#include <sys/stat.h>

#include <iostream>
#include <ctime>

#include "SFont.h"

//---------------------------------------------------//

SDL_Surface *screen;
SDL_Surface *allcans;
SDL_Surface *background;
SDL_Surface *borders;
SDL_Surface *help;
SDL_Surface *p_upleft;
SDL_Surface *p_upleft_b;
SDL_Surface *p_upright;
SDL_Surface *p_upright_b;
SDL_Surface *p_downleft;
SDL_Surface *p_downleft_b;
SDL_Surface *p_downright;
SDL_Surface *p_downright_b;

Mix_Chunk *sound_lvl_01 = NULL;
Mix_Chunk *sound_lvl_02 = NULL;
Mix_Chunk *sound_lvl_03 = NULL;
Mix_Chunk *sound_lvl_04 = NULL;
Mix_Chunk *sound_lvl_05 = NULL;
Mix_Chunk *sound_lvl_06 = NULL;
Mix_Chunk *sound_lvl_07 = NULL;
Mix_Chunk *sound_lvl_08 = NULL;
Mix_Chunk *sound_lvl_09 = NULL;
Mix_Chunk *sound_lvl_10 = NULL;
Mix_Chunk *sound_lvl_11 = NULL;
Mix_Chunk *sound_move = NULL;
Mix_Chunk *sound_good = NULL;
Mix_Chunk *sound_bad = NULL;
Mix_Chunk *sound_miss = NULL;
Mix_Chunk *sound_gameover = NULL;
Mix_Chunk *sound_music = NULL;

static SFont_Font* font;
static SDL_Surface *font_bitmap_surface = NULL;

//init global game variables
int quit = 0;
int gameab = 0; // 0 - game A / 1 - game B
int gameover = 0;
int quittomenu = 0;
int pausegame = 0;
int score = 0;
int level = 1;
int highestlevel = 1;
int misses = 0;
int speed = 928;
int cans[5][2] = {{8, 8}, {8, 8}, {8, 8}, {8, 8},{8, 8}}; // {color, position} - [0 = white can / 1 = black can] - [0 = upleft / 1 = upright / 2 = downleft / 3 = downright] // 8 is just a forced non-valid value so it treats it as "no-cans"
int player = 0; // 0 = upleft / 1 = upright / 2 = downleft / 3 = downright
int dirty = 0;
int can_coordx = 1;
int can_coordy = 14;
int dest_coordx = 1;
int dest_coordy = 38;
int can_width = 24;
int can_height = 24;
int alternate = 0;
int dropped = 0;
int badmiss = 0;
int blink = 0;
int hiscore_a = 0;
int hiscore_b = 0;
int hiscore_aplus = 0;
int hiscore_bplus = 0;
int konamicode = 0; // what's this thing?
uint32_t curr_time = 0;
uint32_t old_time = 0;
std::string homedir = getenv("HOME");

void loadGraphics() {
	allcans = IMG_Load("assets/trans_cans.png");
	background = IMG_Load("assets/background.png");
	borders = IMG_Load("assets/borders.png");
	help = IMG_Load("assets/help.png");
	p_upleft = IMG_Load("assets/player_left_up.png");
	p_upleft_b = IMG_Load("assets/player_left_up_b.png");
	p_upright = IMG_Load("assets/player_right_up.png");
	p_upright_b = IMG_Load("assets/player_right_up_b.png");
	p_downleft = IMG_Load("assets/player_left_down.png");
	p_downleft_b = IMG_Load("assets/player_left_down_b.png");
	p_downright = IMG_Load("assets/player_right_down.png");
	p_downright_b = IMG_Load("assets/player_right_down_b.png");

	if ((!allcans) || (!background) || (!borders) || (!help) || (!p_upleft) || (!p_upright) || (!p_downleft) || (!p_downright) || (!p_upleft_b) || (!p_upright_b) || (!p_downleft_b) || (!p_downright_b)) {
		printf("Graphics failed loading\n");
		exit(1);
	} else {
		printf("Graphics loaded successfully\n");
	}
}

void loadSounds() {
	sound_move = Mix_LoadWAV("assets/move.wav");
	sound_good = Mix_LoadWAV("assets/good.wav");
	sound_bad = Mix_LoadWAV("assets/bad.wav");
	sound_miss = Mix_LoadWAV("assets/miss.wav");
	sound_gameover = Mix_LoadWAV("assets/gameover.wav");
	sound_music = Mix_LoadWAV("assets/music.wav");
    sound_lvl_01 = Mix_LoadWAV("assets/lvl_01.wav");
	sound_lvl_02 = Mix_LoadWAV("assets/lvl_02.wav");
	sound_lvl_03 = Mix_LoadWAV("assets/lvl_03.wav");
	sound_lvl_04 = Mix_LoadWAV("assets/lvl_04.wav");
	sound_lvl_05 = Mix_LoadWAV("assets/lvl_05.wav");
	sound_lvl_06 = Mix_LoadWAV("assets/lvl_06.wav");
	sound_lvl_07 = Mix_LoadWAV("assets/lvl_07.wav");
	sound_lvl_08 = Mix_LoadWAV("assets/lvl_08.wav");
	sound_lvl_09 = Mix_LoadWAV("assets/lvl_09.wav");
	sound_lvl_10 = Mix_LoadWAV("assets/lvl_10.wav");
	sound_lvl_11 = Mix_LoadWAV("assets/lvl_11.wav");
    
    if ((!sound_move) || (!sound_good) || (!sound_bad) || (!sound_miss) || (!sound_gameover) || (!sound_music) || (!sound_lvl_01) || (!sound_lvl_02) || (!sound_lvl_03) || (!sound_lvl_04) || (!sound_lvl_05) || (!sound_lvl_06) || (!sound_lvl_07) || (!sound_lvl_08) || (!sound_lvl_09) || (!sound_lvl_10) || (!sound_lvl_11)) {
		printf("Sounds failed loading\n");
		exit(1);
	} else {
		printf("Sounds loaded successfully\n");
	}
}

void freeSounds() {
	Mix_FreeChunk(sound_lvl_01);
	Mix_FreeChunk(sound_lvl_02);
	Mix_FreeChunk(sound_lvl_03);
	Mix_FreeChunk(sound_lvl_04);
	Mix_FreeChunk(sound_lvl_05);
	Mix_FreeChunk(sound_lvl_06);
	Mix_FreeChunk(sound_lvl_07);
	Mix_FreeChunk(sound_lvl_08);
	Mix_FreeChunk(sound_lvl_09);
	Mix_FreeChunk(sound_lvl_10);
	Mix_FreeChunk(sound_lvl_11);
	Mix_FreeChunk(sound_move);
	Mix_FreeChunk(sound_good);
	Mix_FreeChunk(sound_bad);
	Mix_FreeChunk(sound_miss);
	Mix_FreeChunk(sound_gameover);
	Mix_FreeChunk(sound_music);
}

void init_font() { // loads SFont bitmap font
    SDL_FreeSurface(font_bitmap_surface);
    font_bitmap_surface = IMG_Load("assets/font_watch.png");
    if (!font_bitmap_surface) {
        fprintf(stderr, "font load error\n");
        exit(1);
    }
    font = SFont_InitFont(font_bitmap_surface);
    if (!font) {
        fprintf(stderr, "font init error\n");
        exit(1);
    }
}

void saveHiscores(){ // writes high scores to a file
	if ((gameab == 0) && (konamicode != 10) && (score >= hiscore_a)) {
		hiscore_a = score;
	} else if ((gameab == 1) && (konamicode != 10) && (score >= hiscore_b)) {
		hiscore_b = score;
	} else if ((gameab == 0) && (konamicode == 10) && (score >= hiscore_aplus)) {
		hiscore_aplus = score;
	} else if ((gameab == 1) && (konamicode == 10) && (score >= hiscore_bplus)) {
		hiscore_bplus = score;
	}
	std::string hiscorefile = (homedir + "/.quartermaster/hiscore.hi");
	FILE * cfile;
    cfile = fopen(hiscorefile.c_str(), "w");
    if (cfile == NULL) {
		printf("Failed to open hi-score file for writing.\n");
		return;
	}
    fprintf(cfile,
		"GAMEA %d\n"
		"GAMEB %d\n"
		"GAME+A %d\n"
		"GAME+B %d\n",
		hiscore_a,
		hiscore_b,
		hiscore_aplus,
		hiscore_bplus);
    fclose(cfile);
}

void loadHiscores(){ // reads high scores from a file
	std::string hiscorefile = (homedir + "/.quartermaster/hiscore.hi");
	FILE * cfile;
	char line[4096];
    cfile = fopen(hiscorefile.c_str(), "r");
    if (cfile == NULL) {
		printf("Failed to open hi-score file for reading.\n");
		return;
	}
	while (fgets(line, sizeof(line), cfile)) {
		char *arg = strchr(line, ' ');
		int value;
		if (!arg) {
			continue;
		}
		*arg = '\0';
		arg++;
		if (!strcmp(line, "GAMEA")) {
			sscanf(arg, "%d", &value);
			hiscore_a = value;
		} else if (!strcmp(line, "GAMEB")) {
			sscanf(arg, "%d", &value);
			hiscore_b = value;
		} else if (!strcmp(line, "GAME+A")) {
			sscanf(arg, "%d", &value);
			hiscore_aplus = value;
		} else if (!strcmp(line, "GAME+B")) {
			sscanf(arg, "%d", &value);
			hiscore_bplus = value;
		}
	}
	fclose(cfile);
}

void getCanCoords(int number, int color, int position){ // determines the coordinates of source and destination surfaces for the can sprites.
	can_coordx = 1;
	can_coordy = 14;
	dest_coordx = 1;
	dest_coordy = 54;
	can_width = 24;
	can_height = 24;

	if (number == 1) {
		can_coordx += 23;
		can_coordy += 9;
		dest_coordx += 23;
		dest_coordy += 9;
	} else if (number == 2) {
		can_coordx += 46;
		can_coordy += 16;
		dest_coordx += 46;
		dest_coordy += 16;
	} else if (number == 3) {
		can_coordx += 69;
		can_coordy += 27;
		dest_coordx += 69;
		dest_coordy += 27;
	}

	if ((position == 2) || (position == 3)) {
		can_coordy += 48;
		dest_coordy += 48;
	}

	if (number == 4) {
		can_coordx = 76;
		can_coordy = 137;
		dest_coordx = 76;
		dest_coordy = 177;
		can_width = 32;
		can_height = 24;
	}

	if (color == 0) {
		can_coordy += 24;
	}

	if ((position == 1) || (position == 3)) {
		can_coordx = 320 - can_width - can_coordx;
		dest_coordx = 320 - can_width - dest_coordx;
	}

	if ((position > 3) || (color > 1)) {
		can_coordx = 0;
		can_coordy = 0;
		dest_coordx = 0;
		dest_coordy = 0;
		can_width = 0;
		can_height = 0;
	}
}

void writeScore(SDL_Surface *surface, int score) {
    char buffer[8];
    sprintf(buffer, "%d", score);
    if (score < 10) { // single digit (1 digit - 15 pixels)
    	SFont_Write(surface, font, 299, 19, buffer); // paint current score
    } else if ((score >= 10) && (score < 100)) { // two digits
    	SFont_Write(surface, font, 284, 19, buffer); // paint current score
    } else if ((score >= 100) && (score < 1000)) { // three digits
    	SFont_Write(surface, font, 269, 19, buffer); // paint current score
    } else if ((score >= 1000) && (score < 10000)) { // four digits
    	SFont_Write(surface, font, 254, 19, buffer); // paint current score
    }
    if (((gameab == 0) && (konamicode != 10) && (score >= hiscore_a)) || ((gameab == 1) && (konamicode != 10) && (score >= hiscore_b)) || ((gameab == 0) && (konamicode == 10) && (score >= hiscore_aplus)) || ((gameab == 1) && (konamicode == 10) && (score >= hiscore_bplus))) { // if current score is equal or higher than hi-score
	    SDL_Rect hiscor1;
		hiscor1.x = 218;
		hiscor1.y = 5;
		hiscor1.w = 37;
		hiscor1.h = 12;
		SDL_Rect hiscor2;
		hiscor2.x = 218;
		hiscor2.y = 21;
		hiscor2.w = 37;
		hiscor2.h = 12;
		SDL_BlitSurface(allcans, &hiscor1, screen, &hiscor2); // if current score is equal or higher than the current mode high-score, paint "hi-score" text   
	}	    
}

void writeHiScore() {
    char buffer[8];
    if ((gameab == 0) && (konamicode != 10)) {
    	sprintf(buffer, "%d", hiscore_a);
    	if (hiscore_a < 10) { // single digit
	    	SFont_Write(screen, font, 299, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_a >= 10) && (hiscore_a < 100)) { // two digits
	    	SFont_Write(screen, font, 284, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_a >= 100) && (hiscore_a < 1000)) { // three digits
	    	SFont_Write(screen, font, 269, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_a >= 1000) && (hiscore_a < 10000)) { // four digits
	    	SFont_Write(screen, font, 254, 19, buffer); // paint "game a" high score
	    }
    } else if ((gameab == 1) && (konamicode != 10)) {
    	sprintf(buffer, "%d", hiscore_b);
    	if (hiscore_b < 10) { // single digit
	    	SFont_Write(screen, font, 299, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_b >= 10) && (hiscore_b < 100)) { // two digits
	    	SFont_Write(screen, font, 284, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_b >= 100) && (hiscore_b < 1000)) { // three digits
	    	SFont_Write(screen, font, 269, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_b >= 1000) && (hiscore_b < 10000)) { // four digits
	    	SFont_Write(screen, font, 254, 19, buffer); // paint "game b" high score
	    }
    } else if ((gameab == 0) && (konamicode == 10)) {
    	sprintf(buffer, "%d", hiscore_aplus);
    	if (hiscore_aplus < 10) { // single digit
	    	SFont_Write(screen, font, 299, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_aplus >= 10) && (hiscore_aplus < 100)) { // two digits
	    	SFont_Write(screen, font, 284, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_aplus >= 100) && (hiscore_aplus < 1000)) { // three digits
	    	SFont_Write(screen, font, 269, 19, buffer); // paint "game a" high score
	    } else if ((hiscore_aplus >= 1000) && (hiscore_aplus < 10000)) { // four digits
	    	SFont_Write(screen, font, 254, 19, buffer); // paint "game a" high score
	    }
    } else if ((gameab == 1) && (konamicode == 10)) {
    	sprintf(buffer, "%d", hiscore_bplus);
    	if (hiscore_bplus < 10) { // single digit
	    	SFont_Write(screen, font, 299, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_bplus >= 10) && (hiscore_bplus < 100)) { // two digits
	    	SFont_Write(screen, font, 284, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_bplus >= 100) && (hiscore_bplus < 1000)) { // three digits
	    	SFont_Write(screen, font, 269, 19, buffer); // paint "game b" high score
	    } else if ((hiscore_bplus >= 1000) && (hiscore_bplus < 10000)) { // four digits
	    	SFont_Write(screen, font, 254, 19, buffer); // paint "game b" high score
	    }
    }
	SDL_Rect hiscor1;
	hiscor1.x = 218;
	hiscor1.y = 5;
	hiscor1.w = 37;
	hiscor1.h = 12;
	SDL_Rect hiscor2;
	hiscor2.x = 218;
	hiscor2.y = 21;
	hiscor2.w = 37;
	hiscor2.h = 12;
	SDL_BlitSurface(allcans, &hiscor1, screen, &hiscor2); // paint "hi-score" text   
}

int checkStep(){
	if (cans[4][1] == player) { // if player grabbed a can
		if (cans[4][0] == 0) {
			dropped = 0;
			badmiss = 0;
			return 1; // can is white (+1 point)
		} else if (cans[4][0] == 1) {
			dropped = 0;
			badmiss = 1;
			return 2; // can is black (-3 points)
		} else {
			badmiss = 0;
			return 0; // there's no can (nothing happens)
		}
	} else { // if player didnt grab a can
		if (cans[4][0] == 0) {
			dropped = 1;
			badmiss = 1;
			return 3; // white can fell to the floor (+1 misses)
		} else if (cans[4][0] == 1) {
			dropped = 1;
			badmiss = 0;
			return 4; // black can fell to the floor (nothing happens)
		} else {
			badmiss = 0;
			return 0; // there's no can (nothing happens)
		}
	}
}

void nextStep() {
	for (int i = 4; i > 0; --i) { //move the existing cans one step forward
		cans[i][0] = cans[i-1][0];
		cans[i][1] = cans[i-1][1];
	}
	if (alternate == 0){ // no can
		cans[0][1] = 8;
		cans[0][0] = 8;
		alternate = 1;
	} else if (alternate == 1) {
		cans[0][1] = ((rand() % 4)); // determine a random position for the new can
		if (gameab == 0) {
			cans[0][0] = 0;
		} else if (gameab == 1) {
			int anum = ((rand() % 30));
			if (anum >= level) {
				cans[0][0] = 0; //can is white
			} else {
				cans[0][0] = 1; //can is black
			}
		}	
		alternate = 0;
	}

	int levelup = 5; // level goes up every X score. speed increases every level, but speed and level do not decrease, even if the score decreases.
	if (konamicode == 10) {
		levelup = 5; // the "game plus" modes were originally intended to level up faster than normal modes, but now normal and plus modes level up equally.
	}
	if ((score >= levelup) && (highestlevel == 1)) {
		level = 2;
		highestlevel = 2;
		speed = 774;
	} else if ((score >= (levelup * 2)) && (highestlevel == 2)) {
		level = 3;
		highestlevel = 3;
		speed = 644;
	} else if ((score >= (levelup * 3)) && (highestlevel == 3)) {
		level = 4;
		highestlevel = 4;
		speed = 537;
	} else if ((score >= (levelup * 4)) && (highestlevel == 4)) {
		level = 5;
		highestlevel = 5;
		speed = 448;
	} else if ((score >= (levelup * 5)) && (highestlevel == 5)) {
		level = 6;
		highestlevel = 6;
		speed = 373;
	} else if ((score >= (levelup * 6)) && (highestlevel == 6)) {
		level = 7;
		highestlevel = 7;
		speed = 311;
	} else if ((score >= (levelup * 7)) && (highestlevel == 7)) {
		level = 8;
		highestlevel = 8;
		speed = 260;
	} else if ((score >= (levelup * 8)) && (highestlevel == 8)) {
		level = 9;
		highestlevel = 9;
		speed = 216;
	} else if ((score >= (levelup * 9)) && (highestlevel == 9)) {
		level = 10;
		highestlevel = 10;
		speed = 180;
	} else if ((score >= (levelup * 10)) && (highestlevel == 10) && (konamicode == 10)) { // extra harder speed level only for secret "game plus" modes
		level = 11;
		highestlevel = 11;
		speed = 160;
	}
}

void redraw(){
	SDL_FillRect(screen, NULL, 0x000000); // paint screen black

	SDL_Rect borda;
	borda.x = 0;
	borda.y = 0;
	borda.w = 320;
	borda.h = 16;
	SDL_Rect bordb;
	bordb.x = 0;
	bordb.y = 0;
	bordb.w = 320;
	bordb.h = 16;
	SDL_BlitSurface(borders, &borda, screen, &bordb); // paint upper border

	SDL_Rect bordc;
	bordc.x = 0;
	bordc.y = 16;
	bordc.w = 320;
	bordc.h = 16;
	SDL_Rect bordd;
	bordd.x = 0;
	bordd.y = 224;
	bordd.w = 320;
	bordd.h = 16;
	SDL_BlitSurface(borders, &bordc, screen, &bordd); // paint lower border

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 16;
	rect.w = 320;
	rect.h = 208;

	if (badmiss == 0){ // paint normal character
		if (player == 0) { // 0 = upleft / 1 = upright / 2 = downleft / 3 = downright
			SDL_BlitSurface(p_upleft, NULL, screen, &rect);
		} else if (player == 1) {
			SDL_BlitSurface(p_upright, NULL, screen, &rect);
		} else if (player == 2) {
			SDL_BlitSurface(p_downleft, NULL, screen, &rect);
		} else if (player == 3) {
			SDL_BlitSurface(p_downright, NULL, screen, &rect);
		}
	} else if (badmiss == 1) { // paint sad character (character gets sad if he misses a white can or grabs a black can)
		if (player == 0) { // 0 = upleft / 1 = upright / 2 = downleft / 3 = downright
			SDL_BlitSurface(p_upleft_b, NULL, screen, &rect);
		} else if (player == 1) {
			SDL_BlitSurface(p_upright_b, NULL, screen, &rect);
		} else if (player == 2) {
			SDL_BlitSurface(p_downleft_b, NULL, screen, &rect);
		} else if (player == 3) {
			SDL_BlitSurface(p_downright_b, NULL, screen, &rect);
		}
	}	

	for (int i = 0; i < 5; ++i) {
		getCanCoords(i, cans[i][0], cans[i][1]);
		SDL_Rect rect1;
		rect1.x = can_coordx;
		rect1.y = can_coordy;
		rect1.w = can_width;
		rect1.h = can_height;
		SDL_Rect rect2;
		rect2.x = dest_coordx;
		rect2.y = dest_coordy;
		rect2.w = can_width;
		rect2.h = can_height;
		if ((i < 4) || ((i == 4) && (dropped == 1) && (blink == 0))){
			SDL_BlitSurface(allcans, &rect1, screen, &rect2); // paint cans
		}
	}

	if ((misses >= 1) && (misses <=3)) {
		SDL_Rect rect3;
		rect3.x = 98;
		rect3.y = 184;
		rect3.w = 28 + (29 * misses);
		rect3.h = 24;
		SDL_Rect rect4;
		rect4.x = 98;
		rect4.y = 200;
		rect4.w = 28 + (29 * misses);
		rect4.h = 24;
		SDL_BlitSurface(allcans, &rect3, screen, &rect4); // paint "miss" cans
	}

	if (pausegame == 1){
		SDL_Rect rect5;
		rect5.x = 5;
		rect5.y = 184;
		rect5.w = 36;
		rect5.h = 24;
		SDL_Rect rect6;
		rect6.x = 5;
		rect6.y = 200;
		rect6.w = 36;
		rect6.h = 24;
		SDL_BlitSurface(allcans, &rect5, screen, &rect6); // paint pause text
	}

	if (gameab == 0) { // game A
		SDL_Rect rect7a;
		rect7a.x = 140;
		rect7a.y = 7;
		rect7a.w = 40;
		rect7a.h = 12;
		SDL_Rect rect8a;
		rect8a.x = 4;
		rect8a.y = 23;
		rect8a.w = 40;
		rect8a.h = 12;
		if (konamicode == 10) {
			rect7a.w = 47;
			rect8a.w = 47;
		}
		SDL_BlitSurface(allcans, &rect7a, screen, &rect8a); // paint upper "game a" text
	} else if (gameab == 1) { // game B
		SDL_Rect rect7b;
		rect7b.x = 140;
		rect7b.y = 19;
		rect7b.w = 40;
		rect7b.h = 12;
		SDL_Rect rect8b;
		rect8b.x = 4;
		rect8b.y = 35;
		rect8b.w = 40;
		rect8b.h = 12;
		if (konamicode == 10) {
			rect7b.w = 47;
			rect8b.w = 47;
		}
		SDL_BlitSurface(allcans, &rect7b, screen, &rect8b); // paint upper "game b" text
	}

	if (gameover == 1) {
		SDL_Rect rect9;
		rect9.x = 255;
		rect9.y = 184;
		rect9.w = 59;
		rect9.h = 24;
		SDL_Rect rect10;
		rect10.x = 255;
		rect10.y = 200;
		rect10.w = 59;
		rect10.h = 24;
		SDL_BlitSurface(allcans, &rect9, screen, &rect10); // paint "game over" text
	}

	writeScore(screen, score);
}

void redrawMenu(){
	SDL_FillRect(screen, NULL, 0x000000); // paint screen black

	SDL_Rect borda;
	borda.x = 0;
	borda.y = 0;
	borda.w = 320;
	borda.h = 16;
	SDL_Rect bordb;
	bordb.x = 0;
	bordb.y = 0;
	bordb.w = 320;
	bordb.h = 16;
	SDL_BlitSurface(borders, &borda, screen, &bordb); // paint upper border

	SDL_Rect bordc;
	bordc.x = 0;
	bordc.y = 16;
	bordc.w = 320;
	bordc.h = 16;
	SDL_Rect bordd;
	bordd.x = 0;
	bordd.y = 224;
	bordd.w = 320;
	bordd.h = 16;
	SDL_BlitSurface(borders, &bordc, screen, &bordd); // paint lower border

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 16;
	rect.w = 320;
	rect.h = 208;
	SDL_BlitSurface(background, NULL, screen, &rect); // paint background

	SDL_Rect rect1;
	rect1.x = 104;
	rect1.y = 65;
	rect1.w = 112;
	rect1.h = 38;
	SDL_Rect rect2;
	rect2.x = 104;
	rect2.y = 111;
	rect2.w = 112;
	rect2.h = 38;
	SDL_BlitSurface(allcans, &rect1, screen, &rect2); // paint menu text

	SDL_Rect rect3;
	rect3.x = 132;
	rect3.y = 39;
	if (gameab == 1) {
		rect3.y = 52;
	}
	rect3.w = 54;
	rect3.h = 12;
	SDL_Rect rect4;
	rect4.x = 132;
	rect4.y = 98;
	rect4.w = 54;
	rect4.h = 12;
	SDL_BlitSurface(allcans, &rect3, screen, &rect4); // paint "game a" / "game b" menu text

	if (gameab == 0) { // game A
		SDL_Rect rect7a;
		rect7a.x = 140;
		rect7a.y = 7;
		rect7a.w = 40;
		rect7a.h = 12;
		SDL_Rect rect8a;
		rect8a.x = 4;
		rect8a.y = 23;
		rect8a.w = 40;
		rect8a.h = 12;
		if (konamicode == 10) {
			rect7a.w = 47;
			rect8a.w = 47;
		}
		SDL_BlitSurface(allcans, &rect7a, screen, &rect8a); // paint upper "game a" text
	} else if (gameab == 1) { // game B
		SDL_Rect rect7b;
		rect7b.x = 140;
		rect7b.y = 19;
		rect7b.w = 40;
		rect7b.h = 12;
		SDL_Rect rect8b;
		rect8b.x = 4;
		rect8b.y = 35;
		rect8b.w = 40;
		rect8b.h = 12;
		if (konamicode == 10) {
			rect7b.w = 47;
			rect8b.w = 47;
		}
		SDL_BlitSurface(allcans, &rect7b, screen, &rect8b); // paint upper "game b" text
	}

	writeHiScore();
}

void flushKeyEvents(){ // used to discard all pending key events (usually called after a long SDL_Delay)
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					default:
						break;
				}
			default:
				break;
		}
	}
}

void missAnimation(){ // playts the "miss" animation (broken can blinking)
	SDL_Event event;
	blink = 1;
	int i = 0;
	while (i < 21) {
		if (pausegame == 0){
			if ((i == 3) || (i == 9) || (i == 15)) {
				blink = 1;
			} else if ((i == 0) || (i == 6) || (i == 12) || (i == 18)) {
				blink = 0;
			}
		}
		redraw();
		SDL_Flip(screen);
		SDL_Delay(100);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_RETURN:	// Start button - Pause
							if (pausegame == 0){
								pausegame = 1;
								Mix_PlayChannel(-1, sound_move, 0);
							} else if (pausegame == 1) {
								pausegame = 0;
								Mix_PlayChannel(-1, sound_move, 0);
							}					
							dirty = 1;
							break;
						case SDLK_ESCAPE:	// Select button - Exit to menu when paused
							if (pausegame == 1) {
								quittomenu = 1;	
							}
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
		if(pausegame == 0) {
			i++;
		} else if (quittomenu == 1) {
			return;
		}
	}		
	blink = 0;
	curr_time = SDL_GetTicks();
	old_time = curr_time;
	if (misses >= 3) {
		gameover = 1; // if player accumulates 3 misses, game is over
	}
}

void iterate(){
	nextStep();
	if ((checkStep() == 0) || (checkStep() == 4)){
		// nothing happens
		if (level == 1) {
			Mix_PlayChannel(-1, sound_lvl_01, 0);
		} else if (level == 2) {
			Mix_PlayChannel(-1, sound_lvl_02, 0);
		} else if (level == 3) {
			Mix_PlayChannel(-1, sound_lvl_03, 0);
		} else if (level == 4) {
			Mix_PlayChannel(-1, sound_lvl_04, 0);
		} else if (level == 5) {
			Mix_PlayChannel(-1, sound_lvl_05, 0);
		} else if (level == 6) {
			Mix_PlayChannel(-1, sound_lvl_06, 0);
		} else if (level == 7) {
			Mix_PlayChannel(-1, sound_lvl_07, 0);
		} else if (level == 8) {
			Mix_PlayChannel(-1, sound_lvl_08, 0);
		} else if (level == 9) {
			Mix_PlayChannel(-1, sound_lvl_09, 0);
		} else if (level == 10) {
			Mix_PlayChannel(-1, sound_lvl_10, 0);
		} else if (level == 11) {
			Mix_PlayChannel(-1, sound_lvl_11, 0);
		}
	} else if (checkStep() == 1){
		score++; // add one point to score
		Mix_PlayChannel(-1, sound_good, 0);
	} else if (checkStep() == 2){
		score = score - 3; // subtract 3 from score
		if (score < 0){
			score = 0; //if score goes below zero, score is zero
		}
		Mix_PlayChannel(-1, sound_bad, 0);
	} else if (checkStep() == 3){
		misses++; // add one miss
		Mix_PlayChannel(-1, sound_miss, 0);
		missAnimation();
	} 
}

void startNewGame() {
	// reset game variables to initial state
	quit = 0;
	gameover = 0;
	pausegame = 0;
	quittomenu = 0;
	score = 0;
	level = 1;
	highestlevel = 1;
	misses = 0;
	speed = 928;
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 2; ++j)
		{
			cans[i][j] = 8;
		}
	}
	player = 0;
	dirty = 0;
	alternate = 0;
	dropped = 0;
	badmiss = 0;
	blink = 0;

	Mix_PlayChannel(-1, sound_music, 0);
	redraw();
	SDL_Flip(screen);
	SDL_Delay(1500);
	flushKeyEvents();

	curr_time = SDL_GetTicks();
	old_time = curr_time;

    SDL_Event event;
    while (gameover == 0) {
        dirty = 0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_UP: // upleft
							if ((player != 0) && (pausegame == 0)) {
								Mix_PlayChannel(-1, sound_move, 0);
								player = 0;
								dirty = 1;
							}
							break;
						case SDLK_DOWN: // downleft
							if ((player != 2) && (pausegame == 0)) {
								Mix_PlayChannel(-1, sound_move, 0);
								player = 2;
								dirty = 1;
							}
							break;
						case SDLK_LSHIFT: // upright
							if ((player != 1) && (pausegame == 0)) {
								Mix_PlayChannel(-1, sound_move, 0);
								player = 1;
								dirty = 1;
							}
							break;
						case SDLK_LCTRL: // downright
							if ((player != 3) && (pausegame == 0)) {
								Mix_PlayChannel(-1, sound_move, 0);
								player = 3;
								dirty = 1;
							}
							break;
						case SDLK_RETURN:	// Start button - Pause/unpause
							if (pausegame == 0){
								pausegame = 1;
								Mix_PlayChannel(-1, sound_move, 0);
								dirty = 1;
							} else if (pausegame == 1) {
								pausegame = 0;
								curr_time = SDL_GetTicks();
								old_time = curr_time;
								Mix_PlayChannel(-1, sound_move, 0);
								dirty = 1;
							}					
							break;
						case SDLK_ESCAPE:	// Select button - Exit to menu when paused
							if (pausegame == 1) {
								quittomenu = 1;
								dirty = 1;
							}
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
		SDL_Delay(0);
		if (pausegame == 0) {
			curr_time = SDL_GetTicks();
			if (curr_time >= old_time + speed) {
				old_time = curr_time;
				iterate();
				dirty = 1;
			}	
		}
		if (quittomenu == 1) {
			return;
		}
		if (dirty == 1) {
			redraw();
			SDL_Flip(screen);
		}
	}
	saveHiscores();
	SDL_Delay(50);
	redraw();
	SDL_Flip(screen);
	Mix_PlayChannel(-1, sound_gameover, 0);
	SDL_Delay(600);
	flushKeyEvents();
	while (quittomenu == 0){
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_RETURN:	// Start button - Return to main menu
						case SDLK_ESCAPE: // Select button
						case SDLK_LCTRL: // B button
						case SDLK_LALT: // A button
						case SDLK_LSHIFT: // X button
						case SDLK_SPACE : // Y button
							quittomenu = 1;					
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
		SDL_Delay(0);
	}
}

void showHelp() {
	int goback = 0;
	SDL_FillRect(screen, NULL, 0x000000);

	SDL_Rect borda;
	borda.x = 0;
	borda.y = 0;
	borda.w = 320;
	borda.h = 16;
	SDL_Rect bordb;
	bordb.x = 0;
	bordb.y = 0;
	bordb.w = 320;
	bordb.h = 16;
	SDL_BlitSurface(borders, &borda, screen, &bordb);

	SDL_Rect bordc;
	bordc.x = 0;
	bordc.y = 16;
	bordc.w = 320;
	bordc.h = 16;
	SDL_Rect bordd;
	bordd.x = 0;
	bordd.y = 224;
	bordd.w = 320;
	bordd.h = 16;
	SDL_BlitSurface(borders, &bordc, screen, &bordd);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 16;
	rect.w = 320;
	rect.h = 208;
	SDL_BlitSurface(help, NULL, screen, &rect);
	SDL_Flip(screen);

    SDL_Event event;
    while (goback == 0) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_LSHIFT: // release X button
						case SDLK_SPACE : // release Y button - stop showing help
							goback = 1;
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
		SDL_Delay(0);
	}
}

void startMenu() {
	quit = 0;
	dirty = 0;
	konamicode = 0;
	redrawMenu();
	SDL_Flip(screen);
	Mix_PlayChannel(-1, sound_move, 0);

    SDL_Event event;
    while (quit == 0) {
        dirty = 0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_UP:
							if ((konamicode == 0) || (konamicode == 1)) { // i don't know the purpose of this piece of code, i swear!
								konamicode++;
							} else if (konamicode == 2) {
								// do nothing
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							break;
						case SDLK_DOWN:
							if ((konamicode == 2) || (konamicode == 3)) {
								konamicode++;
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							break;
						case SDLK_LEFT:
							if ((konamicode == 4) || (konamicode == 6)) {
								konamicode++;
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							if (gameab == 0) {
								gameab = 1;
							} else if (gameab == 1) {
								gameab = 0;
							}
							Mix_PlayChannel(-1, sound_move, 0);
							dirty = 1;
							break;
						case SDLK_RIGHT:
							if ((konamicode == 5) || (konamicode == 7)) {
								konamicode++;
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							if (gameab == 0) {
								gameab = 1;
							} else if (gameab == 1) {
								gameab = 0;
							}
							Mix_PlayChannel(-1, sound_move, 0);
							dirty = 1;
							break;
						case SDLK_LCTRL: // B button
							if (konamicode == 8) {
								konamicode++;
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							break;
						case SDLK_LALT: // A button
							if (konamicode == 9) {
								konamicode++;
								Mix_PlayChannel(-1, sound_good, 0);
								dirty = 1;
							} else if (konamicode == 10) {
								// do nothing
							} else {
								konamicode = 0;
							}
							break;
						case SDLK_LSHIFT: // X button
						case SDLK_SPACE : // Y button - Show help
							showHelp();
							dirty = 1;
							break;
						case SDLK_RETURN:	// Start button - Start game
							startNewGame();
							Mix_PlayChannel(-1, sound_move, 0);
							konamicode = 0;		
							break;
						case SDLK_ESCAPE:	// Select button - Exit
							quit = 1;
							break;
						default:
							break;
					}
					if ((event.key.keysym.sym != SDLK_UP) && (event.key.keysym.sym != SDLK_DOWN) && (event.key.keysym.sym != SDLK_LEFT) && (event.key.keysym.sym != SDLK_RIGHT) && (event.key.keysym.sym != SDLK_LCTRL) && (event.key.keysym.sym != SDLK_LALT) && (konamicode != 10)) {
						konamicode = 0;
					}
				default:
					break;
			}
		}
		SDL_Delay(0);
		if (dirty == 1) {
			redrawMenu();
			SDL_Flip(screen);
		}
	}
}

int main() {
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	printf("Starting...\n");

	//create necessary directories
	std::string basicdirpath;
	basicdirpath = (homedir + "/.quartermaster/");
	mkdir(basicdirpath.c_str(), 0777);
	
	// init stuff
	screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);

	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
	Mix_AllocateChannels(2);

	loadGraphics();
	loadSounds();
	init_font();
	loadHiscores();
	
	// start
	startMenu();

	// close and exit
	printf("Exiting...\n");
	freeSounds();
	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}
