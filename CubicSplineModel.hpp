#pragma once

#include <QObject>

namespace Spline
{

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
    QVector<double> getX() const;

    double function(double x) const;

signals:
    void splineUpdated();

private:
    QVector<double> x, y;
    QVector<double> a, b, c, d;

};
}
