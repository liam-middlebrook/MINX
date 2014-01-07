#!/bin/bash
#    MINX - A C++ Graphics and Input Wrapper Library
#    Copyright (C) 2013  MINX Team
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
echo "Please type the directory in which you would like to create the new MINX project: "

read projectDir

echo "Please enter the name you would like to use for the source file of your new project (no extension): "

read fileName

fileName=$(echo $fileName | tr -d "[:space:].*!@#$%^&()}][{+=?/:;|~\`\"\'<>,")


test -d "$projectDir" || mkdir -p "$projectDir";

echo "/*
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
//AUTOGENERATED FILE: DO NOT EDIT
#include \"MINX/Game.h\"
#include \"MINX/GameTime.h\"
#ifndef ${fileName^^}_H_
#define ${fileName^^}_H_

using namespace MINX;

namespace MINX_${fileName^^}
{
	class $fileName : public Game
	{
		public:
			$fileName();
			void Initialize();
			void LoadContent();
			void UnloadContent();
			void Update(GameTime * gametime);
			void Draw(GameTime * gametime);
	};
}

#endif" > "$projectDir/$fileName.h"

echo "/*
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
#include \"$fileName.h\"

using namespace MINX_${fileName^^};


$fileName::$fileName() : Game::Game()
{
	//This is the constructor. Put stuff here that should happen when the Game is created.
}

void $fileName::Initialize()
{
	//Put stuff here that should happen when the Game is initialized.
	Game::Initialize();
}

void $fileName::LoadContent()
{
	//Put stuff here that loads content for your game.
	Game::LoadContent();
}

void $fileName::UnloadContent()
{
	//Put stuff here that unloads content from your game.
	Game::UnloadContent();
}

void $fileName::Update(GameTime * gametime)
{
	//Put stuff here to update the logic in your game each tick.
	Game::Update(gametime);
}

void $fileName::Draw(GameTime * gametime)
{
	//Put stuff here to draw your game each frame.
	Game::Draw(gametime);
}" > "$projectDir/$fileName.cpp"

echo "/*
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
//AUTOGENERATED FILE: DO NOT EDIT
#include \"$filename.h\"
using namespace MINX_${fileName^^};
int main(int argc, char ** argv)
{
	${fileName} * game = new ${fileName}();
	game->Run();
	exit(0);
}" > "$projectDir/Program.cpp"


echo "INCDIR = -I/usr/include/freetype2
CPPFLAGS += -g -Wall -W \$(INCDIR) -std=c++11
LFLAGS = -lMINX -lGL -lfreetype -lglfw3 -lX11 -lXxf86vm -lpthread -lXrandr -lXi
CXX = g++
SOURCES = \$(wildcard *.cpp)
OBJECTS = \$(patsubst %.cpp,%.o,\$(wildcard *.cpp))
TARGET = $fileName

all: \$(OBJECTS)
	\$(CXX) \$(CPPFLAGS) \$(OBJECTS) \$(LFLAGS) -o \$(TARGET)" > "$projectDir/makefile"

