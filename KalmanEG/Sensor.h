
/* 
 * File:   Sensor.h
 * Author: LRD
 *
 * Created on 20 March 2017, 3:14 PM
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "kalman/ekfilter.hpp"


class XSensor : public Kalman::EKFilter<double,1,false,true,false> {
public:
        XSensor();
        double Time;
		int H1,H2;

protected:
        void makeBaseA();
        void makeBaseH();
        void makeBaseV();
        void makeBaseR();
        void makeBaseW();
        void makeBaseQ();

        void makeA();
        void makeH();
        void makeProcess();
        void makeMeasure();

};

typedef XSensor::Vector Vector;
typedef XSensor::Matrix Matrix;

#endif /* SENSOR_H */

