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
#include "trackingdemo.h"
#include "trackingsettings.h"
#include "wiiheadtracking.h"

#define COORD_X 0
#define COORD_Y 1
#define COORD_Z 2

// Global variables
// =====================================================================================

head_info_t my_camera = {{INITIALEYEHEIGHT,INITIALEYEXOFFSET,INITIALDISTANCE}, // initial position
			{INITIALEYEHEIGHT,INITIALEYEXOFFSET,INITIALDISTANCE}, 
			2*TANWIIFOVH*INITIALDISTANCE/HRANGE, // initial conversion factor
			2*TANWIIFOVH*INITIALDISTANCE/HRANGE, 
			HRANGE*IRBARWIDTH/(2*TANWIIFOVH*INITIALDISTANCE), // IR bar width (in wii coords) at initial position
			false }; // calibrate request

screen_info_t my_screen = {DEFAULTWIDTH,DEFAULTHEIGHT};

targets_info_t my_targets;


// Wiimote Global Variables
// =========================
double base_camera_pt_width = IRBARWIDTH; 
TrackWiimote wiimote_1;

int main (int argc, char **argv)
{
	/* process the command line arguments */
	int i;

	for (i = 1; i < argc; i++)
	{
		if (strcmp("-800",argv[i]))
		{	my_screen.res_x = 800;
			my_screen.res_y = 600;	}
		else if (strcmp("-1024",argv[i]))
		{	my_screen.res_x = 1024;
			my_screen.res_y = 768;	}
		else if (strcmp("-1280",argv[i]))
		{	my_screen.res_x = 1280;
			my_screen.res_y = 960;	}
		else if (strcmp("-1600",argv[i]))
		{	my_screen.res_x = 1600;
			my_screen.res_y = 1200;	}
	}
	/* initialize GLUT toolkit */
  	glutInit(&argc, argv);
	

	/* set display mode for double buffering with RGBA (RGB with alpha channel) and depth buffer */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	/* initialize the window size and position */
	glutInitWindowSize(my_screen.res_x, my_screen.res_y);
	glutInitWindowPosition(100,100);

	/* create the application window object */
	glutCreateWindow("Wiimote Tracking Demo");

	/* more initialization stuff */
	TrackingDemoInit();

	positionTargets(&my_targets);


	if(!wiimote_1.connect())
	{
		printf("Unable to connect to Wii remote.\n");
		exit(0);
	}
	else printf("Connected to Wii remote. Starting rendering engine.\n");

	/* start rendering */
	glutMainLoop();

	return(0);
}


// Initialize openGL settings
void TrackingDemoInit()
{	
    /* register our display and keyboard control functions */
	glutDisplayFunc(display);

	/* window resize function */
	glutReshapeFunc(resizeWindow);
	glutIgnoreKeyRepeat(false);
	glutSpecialFunc(specialKeyPress);
	glutKeyboardFunc(normalKeyPress);


	glClearColor(0.0,0.0,0.0,1.0); /* background color solid black */
	glColor3f(1.0,1.0,1.0); /* current color white */
	glShadeModel(GL_SMOOTH);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); 

 	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   	glEnable(GL_COLOR_MATERIAL);
}

void resizeWindow(int width, int height)
{
}

