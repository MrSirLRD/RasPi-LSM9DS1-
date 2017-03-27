/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: LRD
 *
 * Created on 20 March 2017, 2:10 PM
 */

#include <cstdlib>
#include "Sensor.h"
#include <wiringPiI2C.h>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "SparkFunLSM9DS1.h"

using namespace std;
using namespace Kalman;
LSM9DS1 imu1;
LSM9DS1 imu2;

#define LSM9DS1_M  0x1C // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6A // Would be 0x6A if SDO_AG is LOW

#define LSM9DS1_M1	0x1E // HIGH
#define LSM9DS1_AG1	0x6B 

#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

#define BILLION 1E9

int main(int argc, char** argv) {
	
struct timespec requestStart, requestEnd;
clock_gettime(CLOCK_REALTIME, &requestStart);
double PrevTime=0;
	
	imu1.settings.device.commInterface = IMU_MODE_I2C;
	imu1.settings.device.mAddress = LSM9DS1_M;
	imu1.settings.device.agAddress = LSM9DS1_AG;
  
	imu2.settings.device.commInterface = IMU_MODE_I2C;
    imu2.settings.device.mAddress = LSM9DS1_M1;
    imu2.settings.device.agAddress = LSM9DS1_AG1;
	const unsigned n = 3;	//nb states
	const unsigned m = 2;	//nb measures
	
if (!imu1.begin())
  {
    cout<<"Failed to communicate with LSM9DS1 1."<<endl;
    cout<<"Double-check wiring."<<endl;
    while (1);
  }
	
	if (!imu2.begin())
  {
    cout<<"Failed to communicate with LSM9DS1 2."<<endl;
    cout<<"Double-check wiring."<<endl;
    while (1);
  }

	XSensor filter;
	static const double _P0[] = {100.0*100.0, 0.0, 0.0,
                                 0.0, 10.0*10.0, 0.0,
                                 0.0, 0.0, 25.0*25.0}; 
	Vector x(n);
	Matrix P0(n, n, _P0);
	
	x(1) = 0;
	x(2) = 0;
	x(3) = 0;

	filter.init(x, P0);

	Vector u;
	Vector z(m);
	z(1)=0;
	z(2)=0;
	Vector Out;
	
	double accum;

	 while(1){
		 
		clock_gettime(CLOCK_REALTIME, &requestEnd);
		
		 accum = ( requestEnd.tv_sec - requestStart.tv_sec )
			  + ( requestEnd.tv_nsec - requestStart.tv_nsec )
			  / BILLION;
			
			if ( imu1.accelAvailable() )
				{
					  imu1.readAccel();

				z(1)=imu1.calcAccel(imu1.ax)*9.81;
				}
			
			if ( imu2.accelAvailable() )
				{
					  imu2.readAccel();

				z(2)=imu2.calcAccel(imu2.ax)*9.81;

				}
			
			filter.Time=double(accum-PrevTime);
			cout<<"raw "<<z(1)<<"  "<<z(2)<<" "<< accum <<endl;
			cout<<z(1)<<" "<<z(2)<<endl;
			PrevTime=accum;
			filter.H1=1;
			filter.H2=0;
			filter.measureUpdateStep(z);
			filter.H1=0;
			filter.H2=1;
			filter.measureUpdateStep(z);
			filter.timeUpdateStep(z);
			
			Out = filter.getX();
			 
			cout<<"X-"<<Out(1)<<" V-"<<Out(2)<<" A-"<<Out(3)<<endl;
			
  }
	

	return 0;
}

