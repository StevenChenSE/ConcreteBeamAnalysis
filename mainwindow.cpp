#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVectorIterator>
#include <QMessageBox>
#include <QTime>
#include <cmath>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),status(false)
{
    ui->setupUi(this);
    initData();
    initUi();
    initTable();
    plotDialog=new PlotDialog(this,&beam);
    status=true;
    on_checkBox_toggled(false);
    refresh();
    initMode();
    ui->nSpinBox->setVisible(false);
    ui->label_9->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData()
{
    if(!status)
        return;
    statusBar()->showMessage(tr("Calculating..."));
    QTime time;
    time.start();
    beam.simulate();
    setTableView();
    statusBar()->showMessage(tr("Data updated.Calculation time:%1").arg(time.elapsed()/1000.0));
}

void MainWindow::initData()
{
    concreteList.append(Concrete("C15", 2.21, 0.41, 1370e-6, 0.91, 7.2 ,0.002,0.0033,2));
    concreteList.append(Concrete("C20", 2.15, 0.74, 1470e-6, 1.1 , 9.6 ,0.002,0.0033,2));
    concreteList.append(Concrete("C25", 2.09, 1.06, 1560e-6, 1.27, 11.9,0.002,0.0033,2));
    concreteList.append(Concrete("C30", 2.03, 1.36, 1640e-6, 1.43, 14.3,0.002,0.0033,2));
    concreteList.append(Concrete("C35", 1.96, 1.65, 1720e-6, 1.57, 16.7,0.002,0.0033,2));
    concreteList.append(Concrete("C40", 1.90, 1.94, 1790e-6, 1.71, 19.1,0.002,0.0033,2));
    concreteList.append(Concrete("C45", 1.84, 2.21, 1850e-6, 1.80, 21.2,0.002,0.0033,2));
    concreteList.append(Concrete("C50", 1.78, 2.48, 1920e-6, 1.89, 23.1,0.002,0.0033,2));
    concreteList.append(Concrete("C55", 1.71, 2.74, 1980e-6, 1.96, 25.3,0.002025,0.00325,1.915));
    concreteList.append(Concrete("C60", 1.65, 3.00, 2030e-6, 2.04, 27.5,0.00205,0.0032,1.83));
    barList.append(Steel("HPB300",2e5,270));
    barList.append(Steel("HRB335",2e5,300));
    barList.append(Steel("HRB400",2e5,360));
    barList.append(Steel("HRB500",2e5,435));
}

void MainWindow::initUi()
{
    QVectorIterator<Concrete> cit(concreteList);
    while(cit.hasNext())
        ui->concreteComboBox->addItem(QString::fromStdString(cit.next().getName()));
    QVectorIterator<Steel> sit(barList);
    while(sit.hasNext())
        ui->barComboBox->addItem(QString::fromStdString(sit.next().getName()));
    ui->concreteComboBox->setCurrentIndex(2);
    ui->barComboBox->setCurrentIndex(2);
}

void MainWindow::initTable()
{
    ui->tableWidget->setRowCount(11);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    QStringList vHeader;
    vHeader<<QStringLiteral("Ultimate Moment(kN·m)")<<QStringLiteral("ε(cu)")
          <<QStringLiteral("Φ(u)")<<QStringLiteral("Φ(y)")
         <<QStringLiteral("ξn")<<QStringLiteral("Failure type")<<"Estimated error"
        <<"Reinforcement ratio(%)"<<QStringLiteral("αs")<<QStringLiteral("ξ")<<QStringLiteral("γs");
    ui->tableWidget->setVerticalHeaderLabels(vHeader);
    ui->tableWidget->show();
    ui->tableWidget->verticalHeader()->setVisible(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnWidth(0,600);
}



void MainWindow::setTableView()
{
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(QString::number(beam.getMu())));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem(QString::number(beam.getEu())));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem(QString::number(beam.getFaiu())));
    ui->tableWidget->setItem(3,0,new QTableWidgetItem(QString::number(beam.getFaiY())));
    ui->tableWidget->setItem(4,0,new QTableWidgetItem(QString::number(beam.getksain())));
    ui->tableWidget->setItem(5,0,new QTableWidgetItem((beam.failureType())));
    ui->tableWidget->setItem(6,0,new QTableWidgetItem(QString::number(beam.getError())));
    ui->tableWidget->setItem(7,0,new QTableWidgetItem(QString::number(beam.getRatio())));
    double ys=beam.getGamas();
    double ksai=2*(1-ys);
    double as=ksai*ys;
    if(ys==-1)
    {
        ys=0,ksai=0,as=0;
    }
    ui->tableWidget->setItem(10,0,new QTableWidgetItem(QString::number(ys)));
    ui->tableWidget->setItem(9,0,new QTableWidgetItem(QString::number(ksai)));
    ui->tableWidget->setItem(8,0,new QTableWidgetItem(QString::number(as)));
    ui->tableWidget->reset();
}

void MainWindow::refresh()
{
        beam.setBar(barList.at(ui->barComboBox->currentIndex()));
        beam.setConcrete(concreteList.at(ui->concreteComboBox->currentIndex()));
        beam.setAs(ui->asSpinBox->value());
        beam.setH(ui->hSpinBox->value());
        beam.setB(ui->bSpinBox->value());
        beam.setA(ui->aSpinBox->value());
        updateData();
}

