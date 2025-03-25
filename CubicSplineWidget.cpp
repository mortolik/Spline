#include <QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include "CubicSplineModel.hpp"
#include "CubicSplineWidget.hpp"
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

    axisX->setTitleText("X");
    axisY->setTitleText("Y");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    functionSeries->attachAxis(axisX);
    functionSeries->attachAxis(axisY);
    splineSeries->attachAxis(axisX);
    splineSeries->attachAxis(axisY);

    functionSeries->setName("Оригинальная функция");
    splineSeries->setName("Кубический сплайн");

    clearChart();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

void CubicSplineWidget::clearChart()
{
    functionSeries->clear();
    splineSeries->clear();
    chart->axes(Qt::Horizontal).first()->setRange(0, 1);
    chart->axes(Qt::Vertical).first()->setRange(0, 1);
    chart->update();
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
    double a = splineModel->getIntervalA(); 
    double b = splineModel->getIntervalB();
    double step = (b - a) / points;

    for (int i = 0; i <= points; ++i) 
{
        double x = a + i * step;
        functionSeries->append(x, splineModel->function(x));
        splineSeries->append(x, splineModel->evaluate(x));
    }


    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(a, b);
    chart->axes(Qt::Vertical).first()->setRange(
        qMin(functionSeries->points().first().y(), splineSeries->points().first().y()),
        qMax(functionSeries->points().last().y(), splineSeries->points().last().y())
        );

    chart->update();
}
}
