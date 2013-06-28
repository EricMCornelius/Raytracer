#ifndef GenericCSGNode_H
#define GenericCSGNode_H

#include <set>
#include <iostream>

struct OP_TYPE {
  enum OP {
    UNION,
    INTERSECTION,
    EXCLUSION
  };
};

template <typename T>
class GenericCSGNode {
public:
  enum TYPE {
    QUADRIC,
    OPERATOR
  };

  GenericCSGNode(GenericQuadric<T>* shape);
  GenericCSGNode(const OP_TYPE::OP& op, GenericCSGNode* left, GenericCSGNode* right);
  bool contains(const Vector& point);
  void intersection(const GenericRay<T>& ray, std::set<T>& dists);
  Vector normal(const Vector& point);
  void setPosition(const Vector& position);
  void setOrientation(const Vector& orientation);
  void setScale(const Vector& scale);
  void print(std::ostream& out = std::cout) const;

private:
  TYPE d_type;
  OP_TYPE::OP d_op;
  GenericQuadric<T>* d_shape;
  GenericCSGNode<T>* d_left;
  GenericCSGNode<T>* d_right;

  void print(const OP_TYPE::OP& op, std::ostream& out = std::cout) const;
};

template <typename T>
GenericCSGNode<T>::GenericCSGNode(GenericQuadric<T>* shape)
    : d_shape(shape), d_left(0), d_right(0), d_type(QUADRIC)
{
}

template <typename T>
GenericCSGNode<T>::GenericCSGNode(const OP_TYPE::OP& op, GenericCSGNode* left, GenericCSGNode* right)
    : d_shape(0), d_left(left), d_right(right), d_type(OPERATOR), d_op(op)
{
}

template <typename T>
void GenericCSGNode<T>::print(std::ostream& out) const
{
  switch (d_type) {
    case QUADRIC:
      d_shape->print(out);
      break;
    case OPERATOR:
      d_left->print(out);
      print(d_op, out);
      d_right->print(out);
      break;
    default:
      break;
  }
}

template <typename T>
void GenericCSGNode<T>::print(const OP_TYPE::OP& op, std::ostream& out) const
{
  switch (op) {
    case OP_TYPE::UNION:
      out << " UNION ";
      break;
    case OP_TYPE::INTERSECTION:
      out << " INTERSECTION ";
      break;
    case OP_TYPE::EXCLUSION:
      out << " EXCLUSION ";
      break;
    default:
      break;
  }
}

template <typename T>
bool GenericCSGNode<T>::contains(const Vector& point)
{
  switch (d_type) {
    case OPERATOR:
      switch (d_op) {
        case OP_TYPE::UNION:
          return d_left->contains(point) || d_right->contains(point);
          break;
        case OP_TYPE::INTERSECTION:
          return (d_left->contains(point) && d_right->contains(point));
          break;
        case OP_TYPE::EXCLUSION:
          return (d_left->contains(point) && !d_right->contains(point));
          break;
        default:
          break;
      }
    case QUADRIC:
      return d_shape->contains(point);
      break;
  }

  return false;
}

template <typename T>
void GenericCSGNode<T>::intersection(const GenericRay<T>& ray, std::set<T>& dists)
{
  switch (d_type) {
    case OPERATOR:
      d_left->intersection(ray, dists);
      d_right->intersection(ray, dists);
      break;
    case QUADRIC:
      d_shape->intersection(ray, dists);
      break;
    default:
      break;
  }
  return;
}

template <typename T>
void GenericCSGNode<T>::setPosition(const Vector& position)
{
  switch (d_type) {
    case OPERATOR:
      d_left->setPosition(position);
      d_right->setPosition(position);
      break;
    case QUADRIC:
      d_shape->setPosition(position);
      break;
    default:
      break;
  }
}

template <typename T>
void GenericCSGNode<T>::setOrientation(const Vector& orientation)
{
  switch (d_type) {
    case OPERATOR:
      d_left->setOrientation(orientation);
      d_right->setOrientation(orientation);
      break;
    case QUADRIC:
      d_shape->setOrientation(orientation);
      break;
    default:
      break;
  }
}

template <typename T>
void GenericCSGNode<T>::setScale(const Vector& scale)
{
  switch (d_type) {
    case OPERATOR:
      d_left->setScale(scale);
      d_right->setScale(scale);
      break;
    case QUADRIC:
      d_shape->setScale(scale);
      break;
    default:
      break;
  }
}

template <typename T>
Vector GenericCSGNode<T>::normal(const Vector& point)
{
  return NULL;
}

typedef GenericCSGNode<float> CSGNode;

#endif
