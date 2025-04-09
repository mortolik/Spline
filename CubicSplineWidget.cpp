#include <QSpinBox>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
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
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    m_nSpinBox = new QSpinBox(this);
    m_nSpinBox->setMaximumWidth(150);
    m_nSpinBox->setRange(2, 10000);
    m_nSpinBox->setValue(4);

    m_updateButton = new QPushButton("Построить сплайн", this);
    controlLayout->addWidget(m_nSpinBox);
    controlLayout->addWidget(m_updateButton);
    mainLayout->addLayout(controlLayout);

    m_chartsTabWidget = new QTabWidget(this);
    createCharts();
    mainLayout->addWidget(m_chartsTabWidget);

    m_tablesTabWidget = new QTabWidget(this);
    m_coeffTable = new QTableWidget(this);
    m_errorTable = new QTableWidget(this);
    m_errorDerivTable = new QTableWidget(this);
    m_errorSecondDerivTable = new QTableWidget(this);
    setupTables();

    m_tablesTabWidget->addTab(m_coeffTable, "Коэффициенты");
    m_tablesTabWidget->addTab(m_errorTable, "Погрешности функции");
    m_tablesTabWidget->addTab(m_errorDerivTable, "Погрешности производной");
    m_tablesTabWidget->addTab(m_errorSecondDerivTable, "Погрешности второй производной");
    m_tablesTabWidget->addTab(m_combinedDerivativesTable, "Общая таблица производных");
    mainLayout->addWidget(m_tablesTabWidget);

    setLayout(mainLayout);

    connect(m_updateButton, &QPushButton::clicked, this, &CubicSplineWidget::updateSpline);
}

