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

    void setInterval(double a, double b);
    double getIntervalA() const;
    double getIntervalB() const;

signals:
    void splineUpdated();

private:
    QVector<double> x, y;
    QVector<double> a, b, c, d;

    double intervalA = 0.2;
    double intervalB = 2.0;

};
}
