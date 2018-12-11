/*
 * @file: GameManager.cpp
 * @author: Stig M. Halvorsen <halsti@nith.no>
 * @version: 1.0.0 <11.02.2013>
 *
 * @description: A singleton class to control all gameplay.
 * @new authors: Nireban Sivakumaran & Jan-Philippe Rasay
 */

#include "SDL/SDLBmp.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Timer.h"

/* Initializes SDL, creates the game window and fires off the timer. */
GameManager::GameManager()
{
	SDLManager::Instance().init();
	m_window = SDLManager::Instance().createWindow("Pokesnake",width,height);
	Timer::Instance().init();
}


/* Kicks off/is the the gameloop */
void GameManager::play()
{
	bool gameOver = false;
	

	// Load bitmaps
	SDLBmp backround("Assets/gfx/grass.bmp");
	SDLBmp player("Assets/gfx/pokeball.bmp");
	SDLBmp pika("Assets/gfx/pikachu.bmp");
	SDLBmp dig("Assets/gfx/output.bmp");

	// Calculate render frames per second (second / frames) (60)
	float render_fps = 1.f / 60.f;
	m_lastRender = render_fps; // set it to render immediately

	// Gameloop
	while (!gameOver)
	{
		// Update input and deltatime
		InputManager::Instance().Update();
		Timer::Instance().update();


		/* Input Management */

		// Left key
		if (InputManager::Instance().KeyDown(SDL_SCANCODE_LEFT)) {
			if (snake.last != RIGHT) {
				snake.next = LEFT;
			}
		}

		// Right key
		if (InputManager::Instance().KeyDown(SDL_SCANCODE_RIGHT)) {
			if (snake.last != LEFT) {
				snake.next = RIGHT;
			}
		}

		// Key up
		if (InputManager::Instance().KeyDown(SDL_SCANCODE_UP)) {
			if (snake.last != DOWN) {
				snake.next = UP;
			}
		}

		// Key down
		if (InputManager::Instance().KeyDown(SDL_SCANCODE_DOWN)) {
			if (snake.last != UP) {
				snake.next = DOWN;
			}
		}

		// Exit on [Esc], or window close (user X-ed out the window)
		if (InputManager::Instance().hasExit() || InputManager::Instance().KeyDown(SDL_SCANCODE_ESCAPE)) {
			gameOver = true;
		}

		// Update time since last render
		m_lastRender += Timer::Instance().deltaTime();
		snake.time += Timer::Instance().deltaTime();
		snake.move();
		// Check if it's time to render
		if (m_lastRender >= render_fps)
		{
			// Add bitmaps to render
			backround.draw();
			pika.x = snake.food.x*square;
			pika.y = snake.food.y*square;
			pika.draw();
			for (int i = 0; i < snake.v.size(); i++)
			{
				player.x = snake.v[i].x*square;
				player.y = snake.v[i].y*square;
				player.draw();
			}
			for (int i = 0; i < snake.obs.size(); i++)
			{
				dig.x = snake.obs[i].x*square;
				dig.y = snake.obs[i].y*square;
				dig.draw();
			}
			// Render window
			SDLManager::Instance().renderWindow(m_window);
			m_lastRender = 0.f;
		}

		// Sleep to prevent CPU exthaustion (1ms == 1000 frames per second)
		SDL_Delay(1);
	}
}