void display()
{
	// Update Wiimote State
	// ====================
	double ir_positions[4][2] = { {0,0},{0,0},{0,0},{0,0} };
	int ir_sizes[4] = {4,4,4,4};
	double nearplane = 0.05f;

	int i;

	wiimote_1.updateState(); /* Update the current wiimote-state. */
	if (wiimote_1.updateIRPositions(ir_positions,ir_sizes)) /* if update positions succeeds, update the camera position */
	{	/* if the user has requested recalibration, perform it */
		if (my_camera.calibrate_request)
		{	
			wiimote_1.calibrate(&my_camera,ir_positions,ir_sizes,0,1);
			my_camera.calibrate_request = false;
		}

		/* update the camera position */
		wiimote_1.updateHeadPos(&my_camera,ir_positions,ir_sizes,0,1); 
	}
	

	// ============================
	// Render Scene
	// ============================

	// clear the back buffer and z buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	// disable lighting
	glDisable(GL_LIGHTING);

	// Projection Transformation
	// =========================
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	glFrustum(	(H_SCREENEDGE-my_camera.eye_pos[COORD_X])*nearplane/my_camera.eye_pos[COORD_Z],  // left
			(-H_SCREENEDGE-my_camera.eye_pos[COORD_X])*nearplane/my_camera.eye_pos[COORD_Z], // right
			(-V_SCREENEDGE-my_camera.eye_pos[COORD_Y])*nearplane/my_camera.eye_pos[COORD_Z], // bottom
			(V_SCREENEDGE-my_camera.eye_pos[COORD_Y])*nearplane/my_camera.eye_pos[COORD_Z],  // top
			nearplane,		// zNear
			VIEWFARPLANE);	// zFar

	// This code works great, except it does not render anything that exists between the screen and the viewer
	/*glFrustum(	H_SCREENEDGE-my_camera.eye_pos[COORD_X],  // left
			-H_SCREENEDGE-my_camera.eye_pos[COORD_X], // right
			-V_SCREENEDGE-my_camera.eye_pos[COORD_Y], // bottom
			V_SCREENEDGE-my_camera.eye_pos[COORD_Y],  // top
			my_camera.eye_pos[COORD_Z],		// zNear
			VIEWFARPLANE);	// zFar
	*/	

	// Objects
	// =======
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Need to translate the model geometry due to the camera position transformation.
	glTranslatef(-my_camera.eye_pos[COORD_X],-my_camera.eye_pos[COORD_Y],-my_camera.eye_pos[COORD_Z]);


	// Grid
	// ----
	drawGrid(SCREENWIDTH,SCREENHEIGHT,100);

	// Targets
	// -------
	for (i=0;i<NUMTARGETS;i++)
		drawTarget(my_targets.targets[i].coord, my_targets.targets[i].size);

	// write the locations of the IR dots and the camera to the screen
	writePositions(ir_positions,ir_sizes);

	glutSwapBuffers();
	glutPostRedisplay();
}





void positionTargets(targets_info_t *targets)
{	
	int i;
	for (i = 0; i<NUMTARGETS; i++)
	{
		targets->targets[i].coord[0] = randCoord(-SCREENWIDTH/2.0, SCREENWIDTH/2.0);
		targets->targets[i].coord[1] = randCoord(-SCREENHEIGHT/2.0, SCREENHEIGHT/2.0);
		targets->targets[i].coord[2] = randCoord(-50.0,25.0);
		targets->targets[i].size = randCoord(1.0,4.0);
	}
}

/* text printing functions for 3d environment */
void printString(void *font,char *str)
{
   	int i;
	int len= strlen(str);

	for(i=0; i<len ;i++)
		glutBitmapCharacter(font,*str++);
}

int writePositions(double ir_positions[][2], int ir_sizes[])
{
	char buf[80]; /* string buffer */
	int window_width = glutGet(GLUT_WINDOW_WIDTH);
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glOrtho(-window_width/2,window_width/2,window_height/2,-window_height/2,-1.0,1.0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glLoadIdentity();

		sprintf(buf,"IR dot locations: 1: x:%.1f,y:%.1f,size:%d.\n",
			ir_positions[0][COORD_X],ir_positions[0][COORD_Y],ir_sizes[0]);
		glRasterPos2d(-1,0.9); printString(GLUT_BITMAP_HELVETICA_12,buf);

		sprintf(buf,"                       x:%.1f,y:%.1f,size:%d.\n",
			ir_positions[1][COORD_X],ir_positions[1][COORD_Y],ir_sizes[1]);
		glRasterPos2d(-1,0.8); printString(GLUT_BITMAP_HELVETICA_12,buf);

		sprintf(buf,"                       x:%.1f,y:%.1f,size:%d.\n",
			ir_positions[2][COORD_X],ir_positions[2][COORD_Y],ir_sizes[2]);
		glRasterPos2d(-1,0.7); printString(GLUT_BITMAP_HELVETICA_12,buf);

		sprintf(buf,"                       x:%.1f,y:%.1f,size:%d.\n",
			ir_positions[3][COORD_X],ir_positions[3][COORD_Y],ir_sizes[3]);
		glRasterPos2d(-1,0.6); printString(GLUT_BITMAP_HELVETICA_12,buf);

		sprintf(buf,"Head position : x:%.1fcm,y:%.1fcm,z:%.1fcm.\n",
			my_camera.eye_pos[COORD_X],my_camera.eye_pos[COORD_Y],my_camera.eye_pos[COORD_Z]);
		glRasterPos2d(-1,0.5); printString(GLUT_BITMAP_HELVETICA_12,buf);

		sprintf(buf,"Initial position : x:%.1fcm,y:%.1fcm,z:%.1fcm.\n",
			my_camera.ini_pos[COORD_X],my_camera.ini_pos[COORD_Y],my_camera.ini_pos[COORD_Z]);
		glRasterPos2d(-1,0.4); printString(GLUT_BITMAP_HELVETICA_12,buf);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	return 0;
}



void printControls()
{
	printf("*** Wii Remote Tracking Demo by Steven Snyder (stsnyder@ucla.edu) ***\n");
	printf("\n\n ==Controls==\n");
	printf(" Camera movement:   Move your head! (if Wii remote is hooked up) \n");
	printf(" 		    (otherwise just use the keyboard arrows) \n");
	printf("		    and 'a' to more forward, 'z' to move back \n");
	printf(" Calibration:	   Move your head to %.2f from the screen,\n",INITIALDISTANCE);
	printf("			then press c.\n");
	printf("Targets:	Press 't' to reposition targets.\n");
}

void normalKeyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'h':
			printControls();
			break;
		case 'c':
			my_camera.calibrate_request = true;
			printf("\nCalibration requested!\n");
			break;
		case 'a':
			my_camera.eye_pos[COORD_Z]-=0.5;
			break;
		case 'z':
			my_camera.eye_pos[COORD_Z]+=0.5;
			break;
		case 't':
			positionTargets(&my_targets);
			break;
	}
	display(); /* redraw the window */
}

