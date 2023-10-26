#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>

#include "sensors/proximity.h"//proximity sensors
#include "epuck1x/uart/e_uart_char.h" //bluetooth
#include "stdio.h"
#include "serial_comm.h"
#include "motors.h"//motor
#include "sensors/VL53L0X/VL53L0X.h"//distance

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);//proximity


int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    messagebus_init(&bus, &bus_lock, &bus_condvar);//Proximity
    proximity_start(0);
    calibrate_ir();

    motors_init();//Initialise the motors
    VL53L0X_start();//Initiate the distance sensor module_distance
    serial_start();//Initialise the UART1 channel_bluetooth

    /* Infinite loop. */
    while (1) {

    	int p0=get_calibrated_prox(0);
    	int p1=get_calibrated_prox(1);
    	int p2=get_calibrated_prox(2);
    	int p3=get_calibrated_prox(3);
    	int p4=get_calibrated_prox(4);
    	int p5=get_calibrated_prox(5);
    	int p6=get_calibrated_prox(6);
    	int p7=get_calibrated_prox(7);
//    	printf("num0: %d/n", p0);
//    	printf("num1: %d/n", p1);
//    	printf("num2: %d/n", p2);
//    	printf("num3: %d/n", p3);
//    	printf("num4: %d/n", p4);
//    	printf("num5: %d/n", p5);
//    	printf("num6: %d/n", p6);
//    	printf("num7: %d/n", p7);
//    	chThdSleepMilliseconds(10);
    	if (VL53L0X_get_dist_mm()>=60 && VL53L0X_get_dist_mm()<=150){
    		if(p0>1.3*p7){
    			left_motor_set_speed(-800);
    			right_motor_set_speed(800);
    		}
    		else if(1.3*p0<p7){
    			left_motor_set_speed(800);
    			right_motor_set_speed(-800);
    		}
//    		else if(p0>1.5*p7 || 1.5*p0<p7) {
//    			left_motor_set_speed(1000);
//    			right_motor_set_speed(-1000);
//    		}
    		else{
				left_motor_set_speed(500);
				right_motor_set_speed(500);
    		}
    		chThdSleepMilliseconds(100);
    	 }



//    	else if(VL53L0X_get_dist_mm()>120 && VL53L0X_get_dist_mm()<=180){
//    		left_motor_set_speed(500);
//    		right_motor_set_speed(500);
//    	}
//    	else if(VL53L0X_get_dist_mm()>=25 && VL53L0X_get_dist_mm()<70) {
//    		if(p0>1.1*p7 || 1.1*p0<p7){
//    			left_motor_set_speed(500);
//    			right_motor_set_speed(-500);
//    		}
//    		else{
//    			left_motor_set_speed(300);
//    			right_motor_set_speed(300);
//    		}
//    	}
    	else if(VL53L0X_get_dist_mm()<60) {
    		left_motor_set_speed(-500);
    		right_motor_set_speed(-500);
    	}
    	else{
    		left_motor_set_speed(500);
    		right_motor_set_speed(500);
    	}
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
