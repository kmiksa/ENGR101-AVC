#include <stdio.h>
#include <time.h>
#include "E101.h"

#define CAMERA_MAX_X 320
#define CAMERA_MAX_Y 240
#define LEFT_MOTOR_PIN 1
#define LEFT_MOTOR_OFFSET  0.0 //decimal multipliers to balance motor speed.
#define RIGHT_MOTOR_PIN 2
#define RIGHT_MOTOR_OFFSET 0.0

int get_direction_from_camera(int white_threshold){
	int err = 0, white_count = 0;

	take_picture();

	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int y = CAMERA_MAX_Y / 4; y < (CAMERA_MAX_Y / 4) * 3; y++){
			int pix = get_pixel(x, y, 3);
			if(pix > white_threshold) white_count++;
			if(x > (CAMERA_MAX_X / 2) )
				err += pix;
			else
				err -= pix;
		}
	}
	return err/white_count; //normalized value
}

int adjust_heading(int direction){
	return 0;
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
	stop(1);
	stop(2);
}

void move_forward(){
	left_motor(100);
	right_motor(100);
	sleep1(0, 500)
	dead_stop();
}

int calculate_white_threshold(){
	int count = 0, total = 0;
	take_picture();
	for(int x = 0; x < CAMERA_MAX_X; x++){
		for(int y = 0; y < CAMERA_MAX_Y ; y++){
			int pix = get_pixel(x, y, 3);
			total += pix;
			count++;
		}
	}
	return total/count;
}

int main(int argc, char **argv){
	static int white_threshold = calculate_white_threshold();

	move_forward();
	return 0;
}

