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
varying vec2 texCoord;
varying vec3 lightDir,normal,halfVector;
varying vec4 ambient,diffuse,specular,ecPos;

uniform sampler2D TextureUnit0;
uniform sampler2D TextureUnit1;
uniform float rimlighting, shininess;
uniform int bumpmap;

void main()
{

	float intensity, fresnel, bumpFresnel, bumpDiffuse;
	vec4 color,texel,shine,bumpShine, normaltexel, diffuseTerm;
	vec3 bump;
	texel = texture2D(TextureUnit0,texCoord);
	vec3 n = normalize(normal);
	diffuseTerm = diffuse;
	intensity = max(dot(lightDir,n),0.0);
	color = ambient;
	if(bumpmap != 0)
	{
	        normaltexel = texture2D(TextureUnit1,texCoord);
	        bump = vec3(normaltexel*2.0 - 1.0);
                bump.y = -bump.y;
	        bumpShine = vec4(pow(max(dot(normalize(bump+n),halfVector),0.0),100/shininess));
	        diffuseTerm *=  max(dot(lightDir,normalize(n+bump)),0.001);
	        bumpFresnel = abs(length(cross(bump+n,lightDir)));
        }
        else
        {
                bumpShine = vec4(1.0);
                bumpFresnel = 0.0;
        }
        color += intensity * diffuseTerm;
        if(rimlighting != 0.0)
        {
	fresnel = abs(length(cross(n,lightDir)))*rimlighting;
	color += (fresnel + bumpFresnel)*ambient*vec4(normalize(vec3(diffuseTerm)),diffuseTerm.a);
	}
	shine = vec4(max(pow(dot(n,halfVector),100/shininess),0.0));
        color += (shine*bumpShine);
        if(texel.r == 0.0)
        {
              texel.r += 0.1;
	}
        if(texel.g == 0.0)
        {
              texel.g += 0.1;
	}
        if(texel.b == 0.0)
        {
              texel.b += 0.1;
	}
	//color *= texel;
	gl_FragColor = color;
} 