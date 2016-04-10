#include "plotdialog.h"
#include "ui_plotdialog.h"
#include "mainwindow.h"
#include <QSettings>
#include <QFileDialog>

PlotDialog::PlotDialog(QWidget *parent,RCBeam* beam):
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);
    plotter=new Plotter(this,beam);
    initData();
    connect(plotter,SIGNAL(plotFinished()),this,SLOT(displayDiagram()));
    connect(ui->lineCheckBox,SIGNAL(toggled(bool)),plotter,SLOT(setPlotInLine(bool)));
    connect(ui->highResolutionCheckBox,SIGNAL(toggled(bool)),plotter,SLOT(setHighResolution(bool)));
    connect(ui->showGridCheckBox,SIGNAL(toggled(bool)),plotter,SLOT(setShowGrid(bool)));
    connect(ui->smoothCheckBox,SIGNAL(toggled(bool)),plotter,SLOT(setSmooth(bool)));
    ui->groupBox->setAlignment(Qt::AlignCenter);
    setWindowModality(Qt::NonModal);
    initGnuplotPath();
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::initData()
{
    QStringList argList;
    argList<<QStringLiteral("M(kN·m)")<<QStringLiteral("σs(N/mm^2)")<<QStringLiteral("σc(N/mm^2)")
          <<QStringLiteral("φ")<<QStringLiteral("ξn")<<QStringLiteral("εc");
    ui->xCombo->addItems(argList);
    argList<<tr("error");
    ui->yCombo->addItems(argList);
    ui->xCombo->setCurrentIndex(3);
    ui->yCombo->setCurrentIndex(0);
}

void PlotDialog::initGnuplotPath()
{
    QSettings settings("Tsinghua","Concrete");
    plotter->setGnuplot(settings.value("PATH").toString());
    ui->lineEdit->setText(settings.value("PATH").toString());
}

void PlotDialog::on_pushButton_clicked()
{
    plotter->plot(ui->xCombo->currentIndex(),ui->yCombo->currentIndex(),
                  ui->xCombo->currentText(),ui->yCombo->currentText());
}

void PlotDialog::displayDiagram()
{
    ui->diplayLabel->setPixmap(QPixmap("tmp.png"));
    ui->diplayLabel->setSizeIncrement(ui->diplayLabel->pixmap()->size());
    ui->diplayLabel->setAlignment(Qt::AlignCenter);
}


void PlotDialog::on_setPathPushButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,tr("select Path of gnuplot"),"C:","gnuplot executable (gnuplot.exe)");
    ui->lineEdit->setText(path);
    if(!path.isEmpty())
    {
        QSettings settings("Tsinghua","Concrete");
        settings.setValue("PATH",path);
        plotter->setGnuplot(path);
    }
}


