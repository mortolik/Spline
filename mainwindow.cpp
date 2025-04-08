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
                                                "Тестовая функция: φ(x)");
    m_tabWidget->addTab(testFunctionTab, "Тестовая функция");

    m_splineModelMain = new Spline::CubicSplineModel(this);
    QWidget* mainFunctionTab = setupFunctionTab(m_splineModelMain,
                                                "Основная функция: ln(x+1)/(x+1)");
    m_tabWidget->addTab(mainFunctionTab, "Основная функция");

    m_splineModelOscillating = new Spline::CubicSplineModel(this);
    m_splineModelOscillating->setOscillatingMode(true);
    QWidget* oscillatingFunctionTab = setupFunctionTab(m_splineModelOscillating,
                                                       "Осциллирующая функция: ln(x+1)/(x+1) + cos(10x)");
    m_tabWidget->addTab(oscillatingFunctionTab, "Осциллирующая функция");

    setCentralWidget(m_tabWidget);
}

QWidget *MainWindow::setupFunctionTab(Spline::CubicSplineModel *splineModel,
                                      QString text)
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    Spline::CubicSplineWidget *splineWidget = new Spline::CubicSplineWidget(this);
    splineWidget->setModel(splineModel);
    splineWidget->clearCharts();

    layout->addWidget(new QLabel(text));
    layout->addWidget(splineWidget);

    return tab;
}

