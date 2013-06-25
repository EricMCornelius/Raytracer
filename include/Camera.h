#ifndef CAMERA_H
#define CAMERA_H

template <typename T>
class GenericCamera {
public:
  GenericCamera();
  GenericCamera(Vector position, Vector direction, Vector up, T focalDistance);
  void print(std::ostream& out = std::cout) const;

  Vector position;
  Vector direction;
  Vector up;
  T focalDistance;
  T dx;
  T dy;
};

template <typename T>
GenericCamera<T>::GenericCamera() 
  : position(ZVec3), direction(ZVec3), up(ZVec3), focalDistance(1.0), dx(0.01), dy(0.01) 
{
  position[2] = 5;
  direction[2] = -1;
  up[1] = 1;
}

template <typename T>
GenericCamera<T>::GenericCamera(Vector _position, Vector _direction, Vector _up, T _focalDistance)
  : position(_position), direction(_direction), up(_up), focalDistance(_focalDistance), dx(0), dy(0) { }

template <typename T>
void GenericCamera<T>::print(std::ostream& out) const {
  position.print(out, "Position:");
  direction.print(out, "Direction:");
  up.print(out, "Up:");
  std::cout << "dx:\n\t" << dx << std::endl;
  std::cout << "dy:\n\t" << dy << std::endl;
}

typedef GenericCamera<float> Camera;

#endif
