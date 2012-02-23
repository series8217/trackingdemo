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
    along with this program (gpl.txt).  If not, see <http://www.gnu.org/licenses/>.

This software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details.
FreeImage is used under the (GNU GPL or FIPL), version (license version).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
trackingdemo 0.30x by Steven Snyder (stsnyder@ucla.edu), http://steventsnyder.com
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Version info at bottom of this file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


HOW TO USE
==========

This is still a very rough version. I apologize about any accessibility problems. I plan to add easy access to screen size, IR bar width calibration, etc very soon. It inconveniences me as well, since I use a few different computers and screens to develop this on, so I do have motivation to do so!

Hardware required:

[] Wiimote: ~$45 from any store that sells video games and systems.
[] IR LED bar: ~$5 in parts from Radioshack. You need two infrared LEDs on a bar mounted to your head. You can use any spacing you like. I recommend at least 6cm.
[] Bluetooth interface: A linux compatible blue-tooth interface. You can get a USB Bluetooth adapter if your computer doesn't have built-in bluetooth support.

Software required:

[] Cwiid: available at http://abstrakraft.org/cwiid/
[] BlueZ: the official Linux bluetooth protocol stack: http://www.bluez.org/
[] FreeImage: Open source image library used for loading textures. http://freeimage.sourceforge.net
[] Mesa GL: This is the Linux OpenGL implementation. http://mesa3d.sourceforge.net/ It should include GLUT. If not, you'll need to get that seperately.
[] gcc: GNU compiler collection. you need the c++ compiler. you could probably use another c++ compiler but you will need to edit the included makefile.

What to do:
Extract all of the files in this archive to your desired directory. Assuming you have all of the software required above, change to this directory in a terminal and type: make
and press enter. It should compile.
Now run the program by typing: ./trackingdemo then press enter. Press 1 and 2 on your wiimote to connect.


How to calibrate for your IR bar
================================
Move your head 50cm from the screen and press 'c'.

You can also set this at compile time. It's currently set for 6cm because that's the size of my bar.
Change this line in trackingsettings.h:
#define IRBARWIDTH 6.0
Replace the "6.0" with the distance between your LEDs in centimeters.

How to change resolutions
=========================
For now, the default resolution is 800 x 600. If you want another resolution, use one of the following commands line options:
-800
-1024
-1280
-1600
The number corresponds to the width of the screen. I assume you are using a screen with a 4:3 aspect ratio. If not... wait for the next version, or recompile with your desired default resolution and run without any command line options. To change the resolution at compile time, change the following lines in trackingsettings.h:
#define DEFAULTWIDTH 1600
#define DEFAULTHEIGHT 1200


How to make it work better for your screen:
===========================================
Change the following lines in trackingsettings.h
#define SCREENHEIGHT 30.80
#define SCREENWIDTH 40.64
Replace the height and width values with the height and width of your screen in centimeters.



=========
Revisions
=========

0.30
=====
-code rewritten in C++

0.20
=====
-objects in front of the screen plane are now rendered properly.
-head tracking works better

0.10x
=====
-first version
-head tracking works
-objects in front of the screen plane will not appear.
