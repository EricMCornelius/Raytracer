#ifndef QUATERNION_H
#define QUATERNION_H

template <typename T>
class GenericQuaternion {
public:
  GenericQuaternion(const Vector& axis, const T& theta);
  Matrix& matrix();

private:
  void calcMatrix();
  Vector d_axis;
  T d_theta;
  Matrix d_matrix;
};

template <typename T>
GenericQuaternion<T>::GenericQuaternion(const Vector& axis, const T& theta) : d_axis(axis), d_theta(theta), d_matrix(ZMat4) {
  calcMatrix();
}

template <typename T>
Matrix& GenericQuaternion<T>::matrix() {
  return d_matrix;
}

template <typename T>
void GenericQuaternion<T>::calcMatrix() {
  T cosine = cos(d_theta);
  T oneMinusCosine = 1 - cosine;
  T sine = sin(d_theta);

  d_matrix(0,0) = d_axis[0]*d_axis[0]*oneMinusCosine + cosine;
  d_matrix(0,1) = d_axis[0]*d_axis[1]*oneMinusCosine - d_axis[2]*sine;
  d_matrix(0,2) = d_axis[0]*d_axis[2]*oneMinusCosine + d_axis[1]*sine;
  d_matrix(0,3) = 0;
  d_matrix(1,0) = d_axis[1]*d_axis[0]*oneMinusCosine + d_axis[2]*sine;
  d_matrix(1,1) = d_axis[1]*d_axis[1]*oneMinusCosine + cosine;
  d_matrix(1,2) = d_axis[1]*d_axis[2]*oneMinusCosine - d_axis[0]*sine;
  d_matrix(1,3) = 0;
  d_matrix(2,0) = d_axis[2]*d_axis[0]*oneMinusCosine - d_axis[1]*sine;
  d_matrix(2,1) = d_axis[2]*d_axis[1]*oneMinusCosine + d_axis[0]*sine;
  d_matrix(2,2) = d_axis[2]*d_axis[2]*oneMinusCosine + cosine;
  d_matrix(2,3) = 0;
  d_matrix(3,0) = 0;
  d_matrix(3,1) = 0;
  d_matrix(3,2) = 0;
  d_matrix(3,3) = 1;
}

typedef GenericQuaternion<float> Quaternion;

#endif
