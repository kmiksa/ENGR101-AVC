#include <stdio.h>
#include <time.h>
#include "E101.h"

#define CAMERA_MAX_X 319
#define CAMERA_MAX_Y  239
#define LEFT_MOTOR_PIN 2
#define LEFT_MOTOR_OFFSET  0.95 //decimal multipliers to balance motor speed.
#define RIGHT_MOTOR_PIN 1
#define RIGHT_MOTOR_OFFSET 1
#define HARD_TURN 255

void move_back();

int get_direction_from_camera(int white_threshold){
	int err = 0, white_count = 0;

	take_picture();

	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int i = 0; i <= 3; i++){
			int pix = get_pixel(115 + i, x, 3);
			if(pix > white_threshold) white_count++;
			if(x > (CAMERA_MAX_X / 2) )
				err += pix;
			else
				err -= pix;
		}
 	}
	if(white_count < 32)
		return -500;
	 return white_count < 720 ? (err/white_count) % 255 : 500; //normalized value
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


int get_direction_from_maze_position(int white_threshold){
		take_picture();
        
        int xf = scan_line(0,CAMERA_MAX_X,CAMERA_MAX_Y-20,CAMERA_MAX_Y-20);
        int yl = scan_line(20,20,0,CAMERA_MAX_Y);
        int yr = scan_line(CAMERA_MAX_X-20,CAMERA_MAX_X-20,0,CAMERA_MAX_Y);
		
		if(xf > 0)
			return 0;
			
		if(xf < 0 && (yl > white_threshold && yr > white_threshold)){
			return -255;
		}
		
		if(xf < 0 && (yl < white_threshold && yr > white_threshold)) {
			return 255;
		}
		
        if(xf < 0 && (yl > white_threshold && yr < white_threshold)){
			return -255;
		}
        
		else {
		return get_direction_from_camera(white_threshold);	
	}
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
		sleep1( 0 ,20000);
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
        int quadrant = 0;

        while(1){
			if(quadrant == 0){
				connect_to_server("130.195.6.196", 1024);
				send_to_server("Please");
				char pwd[24];
				receive_from_server(pwd);
				send_to_server(pwd);
				sleep1(0, 5000);
				quadrant++;
				
			}
			if(quadrant == 1){
				direction =  get_direction_from_camera(white_threshold);
			    if(direction == 500){
				quadrant++;
				move_forward();
				}
				else if( direction == -500){
					dead_stop();
					move_back();
				}
				else{
					adjust_heading(direction);
					move_forward();
				}
                
			}
			
			if(quadrant == 2){
				direction =  get_direction_from_maze_position(white_threshold);
			     if(direction == 500){
				move_forward();
				}
				else if( direction == -500){
					dead_stop();
					move_back();
				}
				else{
					adjust_heading(direction);
					move_forward();
			}
			if(quadrant == 3){
				break;
			}
          }
        return 0;
}






               
