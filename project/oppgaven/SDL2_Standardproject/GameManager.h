/*
* @file: GameManager.h
* @author: Stig M. Halvorsen <halsti@nith.no>
* @version: 1.0.0 <11.02.2013>
*
* @description: A singleton class to control all gameplay.
* @new authors: Nireban Sivakumaran & Jan-Philippe Rasay
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "SDL/SDLManager.h"
#include "SDL_audio.h"


#include <vector>
#include <iostream>
#include <windows.h>   //POINT 
#include <algorithm>  
using namespace std;
#define width 800
#define height 600
#define square 50
#define SOUND_PATH "Assets/gfx/back.wav"
#define SOUND_PIK "Assets/gfx/pik.wav"
#define SOUND_DIG "Assets/gfx/dig.wav"

//Possible directions
enum Dir
{
	UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2
};

struct Snake
{
	SDL_AudioSpec wanted;
	int chancePik, chanceDig;
	POINT food;
	float time;
	float speed;
	vector<POINT> v;
	vector<POINT> obs; //obstacle
	Dir last;
	Dir next;

	//Removes digletts on window randomely
	void removeObstacle()
	{
		if (obs.size())
		{
			obs.erase(obs.begin() + rand() % obs.size());
		}

	}

	//Creates digletts on window randomely
	void obstacle()
	{
		vector<POINT> all;
		for (int i = 0; i < height / square; i++)
			for (int j = 0; j < width / square; j++)
			{
				POINT p;
				p.x = j;
				p.y = i;
				all.push_back(p);
			}
		
		for (int i = 0; i < v.size(); i++)
			for (int j = 0; j < all.size(); j++)
				if (all[j].x == v[i].x && all[j].y == v[i].y)
				{
					all.erase(all.begin() + j);
					j = all.size();
				}

		for (int i = 0; i < all.size(); i++)
			if (all[i].x == food.x && all[i].y == food.y)
			{
				all.erase(all.begin() + i);
				i = all.size();
			}
		obs.push_back(all[rand() % all.size()]);
	}

	//Logic for catching pikachu
	void pickFood(bool sound = true)
	{
		if (sound)PlaySoundA(SOUND_PIK, 0, SND_ASYNC | SND_FILENAME);
		vector<POINT> all;
		for (int i = 0; i < height / square; i++)
			for (int j = 0; j < width / square; j++)
			{
				POINT p;
				p.x = j;
				p.y = i;
				all.push_back(p);
			}
		for (int i = 0; i < v.size(); i++)
			for (int j = 0; j < all.size(); j++)
				if (all[j].x == v[i].x && all[j].y == v[i].y)
				{
					all.erase(all.begin() + j);
					j = all.size();
				}
		for (int i = 0; i < obs.size(); i++)
			for (int j = 0; j < all.size(); j++)
				if (all[j].x == obs[i].x && all[j].y == obs[i].y)
				{
					all.erase(all.begin() + j);
					j = all.size();
				}
		food = all[rand() % all.size()];
	}

	//Logic for "snake" movements
	void move()
	{
		if (time >= speed)
		{
			POINT f = v.back();
			if (next == UP)f.y--;
			if (next == DOWN)f.y++;
			if (next == LEFT)f.x--;
			if (next == RIGHT)f.x++;
			v.push_back(f);
			if (f.x != food.x || f.y != food.y)
			{
				v.erase(v.begin());
				if (rand() % 100 < chancePik)pickFood(0);
				if (rand() % 100 < chanceDig)obstacle();
				if (rand() % 100 < chanceDig / 2)removeObstacle();
			}
			else
			{
				speed *= 0.9;
				pickFood();
			}
			time -= speed;
			last = next;

			//check for end
			bool fou = false;
			bool digle = false;
			for (int i = 0; i < v.size() - 1; i++)
				if (v[i].x == f.x && v[i].y == f.y)
					fou = true;
			for (int i = 0; i < obs.size(); i++)
				if (obs[i].x == f.x && obs[i].y == f.y)
					digle = true;
			if (f.x<0 || f.y<0 || f.x>width / square || f.y>height / square || fou || digle)
			{
				if (digle)
					PlaySoundA(SOUND_DIG, 0, SND_ASYNC | SND_FILENAME);
				Sleep(500);
				init();
			}
		}
	}
	void init()
	{
		srand(GetTickCount());
		//800x600 -> 16x12
		POINT q;
		q.x = 1;
		q.y = height / square / 2;
		v.clear();
		obs.clear();
		v.push_back(q);
		q.x = 2; v.push_back(q);
		q.x = 3; v.push_back(q);
		last = next = RIGHT;
		speed = 0.3;
		time = 0;
		pickFood(0);

	}
	Snake()
	{
		chancePik = 7;  //%
		chanceDig = 4;
		init();
	}
};


class GameManager
{
public:
	/* Meyer's singleton style instance call */
	static GameManager& Instance()
	{
		static GameManager g;
		return g;
	}
	Snake snake;

	/* Kicks off/is the the gameloop */
	void play();
private:
	GameManager();								// Hidden constructor
	GameManager(const GameManager&);			// Hidden copy constructor
	GameManager& operator=(const GameManager&); // Hidden assign operator

	unsigned int m_window; // pointer to main window
	float m_lastRender; // Time in seconds since last render
};

#endif