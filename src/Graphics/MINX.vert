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
uniform float TIME_FROM_INIT;

varying vec2 texCoord;
varying vec3 lightDir,normal,halfVector;
varying vec4 ambient,diffuse,specular,ecPos;

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	ecPos = gl_Position;
	texCoord = gl_MultiTexCoord0.xy;
	lightDir = normalize(vec3(gl_LightSource[0].position-gl_Position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
	halfVector = normalize(gl_LightSource[0].halfVector - gl_Position);
	ambient = gl_FrontMaterial.ambient*(gl_LightSource[0].ambient+gl_LightModel.ambient);
	diffuse = gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse;
	specular = gl_FrontMaterial.specular*gl_LightSource[0].specular;
        gl_TexCoord[0] = gl_MultiTexCoord0;
} 
