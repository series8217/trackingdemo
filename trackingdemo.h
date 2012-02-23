#ifndef TRACKINGDEMOINCLUDED
#define TRACKINGDEMOINCLUDED

// wiimote tracking demo for linux

// by Steven Thomas Snyder, stsnyder@ucla.edu

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
*/

#include <GL/glut.h> 
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include <cwiid.h> /* cwiid wii remote library */
#include "trackingsettings.h"
#include "wiiheadtracking.h"

#define COORD_X 0
#define COORD_Y 1
#define COORD_Z 2


// Screen information structure
// ===============================
typedef struct {
	int res_x; // X resolution in pixels
	int res_y; // Y resolution in pixels
} screen_info_t;


// Targets
// ================
#define NUMTARGETS 8

typedef struct {
	GLfloat coord[3]; // x,y,z coordinates
	float size; // size
} target_t;

typedef struct {
	target_t targets[NUMTARGETS];
} targets_info_t;




// 3D Drawing
// ===================================================================================
// Draw a line from (x1,y1,z1) to (x2,y2,z2), starting at color1 and blending into color2 by the second point
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, GLfloat color1[3], GLfloat color2[3]);

// Draw a "target" with size expressed in radius.
void drawTarget(GLfloat coord[3], float size);

// draw a grid box (with no front face)
void drawGrid(float width,float height, float depth);


// Other functions
// ===================================================================================
void printControls(); // print controls to the terminal window
void display(); // render the scene
void normalKeyPress(unsigned char key, int x, int y); // handle keyboard input
void specialKeyPress(int key, int x, int y); // handle special keyboard input
int writePositions(double ir_positions[][2], int ir_sizes[]); // draw some raw wiimote data to the screen
void printString(void *font,char *str); // draw a string to the screen
void TrackingDemoInit(); // initialize some openGL stuff
void resizeWindow(int width, int height); // called to change the size of the window **** NOT IMPLEMENTED YET ***
void positionTargets(targets_info_t* targets); // moves the targets to new, random positions

void TrackingDemoInit();

void resizeWindow(int width, int height);

void display();

void positionTargets(targets_info_t *targets);

/* text printing functions for 3d environment */
void printString(void *font,char *str);

int writePositions(double ir_positions[][2], int ir_sizes[]);



void printControls();

void normalKeyPress(unsigned char key, int x, int y);

void specialKeyPress(int key, int x, int y);


#endif // include guard
