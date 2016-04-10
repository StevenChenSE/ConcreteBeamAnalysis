#include"materials.h"
#include <iostream>
using namespace std;
bool Steel::isYielded(double strain)const
{
    return strain*Es>=fy;
}

string Steel::getName() const
{
    return name;
}

double Steel::getFy() const
{
    return fy;
}
Steel::Steel(const string &str, double E, double f):name(str),Es(E),fy(f)
{
}




string Concrete::getName() const
{
    return name;
}

double Concrete::getE0() const
{
    return e0;
}
Concrete::Concrete(const string &str, double _Aa, double _Ad, double  _e0,
                   double _ft, double _fc, double _e00, double _eu, double _n):
    name(str),Aa(_Aa),Ad(_Ad),e0(_e0),fc(_fc),ft(_ft),
    e00(_e00),eu(_eu),n(_n)
{
    et0=0.65*pow(ft,0.54)*1e-4;
}




bool Concrete::compressionFailure(double e,bool ver)
{
    return e>e0&&-stress(-e,ver)<0.5*fc;
}

bool Concrete::tensialFailure(double e)
{
    return e>=et0;
}


bool operator!=(const Steel &a, const Steel &b)
{
    return a.getName()!=b.getName();
}


bool operator!=(const Concrete &a, const Concrete &b)
{
    return a.getName()!=b.getName();
}
