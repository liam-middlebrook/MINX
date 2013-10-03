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
#include "Model.h"
#include "iqmutils.h"
#include <iostream>
#include <stdio.h>
#include <cstring>

extern GLuint loadtexture(const char *name, int clamp);

using namespace MINX::Graphics::MINX3D;
using namespace std;

Model::Model(string filename)
{
	FILE *f = fopen(filename.c_str(), "rb");
	if(!f)
	{
		cout << "FATAL: could not open file" << endl;
	}
	uchar *buf = NULL;
	iqmheader hdr;
	if(fread(&hdr, 1, sizeof(hdr), f) != sizeof(hdr) || memcmp(hdr.magic, IQM_MAGIC, sizeof(hdr.magic)))
	{
		cout << "FATAL: could not open file" << endl;
	}
	lilswap(&hdr.version, (sizeof(hdr) - sizeof(hdr.magic))/sizeof(uint));
	if(hdr.version != 1)
	{
		cout << "FATAL: could not open file" << endl;
	}
    	if(hdr.filesize > (16<<20))
	{
		cout << "FATAL: could not open file" << endl;
	}
    	buf = new uchar[hdr.filesize];
    	if(fread(buf + sizeof(hdr), 1, hdr.filesize - sizeof(hdr), f) != hdr.filesize - sizeof(hdr))
	{
		cout << "FATAL: could not open file" << endl;
	}
	if(hdr.num_meshes > 0 && !loadMeshes(filename, hdr, buf))
	{
		cout << "FATAL: could not open file" << endl;
	}
	if(hdr.num_anims > 0 && !loadAnims(filename, hdr, buf))
	{
		cout << "FATAL: could not open file" << endl;
	}
	fclose(f);
}

bool Model::loadMeshes(string filename, iqmheader hdr, unsigned char buf[])
{
	if(meshdata)
	{
		return false;
	}

	lilswap((uint *)&buf[hdr.ofs_vertexarrays], hdr.num_vertexarrays*sizeof(iqmvertexarray)/sizeof(uint));
	lilswap((uint *)&buf[hdr.ofs_triangles], hdr.num_triangles*sizeof(iqmtriangle)/sizeof(uint));
	lilswap((uint *)&buf[hdr.ofs_meshes], hdr.num_meshes*sizeof(iqmmesh)/sizeof(uint));
	lilswap((uint *)&buf[hdr.ofs_joints], hdr.num_joints*sizeof(iqmjoint)/sizeof(uint));
	
	if(hdr.ofs_adjacency)
	{
		lilswap((uint *)&buf[hdr.ofs_adjacency], hdr.num_triangles*sizeof(iqmtriangle)/sizeof(uint));
	}

	meshdata = buf;
	nummeshes = hdr.num_meshes;
	numtris = hdr.num_triangles;
	numverts = hdr.num_vertexes;
	numjoints = hdr.num_joints;
	outposition = new float[3*numverts];
	outnormal = new float[3*numverts];
	outtangent = new float[3*numverts];
	outbitangent = new float[3*numverts];
	outframe = new Matrix3x4[hdr.num_joints];
	textures = new GLuint[nummeshes];
	memset(textures, 0, nummeshes*sizeof(GLuint));

	const char *str = hdr.ofs_text ? (char *)&buf[hdr.ofs_text] : "";
	iqmvertexarray *vas = (iqmvertexarray *)&buf[hdr.ofs_vertexarrays];
	for(int i = 0; i < (int)hdr.num_vertexarrays; i++)
	{
        iqmvertexarray &va = vas[i];
        switch(va.type)
        {
        case IQM_POSITION: if(va.format != IQM_FLOAT || va.size != 3) return false; inposition = (float *)&buf[va.offset]; lilswap(inposition, 3*hdr.num_vertexes); break;
        case IQM_NORMAL: if(va.format != IQM_FLOAT || va.size != 3) return false; innormal = (float *)&buf[va.offset]; lilswap(innormal, 3*hdr.num_vertexes); break;
        case IQM_TANGENT: if(va.format != IQM_FLOAT || va.size != 4) return false; intangent = (float *)&buf[va.offset]; lilswap(intangent, 4*hdr.num_vertexes); break;
        case IQM_TEXCOORD: if(va.format != IQM_FLOAT || va.size != 2) return false; intexcoord = (float *)&buf[va.offset]; lilswap(intexcoord, 2*hdr.num_vertexes); break;
        case IQM_BLENDINDEXES: if(va.format != IQM_UBYTE || va.size != 4) return false; inblendindex = (uchar *)&buf[va.offset]; break;
        case IQM_BLENDWEIGHTS: if(va.format != IQM_UBYTE || va.size != 4) return false; inblendweight = (uchar *)&buf[va.offset]; break;
        }
    }
    tris = (iqmtriangle *)&buf[hdr.ofs_triangles];
    meshes = (iqmmesh *)&buf[hdr.ofs_meshes];
    joints = (iqmjoint *)&buf[hdr.ofs_joints];
    if(hdr.ofs_adjacency) adjacency = (iqmtriangle *)&buf[hdr.ofs_adjacency];

    baseframe = new Matrix3x4[hdr.num_joints];
    inversebaseframe = new Matrix3x4[hdr.num_joints];
    for(int i = 0; i < (int)hdr.num_joints; i++)
    {
        iqmjoint &j = joints[i];
        baseframe[i] = Matrix3x4(Quat(Vec3(j.rotate)), Vec3(j.translate), Vec3(j.scale));
        inversebaseframe[i].invert(baseframe[i]);
        if(j.parent >= 0) 
        {
            baseframe[i] = baseframe[j.parent] * baseframe[i];
            inversebaseframe[i] *= inversebaseframe[j.parent];
        }
    }

    for(int i = 0; i < (int)hdr.num_meshes; i++)
    {
        iqmmesh &m = meshes[i];
        printf("%s: loaded mesh: %s\n", filename.c_str(), &str[m.name]);
        textures[i] = loadtexture(&str[m.material], 0);
        if(textures[i]) printf("%s: loaded material: %s\n", filename.c_str(), &str[m.material]);
    }

    return true;
}
