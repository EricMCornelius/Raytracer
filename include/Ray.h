#ifndef RAY_H
#define RAY_H

template <typename T>
class GenericRay {
public:
  GenericRay() { };
  GenericRay(const Vector& direction, const Vector& position);
  Vector direction;
  Vector position;
  void print(std::ostream& out = std::cout) const;
};

template <typename T>
GenericRay<T>::GenericRay(const Vector& _position, const Vector& _direction) : position(_position), direction(_direction) { }

template <typename T>
void GenericRay<T>::print(std::ostream& out) const {
  position.print(out);
  direction.print(out);
  std::cout << std::endl;
}

typedef GenericRay<float> Ray;

#endif
