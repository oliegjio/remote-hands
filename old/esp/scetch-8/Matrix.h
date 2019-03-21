#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct Matrix {
    size_t r, c;
    float d[9];
};

void matrix_generate(Matrix &r, const Matrix a, const Matrix b) {
    r.r = a.r;
    r.c = b.c;
    for (size_t i = 0; i < r.r * r.c; ++i)
        r.d[i] = 0;
}

Matrix matrix_sum(const Matrix a, const Matrix b) {
    Matrix r;
    r.r = a.r;
    r.r = b.r;

    for (size_t i = 0; i < a.r; ++i) {
        for (size_t j = 0; j < a.c; ++j) {
            r.d[j + i * a.r] = a[j + i * a.r] + b[j + i * b.r];
        }
    }

    return r;
}

Matrix matrix_multiply(const Matrix a, const Matrix b) {
    Matrix r;
    matrix_generate(r, a, b);

    for (size_t i = 0; i < r.r; ++i) {
        for (size_t j = 0; j < r.c; ++j) {
            for (size_t k = 0; k < a.c; ++k) {
                r.d[j + i * r.c] += a.d[k + i * a.c] * b.d[j + k * b.c];
            }
        }
    }

    return r;
}

#endif /* _MATRIX_H_ */