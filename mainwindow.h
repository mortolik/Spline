#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
namespace Spline
{
class CubicSplineModel;
class CubicSplineWidget;
}
QT_FORWARD_DECLARE_CLASS(QSpinBox);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QTableWidget);
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Spline::CubicSplineModel *splineModel;
    Spline::CubicSplineWidget *splineWidget;
    QSpinBox *nSpinBox;
    QPushButton *updateButton;
    QTableWidget *coeffTable;

    void setupUI();
    void updateSpline();
    void updateTable();
};
#endif // MAINWINDOW_H
