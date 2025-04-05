#include "mainwindow.h"
#include "CubicSplineModel.hpp"
#include "CubicSplineWidget.hpp"

#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QLabel>

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

    QWidget* testFunctionTab = setupTestFunctionTab();
    m_tabWidget->addTab(testFunctionTab, "Тестовая функция");

    QWidget* mainFunctionTab = setupMainFunctionTab();
    m_tabWidget->addTab(mainFunctionTab, "Основная функция");

    setCentralWidget(m_tabWidget);
}

QWidget* MainWindow::setupTestFunctionTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_splineModelTest = new Spline::CubicSplineModel(this);
    m_splineWidgetTest = new Spline::CubicSplineWidget(this);
    m_splineWidgetTest->setModel(m_splineModelTest);
    m_splineWidgetTest->clearChart();

    m_nSpinBox = new QSpinBox(this);
    m_nSpinBox->setRange(2, 100);
    m_nSpinBox->setValue(4);

    m_updateButton = new QPushButton("Построить сплайн", this);

    m_coeffTable = new QTableWidget(this);
    m_coeffTable->setColumnCount(7);
    m_coeffTable->setHorizontalHeaderLabels({"i", "xi-1", "xi", "ai", "bi", "ci", "di"});

    layout->addWidget(new QLabel("Тестовая функция: φ(x)"));
    layout->addWidget(m_nSpinBox);
    layout->addWidget(m_updateButton);
    layout->addWidget(m_splineWidgetTest);
    layout->addWidget(m_coeffTable);

    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateTestSpline);
    return tab;
}

QWidget* MainWindow::setupMainFunctionTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);

    m_splineModelMain = new Spline::CubicSplineModel(this);
    m_splineWidgetMain = new Spline::CubicSplineWidget(this);
    m_splineWidgetMain->setModel(m_splineModelMain);
    m_splineWidgetMain->clearChart();

    m_nSpinBox = new QSpinBox(this);
    m_nSpinBox->setRange(2, 100);
    m_nSpinBox->setValue(4);

    m_updateButton = new QPushButton("Построить сплайн", this);

    m_coeffTable = new QTableWidget(this);
    m_coeffTable->setColumnCount(7);
    m_coeffTable->setHorizontalHeaderLabels({"i", "xi-1", "xi", "ai", "bi", "ci", "di"});

    layout->addWidget(new QLabel("Основная функция: ln(x+1)/(x+1)"));
    layout->addWidget(m_nSpinBox);
    layout->addWidget(m_updateButton);
    layout->addWidget(m_splineWidgetMain);
    layout->addWidget(m_coeffTable);

    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateMainSpline);
    return tab;
}

void MainWindow::updateMainSpline()
{
    int n = m_nSpinBox->value();
    m_splineModelMain->setPoints(n);
    updateTable(m_splineModelMain);
}

void MainWindow::updateTestSpline()
{
    static bool testMode = false;
    testMode = !testMode;
    m_splineModelTest->setTestMode(testMode);
    m_splineModelTest->setPoints(m_nSpinBox->value());
    updateTable(m_splineModelTest);
}

void MainWindow::updateTable(Spline::CubicSplineModel *splineModel)
{
    QVector<double> xValues = splineModel->getX();

    QVector<double> a = splineModel->getCoefficientsA();
    QVector<double> b = splineModel->getCoefficientsB();
    QVector<double> c = splineModel->getCoefficientsC();
    QVector<double> d = splineModel->getCoefficientsD();

    int n = a.size() - 1;
    m_coeffTable->setRowCount(n);

    for (int i = 0; i < n; ++i)
    {
        m_coeffTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        m_coeffTable->setItem(i, 1, new QTableWidgetItem(QString::number(xValues[i])));
        m_coeffTable->setItem(i, 2, new QTableWidgetItem(QString::number(xValues[i + 1])));
        m_coeffTable->setItem(i, 3, new QTableWidgetItem(QString::number(a[i])));
        m_coeffTable->setItem(i, 4, new QTableWidgetItem(QString::number(b[i])));
        m_coeffTable->setItem(i, 5, new QTableWidgetItem(QString::number(c[i])));
        m_coeffTable->setItem(i, 6, new QTableWidgetItem(QString::number(d[i])));
    }
}
