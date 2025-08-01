#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Constante PI
constexpr double PI = 3.141592653589793;

// Valor absoluto
inline double absd(double x) {
    return (x < 0) ? -x : x;
}

// Raíz cuadrada (Newton-Raphson)
inline double sqrtd(double x) {
    if (x <= 0) return 0;
    double guess = x;
    double tolerance = 1e-7;
    while (true) {
        double next = 0.5 * (guess + x / guess);
        if (absd(next - guess) < tolerance) break;
        guess = next;
    }
    return guess;
}

// Aproximación de arctan usando fórmula racional (mejor que Taylor)
inline double atand(double x) {
    // Bhaskara-Brouncker approximation
    return (PI / 4) * x - x * (absd(x) - 1) * (0.2447 + 0.0663 * absd(x));
}

// atan2 sin cmath
inline double atan2d(double y, double x) {
    if (x > 0) return atand(y / x);
    if (x < 0 && y >= 0) return atand(y / x) + PI;
    if (x < 0 && y < 0) return atand(y / x) - PI;
    if (x == 0 && y > 0) return PI / 2;
    if (x == 0 && y < 0) return -PI / 2;
    return 0.0;
}

// Potencia entera positiva
inline double powd(double base, int exp) {
    if (exp == 0) return 1;
    double res = 1;
    for (int i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

#endif
