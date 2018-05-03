/*
 * avc.c
 * 
 * Copyright 2018 Phillip Lange <phillip@phillip-Notebook>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <time.h>
#include "E101.h"

#define  CAMERA_MAX_X 320
#define  CAMERA_MAX_Y 240


int get_direction_from_camera(){
	int dir = 0, total = 0;
	take_picture();
	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int y = 0; y < CAMERA_MAX_Y; y++){
			if(get_pixel(x, y, 3) > 155){
				total++;
				if(x > (CAMERA_MAX_X / 2) )
					dir++;
				else
					dir--;
			}
		}
	} 
	if (total < 10) dir = 90;	
	return dir;
}

int main(int argc, char **argv)
{
	
	return 0;
}

