/* wiimote tracking demo for linux, version 0.20 */

/* trackingtexture.h  ----- 3d object texture mapping functions for wiimote tracking demo*/

/* by Steven Thomas Snyder, stsnyder@ucla.edu */
/* last modified: monday, june 9, 2008 */
#ifndef TRACKINGTEXINCLUDED
#define TRACKINGTEXINCLUDED
/*
(C) copyright 2008, Steven Snyder, All Rights Reserved

LICENSING INFORMATION:
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

This software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details.
FreeImage is used under the (GNU GPL or FIPL), version (license version).
*/


#include <GL/glut.h> 
#include <GL/glu.h>
#include <freeimage.h>

int my_texturescount = 0;

int my_loadTexture()
{
    // loads a texture file into video memory and returns the texture ID
    int old_tex_id;
    glGet(GL_TEXTURE_BINDING_2D,&old_tex_id);
    int tex_id = my_texturescount;
    my_texturescount++;

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, old_tex_id);

    return texid;
}



#endif
