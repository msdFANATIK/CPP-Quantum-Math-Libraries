#pragma once

#include "types.hpp"
#include "gates.hpp"

namespace qm {

inline Vector tensor_product(const Vector& a, const Vector& b) {
    Vector result(a.size * b.size);

    for (unsigned int i = 0; i < a.size; ++i) {
        for (unsigned int j = 0; j < b.size; ++j) {
            result[i * b.size + j] = a[i] * b[j];
    }
    }
    return result;
}

inline Vector qubit_to_vector(const Qubit& q) {
    Vector v(2);
    v[0] = q.alpha;
    v[1] = q.beta;
    return v;
}

struct StateVector4 {
    Complex v[4];

    StateVector4() {
        v[0] = {1.0, 0.0};
        v[1] = {0.0, 0.0};
        v[2] = {0.0, 0.0};
        v[3] = {0.0, 0.0};
    }
};

struct Matrix4x4 {
    Complex m[4][4];

    StateVector4 apply(const StateVector4& state) const {
        StateVector4 res;
        for (int i = 0; i < 4; ++i) {
            Complex sum = {0.0, 0.0};
            for (int j = 0; j < 4; ++j) {
                sum = sum + (m[i][j] * state.v[j]);
            }
            res.v[i] = sum;
        }
        return res;
    }
};

inline StateVector4 tensor_product(const Qubit& q1, const Qubit& q2) {
    StateVector4 res;
    res.v[0] = q1.alpha * q2.alpha;
    res.v[1] = q1.alpha * q2.beta;
    res.v[2] = q1.beta * q2.alpha;
    res.v[3] = q1.beta * q2.beta;
    return res;
}

inline Matrix4x4 tensor_product(const Matrix2x2& A, const Matrix2x2& B) {
    Matrix4x4 res;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            
            for (int k = 0; k < 2; ++k) {
                for (int l = 0; l < 2; ++l) {
                    res.m[i * 2 + k][j * 2 + l] = A.m[i][j] * B.m[k][l];
                }
            }

        }
    }
    return res;
}
}
