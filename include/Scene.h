#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Camera.h"
#include "Image.h"

template <typename T>
class GenericScene
{
  public:
    GenericScene();
    void addObject(GenericCSGObject<T>* object);
    void setCamera(const GenericCamera<T>& camera);
    void draw(int rows, int cols);
    void print(std::ostream& out = std::cout) const;
  private:
    std::vector<GenericCSGObject<T>*> d_objects;
    GenericCamera<T> d_camera;
    GenericImage<T> d_image;
    const static int d_rows = 720;
    const static int d_cols = 720;
};

template <typename T>
GenericScene<T>::GenericScene() : d_camera(), d_image(d_rows, d_cols) { }

template <typename T>
void GenericScene<T>::addObject(GenericCSGObject<T>* object) {
  d_objects.push_back(object);
  return;
}

template <typename T>
void GenericScene<T>::setCamera(const GenericCamera<T>& camera) {
  d_camera = camera;
}

template <typename T>
void GenericScene<T>::draw(int rows, int cols) {
  T dist;
  T thisDistance;
  Vector intersection;
  GenericRay<T> ray;
  GenericCSGObject<T>* closest;
  static int counter = 0;
  typename std::vector<GenericCSGObject<T>*>::iterator itr;
  ray.position = d_camera.position;
  Vector xAxis(arma::cross(d_camera.direction, d_camera.up));
  Vector xIncrement(d_camera.dx * xAxis);
  Vector yOffset;
  Vector yIncrement(d_camera.dy * d_camera.up);
  Vector focalVec(d_camera.direction * d_camera.focalDistance);

  for(int i = 0; i < rows; ++i) {
    yOffset = yIncrement * (T)(i - rows/2);

    for(int j = 0; j < cols; ++j) {
      ray.direction = yOffset + (xIncrement * (T)(j - cols/2)) + focalVec;
      ray.direction = (ray.direction / arma::norm(ray.direction, 2));

      dist = 9999999;
      for(itr = d_objects.begin(); itr != d_objects.end(); ++itr) {
         thisDistance = 9999999;
         (*itr)->intersection(ray, thisDistance);
         if(thisDistance < dist) {
           dist = thisDistance;
           closest = *itr;
         }
      }

      float* imageContents = &d_image(i, j);
      if (dist < 1000) {
        imageContents[0] = 1.0 / dist;
        //imageContents[1] = 1.0 / dist;
        //imageContents[2] = 1.0 / dist;
      }
      else {
        imageContents[0] = 0;
        //imageContents[1] = 0;
        //imageContents[2] = 0;
      }
    }
  }

  d_image.normalize();
  Vector base(ZVec3);
  base[1] = -1;
  GenericQuaternion<T> quaternion(base, 1.0 / 33);
  Matrix position(d_camera.position);
  position.reshape(1, 4);
  position[3] = 1;
  position = position * quaternion.matrix();
  position.reshape(1, 3);
  d_camera.position = position;
  d_camera.direction = ZVec3 - d_camera.position;
  d_camera.direction = (d_camera.direction / arma::norm(d_camera.direction, 2));
  std::cout << ++counter << std::endl;
}

template <typename T>
void GenericScene<T>::print(std::ostream& out) const {
  d_image.print();
}

typedef GenericScene<float> Scene;

#endif
