#ifndef SHAPES_H
#define SHAPES_H

#include "Quadric.h"
#include "CSGObject.h"

template <typename T>
class GenericSphere {
public:
  GenericSphere() : d_sphere(GenericQuadric<T>::SPHERE) {};

  GenericCSGObject<T>* getObject() {
    GenericCSGObject<T>* objPtr = new GenericCSGObject<T>(d_sphere);
    return objPtr;
  }
  ;

private:
  GenericQuadric<T> d_sphere;
};

typedef GenericSphere<float> Sphere;

template <typename T>
class GenericCone {
public:
  GenericCone() : d_cone(GenericQuadric<T>::SHAPE_TYPE::CONE) {};

  GenericCSGObject<T>* getObject() {
    GenericCSGObject<T>* objPtr = new GenericCSGObject<T>(d_cone);
    return objPtr;
  }
  ;

private:
  GenericQuadric<T> d_cone;
};

typedef GenericCone<float> Cone;

template <typename T>
class GenericCube {
public:
  GenericCube() {
    GenericQuadric<T> plane(2);
    Vector ori;
    Vector pos;
    pos[0] = 0;
    pos[1] = 0.5;
    pos[2] = 0;
    ori[0] = 0;
    ori[1] = 1;
    ori[2] = 0;

    plane.setPosition(pos);
    plane.setOrientation(ori);
    GenericCSGObject<T> top(plane);

    pos[1] = -0.5;
    ori[1] = -1;
    plane.setPosition(pos);
    plane.setOrientation(ori);
    GenericCSGObject<T> bottom(plane);
    top *= bottom;

    pos[0] = 0.5;
    pos[1] = 0;
    ori[0] = 1;
    ori[1] = 0;
    plane.setPosition(pos);
    plane.setOrientation(ori);
    GenericCSGObject<T> right(plane);
    top *= right;

    pos[0] = -0.5;
    ori[0] = -1;
    plane.setPosition(pos);
    plane.setOrientation(ori);
    GenericCSGObject<T> left(plane);
    top *= left;

    pos[0] = 0;
    pos[2] = 0.5;
    ori[0] = 0;
    ori[2] = 1;
    plane.setPosition(pos);
    plane.setOrientation(ori);
    CSGObject near(plane);
    top *= near;

    pos[2] = -0.5;
    ori[2] = -1;
    plane.setPosition(pos);
    plane.setOrientation(ori);
    GenericCSGObject<T> far(plane);
    top *= far;

    d_cube = top;
  }
  ;

  GenericCSGObject<T> getObject() { return d_cube; }
  ;

private:
  GenericCSGObject<T> d_cube;
};

typedef GenericCube<float> Cube;

template <typename T>
class GenericCylinder {
public:
  GenericCylinder() : d_cylinder(GenericQuadric<T>::CYLINDER) {}

  GenericCSGObject<T>* getObject() {
    GenericCSGObject<T>* objPtr = new GenericCSGObject<T>(d_cylinder);
    return objPtr;
  }
  ;

private:
  GenericQuadric<T> d_cylinder;
};

typedef GenericCylinder<float> Cylinder;

#endif
