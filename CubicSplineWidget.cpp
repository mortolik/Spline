#include <QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include "CubicSplineModel.hpp"
#include "CubicSplineWidget.hpp"
namespace Spline
{
CubicSplineWidget::CubicSplineWidget(QWidget *parent)
    : QWidget(parent), m_splineModel(nullptr)
{
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart);
    m_functionSeries = new QLineSeries();
    m_splineSeries = new QLineSeries();

    m_chart->setMinimumSize(800, 600);
    m_chart->setMaximumSize(1200, 900);
    m_chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_chartView->setMinimumSize(400, 300);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_chart->addSeries(m_functionSeries);
    m_chart->addSeries(m_splineSeries);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisX->setTitleText("X");
    axisY->setTitleText("Y");

    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_functionSeries->attachAxis(axisX);
    m_functionSeries->attachAxis(axisY);
    m_splineSeries->attachAxis(axisX);
    m_splineSeries->attachAxis(axisY);

    m_functionSeries->setName("Оригинальная функция");
    m_splineSeries->setName("Кубический сплайн");

    clearChart();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    setLayout(layout);
}

void CubicSplineWidget::clearChart()
{
    m_functionSeries->clear();
    m_splineSeries->clear();
    m_chart->axes(Qt::Horizontal).first()->setRange(0, 1);
    m_chart->axes(Qt::Vertical).first()->setRange(0, 1);
    m_chart->update();
}
void CubicSplineWidget::setModel(CubicSplineModel *model)
{
    m_splineModel = model;
    connect(m_splineModel, &CubicSplineModel::splineUpdated, this, &CubicSplineWidget::updateChart);
    updateChart();
}

void CubicSplineWidget::updateChart()
{
    if (!m_splineModel)
    {
        return;
    }
    m_functionSeries->clear();
    m_splineSeries->clear();

    int points = 100;
    double a = m_splineModel->getIntervalA();
    double b = m_splineModel->getIntervalB();
    double step = (b - a) / points;

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (int i = 0; i <= points; ++i)
    {
        double x = a + i * step;
        double funcY = m_splineModel->function(x);
        double splineY = m_splineModel->evaluate(x);

        m_functionSeries->append(x, funcY);
        m_splineSeries->append(x, splineY);

        minY = qMin(minY, qMin(funcY, splineY));
        maxY = qMax(maxY, qMax(funcY, splineY));
    }

    if (qFuzzyCompare(minY, maxY))
    {
        minY -= 1.0;
        maxY += 1.0;
    }

    m_chart->createDefaultAxes();
    m_chart->axes(Qt::Horizontal).first()->setRange(a, b);
    m_chart->axes(Qt::Vertical).first()->setRange(minY, maxY);

    m_chart->update();
}
}