void specialKeyPress(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			my_camera.eye_pos[COORD_Y]+=0.5;
			break;
		case GLUT_KEY_DOWN:
			my_camera.eye_pos[COORD_Y]-=0.5;
			break;
		case GLUT_KEY_LEFT:
			my_camera.eye_pos[COORD_X]+=0.5;
			break;
		case GLUT_KEY_RIGHT:
			my_camera.eye_pos[COORD_X]-=0.5;
			break;
	}
	display(); /* redraw the window */
}




// 3d drawing functions
//========================


// Draw a line from (x1,y1,z1) to (x2,y2,z2), starting at color1 and blending into color2 by the second point
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, GLfloat color1[3], GLfloat color2[3])
{
	glBegin(GL_LINES);
		glColor3fv(color1);
		glVertex3d(x1,y1,z1);
		glColor3fv(color2);
		glVertex3d(x2,y2,z2);
	glEnd();
}

// Draw a "target" with size expressed in radius.
void drawTarget(GLfloat coord[3], float size)
{
	GLfloat colorscale = ((coord[2]+50.0)/75.0) + 0.5;
	GLfloat x = coord[0];
	GLfloat y = coord[1];
	GLfloat z = coord[2];

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	GLfloat color1[3] = {0.50,0.50,0.50};
	GLfloat color2[3] = {0.00,0.00,0.00};

	drawLine(x,y,z,x,y,-100,color1,color2);

	glBegin(GL_QUADS);
		glColor3f(colorscale,colorscale,colorscale);
		// front face
		glVertex3d(x+size,y+size,z+size); glVertex3d(x-size,y+size,z+size);
		glVertex3d(x-size,y-size,z+size); glVertex3d(x+size,y-size,z+size);

		glColor3f(colorscale,0.00,0.00);
		// bullseye
		glVertex3d(x+size/2,y+size/2,z+size+0.1); glVertex3d(x-size/2,y+size/2,z+size+0.1);
		glVertex3d(x-size/2,y-size/2,z+size+0.1); glVertex3d(x+size/2,y-size/2,z+size+0.1);
	glEnd();
}

// draw a rectangular grid tube (a box with no front face or rear face)
void drawGrid(float width,float height, float depth)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	float x;
	float y;
	//float z;
	GLfloat ambient[4] = {1.0000, 0.00000, 0.00000, 1.0};
	GLfloat diffuse[4] = {1.0000, 0.00000, 0.00000, 1.0};
	GLfloat specular[4] = {1.0000, 0.00000, 0.00000, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);

	GLfloat color1[3] = {0.50,0.50,0.50};
	GLfloat color2[3] = {0.00,0.00,0.00};

	width = width/2;
	height = height/2;

	for(x = -width; x <= width; x+=1)
	{
		glBegin(GL_LINES);
			//drawLine(x,-height,0,x,height,0,color1,color1);
			drawLine(x,-height,0,x,-height,-depth,	color1,color2);
			drawLine(x,height,0,x,height,-depth,color1,color2);
		glEnd();
	}

	for(y = -height; y <= height; y+=1)
	{
		glBegin(GL_LINES);
			drawLine(-width,y,0,-width,y,-depth,color1,color2);
			drawLine(width,y,0,width,y,-depth,color1,color2);
		glEnd();
	}

	/* for(z = 0; z <= depth; z+=1)
	{
		glBegin(GL_LINES);
			drawLine(-width,-height,-z,width,-height,-z,color1,color2);
			drawLine(-width,-height,-z,-width,height,-z,color1,color2);
			drawLine(width,-height,-z,width,height,-z,color1,color2);
			drawLine(-width,height,-z,width,height,-z,color1,color2);
		glEnd();
	} */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}
