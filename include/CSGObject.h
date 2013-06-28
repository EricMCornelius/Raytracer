#ifndef CSGOBJECT_H
#define CSGOBJECT_H

#include <string>
#include <iostream>

#include "Quadric.h"
#include "CSGNode.h"

template <typename T>
class GenericCSGObject {
public:
  GenericCSGObject();
  GenericCSGObject(const OP_TYPE::OP& op, const GenericCSGObject<T>& obj1, const GenericCSGObject<T>& obj2);
  GenericCSGObject(const OP_TYPE::OP& op, const GenericCSGObject<T>* obj1, const GenericCSGObject<T>* obj2);
  GenericCSGObject(GenericQuadric<T>& shape);
  GenericCSGObject<T> operator+(const GenericCSGObject<T>& rightObj);
  GenericCSGObject<T>& operator+=(const GenericCSGObject<T>& rightObj);
  GenericCSGObject<T> operator-(const GenericCSGObject<T>& rightObj);
  GenericCSGObject<T>& operator-=(const GenericCSGObject<T>& rightObj);
  GenericCSGObject<T> operator*(const GenericCSGObject<T>& rightObj);
  GenericCSGObject<T>& operator*=(const GenericCSGObject<T>& rightObj);
  Vector intersection(const GenericRay<T>& ray, T& dist);
  Vector normal(const Vector& point);
  void setPosition(const Vector& position);
  void setOrientation(const Vector& orientation);
  void setScale(const Vector& scale);
  void print(std::ostream& out = std::cout) const;
  GenericCSGNode<T>* root;
  std::string name;
};

template <typename T>
GenericCSGObject<T>::GenericCSGObject()
{
}

template <typename T>
GenericCSGObject<T>::GenericCSGObject(const OP_TYPE::OP& op, const GenericCSGObject<T>& obj1, const GenericCSGObject<T>& obj2)
{
  root = new GenericCSGNode<T>(op, obj1.root, obj2.root);
  return;
}

template <typename T>
GenericCSGObject<T>::GenericCSGObject(const OP_TYPE::OP& op, const GenericCSGObject<T>* obj1, const GenericCSGObject<T>* obj2)
{
  root = new GenericCSGNode<T>(op, obj1->root, obj2->root);
  return;
}

template <typename T>
GenericCSGObject<T>::GenericCSGObject(GenericQuadric<T>& shape)
{
  root = new GenericCSGNode<T>(&shape);
  return;
}

template <typename T>
GenericCSGObject<T> GenericCSGObject<T>::operator+(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::UNION, obj1, obj2);

  return obj3;
}

template <typename T>
GenericCSGObject<T>& GenericCSGObject<T>::operator+=(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::UNION, obj1, obj2);
  root = obj3.root;

  return *this;
}

template <typename T>
GenericCSGObject<T> GenericCSGObject<T>::operator-(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::EXCLUSION, obj1, obj2);

  return obj3;
}

template <typename T>
GenericCSGObject<T>& GenericCSGObject<T>::operator-=(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::EXCLUSION, obj1, obj2);
  root = obj3.root;

  return *this;
}

template <typename T>
GenericCSGObject<T> GenericCSGObject<T>::operator*(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::INTERSECTION, obj1, obj2);

  return obj3;
}

template <typename T>
GenericCSGObject<T>& GenericCSGObject<T>::operator*=(const GenericCSGObject<T>& obj2)
{
  GenericCSGObject<T> obj1 = *this;
  GenericCSGObject<T> obj3(OP_TYPE::INTERSECTION, obj1, obj2);
  root = obj3.root;

  return *this;
}

template <typename T>
Vector GenericCSGObject<T>::intersection(const GenericRay<T>& ray, T& dist)
{
  static std::set<T> dists;
  static typename std::set<T>::iterator itr;
  static Vector point;
  dists.clear();

  root->intersection(ray, dists);
  for (itr = dists.begin(); itr != dists.end(); ++itr) {
    // point = (ray.position + (ray.direction * *itr));

    if (root->contains(ray.position + (ray.direction * *itr))) {
      dist = *itr;
      return point;
    }
  }
  return ZVec3;
}

template <typename T>
void GenericCSGObject<T>::setPosition(const Vector& position)
{
  root->setPosition(position);
}

template <typename T>
void GenericCSGObject<T>::setOrientation(const Vector& orientation)
{
  root->setOrientation(orientation);
}

template <typename T>
void GenericCSGObject<T>::setScale(const Vector& scale)
{
  root->setScale(scale);
}

template <typename T>
Vector GenericCSGObject<T>::normal(const Vector& point)
{
  return NULL;
}

template <typename T>
void GenericCSGObject<T>::print(std::ostream& out) const
{
  root->print(out);
}

typedef GenericCSGObject<float> CSGObject;

#endif
