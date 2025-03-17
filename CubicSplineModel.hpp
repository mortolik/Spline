#pragma once

#include <QObject>

class CubicSplineModel : public QObject
{
    Q_OBJECT

public:
    explicit CubicSplineModel(QObject *parent = nullptr);

    void setPoints(int n);
    void computeSpline();
    double evaluate(double x) const;

    QVector<double> getCoefficientsA() const;
    QVector<double> getCoefficientsB() const;
    QVector<double> getCoefficientsC() const;
    QVector<double> getCoefficientsD() const;

signals:
    void splineUpdated();

private:
    QVector<double> x, y;
    QVector<double> a, b, c, d;

    double function(double x) const;
};
