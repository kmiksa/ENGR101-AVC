#include <stdio.h>
#include <time.h>
#include "E101.h"

#define CAMERA_MAX_X 319
#define CAMERA_MAX_Y  240
#define LEFT_MOTOR_PIN 1
#define RIGHT_MOTOR_PIN 2


void move(int error){
	int Kp = 0.005; // 0.005,0.05 test it bitch !
	unsigned char forward_speed = 25;
	unsigned char difference = error*Kp;
	int v_left = forward_speed + difference; 
	int v_right = forward_speed - difference; 
	set_motor(LEFT_MOTOR_PIN, v_left);
	set_motor(RIGHT_MOTOR_PIN, v_right);
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

int measure_error(int white_threshold){
	int whi[320];
	int error = 0;
	int np = 0;  // number of white pixels
    for (int i = 0; i <320;i++)
	{
		whi[i]= 0 ;
		int pix = get_pixel(120,i,3);
		if ( pix > white_threshold)
		{
			whi[i] = 1;
		}
    }
    for (int i = 0; i < 320; i++){
		error = error + (i-160)*whi[i];
		if (whi[i] == 1){
			np = np + 1;
			}
		}
    if(np == 0){
		return error = -10000;
		}
	else if(np == 320) {
		return error = 10000;
		}
	else{
		return error = error/np; 
		}
	
}

void dead_stop(){
        set_motor(1, 0);
        set_motor(2, 0);
}

void move_forward(){
        set_motor(1,15*1.5);
        set_motor(2,75*1.68);
        sleep1(0 , 10000);
        dead_stop();

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

void get_corners(int white_threshold){
    take_picture();
    
    int error = measure_error(white_threshold);
    int yl = scan_line(20,20,0,CAMERA_MAX_Y);
    int yr = scan_line(CAMERA_MAX_X-20,CAMERA_MAX_X-20,0,CAMERA_MAX_Y);
    
    while(error > -10000 && error < 10000) {
        error = measure_error(white_threshold);
        move(error);
     if((error > -10000 && error < 10000) && (yl > white_threshold && yr > white_threshold))  {
      move_forward();
      while(error != 0){
		  //turn right 
		  set_motor(1,75*1.5);
		  set_motor(2,75*1.68);
		  }
    } else{
        //dead end turn back 
        move_forward();
		while(error != 0){
		  //turn right 
		  set_motor(1,75*1.5);
		  set_motor(2,75*1.68);
		  }
    }
}
}

int main(){
	init();
	
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
			int white_threshold = calculate_white_threshold();
			int error = measure_error(white_threshold);
			if(error == 10000){
				quadrant ++;
				}
			move(error);
			}
		if(quadrant == 2){
			int white_threshold = calculate_white_threshold();
			get_corners(white_threshold);
			
			}	
		}
	}
