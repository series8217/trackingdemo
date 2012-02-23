#ifndef TRACKINGSETTINGSINCLUDED
#define TRACKINGSETTINGSINCLUDED

// wiimote tracking demo for linux

// by Steven Thomas Snyder, stsnyder@ucla.edu

/* trackingdemo

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

// Settings for rendering engine
// ==================================================================================
#define DEFAULTWIDTH 1600
#define DEFAULTHEIGHT 1200

// maximum view distance
#define VIEWFARPLANE 500.0


// Head tracking settings
// =====================================

// this defines the units we will be using for the coordinate system. 
// use #define UNITS xxx where xxx is the number of your units in a real meter.
// for example, if you want to use centimeters, you would write #define UNITS 100.0
#define UNITS 100.0
// from here on, the units will be referred to as cm, since this is the default.

// screen dimensions in cm
#define SCREENHEIGHT 30.80
#define SCREENWIDTH 40.64

// distance from origin (center of screen) to vertical screen edges, horizontal screen edges (in cm)
#define V_SCREENEDGE SCREENHEIGHT/2
#define H_SCREENEDGE SCREENWIDTH/2

// Wiimote related settings
// ========================

// initial head distance from wiimote in cm
#define INITIALDISTANCE 50.0
#define INITIALEYEHEIGHT 0.0
#define INITIALEYEXOFFSET 0.0

// vertical distance from center of screen to wiimote//#define WIIDISABLE  // define this to disable all wiimote support at compile time
#define WIIMOTEOFFSET 0.0

// IR bar width (distance between IR leds on head bar) in cm
#define IRBARWIDTH 6.0


#define WIIFOVH 45 // wiimote's horizontal field of view in degrees
#define TANWIIFOVH 0.4142 // tangent of half the horizontal field of view, i.e. tan(WIIFOVH/2)
#define WIIFOVV 37 // wiimote's vertical field of view
#define TANWIIFOVV 0.3346 // tangent of half the vertical field of view

// desired IR dot coordinate range.
// the coordinates will range from -RANGE/2 to +RANGE/2
// with 0,0 always being center of the wiimote's field of view
#define HRANGE 100
#define VRANGE HRANGE*TANWIIFOVV*2





#endif
