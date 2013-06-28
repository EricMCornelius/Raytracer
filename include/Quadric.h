#ifndef QUADRIC_H
#define QUADRIC_H

#include <set>
#include "Ray.h"
#include "Quaternion.h"
#include "typedefs.h"

template <typename T>
class GenericQuadric {
public:
  enum SHAPE_TYPE {
    SPHERE,
    CONE,
    PLANE,
    CYLINDER,
    PARABOLOID
  };

  SHAPE_TYPE shape;

  GenericQuadric(const SHAPE_TYPE& type);
  GenericQuadric(const Matrix& matrix);
  GenericQuadric(const GenericQuadric<T>& copyQuad);
  bool contains(const Vector& point);
  void print(std::ostream& out = std::cout) const;
  void setPosition(const Vector& position);
  void setOrientation(const Vector& orientation);
  void setScale(const Vector& scale);
  void intersection(const GenericRay<T>& ray, std::set<T>& dists);
  Vector normal(const Vector& point);

private:
  void initialize();
  void calculateMatrix();
  Matrix oMat;
  Matrix qMat;
  Vector position;
  Vector orientation;
  Vector scale;
  Matrix transMat;
  Matrix scaleMat;
  Matrix rotMat;
};

template <typename T>
GenericQuadric<T>::GenericQuadric(const SHAPE_TYPE& type)
    : transMat(IMat4), scaleMat(IMat4), rotMat(IMat4), oMat(ZMat4), qMat(ZMat4), position(ZVec3), orientation(ZVec3), scale(ZVec3)
{

  initialize();

  switch (type) {
    case SPHERE:
      shape = SPHERE;
      oMat(0, 0) = 1;
      oMat(1, 1) = 1;
      oMat(2, 2) = 1;
      oMat(3, 3) = -1;
      qMat = Matrix(oMat);
      break;
    case CYLINDER:
      shape = CYLINDER;
      oMat(0, 0) = 1;
      oMat(2, 2) = 1;
      oMat(3, 3) = -1;
      qMat = Matrix(oMat);
      break;
    case PLANE:
      shape = PLANE;
      oMat(1, 3) = 1;
      oMat(3, 1) = 1;
      qMat = Matrix(oMat);
      break;
    case CONE:
      shape = CONE;
      oMat(0, 0) = 1;
      oMat(1, 1) = -1;
      oMat(2, 2) = 1;
      oMat(3, 3) = -1;
      qMat = Matrix(oMat);
      break;
    case PARABOLOID:
      shape = PARABOLOID;
      oMat(0, 0) = 1;
      oMat(2, 2) = 1;
      oMat(1, 3) = -2;
      oMat(3, 1) = -2;
      qMat = Matrix(oMat);
      break;
  }
}

template <typename T>
GenericQuadric<T>::GenericQuadric(const Matrix& matrix)
    : transMat(IMat4), scaleMat(IMat4), rotMat(IMat4), oMat(matrix), qMat(matrix), position(ZVec3), orientation(ZVec3), scale(ZVec3)
{

  initialize();
  return;
}

template <typename T>
GenericQuadric<T>::GenericQuadric(const GenericQuadric<T>& copyQuad)
    : transMat(copyQuad.transMat),
      scaleMat(copyQuad.scaleMat),
      rotMat(copyQuad.rotMat),
      oMat(copyQuad.oMat),
      qMat(copyQuad.qMat),
      position(copyQuad.position),
      orientation(copyQuad.orientation),
      scale(copyQuad.scale)
{

  initialize();
  return;
}

template <typename T>
void GenericQuadric<T>::initialize()
{
  scale[0] = 1;
  scale[1] = 1;
  scale[2] = 1;
  orientation[0] = 0;
  orientation[1] = 1;
  orientation[2] = 0;
  position[0] = 0;
  position[1] = 0;
  position[2] = 0;
}

template <typename T>
void GenericQuadric<T>::intersection(const GenericRay<T>& ray, std::set<T>& dists)
{
  static Matrix positionR;
  positionR = ray.position;
  positionR.reshape(1, 4);
  positionR(0, 3) = 1;

  static Matrix directionR;
  directionR = ray.direction;
  directionR.reshape(1, 4);
  directionR(0, 3) = 0;

  static Vector params(3);
  static Vector paramVec1;

  paramVec1 = (directionR * qMat * arma::trans(directionR));
  params[0] = paramVec1[0];
  paramVec1 = 2 * (positionR * qMat * arma::trans(directionR));
  params[1] = paramVec1[0];
  // Vector paramVec2_1 = 2 * (positionR * qMat * directionC);
  // Vector paramVec2_2 = (directionR * qMat * positionC);
  // params[1] = paramVec2_1[0] + paramVec2_2[0];
  paramVec1 = (positionR * qMat * arma::trans(positionR));
  params[2] = paramVec1[0];

  static T discriminant;
  discriminant = (params[1] * params[1] - 4 * params[0] * params[2]);
  if (discriminant < 0)
    return;
  else if (params[1] != 0) {
    discriminant = sqrt(discriminant);
    T pVal = (-params[1] + discriminant) / (2 * params[0]);
    T mVal = (-params[1] - discriminant) / (2 * params[0]);

    if (pVal > 0) dists.insert(pVal);
    if (mVal > 0) dists.insert(mVal);
  } else {
    T lVal = -params[2] / params[1];

    if (lVal > 0) dists.insert(lVal);
  }
}

template <typename T>
Vector GenericQuadric<T>::normal(const Vector& point)
{
  using namespace arma;
  Matrix mat = point.t() * qMat;
  Vector vec = mat.row(0);
  vec /= norm(vec, 2);
  return vec;
}

template <typename T>
void GenericQuadric<T>::setPosition(const Vector& position)
{
  T translateVals[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position[0], position[1], position[2], 1};
  transMat = arma::inv(Matrix(translateVals, 4, 4));
  calculateMatrix();
}

template <typename T>
void GenericQuadric<T>::setOrientation(const Vector& orientation)
{
  Vector standardOri(3);
  standardOri[0] = 0;
  standardOri[1] = 1;
  standardOri[2] = 0;
  Vector normalizedOrientation(orientation / arma::norm(orientation, 2));

  Vector rotationAxis = arma::cross(standardOri, normalizedOrientation);
  T theta = acos(arma::dot(standardOri, normalizedOrientation));
  GenericQuaternion<T> quaternion(rotationAxis, theta);
  rotMat = arma::inv(quaternion.matrix());
  calculateMatrix();
}

template <typename T>
void GenericQuadric<T>::setScale(const Vector& scale)
{
  T scaleVals[16] = {scale[0], 0, 0, 0, 0, scale[1], 0, 0, 0, 0, scale[2], 0, 0, 0, 0, 1};
  scaleMat = arma::inv(Matrix(scaleVals, 4, 4));
  calculateMatrix();
}

template <typename T>
bool GenericQuadric<T>::contains(const Vector& point)
{
  static Matrix pointR;
  static Vector valVec;
  static T val;

  pointR = point;
  pointR.reshape(1, 4);
  pointR(0, 3) = 1;

  valVec = (pointR * qMat * arma::trans(pointR));
  val = valVec[0];
  if (val <= 0.001)
    return true;
  else
    return false;
}

template <typename T>
void GenericQuadric<T>::calculateMatrix()
{
  qMat = arma::trans(transMat) * (arma::trans(rotMat) * (arma::trans(scaleMat) * oMat * scaleMat) * rotMat) * transMat;
}

template <typename T>
void GenericQuadric<T>::print(std::ostream& out) const
{
  qMat.print(out);
}

typedef GenericQuadric<float> Quadric;

#endif
