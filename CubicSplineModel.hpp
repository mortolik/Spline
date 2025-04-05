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
    double functionDerivative(double x) const;
    double functionSecondDerivative(double x) const;

    void setInterval(double a, double b);
    double getIntervalA() const;
    double getIntervalB() const;

    void setTestMode(bool testMode);

signals:
    void splineUpdated();

private:
    QVector<double> m_x, m_y;
    QVector<double> m_a, m_b, m_c, m_d;

    double m_intervalA = 0.2;
    double m_intervalB = 2.0;

    bool m_testMode = false;

};
}
