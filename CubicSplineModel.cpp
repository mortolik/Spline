#include <QDebug>
#include <QtMath>

#include "CubicSplineModel.hpp"
namespace Spline
{
CubicSplineModel::CubicSplineModel(QObject *parent)
    : QObject(parent)
{
}

void CubicSplineModel::setInterval(double a, double b)
{
    if (a >= b)
    {
        qWarning() << "Ошибка: a должно быть меньше b!";
        return;
    }
    m_intervalA = a;
    m_intervalB = b;
}

double CubicSplineModel::getIntervalA() const
{
    return m_intervalA;
}

double CubicSplineModel::getIntervalB() const
{
    return m_intervalB;
}

void CubicSplineModel::setPoints(int n)
{
    if (n < 2)
    {
        qWarning() << "Ошибка: n должно быть >= 2!";
        return;
    }

    double a = m_intervalA;
    double b = m_intervalB;
    m_x.clear();
    m_y.clear();
    m_x.resize(n + 1);
    m_y.resize(n + 1);

    double h = (b - a) / n;
    for (int i = 0; i <= n; ++i)
    {
        m_x[i] = a + i * h;
        m_y[i] = function(m_x[i]);
    }
    computeSpline();
}

void CubicSplineModel::computeSpline()
{
    int n = m_x.size() - 1;
    if (n < 2) return;

    m_a = m_y;
    m_b.resize(n);
    m_c.resize(n + 1);
    m_d.resize(n);

    QVector<double> h(n);
    for (int i = 0; i < n; ++i)
    {
        h[i] = m_x[i + 1] - m_x[i];
    }

    QVector<double> A(n), B(n), C(n), F(n);
    for (int i = 1; i < n; ++i)
    {
        A[i] = h[i - 1];
        B[i] = 2.0 * (h[i - 1] + h[i]);
        C[i] = h[i];
        F[i] = 3.0 * ((m_a[i + 1] - m_a[i]) / h[i] - (m_a[i] - m_a[i - 1]) / h[i - 1]);
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

    m_c[n - 1] = F[n - 1] / B[n - 1];
    for (int i = n - 2; i > 0; --i)
    {
        m_c[i] = (F[i] - C[i] * m_c[i + 1]) / B[i];
    }
    m_c[0] = 0;
    m_c[n] = 0;

    for (int i = 0; i < n; ++i)
    {
        m_b[i] = (m_a[i + 1] - m_a[i]) / h[i] - h[i] * (m_c[i + 1] + 2.0 * m_c[i]) / 3.0;
        m_d[i] = (m_c[i + 1] - m_c[i]) / (3.0 * h[i]);
    }

    emit splineUpdated();
}

double CubicSplineModel::evaluate(double xVal) const
{
    if (m_x.isEmpty() || m_y.isEmpty()) return 0.0;

    int i = 0;
    while (i < m_x.size() - 1 && xVal > m_x[i + 1])
    {
        ++i;
    }

    double dx = xVal - m_x[i];
    return m_a[i] + m_b[i] * dx + m_c[i] * dx * dx + m_d[i] * dx * dx * dx;
}

QVector<double> CubicSplineModel::getCoefficientsA() const
{
    return m_a;
}

QVector<double> CubicSplineModel::getCoefficientsB() const
{
    return m_b;
}

QVector<double> CubicSplineModel::getCoefficientsC() const
{
    return m_c;
}

QVector<double> CubicSplineModel::getCoefficientsD() const
{
    return m_d;
}

QVector<double> CubicSplineModel::getX() const
{
    return m_x;
}

double CubicSplineModel::function(double x) const
{
    return qLn(x + 1) / (x + 1);
}
}
