#pragma once

#include "types.hpp"

namespace qm {

inline double sqrt(double n) {
    if (n <= 0.0) return 0.0;
    double x = n;
    for (int i = 0; i < 10; ++i) {
        x = 0.5 * (x + n / x);
    }
    return x;
}

struct Matrix2x2 {
    Complex m[2][2];

    Qubit apply(const Qubit& q) const {
        Complex new_alpha = m[0][0] * q.alpha + m[0][1] * q.beta;
        Complex new_beta = m[1][0] * q.alpha + m[1][1] * q.beta;
        return Qubit(new_alpha, new_beta);
    }
};

namespace gates {

inline Matrix2x2 X() {
    return Matrix2x2{{{ {0.0, 0.0}, {1.0, 0.0} },
                    { {1.0, 0.0}, {0.0, 0.0} }}};
}

inline Matrix2x2 Y() {
    return Matrix2x2{{{ {0.0, 0.0}, {0.0, -1.0} },
                    { {0.0, 1.0}, {0.0, 0.0} }}};
}

inline Matrix2x2 Z() {
    return Matrix2x2{{{ {1.0, 0.0}, {0.0, 0.0} },
                    { {0.0, 0.0}, {-1.0, 0.0} }}};
}

inline Matrix2x2 H() {
    double inv_sqrt2 = 1.0 / sqrt(2.0);
    return Matrix2x2{{{ {inv_sqrt2, 0.0}, {inv_sqrt2, 0.0} },
                    { {inv_sqrt2, 0.0}, {-inv_sqrt2, 0.0} }}};
}
} // namespace gates
}
