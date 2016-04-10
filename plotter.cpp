#include "plotter.h"
#include <vector>
#include <fstream>
#include <QMessageBox>
using namespace std;


void Plotter::setHighResolution(bool value)
{
    highResolution = value;
}

void Plotter::setShowGrid(bool value)
{
    showGrid = value;
}
void Plotter::setSmooth(bool value)
{
    smooth = value;
}

Plotter::Plotter(QObject *parent, const RCBeam *b):
    QObject(parent),beam(b),
    plotInLine(false),highResolution(false),showGrid(false)
{
    process=new QProcess(this);
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(errorFound(QProcess::ProcessError)));
}
void Plotter::setGnuplot(const QString &value)
{
    gnuplot = value;
}

void Plotter::errorFound(QProcess::ProcessError error)
{
    QString errorMessage;
    switch (error) {
    case QProcess::FailedToStart:
        errorMessage="gnuplot not found!\nPlease install gnuplot and set gnuplot PATH!";
        break;
    case QProcess::Crashed:
        errorMessage="Crashed!";
        break;
    case QProcess::WriteError:
        errorMessage="Write error!";
        break;
    case QProcess::ReadError:
        errorMessage="Read error!";
        break;
    case QProcess::UnknownError:
        errorMessage="Unknown error!";
        break;
    case QProcess::Timedout:
        errorMessage="Time out!";
        break;
    }
    QMessageBox::warning(qobject_cast<QWidget*>(this->parent()),"error",errorMessage);

}

void Plotter::processFinished(int exitCode,QProcess::ExitStatus)
{
    if(exitCode)
    {
        QMessageBox::warning(qobject_cast<QWidget*>(this->parent()),"error","Process error!");
    }
    else
        emit plotFinished();
}


void Plotter::setPlotInLine(bool value)
{
    plotInLine = value;
}

void Plotter::plot(int a, int b,const QString & name1,const QString & name2)
{
    const vector<double> & va=beam->getData(a);
    const vector<double> & vb=beam->getData(b);
    ofstream fout("tmp.data");
    for(size_t i=0;i<va.size();i++)
        fout<<va[i]<<" "<<vb[i]<<endl;
    fout.close();
    fout.open("plot.plt");
    if(highResolution)
        fout<<"set term png size 1024,768"<<endl;
    else
        fout<<"set term png size 640,480"<<endl;
    if(showGrid)
        fout<<"set grid"<<endl;
    else
        fout<<"unset grid"<<endl;
    fout<<"set title \"Plot "<<name1.toStdString()<<" against "<<name2.toStdString()<<" \""<<endl;
    fout<<"set output 'tmp.png'"<<endl;
    fout<<"set xlabel \""<<name1.toStdString()<<"\""<<endl;
    fout<<"set ylabel \""<<name2.toStdString()<<"\""<<endl;
    fout<<"plot 'tmp.data' ";
    if(plotInLine)
        fout<<" with lines";
    if(smooth)
        fout<<" smooth bezier";
    fout<<endl;
    fout.close();
    QStringList args;
    args<<"plot.plt";
    if(process->state()==QProcess::NotRunning)
        process->start(gnuplot,args);

}

