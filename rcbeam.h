#ifndef RCBEAM_H
#define RCBEAM_H
#include"materials.h"
#include <vector>
#include <QString>
using namespace std;
class RCBeam
{
    friend class MainWindow;
public:
    enum plotMode{
        fai_plot,sigmas_plot,sigmac_plot,xn_plot
    };

    RCBeam(Steel _steelBar=Steel(), Concrete _concrete=Concrete(), double _b=0.250, double _h=0.500, double _a=0.025, double ro=0.0130);
    void setFai(double value);
    double getM() const;
    void checkAssumption(double ec);
    void calculate();
    void simulate();
    void plot(plotMode mode);

   //setter
    void setReinforcementRatio(double value);
    void setConcrete(const Concrete &value);
    bool setA(double value);
    void setB(double value);
    void setH(double value);
    void setBar(const Steel &value);
    void setPrecision(double error_data,double epsilon);
    void setAs(double value);
    bool setADot(double value);
    void setAsDot(double value);
    void setN(double value);
    double getA() const;
    void setFaiMax(double value);
    void setErrorControl(bool value);
    void setModelVersion(bool value);
   //getter
    double getEu();
    double getFaiu();
    double eu() const;
    double faiu() const;
    double getMu() const;
    double getFaiY();
    double getksain();
    double getError() const;
    double getRatio() const;
    double getADot() const;
    const vector<double> & getData(int index) const;
    double getGamas()const;
    QString failureType() const;

private:
    double PRECISION,EPSILON;
    Steel bar;
    Concrete concrete;
    double b;
    double h;
    double a;
    double As;
    double fai;
    double h0;
    double e;
    double aDot;
    double AsDot;
    double N;

    double C,yc,Tc,yt,Ts,xn,es,TsDot,esDot;

    double Nc;
    double M;
    int index;
    vector<double> M_data,fai_data,ksain_data,sigmas_data,sigmac_data,e_data,error_data;
    double Mu;
    double startPoint;
    double faiMax;
    bool errorControl;
    bool modelVersion;

};

#endif // RCBEAM_H
