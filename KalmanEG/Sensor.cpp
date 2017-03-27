#include "Sensor.h"
#include <cmath>
#include <iostream>

using namespace std;

XSensor::XSensor() 
{
        setDim(3, 0, 3, 2, 2);
		//x u w z v
		// sets the number of states, the number of inputs, the number of process noise random variables, the number of measures and the number of measurement noise random variables
        Time =0.001;
		H1=1;
		H2=0;
}

void XSensor::makeBaseA()
{
        A(1,1) = 1;
       // A(1,2) = Time;
      //  A(1,3) = 0.5*Time*Time;

        A(2,1) = 0.0;
        A(2,2) = 1;
        //A(2,3) = Time;

        A(3,1) = 0.0;
        A(3,2) = 0.0;
        A(3,3) = 1.0;

}

void XSensor::makeA()
{
       // A(1,1) = 1;
        A(1,2) = Time;
        A(1,3) = 0.5*Time*Time;

        //A(2,1) = 0.0;
        //A(2,2) = 0.0;
        A(2,3) = Time;

        //A(3,1) = 0.0;
        //A(3,2) = 0.0;
        //A(3,3) = 1.0;
}

void XSensor::makeBaseW()
{
        W(1,1) = 1.0;
        W(1,2) = 0.0;
		W(1,3) = 0.0;
        W(2,1) = 0.0;
        W(2,2) = 1.0;
		W(2,3) = 0.0;
        W(3,1) = 0.0;
        W(3,2) = 0.0;
        W(3,3) = 1.0;
}

void XSensor::makeBaseQ()
{
        Q(1,1) =	0.0001;
        Q(1,2) =	0.0;
		Q(1,3) =	0.0;
		
        Q(2,1) =	0.0;
        Q(2,2) =	0.0001;
		Q(2,3) =	0.0;
		
		Q(3,1) =	0.0;
        Q(3,2) =	0.0;
		Q(3,3) =	0.0001;
}

void XSensor::makeBaseH()
{
        H(1,1) = 0.0;
        H(1,2) = 0.0;

        H(2,1) = 0.0;
        H(2,2) = 0.0;
}

void XSensor::makeH()
{
       H(1,3) = H1;

       H(2,3) = H2;
}

void XSensor::makeBaseV()
{
        V(1,1) = 1;
        V(2,2) = 1;
}

void XSensor::makeBaseR()
{
        R(1,1) = 0.00001;
        R(2,2) = 0.00001;
}

void XSensor::makeProcess()
{
        Vector x_(x.size());
        x_(1) = x(1) +Time*x(2)+ 0.5*Time*Time*x(3);
        x_(2) = x(2) + Time*x(3);
        x_(3) = x(3);
        x.swap(x_);
}

void XSensor::makeMeasure()
{
        z(1)=x(3);
        z(2)=x(3);
}