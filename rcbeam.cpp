#include "rcbeam.h"
#include <cmath>
#include <integral.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <QDebug>
using namespace std;
RCBeam::RCBeam(Steel _steelBar, Concrete _concrete, double _b, double _h, double _a, double ro):PRECISION(100),EPSILON(1e-5),
    bar(_steelBar),concrete(_concrete),b(_b),h(_h),a(_a),As(ro*b*h),
    fai(0),h0(h-a),aDot(0.025),AsDot(0),N(0),
    M_data(),fai_data(),ksain_data(),sigmas_data(),sigmac_data(),e_data(),error_data(),
    startPoint(0),faiMax(0.02),errorControl(false),modelVersion(true)
{
}
void RCBeam::setFai(double value)
{
    fai = value;
}
double RCBeam::getM() const
{
    return M;
}

void RCBeam::calculate()
{
    double emin=0,emax=h*fai;
    double e=emax/2;
    checkAssumption(e);
    int cnt=0;
    double tmpError=0;
    while(fabs(Tc+Ts+C+TsDot+N)>EPSILON)
    {
        cnt++;
        if(Tc+Ts+TsDot+C+N<0)
        {
            emax=e;
        }
        else{
            emin=e;
        }
        e=(emax+emin)/2;
        checkAssumption(e);
        tmpError=fabs(Tc+Ts+TsDot+C+N)/(Tc+Ts+N);
        if(tmpError<EPSILON||emax-e<1e-10)
            break;
    }
    if(errorControl&&tmpError>EPSILON)
    {
        return;
    }
    yc=IntegrateToMoment(-xn*fai,0,concrete,xn*fai/PRECISION,modelVersion)*b/fai/fai/C*1e6;
    yt=b/fai/fai*IntegrateToMoment(0,(h-xn)*fai,concrete,(h-xn)*fai/PRECISION,modelVersion)/Tc*1e6;
    M=C*yc+Tc*yt+Ts*(h0-xn)+TsDot*(aDot-xn);
    fai_data.push_back(fai);
    M_data.push_back(M/1000.0);
    sigmas_data.push_back(bar.stress(es));
    sigmac_data.push_back(-concrete.stress(-e,modelVersion));
    ksain_data.push_back(xn/h);
    e_data.push_back(e);
    error_data.push_back(fabs(Tc+Ts+TsDot+C+N)/(Tc+Ts+N));
}
void RCBeam::checkAssumption(double ec)
{
    xn=ec/fai;
    es=fai*h0-ec;
    esDot=fai*aDot-ec;
    C=b/fai*IntegrateToForce(-xn*fai,0,concrete,xn*fai/PRECISION,modelVersion)*1e6;
    Tc=b/fai*IntegrateToForce(0,(h-xn)*fai,concrete,(h-xn)*fai/PRECISION,modelVersion)*1e6;
    Ts=bar.stress(es)*As*1e6;
    TsDot=bar.stress(esDot)*AsDot*1e6;
}
void RCBeam::simulate()
{
    M_data.clear();
    e_data.clear();
    sigmac_data.clear();
    sigmas_data.clear();
    ksain_data.clear();
    fai_data.clear();
    error_data.clear();
    h0=h-a;
    for(fai=0.0001;fai<faiMax;fai+=faiMax/1000)
    {
        calculate();
    }
    Mu=0;
    for(size_t i=0;i<M_data.size();i++)
    {
        if(M_data[i]>Mu)
        {
            Mu=M_data[i];
            index=i;
        }
    }
}

void RCBeam::plot(plotMode mode)
{
    ofstream fout;
    switch (mode) {
    case fai_plot:
        fout.open("fai.dat");
        for(size_t i=0;i<fai_data.size();i++)
        {
            fout<<fai_data[i]<<"\t"<<M_data[i]<<endl;
        }
        fout.close();
        break;
    case sigmas_plot:
        fout.open("sigmas.dat");
        for(size_t i=0;i<fai_data.size();i++)
        {
            fout<<sigmas_data[i]<<"\t"<<M_data[i]<<endl;
        }
        fout.close();
        break;
    case sigmac_plot:
        fout.open("sigmac.dat");
        for(size_t i=0;i<fai_data.size();i++)
        {
            fout<<sigmac_data[i]<<"\t"<<M_data[i]<<endl;
        }
        fout.close();
        break;
    case xn_plot:
        fout.open("xn.dat");
        for(size_t i=0;i<fai_data.size();i++)
        {
            fout<<ksain_data[i]<<"\t"<<M_data[i]<<endl;
        }
        fout.close();
        break;
    }
}

