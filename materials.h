#ifndef MATERIALS
#define MATERIALS
#include <string>
#include <cmath>
using namespace std;
class Steel
{
private:
    string name;
    double Es;
    double fy;
public:
    Steel(const string & name="HRB335",double Es=2e5,double fy=300);
    double stress(double strain) const
    {
        return strain*Es<fy?strain*Es:fy;
    }
    bool isYielded(double strain) const;
    string getName() const;
    double getFy() const;
};
bool operator!=(const Steel & a,const Steel & b);
class Concrete
{
private:
    string name;
    double Aa,Ad,e0,fc;//pressed parameters
    double ft;
    double et0;

    double e00,eu,n;
public:
    Concrete(const string & str="C25", double _Aa=2.09, double _Ad=1.06, double _e0=1560e-6,  double _ft=1.27, double _fc=11.9, double _e00=0.002, double _eu=0.0033, double _n=2);
    double stress(double strain,bool version)const
    {
        if(strain<0)
        {
            if(version)
            {
                double x=-(strain/e0);
                double y=(x<=1)?(Aa*x+(3-2*Aa)*x*x+(Aa-2)*x*x*x):(x/(Ad*(x-1)*(x-1)+x));
                return -y*fc;
            }
            else
            {
                return (-strain<=e00)?-fc*(1-pow((1+strain/e00),n)):((-strain<eu)?-fc:0);
            }
        }
        else
        {
            double x=(strain/et0);
            double At=0.312*ft*ft;
            double y=(x<=1)?(1.2*x-0.2*x*x*x*x*x*x):(x/(At*pow(x-1,1.7)+x));
            return y*ft;
        }
    }
    bool compressionFailure(double e, bool ver);
    bool tensialFailure(double e);
    string getName() const;
    double getE0() const;
};
bool operator!=(const Concrete & a,const Concrete & b);

#endif // MATERIALS

