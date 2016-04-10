#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QButtonGroup>
#include "materials.h"
#include "rcbeam.h"
#include "plotdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    const RCBeam  & getBeam() const;

private slots:
    void updateData();
    void on_aSpinBox_editingFinished();
    void on_barComboBox_currentIndexChanged(int index);
    void on_concreteComboBox_currentIndexChanged(int index);
    void on_hSpinBox_editingFinished();
    void on_bSpinBox_editingFinished();
    void on_pushButton_2_clicked();
    void on_checkBox_toggled(bool checked);
    void on_qtPushButton_clicked();
    void on_aboutGnuplotPushButton_clicked();
    void on_aboutAuthorPushButton_clicked();
    void on_asSpinBox_editingFinished();
    void on_asDotSpinBox_editingFinished();
    void on_aDotSpinBox_editingFinished();
    void on_nSpinBox_editingFinished();
    void modeChange();

    void on_faiMaxDoubleSpinBox_editingFinished();

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_3_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QVector<Concrete> concreteList;
    QVector<Steel> barList;
    RCBeam beam;
    PlotDialog* plotDialog;
    QButtonGroup* mode;
    bool status;
    void initData();
    void initUi();
    void initTable();
    void setTableView();
    void refresh();
    void initMode();
    QString failureType();
};

#endif // MAINWINDOW_H
