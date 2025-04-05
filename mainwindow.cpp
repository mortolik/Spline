#include "mainwindow.h"
#include "CubicSplineModel.hpp"
#include "CubicSplineWidget.hpp"

#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    setupUI();
    setMinimumSize(800, 800);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_splineModel = new Spline::CubicSplineModel(this);
    m_splineWidget = new Spline::CubicSplineWidget(this);
    m_splineWidget->setModel(m_splineModel);
    m_splineWidget->clearChart();

    m_nSpinBox = new QSpinBox(this);
    m_nSpinBox->setRange(2, 100);
    m_nSpinBox->setValue(4);

    m_updateButton = new QPushButton("Построить сплайн", this);
    m_testButton = new QPushButton("Переключить функцию", this);

    m_coeffTable = new QTableWidget(this);
    m_coeffTable->setColumnCount(7);
    m_coeffTable->setHorizontalHeaderLabels({"i", "xi-1", "xi", "ai", "bi", "ci", "di"});

    layout->addWidget(m_nSpinBox);
    layout->addWidget(m_updateButton);
    layout->addWidget(m_testButton);
    layout->addWidget(m_splineWidget);
    layout->addWidget(m_coeffTable);

    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateSpline);
    connect(m_testButton, &QPushButton::clicked, this, &MainWindow::toggleTestMode);

    setCentralWidget(centralWidget);
}

void MainWindow::updateSpline()
{
    int n = m_nSpinBox->value();
    m_splineModel->setPoints(n);
    updateTable();
}

void MainWindow::toggleTestMode()
{
    static bool testMode = false;
    testMode = !testMode;
    m_splineModel->setTestMode(testMode);
    m_splineModel->setPoints(m_nSpinBox->value());
    updateTable();

    if (testMode)
    {
        m_testButton->setText("Основная функция");
    }
    else
    {
        m_testButton->setText("Тестовая функция");
    }
}

void MainWindow::updateTable()
{
    QVector<double> xValues = m_splineModel->getX();

    QVector<double> a = m_splineModel->getCoefficientsA();
    QVector<double> b = m_splineModel->getCoefficientsB();
    QVector<double> c = m_splineModel->getCoefficientsC();
    QVector<double> d = m_splineModel->getCoefficientsD();


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
