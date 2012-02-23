// wiimote head tracking for linux

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



// Math functions
// =================================================================================
// calculate the distance between (x1,y1) and (x2,y2)
double calc_distance(double x1, double y1, double x2, double y2)
{
	double xdiff = x1-x2;
	double ydiff = y1-y2;
	double distsqr = (xdiff*xdiff)+(ydiff*ydiff);
	return sqrt(distsqr);
}

// generate a random coordinate in the square formed by corners (0,0) and (min,max)
GLfloat randCoord(float min, float max)
{
	float random = ((float)rand()/(float)RAND_MAX) * (max-min) + min;
	return random;
}


// TrackWiimote
// ==================================================================================
TrackWiimote::TrackWiimote(){
        m_connect_attempts = 0; 
        m_max_attempts = 10;
        m_bt_address = *BDADDR_ANY;
        m_wiimote = NULL;
}

int TrackWiimote::updateState()
{
   return cwiid_get_state(this->m_wiimote, &(this->m_state));
}

int TrackWiimote::connect()
{
    printf("Wiimote: Press 1 and 2 simultaneously to connect the Wiimote.\n");

    this->m_wiimote = cwiid_connect(&(this->m_bt_address), CWIID_FLAG_CONTINUOUS|CWIID_FLAG_NONBLOCK);
    if (!this->m_wiimote)
    {
	    printf("Unable to connect to Wiimote.\n");
	    return 0;
    }
    printf("Established connection to Wiimote!\nInitializing...\n");
    if (cwiid_command(this->m_wiimote, CWIID_CMD_LED, CWIID_LED1_ON|CWIID_LED4_ON)!=0)
    {
	    printf("Failed attempting to enable LED1 and LED4.\n");
	    return 0;
    }
    if (cwiid_command(this->m_wiimote, CWIID_CMD_RPT_MODE,CWIID_RPT_IR|CWIID_RPT_ACC|CWIID_RPT_BTN)!=0)
    {
	    printf("Failed to enable Wii functions.\n");
	    return 0;
    }
    if (cwiid_get_state(this->m_wiimote, &(this->m_state))!=0)
    {
	    printf("Failed to get Wii remote state.\n");
	    return 0;
    }
    printf("Initialization complete!\n");
    return 1;
}

int TrackWiimote::updateIRPositions(double ir_positions[][2], int ir_sizes[])
{
	/* returns 1 if 2 or more positions are valid, otherwise returns 0 or -1 */
	int ret = -1;

	if (this->m_wiimote) /* if the wiimote is still connected */
	{
		int i;
		for (i=0; i<4; i++)
		{
			if (this->m_state.ir_src[i].valid)
			{
				ir_positions[i][COORD_X] = ((this->m_state.ir_src[i].pos[CWIID_X] * HRANGE
								/ (double)CWIID_IR_X_MAX) -(HRANGE/2));
				ir_positions[i][COORD_Y] = (this->m_state.ir_src[i].pos[CWIID_Y] * VRANGE
								/ (double)CWIID_IR_Y_MAX) -(VRANGE/2);
				if (this->m_state.ir_src[i].size != -1)
					ir_sizes[i] = this->m_state.ir_src[i].size+1;

				ret++;
			}
		}
		return ret;
	}
	return 0;
}

/* update the camera position using ir points camerapt_1 and camerapt_2 */
int TrackWiimote::updateHeadPos(head_info_t *headinfo, double ir_positions[][2], int ir_sizes[], int pt1, int pt2)
{
	double cur_pt_dist =     calc_distance(ir_positions[pt1][COORD_X],
						ir_positions[pt1][COORD_Y],
						ir_positions[pt2][COORD_X],
						ir_positions[pt2][COORD_Y]);
	double x = (ir_positions[pt1][COORD_X] + ir_positions[pt2][COORD_X])/2;
	double y =  (ir_positions[pt1][COORD_Y] + ir_positions[pt2][COORD_Y])/2;

	double z_scale = (headinfo->ini_point_dist/cur_pt_dist);

	if (z_scale > 100 || cur_pt_dist < 0.01 || cur_pt_dist > 100.0)
		return 0;

	headinfo->cur_wii_to_real = z_scale * headinfo->ini_wii_to_real; // is this needed?
	headinfo->eye_pos[COORD_X] = x * headinfo->cur_wii_to_real;
	headinfo->eye_pos[COORD_Y] = y * headinfo->cur_wii_to_real;
	headinfo->eye_pos[COORD_Z] =  z_scale * headinfo->ini_pos[COORD_Z];
	return 1;
}


void TrackWiimote::calibrate(head_info_t *headinfo, double ir_positions[][2], int ir_sizes[], int pt1, int pt2){
	double cur_pt_dist =  calc_distance(ir_positions[pt1][COORD_X],
						ir_positions[pt1][COORD_Y],
						ir_positions[pt2][COORD_X],
						ir_positions[pt2][COORD_Y]);

	printf("Old distance between IR sources: %.2fcm. New: %.2fcm.\n",headinfo->ini_point_dist,cur_pt_dist);
	headinfo->ini_point_dist = cur_pt_dist;
	printf("Assuming your head is %.2f units (usually cm) from the wiimote, calibration is complete.\n",headinfo->ini_pos[COORD_Z]);
}
