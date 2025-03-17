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
    void setModel(CubicSplineModel *model);

private:
    CubicSplineModel *splineModel;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *functionSeries;
    QLineSeries *splineSeries;

    void updateChart();
};
}
