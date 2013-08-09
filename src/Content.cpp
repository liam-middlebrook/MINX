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
matching

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#include "Content.h"
using namespace MINX;

Content::Content(Game * addTo)
{
	game=addTo;
	textures = new map<std::string, Texture2D*>();
	TTFFonts = new map<std::string, TTF_Font*>();
	audioClips = new map<std::string, AudioClip*>();
}

AudioClip* Content::loadAudioClip(std::string name, std::string assetName, bool autoplay, bool loop)
{
	AudioClip* snd = new AudioClip(name.c_str(),autoplay,loop);
	audioClips->insert(std::pair<string, AudioClip*>(assetName, snd));
	return snd;
}

Texture2D* Content::loadTexture(std::string name, std::string assetName)
{
	Texture2D* tex = new Texture2D(IMG_Load((name).c_str()), game->gameWindow);
	textures->insert(std::pair<string, Texture2D*>(assetName, tex));
	return tex;
}
TTF_Font* Content::loadTTFFont(std::string name, int size, std::string assetName)
{
	TTF_Font* font = TTF_OpenFont(name.c_str(), size);
	TTFFonts->insert(std::pair<string, TTF_Font*>(assetName, font));
	return font;
}