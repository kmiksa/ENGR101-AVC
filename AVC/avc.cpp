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

