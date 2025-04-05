#include "mainwindow.h"
#include "CubicSplineModel.hpp"
#include "CubicSplineWidget.hpp"

#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();
    setMinimumSize(1000, 800);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_tabWidget = new QTabWidget(this);

    m_splineModelTest = new Spline::CubicSplineModel(this);
    m_splineModelTest->setTestMode(true);

    QWidget* testFunctionTab = setupFunctionTab(m_splineModelTest,
                                                m_splineWidgetTest,
                                                "Тестовая функция: φ(x)");

    m_tabWidget->addTab(testFunctionTab, "Тестовая функция");

    m_splineModelMain = new Spline::CubicSplineModel(this);
    QWidget* mainFunctionTab = setupFunctionTab(m_splineModelMain,
                                                m_splineWidgetMain,
                                                "Основная функция: ln(x+1)/(x+1)");
    m_tabWidget->addTab(mainFunctionTab, "Основная функция");

    setCentralWidget(m_tabWidget);
}

QWidget *MainWindow::setupFunctionTab(Spline::CubicSplineModel *splineModel,
                                      Spline::CubicSplineWidget *splineWidget,
                                      QString text)
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    splineWidget = new Spline::CubicSplineWidget(this);
    splineWidget->setModel(splineModel);
    splineWidget->clearChart();

    layout->addWidget(new QLabel(text));
    layout->addWidget(splineWidget);

    return tab;
}
