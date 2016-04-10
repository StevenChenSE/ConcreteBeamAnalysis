#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include "plotter.h"
namespace Ui {
class PlotDialog;
}

class PlotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotDialog(QWidget *parent = 0, RCBeam *beam=new RCBeam());
    ~PlotDialog();

private slots:
    void on_pushButton_clicked();
    void displayDiagram();
    void on_setPathPushButton_clicked();


private:
    Ui::PlotDialog *ui;
    Plotter *plotter;
    void initData();
    void initGnuplotPath();
};

#endif // PLOTDIALOG_H
