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
#include "AudioClip.h"

using namespace MINX::Audio;
using namespace std;

AudioClip::AudioClip(string filename, bool autoplay, bool loop)
{
	if(!mix_initialized)
	{
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
		allocated_channels=MIX_DEFAULT_CHANNELS;
		used_channels = 0;
		mix_initialized=true;
	}
	mix_channel = used_channels++;
	if(used_channels > allocated_channels)
	{
		Mix_AllocateChannels(allocated_channels++);
	}
	Mix_Volume(mix_channel,MIX_MAX_VOLUME);
	// I think that it means it's loading it into a WAV, because according to http://sdl.beuc.net/sdl.wiki/SDL_mixer, "many formats supported". Will test
	audiodata = Mix_LoadWAV(filename.c_str());
	if(autoplay)
	{
		Mix_PlayChannel(mix_channel, audiodata, loop ? -1 : 0);
	}
}

AudioClip::~AudioClip()
{
	used_channels--;
	Mix_FreeChunk(audiodata);
}

void AudioClip::play()
{
	if(Mix_Paused(mix_channel))
	{
		Mix_Resume(mix_channel);
	} else
	{
		Mix_PlayChannel(mix_channel, audiodata, 0);
	}
}

void AudioClip::pause()
{
	if(Mix_Playing(mix_channel))
	{
		Mix_Pause(mix_channel);
	}
}

void AudioClip::stop()
{
	if(Mix_Playing(mix_channel) || Mix_Paused(mix_channel))
	{
		Mix_HaltChannel(mix_channel);
	}
}

void AudioClip::setVolume(float volume)
{
	Mix_Volume(mix_channel,min(0,max(MIX_MAX_VOLUME,int(volume*MIX_MAX_VOLUME))));
}

void AudioClip::setPosition(Sint16 angle, Uint8 distance)
{
	Mix_SetPosition(mix_channel,angle,distance);
}

void AudioClip::disable3DAudio()
{
	setPosition(0,0);
}
