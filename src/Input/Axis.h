
/*
# MINX

Copyright (c) 2013-2014 Liam Middlebrook, Benjamin Pylko

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

> 1\. The origin of this software must not be misrepresented; you must not
> claim that you wrote the original software. If you use this software
> in a product, an acknowledgment in the product documentation would be
> appreciated but is not required.
>
> 2\. Altered source versions must be plainly marked as such, and must not be
> misrepresented as being the original software.
>
> 3\. This notice may not be removed or altered from any source
> distribution.
>
        */
#ifndef AXIS_H_
#define AXIS_H_
namespace MINX
{
	namespace Input{
		/** Represents an axis on an input device such as a mouse or joystick.
		 * Some axes indicate a position (a stick on a game pad), and some indicate relative motion (a ball on a joypad).
		 */
		struct Axis
		{
			/** The current value of the axis.
			 */
			double val;
			/** The previous value of the axis.
			 */
			double prevVal;
		};
	}
}
#endif
