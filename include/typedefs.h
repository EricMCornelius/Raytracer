#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <armadillo>

typedef arma::Mat<float> Matrix;
typedef arma::Row<float> Vector;

Vector ZVec3(arma::zeros<Vector>(3));
Matrix ZMat4(arma::zeros<Matrix>(4,4));
Matrix IMat4(arma::eye<Matrix>(4,4));

#endif
