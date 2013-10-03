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
#include <GL/FreeImage.h>

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
		cout << "FATAL: invalid header" << endl;
	}
	lilswap(&hdr.version, (sizeof(hdr) - sizeof(hdr.magic))/sizeof(uint));
	if(hdr.version != 1)
	{
		cout << "FATAL: invalid header version" << endl;
	}
    	if(hdr.filesize > (16<<20))
	{
		cout << "FATAL: file too large" << endl;
	}
    	buf = new uchar[hdr.filesize];
    	if(fread(buf + sizeof(hdr), 1, hdr.filesize - sizeof(hdr), f) != hdr.filesize - sizeof(hdr))
	{
		cout << "FATAL: lol idk" << endl;
	}
	if(hdr.num_meshes > 0 && !loadMeshes(filename, hdr, buf))
	{
		cout << "FATAL: meshes could not be loaded" << endl;
	}
	if(hdr.num_anims > 0 && !loadAnims(filename, hdr, buf))
	{
		cout << "FATAL: animations could not be loaded" << endl;
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
	glGenTextures((int)hdr.num_meshes,textures);
    for(int i = 0; i < (int)hdr.num_meshes; i++)
    {
        iqmmesh &m = meshes[i];
        printf("%s: loaded mesh: %s\n", filename.c_str(), &str[m.name]);
        //textures[i] = loadtexture(&str[m.material], 0);
        glBindTexture(GL_TEXTURE_2D,textures[i]);
        FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(filename.c_str(), 0),
			filename.c_str());
		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);

		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
        if(textures[i]) printf("%s: loaded material: %s\n", filename.c_str(), &str[m.material]);
    }

    return true;
}

bool Model::loadAnims(string filename, iqmheader hdr, unsigned char buf[])
{
	if((int)hdr.num_poses != numjoints) return false;

    if(animdata)
    {
        delete[] animdata;
        delete[] frames;
        animdata = NULL;
        anims = NULL;
        frames = 0;
        numframes = 0;
        numanims = 0;
    }        

    lilswap((uint *)&buf[hdr.ofs_poses], hdr.num_poses*sizeof(iqmpose)/sizeof(uint));
    lilswap((uint *)&buf[hdr.ofs_anims], hdr.num_anims*sizeof(iqmanim)/sizeof(uint));
    lilswap((ushort *)&buf[hdr.ofs_frames], hdr.num_frames*hdr.num_framechannels);
    if(hdr.ofs_bounds) lilswap((uint *)&buf[hdr.ofs_bounds], hdr.num_frames*sizeof(iqmbounds)/sizeof(uint));

    animdata = buf;
    numanims = hdr.num_anims;
    numframes = hdr.num_frames;

    const char *str = hdr.ofs_text ? (char *)&buf[hdr.ofs_text] : "";
    anims = (iqmanim *)&buf[hdr.ofs_anims];
    poses = (iqmpose *)&buf[hdr.ofs_poses];
    frames = new Matrix3x4[hdr.num_frames * hdr.num_poses];
    ushort *framedata = (ushort *)&buf[hdr.ofs_frames];
    if(hdr.ofs_bounds) bounds = (iqmbounds *)&buf[hdr.ofs_bounds];

    for(int i = 0; i < (int)hdr.num_frames; i++)
    {
        for(int j = 0; j < (int)hdr.num_poses; j++)
        {
            iqmpose &p = poses[j];
            Vec3 translate, rotate, scale;
            translate.x = p.channeloffset[0]; if(p.mask&0x01) translate.x += *framedata++ * p.channelscale[0];
            translate.y = p.channeloffset[1]; if(p.mask&0x02) translate.y += *framedata++ * p.channelscale[1];
            translate.z = p.channeloffset[2]; if(p.mask&0x04) translate.z += *framedata++ * p.channelscale[2];
            rotate.x = p.channeloffset[3]; if(p.mask&0x08) rotate.x += *framedata++ * p.channelscale[3];
            rotate.y = p.channeloffset[4]; if(p.mask&0x10) rotate.y += *framedata++ * p.channelscale[4];
            rotate.z = p.channeloffset[5]; if(p.mask&0x20) rotate.z += *framedata++ * p.channelscale[5];
            scale.x = p.channeloffset[6]; if(p.mask&0x40) scale.x += *framedata++ * p.channelscale[6];
            scale.y = p.channeloffset[7]; if(p.mask&0x80) scale.y += *framedata++ * p.channelscale[7];
            scale.z = p.channeloffset[8]; if(p.mask&0x100) scale.z += *framedata++ * p.channelscale[8];
            // Concatenate each pose with the inverse base pose to avoid doing this at animation time.
            // If the joint has a parent, then it needs to be pre-concatenated with its parent's base pose.
            // Thus it all negates at animation time like so: 
            //   (parentPose * parentInverseBasePose) * (parentBasePose * childPose * childInverseBasePose) =>
            //   parentPose * (parentInverseBasePose * parentBasePose) * childPose * childInverseBasePose =>
            //   parentPose * childPose * childInverseBasePose
            Matrix3x4 m(Quat(rotate), translate, scale);
            if(p.parent >= 0) frames[i*hdr.num_poses + j] = baseframe[p.parent] * m * inversebaseframe[j];
            else frames[i*hdr.num_poses + j] = m * inversebaseframe[j];
        }
    }
 
    for(int i = 0; i < (int)hdr.num_anims; i++)
    {
        iqmanim &a = anims[i];
        printf("%s: loaded anim: %s\n", filename.c_str(), &str[a.name]);
    }
    
    return true;
}

void Model::Draw()
{
static const GLfloat zero[4] = { 0, 0, 0, 0 }, 
                         one[4] = { 1, 1, 1, 1 },
                         ambientcol[4] = { 0.5f, 0.5f, 0.5f, 1 }, 
                         diffusecol[4] = { 0.5f, 0.5f, 0.5f, 1 },
                         lightdir[4] = { cosf(radians(-60)), 0, sinf(radians(-60)), 0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, zero);
    glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, one);
    glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientcol);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusecol);
    glLightfv(GL_LIGHT0, GL_POSITION, lightdir);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glColor3f(1, 1, 1);
    glVertexPointer(3, GL_FLOAT, 0, numframes > 0 ? outposition : inposition);
    glNormalPointer(GL_FLOAT, 0, numframes > 0 ? outnormal : innormal);
    glTexCoordPointer(2, GL_FLOAT, 0, intexcoord);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    
    for(int i = 0; i < nummeshes; i++)
    {
        iqmmesh &m = meshes[i];
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glDrawElements(GL_TRIANGLES, 3*m.num_triangles, GL_UNSIGNED_INT, &tris[m.first_triangle]);
    }

    glDisable(GL_TEXTURE_2D);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_NORMALIZE);    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}
