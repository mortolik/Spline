#pragma once
#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
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

private:
    CubicSplineModel * m_splineModel;
    QChart* m_chart;
    QChartView* m_chartView;
    QLineSeries* m_functionSeries;
    QLineSeries* m_splineSeries;

    void updateChart();
};
}
