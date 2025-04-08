#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
namespace Spline
{
class CubicSplineModel;
class CubicSplineWidget;
}

QT_FORWARD_DECLARE_CLASS(QTabWidget);
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

private slots:

private:
    Spline::CubicSplineModel* m_splineModelMain;
    Spline::CubicSplineWidget* m_splineWidgetMain;

    Spline::CubicSplineModel* m_splineModelTest;
    Spline::CubicSplineWidget* m_splineWidgetTest;

    Spline::CubicSplineModel* m_splineModelOscillating;
    Spline::CubicSplineWidget* m_splineWidgetOscillating;

    QTabWidget *m_tabWidget;

    void setupUI();

    QWidget *setupFunctionTab(Spline::CubicSplineModel* splineModel,
                              Spline::CubicSplineWidget* splineWidget,
                              QString text);
};
#endif // MAINWINDOW_H
