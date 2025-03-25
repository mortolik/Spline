#include <QDebug>
#include <QtMath>

#include "CubicSplineModel.hpp"
namespace Spline
{
CubicSplineModel::CubicSplineModel(QObject *parent)
    : QObject(parent)
{
}

void CubicSplineModel::setPoints(int n)
{
    if (n < 2) {
        qWarning() << "Ошибка: n должно быть >= 2!";
        return;
    }

    double a = 0.2, b = 2.0;
    x.clear();
    y.clear();
    x.resize(n + 1);
    y.resize(n + 1);

    double h = (b - a) / n;
    for (int i = 0; i <= n; ++i)
    {
        x[i] = a + i * h;
        y[i] = function(x[i]);
    }
    computeSpline();
}

void CubicSplineModel::computeSpline()
{
    int n = x.size() - 1;
    if (n < 2) return;

    a = y;
    b.resize(n);
    c.resize(n + 1);
    d.resize(n);

    QVector<double> h(n);
    for (int i = 0; i < n; ++i) {
        h[i] = x[i + 1] - x[i];
    }

    QVector<double> A(n), B(n), C(n), F(n);
    for (int i = 1; i < n; ++i)
    {
        A[i] = h[i - 1];
        B[i] = 2.0 * (h[i - 1] + h[i]);
        C[i] = h[i];
        F[i] = 3.0 * ((a[i + 1] - a[i]) / h[i] - (a[i] - a[i - 1]) / h[i - 1]);
    }

    B[0] = 1; B[n - 1] = 1;
    C[0] = 0; A[n - 1] = 0;
    F[0] = 0; F[n - 1] = 0;

    for (int i = 1; i < n - 1; ++i)
    {
        double m = A[i] / B[i - 1];
        B[i] -= m * C[i - 1];
        F[i] -= m * F[i - 1];
    }

    c[n - 1] = F[n - 1] / B[n - 1];
    for (int i = n - 2; i > 0; --i)
    {
        c[i] = (F[i] - C[i] * c[i + 1]) / B[i];
    }
    c[0] = 0;
    c[n] = 0;

    for (int i = 0; i < n; ++i)
    {
        b[i] = (a[i + 1] - a[i]) / h[i] - h[i] * (c[i + 1] + 2.0 * c[i]) / 3.0;
        d[i] = (c[i + 1] - c[i]) / (3.0 * h[i]);
    }

    emit splineUpdated();
}

double CubicSplineModel::evaluate(double xVal) const
{
    if (x.isEmpty() || y.isEmpty()) return 0.0;

    int i = 0;
    while (i < x.size() - 1 && xVal > x[i + 1])
    {
        ++i;
    }

    double dx = xVal - x[i];
    return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
}

QVector<double> CubicSplineModel::getCoefficientsA() const
{
    return a;
}

QVector<double> CubicSplineModel::getCoefficientsB() const
{
    return b;
}

QVector<double> CubicSplineModel::getCoefficientsC() const
{
    return c;
}

QVector<double> CubicSplineModel::getCoefficientsD() const
{
    return d;
}

double CubicSplineModel::function(double x) const
{
    return qLn(x + 1) / (x + 1);
}
}
