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
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    splineModel = new Spline::CubicSplineModel(this);
    splineWidget = new Spline::CubicSplineWidget(this);
    splineWidget->setModel(splineModel);

    nSpinBox = new QSpinBox(this);
    nSpinBox->setRange(2, 100);
    nSpinBox->setValue(10);

    updateButton = new QPushButton("Построить сплайн", this);

    coeffTable = new QTableWidget(this);
    coeffTable->setColumnCount(5);
    coeffTable->setHorizontalHeaderLabels({"i", "xi-1", "xi", "ai", "bi", "ci", "di"});

    layout->addWidget(nSpinBox);
    layout->addWidget(updateButton);
    layout->addWidget(splineWidget);
    layout->addWidget(coeffTable);

    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateSpline);

    setCentralWidget(centralWidget);
}

void MainWindow::updateSpline()
{
    int n = nSpinBox->value();
    splineModel->setPoints(n);
    updateTable();
}

void MainWindow::updateTable()
{
    QVector<double> a = splineModel->getCoefficientsA();
    QVector<double> b = splineModel->getCoefficientsB();
    QVector<double> c = splineModel->getCoefficientsC();
    QVector<double> d = splineModel->getCoefficientsD();

    int n = a.size() - 1;
    coeffTable->setRowCount(n);

    for (int i = 0; i < n; ++i)
    {
        coeffTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        coeffTable->setItem(i, 1, new QTableWidgetItem(QString::number(a[i])));
        coeffTable->setItem(i, 2, new QTableWidgetItem(QString::number(b[i])));
        coeffTable->setItem(i, 3, new QTableWidgetItem(QString::number(c[i])));
        coeffTable->setItem(i, 4, new QTableWidgetItem(QString::number(d[i])));
    }
}
