#include "integral.h"
#include <cstdio>
#include <cassert>
#include <iostream>
using namespace std;
double IntegrateToForce(double left, double right, const Concrete& concrete, double step,bool ver)
{
    assert(left<right);
    double tmpV=left;
    double sum=0;
    do{
        sum+=concrete.stress(tmpV,ver)*step;
//        printf("%f %f\n",tmpV,concrete.stress(tmpV));
        tmpV+=step;
    }while(tmpV+step<=right);
    return sum;
}


double IntegrateToMoment(double left, double right, const Concrete &concrete, double step,bool ver)
{
    assert(left<right);
    double tmpV=left;
    double sum=0;
    do{
        sum+=concrete.stress(tmpV,ver)*tmpV*step;
        tmpV+=step;
//    printf("%f",sum);
    }while(tmpV+step<=right);
    return sum;

}


double Integrate(double left, double right, double (*func)(double), double step)
{
    assert(left<right);
    double tmpV=left;
    double sum=0;
    do{
        sum+=func(tmpV)*tmpV*step;
//        printf("%f %f\n",tmpV,concrete.stress(tmpV));
        tmpV+=step;
    }while(tmpV+step<=right);
    return sum;

}

