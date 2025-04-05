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
    void updateMainSpline();
    void updateTestSpline();

private:
    Spline::CubicSplineModel* m_splineModelMain;
    Spline::CubicSplineWidget* m_splineWidgetMain;

    Spline::CubicSplineModel* m_splineModelTest;
    Spline::CubicSplineWidget* m_splineWidgetTest;

    QSpinBox* m_nSpinBox;
    QPushButton* m_updateButton;
    QTableWidget* m_coeffTable;
    QTabWidget *m_tabWidget;

    void setupUI();
    void updateTable(Spline::CubicSplineModel* splineModel);
    QWidget* setupTestFunctionTab();
    QWidget* setupMainFunctionTab();
};
#endif // MAINWINDOW_H
