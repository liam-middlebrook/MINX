/*
    MINX - A C++ Graphics and Input Wrapper Library
    Copyright (C) 2013  MINX Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#ifndef GAME_H_
#define GAME_H_
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#ifdef _WIN32
#include "SDL.h"
#include "SDL_mixer.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#endif
#include "GameComponent.h"
#include "GameTime.h"
#include "Content.h"
#include "Graphics/GameWindow.h"

#ifdef _WIN32

#endif
using namespace std;

namespace MINX
{
	/** The class that handles everything.
	 * When using the library, your code will probably construct, Initialize() and Run() your subclass of Game.
	 */
	class Game 
	{
		public:
			/** The constructor, which handles some of the initialization code for the Game.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			Game();
			/** This handles the rest of the initialization code for the game.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			virtual void Initialize();
			/** Loads content used by the game.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			virtual void LoadContent();
			/** Updates the game state.
			 * Should be extended when you create a subclass of Game.
			 * @param gameTime the GameTime to use when updating.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			 virtual void Update(GameTime * gameTime);
			/** Unloads content used by the game.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			 virtual void UnloadContent();
			/** Draws the state of the game every frame.
			 * This method is called by a different thread than Update(), so it is ok to use sleeps in Update() and not worry about freezing the drawing, or vice versa.
			 * @param gameTime the GameTime to use when drawing.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			 virtual void Draw(GameTime * gameTime);
			/** Handles Update()ing, Draw()ing, and event handling.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			 void Run();
			/** A pointer to the window used by the game.
			 */
			Graphics::GameWindow* gameWindow;
			/** A pointer to a vector of pointers to the GameComponents used in the game.
			 */
			vector<GameComponent*> * Components;
			/** A pointer to a queue of pointers to the SDL_Events that the Keyboard class should use
			 */
			queue<SDL_Event*> * keyboardEvents;
			/** A pointer to a queue of pointers to the SDL_Events that the Mouse class should use
			 */
			queue<SDL_Event*> * mouseEvents;
			/** A pointer to a queue of pointers to the SDL_Events that the GamePad class should use
			 */
			queue<SDL_Event*> * gamepadEvents;
			/** Returns a pointer to the GameTime being used by the game.
			 * Currently GameTime is pretty much just stubbed.
			 */
#ifdef _WIN32
			__declspec(dllexport)
#endif
			 GameTime * getGameTime();
			bool isRunning;
		private:
			/** The object that will store the event received by SDL_PollEvent() each cycle.
			 */
			SDL_Event evt;
			/** A pointer to the GameTime being used by the game.
			 */
			GameTime* gameTime;
		protected:
			int desiredFPS;
			int windowWidth, windowHeight, windowBPP;
			Uint32 windowFlags;
			MINX::Content* content;
	};
}
#endif