void RCBeam::setReinforcementRatio(double value)
{
    As=b*h*value/100.0;
}
void RCBeam::setConcrete(const Concrete &value)
{
    concrete = value;
}
bool RCBeam::setA(double value)
{
    if(AsDot)
    {
        if(value/1000.0+aDot<h)
        {
            a=value/1000.0;
            return true;
        }
        return false;
    }
    else
        if(value/1000.0<h)
        {
            a=value/1000.0;
            return true;
        }
    return false;
}
void RCBeam::setB(double value)
{
    b = value/1000.0; //unit convert mm to m
}
void RCBeam::setH(double value)
{
    h = value/1000.0; //unit convert mm to m
}
double RCBeam::getMu() const//unit convert N*m to kN*m
{
    return Mu;
}
void RCBeam::setBar(const Steel &value)
{
    bar = value;
}

double RCBeam::getFaiY()
{
   for(size_t i=0;i<fai_data.size();i++)
   {
       if(sigmas_data[i]==bar.getFy())
       {
           return fai_data[i];
       }
   }
   return -1;
}

double RCBeam::getksain()
{
    return ksain_data[index];
}

const vector<double> &RCBeam::getData(int index)const
{
    switch (index) {
    case 0:
        return M_data;
        break;
    case 3:
        return fai_data;
        break;
    case 2:
        return sigmac_data;
        break;
    case 1:
        return sigmas_data;
        break;
    case 5:
        return e_data;
        break;
    case 4:
        return ksain_data;
        break;
    case 6:
        return error_data;
        break;
    }
}

double RCBeam::getGamas() const
{
    if(AsDot||failureType()!=QStringLiteral("ductile failure(适筋梁)"))
        return -1;
    return getMu()/bar.getFy()/As/h0/1000;
}

QString RCBeam::failureType()const
{
    double ec=eu();
    double es=(h0-ksain_data[index]*h)*faiu();
    if(!bar.isYielded(es))
        return QStringLiteral("brittle failure in compression(超筋梁)");
    if(ec<concrete.getE0())
        return QStringLiteral("brittle failure in tension(少筋梁)");
    return QStringLiteral("ductile failure(适筋梁)");
}

void RCBeam::setAs(double value)
{
    As = value/1000000.0;
}

bool RCBeam::setADot(double value)
{
    if(AsDot)
    {
        if(value/1000.0+a<h)
        {
            aDot = value/1000.0;
            return true;
        }
        return false;
    }
    aDot = value/1000.0;
    return true;
}

void RCBeam::setAsDot(double value)
{
    AsDot = value/1000000.0;
}

void RCBeam::setN(double value)
{
    N = value*1000.0;
}

double RCBeam::getA() const
{
    return a*1000;
}

void RCBeam::setFaiMax(double value)
{
    faiMax = value;
}

void RCBeam::setErrorControl(bool value)
{
    errorControl = value;
}

void RCBeam::setModelVersion(bool value)
{
    modelVersion = value;
}

double RCBeam::getADot() const
{
    return aDot*1000;
}

double RCBeam::getError() const
{
    return error_data[index];
}

double RCBeam::getRatio() const
{
    return As/b/(h-a)*100;
}

void RCBeam::setPrecision(double precision, double epsilon)
{
    PRECISION = precision;
    EPSILON=epsilon;
}



double RCBeam::getEu()
{
    return e_data[index];
}

double RCBeam::getFaiu()
{
    return fai_data[index];
}

double RCBeam::eu()const
{
    return e_data[index];
}

double RCBeam::faiu()const
{
    return fai_data[index];
}