void CubicSplineWidget::setupTables()
{
    m_coeffTable->setColumnCount(7);
    m_coeffTable->setHorizontalHeaderLabels({"i", "xi-1", "xi", "ai", "bi", "ci", "di"});
    m_coeffTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_errorTable->setColumnCount(4);
    m_errorTable->setHorizontalHeaderLabels({"x", "F(x)", "S(x)", "F(x)-S(x)"});
    m_errorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_errorDerivTable->setColumnCount(4);
    m_errorDerivTable->setHorizontalHeaderLabels({"x", "F'(x)", "S'(x)", "F'(x)-S'(x)"});
    m_errorDerivTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_errorSecondDerivTable->setColumnCount(4);
    m_errorSecondDerivTable->setHorizontalHeaderLabels({"x", "F''(x)", "S''(x)", "F''(x)-S''(x)"});
    m_errorSecondDerivTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_combinedDerivativesTable = new QTableWidget(this);
    m_combinedDerivativesTable->setColumnCount(10);
    m_combinedDerivativesTable->setHorizontalHeaderLabels({
        "x", "F(x)", "S(x)", "F(x)-S(x)",
        "F'(x)", "S'(x)", "F'(x)-S'(x)",
        "F''(x)", "S''(x)", "F''(x)-S''(x)"
    });
    m_combinedDerivativesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void CubicSplineWidget::clearCharts()
{
    m_functionSeries->clear();
    m_splineSeries->clear();
    m_functionDerivSeries->clear();
    m_splineDerivSeries->clear();
    m_functionSecondDerivSeries->clear();
    m_splineSecondDerivSeries->clear();
    m_errorSeries->clear();
    m_errorDerivSeries->clear();
    m_errorSecondDerivSeries->clear();

    m_chart->axes(Qt::Horizontal).first()->setRange(0, 1);
    m_chart->axes(Qt::Vertical).first()->setRange(0, 1);
    m_chartDeriv->axes(Qt::Horizontal).first()->setRange(0, 1);
    m_chartDeriv->axes(Qt::Vertical).first()->setRange(0, 1);
    m_chartSecondDeriv->axes(Qt::Horizontal).first()->setRange(0, 1);
    m_chartSecondDeriv->axes(Qt::Vertical).first()->setRange(0, 1);
    m_chartError->axes(Qt::Horizontal).first()->setRange(0, 1);
    m_chartError->axes(Qt::Vertical).first()->setRange(0, 1);
}

void CubicSplineWidget::setModel(CubicSplineModel *model)
{
    m_splineModel = model;
    connect(m_splineModel, &CubicSplineModel::splineUpdated, this, [this]()
            {
                updateCharts();
                updateCoeffTable();
                updateErrorTables();                
                updateCombinedDerivativesTable();
            });
    updateCharts();
}

void CubicSplineWidget::setSpinBoxValue(int val)
{
    m_nSpinBox->setValue(val);
}

void CubicSplineWidget::updateSpline()
{
    m_splineModel->setPoints(m_nSpinBox->value());
}

void CubicSplineWidget::updateCharts()
{
    if (!m_splineModel)
    {
        return;
    }

    clearCharts();

    int points = 100;
    double a = m_splineModel->getIntervalA();
    double b = m_splineModel->getIntervalB();
    double step = (b - a) / points;

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();
    double minDerivY = minY, maxDerivY = maxY;
    double minSecondDerivY = minY, maxSecondDerivY = maxY;
    double minErrorY = minY, maxErrorY = maxY;
    double minErrorDerivY = minY, maxErrorDerivY = maxY;
    double minErrorSecondDerivY = minY, maxErrorSecondDerivY = maxY;

    for (int i = 0; i <= points; ++i)
    {
        double x = a + i * step;

        double funcY = m_splineModel->function(x);
        double splineY = m_splineModel->evaluate(x);
        m_functionSeries->append(x, funcY);
        m_splineSeries->append(x, splineY);
        m_errorSeries->append(x, funcY - splineY);

        double funcDeriv = m_splineModel->functionDerivative(x);
        double splineDeriv = m_splineModel->evaluateDerivative(x);
        m_functionDerivSeries->append(x, funcDeriv);
        m_splineDerivSeries->append(x, splineDeriv);
        m_errorDerivSeries->append(x, funcDeriv - splineDeriv);

        double funcSecondDeriv = m_splineModel->functionSecondDerivative(x);
        double splineSecondDeriv = m_splineModel->evaluateSecondDerivative(x);
        m_functionSecondDerivSeries->append(x, funcSecondDeriv);
        m_splineSecondDerivSeries->append(x, splineSecondDeriv);
        m_errorSecondDerivSeries->append(x, funcSecondDeriv - splineSecondDeriv);

        minY = qMin(minY, qMin(funcY, splineY));
        maxY = qMax(maxY, qMax(funcY, splineY));
        minDerivY = qMin(minDerivY, qMin(funcDeriv, splineDeriv));
        maxDerivY = qMax(maxDerivY, qMax(funcDeriv, splineDeriv));
        minSecondDerivY = qMin(minSecondDerivY, qMin(funcSecondDeriv, splineSecondDeriv));
        maxSecondDerivY = qMax(maxSecondDerivY, qMax(funcSecondDeriv, splineSecondDeriv));
        minErrorY = qMin(minErrorY, funcY - splineY);
        maxErrorY = qMax(maxErrorY, funcY - splineY);
        minErrorDerivY = qMin(minErrorDerivY, funcDeriv - splineDeriv);
        maxErrorDerivY = qMax(maxErrorDerivY, funcDeriv - splineDeriv);
        minErrorSecondDerivY = qMin(minErrorSecondDerivY, funcSecondDeriv - splineSecondDeriv);
        maxErrorSecondDerivY = qMax(maxErrorSecondDerivY, funcSecondDeriv - splineSecondDeriv);
    }

    auto setAxisRange = [&a, &b](QChart* chart, double min, double max)
    {
        if (qFuzzyCompare(min, max))
        {
            min -= 1.0;
            max += 1.0;
        }
        chart->axes(Qt::Horizontal).first()->setRange(a, b);
        chart->axes(Qt::Vertical).first()->setRange(min, max);
    };

    setAxisRange(m_chart, minY, maxY);
    setAxisRange(m_chartDeriv, minDerivY, maxDerivY);
    setAxisRange(m_chartSecondDeriv, minSecondDerivY, maxSecondDerivY);
    setAxisRange(m_chartError, minErrorY, maxErrorY);
    setAxisRange(m_chartErrorDeriv, minErrorDerivY, maxErrorDerivY);
    setAxisRange(m_chartErrorSecondDeriv, minErrorSecondDerivY, maxErrorSecondDerivY);
}

void CubicSplineWidget::updateCoeffTable()
{
    if (!m_splineModel) return;

    QVector<double> xValues = m_splineModel->getX();
    QVector<double> a = m_splineModel->getCoefficientsA();
    QVector<double> b = m_splineModel->getCoefficientsB();
    QVector<double> c = m_splineModel->getCoefficientsC();
    QVector<double> d = m_splineModel->getCoefficientsD();

    int n = b.size();
    m_coeffTable->setRowCount(n);
    m_coeffTable->clearContents();

    for (int i = 0; i < n; ++i)
    {
        m_coeffTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        m_coeffTable->setItem(i, 1, new QTableWidgetItem(QString::number(xValues[i])));
        m_coeffTable->setItem(i, 2, new QTableWidgetItem(QString::number(xValues[i + 1])));
        m_coeffTable->setItem(i, 3, new QTableWidgetItem(QString::number(a[i + 1])));

        if (i + 1 < b.size())
        {
            m_coeffTable->setItem(i, 4, new QTableWidgetItem(QString::number(b[i + 1], 'f', 6)));
        }
        else
        {
            double derivAtLast = m_splineModel->functionDerivative(xValues[i + 1]);
            m_coeffTable->setItem(i, 4, new QTableWidgetItem(QString::number(derivAtLast, 'f', 6)));
        }

        m_coeffTable->setItem(i, 5, new QTableWidgetItem(QString::number(c[i + 1], 'f', 6)));
        m_coeffTable->setItem(i, 6, new QTableWidgetItem(QString::number(d[i], 'f', 6)));
    }
}

void CubicSplineWidget::updateErrorTables()
{
    if (!m_splineModel) return;

    int N = 20;
    double a = m_splineModel->getIntervalA();
    double b = m_splineModel->getIntervalB();
    double step = (b - a) / N;

    m_errorTable->setRowCount(N + 1);
    m_errorDerivTable->setRowCount(N + 1);
    m_errorSecondDerivTable->setRowCount(N + 1);

    for (int j = 0; j <= N; ++j)
    {
        double x = a + j * step;

        double F = m_splineModel->function(x);
        double S = m_splineModel->evaluate(x);
        m_errorTable->setItem(j, 0, new QTableWidgetItem(QString::number(x)));
        m_errorTable->setItem(j, 1, new QTableWidgetItem(QString::number(F)));
        m_errorTable->setItem(j, 2, new QTableWidgetItem(QString::number(S)));
        m_errorTable->setItem(j, 3, new QTableWidgetItem(QString::number(F - S)));

        double F_deriv = m_splineModel->functionDerivative(x);
        double S_deriv = m_splineModel->evaluateDerivative(x);
        m_errorDerivTable->setItem(j, 0, new QTableWidgetItem(QString::number(x)));
        m_errorDerivTable->setItem(j, 1, new QTableWidgetItem(QString::number(F_deriv)));
        m_errorDerivTable->setItem(j, 2, new QTableWidgetItem(QString::number(S_deriv)));
        m_errorDerivTable->setItem(j, 3, new QTableWidgetItem(QString::number(F_deriv - S_deriv)));

        double F_second_deriv = m_splineModel->functionSecondDerivative(x);
        double S_second_deriv = m_splineModel->evaluateSecondDerivative(x);
        m_errorSecondDerivTable->setItem(j, 0, new QTableWidgetItem(QString::number(x)));
        m_errorSecondDerivTable->setItem(j, 1, new QTableWidgetItem(QString::number(F_second_deriv)));
        m_errorSecondDerivTable->setItem(j, 2, new QTableWidgetItem(QString::number(S_second_deriv)));
        m_errorSecondDerivTable->setItem(j, 3, new QTableWidgetItem(QString::number(F_second_deriv - S_second_deriv)));
    }
}

void CubicSplineWidget::updateCombinedDerivativesTable()
{
    if (!m_splineModel) return;

    int N = 400;
    double a = m_splineModel->getIntervalA();
    double b = m_splineModel->getIntervalB();
    double step = (b - a) / N;

    m_combinedDerivativesTable->setRowCount(N + 1);
    m_combinedDerivativesTable->clearContents();

    for (int j = 0; j <= N; ++j)
    {
        double x = a + j * step;

        double Fx = m_splineModel->function(x);
        double Sx = m_splineModel->evaluate(x);
        double Fx_Sx = Fx - Sx;

        double FxDeriv = m_splineModel->functionDerivative(x);
        double SxDeriv = m_splineModel->evaluateDerivative(x);
        double DiffDeriv = FxDeriv - SxDeriv;

        double FxSecondDeriv = m_splineModel->functionSecondDerivative(x);
        double SxSecondDeriv = m_splineModel->evaluateSecondDerivative(x);
        double DiffSecondDeriv = FxSecondDeriv - SxSecondDeriv;

        m_combinedDerivativesTable->setItem(j, 0, new QTableWidgetItem(QString::number(x)));
        m_combinedDerivativesTable->setItem(j, 1, new QTableWidgetItem(QString::number(Fx)));
        m_combinedDerivativesTable->setItem(j, 2, new QTableWidgetItem(QString::number(Sx)));
        m_combinedDerivativesTable->setItem(j, 3, new QTableWidgetItem(QString::number(Fx_Sx)));
        m_combinedDerivativesTable->setItem(j, 4, new QTableWidgetItem(QString::number(FxDeriv)));
        m_combinedDerivativesTable->setItem(j, 5, new QTableWidgetItem(QString::number(SxDeriv)));
        m_combinedDerivativesTable->setItem(j, 6, new QTableWidgetItem(QString::number(DiffDeriv)));
        m_combinedDerivativesTable->setItem(j, 7, new QTableWidgetItem(QString::number(FxSecondDeriv)));
        m_combinedDerivativesTable->setItem(j, 8, new QTableWidgetItem(QString::number(SxSecondDeriv)));
        m_combinedDerivativesTable->setItem(j, 9, new QTableWidgetItem(QString::number(DiffSecondDeriv)));
    }
}

void CubicSplineWidget::createCharts()
{
    m_chart = new QChart();
    m_chart->setTitle("Функция и сплайн");
    m_functionSeries = new QLineSeries();
    m_functionSeries->setName("Оригинальная функция");
    m_splineSeries = new QLineSeries();
    m_splineSeries->setName("Кубический сплайн");
    m_chart->addSeries(m_functionSeries);
    m_chart->addSeries(m_splineSeries);
    setupAxes(m_chart);

    m_chartDeriv = new QChart();
    m_chartDeriv->setTitle("Первые производные");
    m_functionDerivSeries = new QLineSeries();
    m_functionDerivSeries->setName("Производная функции");
    m_splineDerivSeries = new QLineSeries();
    m_splineDerivSeries->setName("Производная сплайна");
    m_chartDeriv->addSeries(m_functionDerivSeries);
    m_chartDeriv->addSeries(m_splineDerivSeries);
    setupAxes(m_chartDeriv);

    m_chartSecondDeriv = new QChart();
    m_chartSecondDeriv->setTitle("Вторые производные");
    m_functionSecondDerivSeries = new QLineSeries();
    m_functionSecondDerivSeries->setName("Вторая производная функции");
    m_splineSecondDerivSeries = new QLineSeries();
    m_splineSecondDerivSeries->setName("Вторая производная сплайна");
    m_chartSecondDeriv->addSeries(m_functionSecondDerivSeries);
    m_chartSecondDeriv->addSeries(m_splineSecondDerivSeries);
    setupAxes(m_chartSecondDeriv);

    m_chartError = new QChart();
    m_chartError->setTitle("Погрешность функции");
    m_errorSeries = new QLineSeries();
    m_errorSeries->setName("F(x) - S(x)");
    m_chartError->addSeries(m_errorSeries);
    setupAxes(m_chartError);

    m_chartErrorDeriv = new QChart();
    m_chartErrorDeriv->setTitle("Погрешность производных");
    m_errorDerivSeries = new QLineSeries();
    m_errorDerivSeries->setName("F'(x) - S'(x)");
    m_chartErrorDeriv->addSeries(m_errorDerivSeries);
    setupAxes(m_chartErrorDeriv);

    m_chartErrorSecondDeriv = new QChart();
    m_chartErrorSecondDeriv->setTitle("Погрешность вторых производных");
    m_errorSecondDerivSeries = new QLineSeries();
    m_errorSecondDerivSeries->setName("F''(x) - S''(x)");
    m_chartErrorSecondDeriv->addSeries(m_errorSecondDerivSeries);
    setupAxes(m_chartErrorSecondDeriv);

    QChartView* chartView = new QChartView(m_chart);
    chartView->setFixedSize(800, 600);
    chartView->setMaximumSize(1200, 900);
    chartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QChartView* chartDerivView = new QChartView(m_chartDeriv);
    chartDerivView->setRenderHint(QPainter::Antialiasing);
    chartDerivView->setMinimumSize(800, 600);
    chartDerivView->setMaximumSize(1200, 900);
    chartDerivView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QChartView* chartSecondDerivView = new QChartView(m_chartSecondDeriv);
    chartSecondDerivView->setRenderHint(QPainter::Antialiasing);
    chartSecondDerivView->setMinimumSize(800, 600);
    chartSecondDerivView->setMaximumSize(1200, 900);
    chartSecondDerivView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QChartView* chartErrorView = new QChartView(m_chartError);
    chartErrorView->setRenderHint(QPainter::Antialiasing);
    chartErrorView->setMinimumSize(800, 600);
    chartErrorView->setMaximumSize(1200, 900);
    chartErrorView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QChartView* chartErrorDerivView = new QChartView(m_chartErrorDeriv);
    chartErrorDerivView->setRenderHint(QPainter::Antialiasing);
    chartErrorDerivView->setMinimumSize(800, 600);
    chartErrorDerivView->setMaximumSize(1200, 900);
    chartErrorDerivView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QChartView* chartErrorSecondDerivView = new QChartView(m_chartErrorSecondDeriv);
    chartErrorSecondDerivView->setRenderHint(QPainter::Antialiasing);
    chartErrorSecondDerivView->setMinimumSize(800, 600);
    chartErrorSecondDerivView->setMaximumSize(1200, 900);
    chartErrorSecondDerivView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_chartsTabWidget->addTab(chartView, "Функция и сплайн");
    m_chartsTabWidget->addTab(chartDerivView, "Производные");
    m_chartsTabWidget->addTab(chartSecondDerivView, "Вторые производные");
    m_chartsTabWidget->addTab(chartErrorView, "Погрешность функции");
    m_chartsTabWidget->addTab(chartErrorDerivView, "Погрешность производных");
    m_chartsTabWidget->addTab(chartErrorSecondDerivView, "Погрешность вторых производных");

    clearCharts();
}

void CubicSplineWidget::setupAxes(QChart* chart)
{
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("X");
    axisY->setTitleText("Y");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for (auto series : chart->series())
    {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}
}