void MainWindow::initMode()
{
    mode=new QButtonGroup(this);
    mode->addButton(ui->singleRadioButton,0);
    mode->addButton(ui->doubleRadioButton,1);
    connect(ui->singleRadioButton,&QRadioButton::toggled,this,&MainWindow::modeChange);
    connect(ui->doubleRadioButton,&QRadioButton::toggled,this,&MainWindow::modeChange);
    ui->singleRadioButton->setChecked(true);
}


void MainWindow::on_aSpinBox_editingFinished()
{
    if((ui->aSpinBox->value()==beam.a))
        return;
    if(beam.setA(ui->aSpinBox->value()))
    {
        updateData();
    }
    else
    {
        QMessageBox::critical(this,"error",tr("Illegal value!"));
        ui->aSpinBox->setValue(beam.getA());
    }
}

void MainWindow::on_barComboBox_currentIndexChanged(int index)
{
    if(index<0||index>=4)
        return;
    if(barList.at(index)!=beam.bar)
    {
        beam.setBar(barList.at(ui->barComboBox->currentIndex()));
        updateData();
    }
}

void MainWindow::on_concreteComboBox_currentIndexChanged(int index)
{
    if(index<0||index>=10)
        return;
    if(concreteList.at(index)!=beam.concrete)
    {
        beam.setConcrete(concreteList.at(ui->concreteComboBox->currentIndex()));
        updateData();
    }

}


void MainWindow::on_hSpinBox_editingFinished()
{
    if((ui->hSpinBox->value()!=beam.h))
    {
        beam.setH(ui->hSpinBox->value());
        updateData();
    }
}

void MainWindow::on_bSpinBox_editingFinished()
{
    if((ui->bSpinBox->value()!=beam.b))
    {
        beam.setB(ui->bSpinBox->value());
        updateData();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    plotDialog->show();
}
const RCBeam &MainWindow::getBeam() const
{
    return beam;
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        QMessageBox::warning(this,"Caution","This will cause lower speed and unstable performance!");
        beam.setPrecision(1000,1e-5);
    }
    else
        beam.setPrecision(100,5e-3);
    updateData();
}

void MainWindow::on_qtPushButton_clicked()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_aboutGnuplotPushButton_clicked()
{
    QMessageBox::about(this,tr("about gnuplot"),tr("Gnuplot is a portable command-line driven graphing utility for Linux,"
                                                   " OS/2, MS Windows, OSX, VMS, and many other platforms. "
                                                   "The source code is copyrighted but freely distributed "
                                                   "(i.e., you don't have to pay for it). "
                                                   "It was originally created to allow scientists "
                                                   "and students to visualize mathematical functions and data interactively, "
                                                   "but has grown to support many non-interactive uses such as web scripting. "
                                                   "It is also used as a plotting engine by third-party applications like Octave. "
                                                   "Gnuplot has been supported and under active development since 1986.\n"
                                                   "Visit http://www.gnuplot.info for more information."));
}

void MainWindow::on_aboutAuthorPushButton_clicked()
{
    QMessageBox::about(this,tr("About This Program "),tr("This is a simple application design for demonstration (also as a alterantive homework)"
                                                  "for \"Concrete Structures\" course in Tsinghua University.\n"
                                                  "The author is Steven Chen(陈建伟),"
                                                  "a undergraduate student in Tsinghua University,department of civil engineering.\n"
                                                         "Contact Info:  chenjw13@mails.tsinghua.edu.cn\n"
                                                         "Feel free to contact me and get source code at "
                                                         "https://github.com/StevenChenSE/RCBeamAnalysis\n"));
}

void MainWindow::on_asSpinBox_editingFinished()
{
    if((ui->asSpinBox->value()!=beam.As*1000000.0))
    {
        beam.setAs(ui->asSpinBox->value());
        updateData();
    }
}

void MainWindow::on_asDotSpinBox_editingFinished()
{
    if(ui->asDotSpinBox->value()!=beam.AsDot*1000000.0)
    {
        beam.setAsDot(ui->asDotSpinBox->value());
        updateData();
    }
}

void MainWindow::on_aDotSpinBox_editingFinished()
{
    if(ui->aDotSpinBox->value()!=beam.aDot*1000.0)
    {
        if(beam.setADot(ui->aDotSpinBox->value()))
            updateData();
        else
        {
            ui->aDotSpinBox->setValue(beam.getADot());
            QMessageBox::critical(this,tr("error"),tr("Illegal value!"));
        }
    }
}

void MainWindow::on_nSpinBox_editingFinished()
{
    if(ui->nSpinBox->value()!=beam.N/1000)
    {
        beam.setN(ui->nSpinBox->value());
        updateData();
    }
}

void MainWindow::modeChange()
{
    int current=mode->checkedId();
    ui->aDotLabel->setVisible(current);
    ui->aDotSpinBox->setVisible(current);
    ui->AsDotLable->setVisible(current);
    ui->asDotSpinBox->setVisible(current);
    on_asDotSpinBox_editingFinished();
    if(!current)
        ui->asDotSpinBox->setValue(0);
    updateData();
}

void MainWindow::on_faiMaxDoubleSpinBox_editingFinished()
{
    beam.setFaiMax(ui->faiMaxDoubleSpinBox->value());
    updateData();
}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    beam.setErrorControl(checked);
    updateData();
}

void MainWindow::on_checkBox_3_toggled(bool checked)
{
    beam.setModelVersion(!checked);
    updateData();
}
