
#include <QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "CubicSplineWidget.hpp"
#include "CubicSplineModel.hpp"
namespace Spline
{
CubicSplineWidget::CubicSplineWidget(QWidget *parent)
    : QWidget(parent), splineModel(nullptr)
{
    chart = new QChart();
    chartView = new QChartView(chart);
    functionSeries = new QLineSeries();
    splineSeries = new QLineSeries();

    chart->addSeries(functionSeries);
    chart->addSeries(splineSeries);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    functionSeries->attachAxis(axisX);
    functionSeries->attachAxis(axisY);
    splineSeries->attachAxis(axisX);
    splineSeries->attachAxis(axisY);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

void CubicSplineWidget::setModel(CubicSplineModel *model)
{
    splineModel = model;
    connect(splineModel, &CubicSplineModel::splineUpdated, this, &CubicSplineWidget::updateChart);
    updateChart();
}

void CubicSplineWidget::updateChart()
{
    if (!splineModel)
        return;

    functionSeries->clear();
    splineSeries->clear();

    int points = 100;
    double a = 0.2, b = 2.0;
    double step = (b - a) / points;

    for (int i = 0; i <= points; ++i)
    {
        double x = a + i * step;
        functionSeries->append(x, splineModel->function(x));
        splineSeries->append(x, splineModel->evaluate(x));
    }

    chart->update();
}
}
