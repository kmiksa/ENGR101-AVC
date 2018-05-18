#include <stdio.h>
#include <time.h>
#include "E101.h"

#define CAMERA_MAX_X 319
#define CAMERA_MAX_Y  240
#define LEFT_MOTOR_PIN 1
#define LEFT_MOTOR_OFFSET  1.5 //decimal multipliers to balance motor speed.
#define RIGHT_MOTOR_PIN 2
#define RIGHT_MOTOR_OFFSET 1.68

void move_back();

int get_direction_from_camera(int white_threshold){
	int err = 0, white_count = 0;

	take_picture();

	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int y = CAMERA_MAX_Y / 4; y < (CAMERA_MAX_Y / 4) * 3; y++){
			int pix = get_pixel(y, x, 3);
			if(pix > white_threshold) white_count++;
			if(x > (CAMERA_MAX_X / 2) )
				err += pix;
			else
				err -= pix;
		}
 	}
	return white_count < 10000  ? (err/white_count) % 256 : -500; //normalized value
}

int scan_line(int sx, int ex, int sy, int ey){ //start x, end x, start y, end  y 
    take_picture();
    
    int max = 0;
    int min =255;
    
    for(int x = sx; x <= ex; x++){
        for(int y = sy; y <= ey; y++){
            int pix = get_pixel(y, x, 3);
            if ( pix > max) {
			max = pix;
            }
            if (pix < min) {
			min =pix;
            }
        }
    }
    return max; 
}
int get_corners(int white_threshold){
    take_picture();
    
    
    
    int x = scan_line(0, CAMERA_MAX_X, CAMERA_MAX_Y - 20, CAMERA_MAX_Y -20);
    int yl = scan_line(20,20,0,CAMERA_MAX_Y);
    int yr = scan_line(CAMERA_MAX_X-20,CAMERA_MAX_X-20,0,CAMERA_MAX_Y);
    
    if(x > white_threshold) {
        return 0; 
    } else if(x < white_threshold && (yl > white_threshold && yr > white_threshold))  {
      // cut the picture in half ? 
     // if we want to cut in only into right half should we define x used in the loop in get direction in the field and set it to CAMERA_MAX_X/2 here ? so we would have loop from xmax/2 to xmax 
    } else{
        //dead end turn back 
    }
}

bool scan_right(int white_threshold){
	int err = 0, white_count = 0;

	take_picture();

	for(int x = CAMERA_MAX_X/2; x < CAMERA_MAX_X; x++){
		for(int y = CAMERA_MAX_Y / 4; y < (CAMERA_MAX_Y / 4) * 3; y++){
			int pix = get_pixel(y, x, 3);
			if(pix > white_threshold) white_count++;
			
		}
 	}
	return white_count > 5000;
}

void left_motor(int speed){
        //Set left motor speed accounting for offset
        //REMEMBER TO STOP THE MOTOR AFTER CALLING THIS
        set_motor(LEFT_MOTOR_PIN, speed * LEFT_MOTOR_OFFSET);
}

void right_motor(int speed){
        //Set right motor speed accounting for offset
        //REMEMBER TO STOP THE MOTOR AFTER CALLING THIS
 	set_motor(RIGHT_MOTOR_PIN, speed * RIGHT_MOTOR_OFFSET);
}


void dead_stop(){
        set_motor(1, 0);
        set_motor(2, 0);
}

void adjust_heading(int direction){
        left_motor(-direction);
        right_motor(direction );
        sleep1(0, 7500);
        dead_stop();
}

void move_forward(){
        left_motor(75);
        right_motor(75);
        sleep1(0 , 10000);
        dead_stop();

}

void move_back(){
		left_motor(-200);
		right_motor(-200);
		sleep1( 0 ,10000);
		dead_stop();
}

int calculate_white_threshold(){
	int count = 0, total = 0;
	take_picture();
	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int y = 0; y < CAMERA_MAX_Y ; y++){
			int pix = get_pixel(y, x, 3);
			total += pix;
			count++;
		}
	}
	return total/count;
}

int main(){
        init();
        int white_threshold = calculate_white_threshold();
        int direction = 0, i = 0;
        int quadrant = 1; // shouldn't be 0 ?

        while(1){
			if(quadrant = 0){
				connect_to_server();
				send_to_server("Please");
				char pwd[24](recieve_from_server());
				send_to_server(pwd);
				sleep1(1,0);
				quadrant++;
				
			}
			if(quadrant = 1){
				direction =  get_direction_from_camera(white_threshold);
				if(direction == -500){
					quadrant++;
					continue;
				}
                adjust_heading(direction);
                move_forward();
                
			}
			if(quadrant = 2){
				break;
			}
			if(quadrant = 3){
				break;
			}
          }

        return 0;
}





               
