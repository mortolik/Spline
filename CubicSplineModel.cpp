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
        h[i] = m_x[i+1] - m_x[i];
    }

    if (m_testMode)
    {
        m_c[0] = 0;
        m_c[n] = 0;
    }

    QVector<double> alpha(n), beta(n);
    alpha[0] = 0;
    beta[0] = 0;

    for (int i = 1; i < n; ++i)
    {
        double A = h[i-1];
        double B = 2*(h[i-1] + h[i]);
        double C = h[i];
        double F = 3*((m_a[i+1] - m_a[i])/h[i] - (m_a[i] - m_a[i-1])/h[i-1]);

        alpha[i] = -C / (A*alpha[i-1] + B);
        beta[i] = (F - A*beta[i-1]) / (A*alpha[i-1] + B);
    }

    for (int i = n-1; i > 0; --i)
    {
        m_c[i] = alpha[i]*m_c[i+1] + beta[i];
    }

    for (int i = 0; i < n; ++i)
    {
        m_b[i] = (m_a[i+1] - m_a[i])/h[i] - h[i]*(m_c[i+1] + 2*m_c[i])/3.0;
        m_d[i] = (m_c[i+1] - m_c[i])/(3.0*h[i]);
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
    if (m_testMode)
    {
        if (x >= -1 && x <= 0)
            return x*x*x + 3*x*x;
        else if (x > 0 && x <= 1)
            return -x*x*x + 3*x*x;
        else
            return 0;
    }
    else if (m_oscillatingMode)
    {
        return qLn(x + 1) / (x + 1) + qCos(10 * x);
    }
    else
    {
        return qLn(x + 1) / (x + 1);
    }
}

double CubicSplineModel::functionDerivative(double x) const
{
    if (m_testMode)
    {
        if (x >= -1 && x <= 0)
            return 3*x*x + 6*x;
        else if (x > 0 && x <= 1)
            return -3*x*x + 6*x;
        else
            return 0;
    }
    else if (m_oscillatingMode)
    {
        return (1 - qLn(x + 1)) / ((x + 1) * (x + 1)) - 10 * qSin(10 * x);
    }
    else
    {
        return (1 - qLn(x + 1)) / ((x + 1)*(x + 1));
    }
}

double CubicSplineModel::functionSecondDerivative(double x) const
{
    if (m_testMode)
    {
        if (x >= -1 && x <= 0)
        {
            return 6*x + 6;
        }
        else if (x > 0 && x <= 1)
        {
            return -6*x + 6;
        }
        else
        {
            return 0;
        }
    }
    else if (m_oscillatingMode)
    {
        return (2 * qLn(x + 1) - 3) / qPow(x + 1, 3) - 100 * qCos(10 * x);
    }
    else
    {
        return (2*qLn(x + 1) - 3) / ((x + 1)*(x + 1)*(x + 1));
    }
}

double CubicSplineModel::evaluateDerivative(double xVal) const
{
    if (m_x.isEmpty() || m_y.isEmpty())
    {
        return 0.0;
    }

    int i = 0;
    while (i < m_x.size() - 1 && xVal > m_x[i + 1])
    {
        ++i;
    }

    double dx = xVal - m_x[i];
    return m_b[i] + m_c[i] * dx + m_d[i] * dx * dx;
}

double CubicSplineModel::evaluateSecondDerivative(double xVal) const
{
    if (m_x.isEmpty() || m_y.isEmpty())
    {
        return 0.0;
    }

    int i = 0;
    while (i < m_x.size() - 1 && xVal > m_x[i + 1])
    {
        ++i;
    }

    double dx = xVal - m_x[i];
    return m_c[i] + 2.0 * m_d[i] * dx;
}

void CubicSplineModel::setTestMode(bool testMode)
{
    m_testMode = testMode;
    if (m_testMode)
    {
        setInterval(-1, 1);
    }
    else
    {
        setInterval(0.2, 2.0);
    }
}
void CubicSplineModel::setOscillatingMode(bool oscillatingMode)
{
    m_oscillatingMode = oscillatingMode;
    emit splineUpdated();
}

bool CubicSplineModel::isOscillatingMode() const
{
    return m_oscillatingMode;
}

QVector<CubicSplineModel::ErrorData> CubicSplineModel::getErrorTableData(int N) const
{
    QVector<ErrorData> data;
    if (m_x.isEmpty())
    {
        return data;
    }
    double a = m_intervalA;
    double b = m_intervalB;
    double step = (b - a) / N;

    for (int j = 0; j <= N; ++j)
    {
        double x = a + j * step;
        ErrorData entry;
        entry.x = x;
        entry.F = function(x);
        entry.S = evaluate(x);
        entry.error = entry.F - entry.S;
        entry.F_deriv = functionDerivative(x);
        entry.S_deriv = evaluateDerivative(x);
        entry.deriv_error = entry.F_deriv - entry.S_deriv;
        entry.F_second_deriv = functionSecondDerivative(x);
        entry.S_second_deriv = evaluateSecondDerivative(x);
        entry.second_deriv_error = entry.F_second_deriv - entry.S_second_deriv;

        data.append(entry);
    }
    return data;
}

double CubicSplineModel::getMaxError(int N) const {
    double maxError = 0.0;
    auto data = getErrorTableData(N);
    for (const auto& entry : data) {
        maxError = qMax(maxError, qAbs(entry.error));
    }
    return maxError;
}

double CubicSplineModel::getMaxDerivativeError(int N) const {
    double maxError = 0.0;
    auto data = getErrorTableData(N);
    for (const auto& entry : data) {
        maxError = qMax(maxError, qAbs(entry.deriv_error));
    }
    return maxError;
}

double CubicSplineModel::getMaxSecondDerivativeError(int N) const {
    double maxError = 0.0;
    auto data = getErrorTableData(N);
    for (const auto& entry : data) {
        maxError = qMax(maxError, qAbs(entry.second_deriv_error));
    }
    return maxError;
}
}
