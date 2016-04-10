#ifndef PLOTTER_H
#define PLOTTER_H

#include <QObject>
#include <QProcess>
#include <vector>
#include "rcbeam.h"
using namespace std;
class Plotter : public QObject
{
    Q_OBJECT
private:
    void generateScript();
    QProcess* process;
    const RCBeam*  beam;
    QString gnuplot;
    bool plotInLine;
    bool highResolution;
    bool showGrid;
    bool smooth;
public:
    explicit Plotter(QObject *parent = 0,const RCBeam * b=new RCBeam());
    void plot(int a, int b, const QString &name1, const QString &name2);
    void setGnuplot(const QString &value);

signals:
    void plotFinished();
public slots:
    void processFinished(int exitCode, QProcess::ExitStatus);
    void errorFound(QProcess::ProcessError error);
    void setPlotInLine(bool value);
    void setHighResolution(bool value);
    void setShowGrid(bool value);
    void setSmooth(bool value);
};

#endif // PLOTTER_H
