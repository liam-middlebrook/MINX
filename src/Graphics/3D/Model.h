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
#ifndef MODEL_H_
#define MODEL_H_
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include "../../Vector3.h"
#include "iqm.h"
#include "iqmgeom.h"
using namespace std;
namespace MINX
{
	namespace Graphics
	{
		namespace MINX3D
		{
			class Model
			{
				public:
					Model(string name);
					void setWorldPosition(double x, double y, double z);
					double getWorldX();
					double getWorldY();
					double getWorldZ();
					void Draw();
				private:
					bool loadMeshes(string filename, iqmheader header, unsigned char buffer[]);
					bool loadAnims(string filename, iqmheader header, unsigned char buffer[]);
					vector<Vector3> vertices;
					unsigned char * meshdata;
					int nummeshes = 0, numtris = 0, numverts = 0, numjoints = 0, numframes = 0, numanims = 0;
					float *outposition = NULL, *outnormal = NULL, *outtangent = NULL, *outbitangent = NULL;
					Matrix3x4 *baseframe = NULL, *inversebaseframe = NULL, *outframe = NULL, *frames = NULL;
					GLuint *textures = NULL;
					float *inposition = NULL, *innormal = NULL, *intangent = NULL, *intexcoord = NULL;
					unsigned char *inblendindex = NULL, *inblendweight = NULL;
					iqmtriangle *tris = NULL, *adjacency = NULL;
					iqmmesh *meshes = NULL;
					iqmjoint *joints = NULL;
					iqmpose *poses = NULL;
					iqmanim *anims = NULL;
					iqmbounds *bounds = NULL;
			
			};
		}
	}
}
#endif
