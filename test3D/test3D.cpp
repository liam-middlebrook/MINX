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
#include "test3D.h"
#include <Graphics/3D/Model.h>
#include <iostream>
using namespace std;

using namespace MINX_TEST3D;
using namespace MINX::Graphics::MINX3D;
test3D::test3D() : Game::Game()
{
isRunning = true;
	//This is the constructor. Put stuff here that should happen when the Game is created.
}

void test3D::Initialize()
{
	
	Game::Initialize();
Model * m = new Model("mrfixit.iqm");
}

void test3D::LoadContent()
{
	//Put stuff here that loads content for your game.
	Game::LoadContent();
}

void test3D::UnloadContent()
{
	//Put stuff here that unloads content from your game.
	Game::UnloadContent();
}

void test3D::Update(GameTime * gametime)
{
	isRunning = true;
	cout << "update" << endl;
	//Put stuff here to update the logic in your game each tick.
	Game::Update(gametime);
}

void test3D::Draw(GameTime * gametime)
{
	cout << "1";
	//m->Draw();
	cout << "2";
	Game::Draw(gametime);
}
