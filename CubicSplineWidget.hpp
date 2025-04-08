#pragma once
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_FORWARD_DECLARE_CLASS(QSpinBox);
QT_FORWARD_DECLARE_CLASS(QTabWidget);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QTableWidget);

QT_CHARTS_USE_NAMESPACE

namespace Spline
{
class CubicSplineModel;
class CubicSplineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CubicSplineWidget(QWidget *parent = nullptr);
    void setModel(CubicSplineModel *model);
    void setSpinBoxValue(int val);
    void clearCharts();

private slots:
    void updateSpline();

private:
    CubicSplineModel *m_splineModel;

    QSpinBox *m_nSpinBox;
    QPushButton *m_updateButton;

    QTabWidget *m_chartsTabWidget;
    QTabWidget *m_tablesTabWidget;
    QTableWidget *m_coeffTable;
    QTableWidget *m_errorTable;
    QTableWidget *m_errorDerivTable;
    QTableWidget *m_errorSecondDerivTable;

    QtCharts::QChart *m_chart;
    QtCharts::QChart *m_chartDeriv;
    QtCharts::QChart *m_chartSecondDeriv;
    QtCharts::QChart *m_chartError;
    QtCharts::QChart *m_chartErrorDeriv;
    QtCharts::QChart *m_chartErrorSecondDeriv;
    QtCharts::QLineSeries *m_functionSeries;
    QtCharts::QLineSeries *m_splineSeries;
    QtCharts::QLineSeries *m_functionDerivSeries;
    QtCharts::QLineSeries *m_splineDerivSeries;
    QtCharts::QLineSeries *m_functionSecondDerivSeries;
    QtCharts::QLineSeries *m_splineSecondDerivSeries;
    QtCharts::QLineSeries *m_errorSeries;
    QtCharts::QLineSeries *m_errorDerivSeries;
    QtCharts::QLineSeries *m_errorSecondDerivSeries;

    void setupTables();
    void createCharts();
    void updateCharts();
    void updateCoeffTable();
    void updateErrorTables();
    void setupAxes(QChart* chart);

};
}
