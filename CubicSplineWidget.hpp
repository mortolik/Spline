#pragma once
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_FORWARD_DECLARE_CLASS(QSpinBox);
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
    void clearChart();
    void setModel(CubicSplineModel *model);
    void setSpinBoxValue(int val);

public slots:
    void updateSpline();
    void updateTable();

private:
    CubicSplineModel * m_splineModel;
    QChart* m_chart;
    QChartView* m_chartView;
    QLineSeries* m_functionSeries;
    QLineSeries* m_splineSeries;    
    QSpinBox* m_nSpinBox;
    QPushButton* m_updateButton;

    QTableWidget* m_coeffTable;

    void createChart();

    void updateChart();
};
}
