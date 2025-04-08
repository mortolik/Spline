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

    double evaluateDerivative(double xVal) const;
    double evaluateSecondDerivative(double xVal) const;

    void setInterval(double a, double b);
    double getIntervalA() const;
    double getIntervalB() const;

    void setTestMode(bool testMode);

    void setOscillatingMode(bool oscillatingMode);
    bool isOscillatingMode() const;

    struct ErrorData
    {
        double x;
        double F;
        double S;
        double error;
        double F_deriv;
        double S_deriv;
        double deriv_error;
        double F_second_deriv;
        double S_second_deriv;
        double second_deriv_error;
    };

    QVector<ErrorData> getErrorTableData(int N = 100) const;

signals:
    void splineUpdated();

private:
    QVector<double> m_x, m_y;
    QVector<double> m_a, m_b, m_c, m_d;

    double m_intervalA = 0.2;
    double m_intervalB = 2.0;

    bool m_testMode = false;
    bool m_oscillatingMode = false;

    double getMaxError(int N = 1000) const;
    double getMaxDerivativeError(int N = 1000) const;
    double getMaxSecondDerivativeError(int N = 1000) const;

};
}
