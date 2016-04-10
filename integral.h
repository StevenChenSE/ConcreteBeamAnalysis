#ifndef INTEGRAL_H
#define INTEGRAL_H
#include "materials.h"
double IntegrateToForce(double left, double right, const Concrete& concrete, double step, bool ver);
double Integrate(double left,double right,double (*func)(double),double step);
double IntegrateToMoment(double left, double right, const Concrete& concrete, double step, bool ver);
double IntegrateSN(double left,double right,double (*func)(double),double step);

#endif // INTEGRAL_H
