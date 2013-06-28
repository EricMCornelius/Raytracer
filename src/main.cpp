#include <iostream>
#include <sstream>
#include "typedefs.h"
#include "Camera.h"
#include "Shapes.h"
#include "Scene.h"
#include "Ray.h"

using namespace std;

void getVector(const string& prompt, Vector& vec)
{
  vec = Vector(3);

  cout << prompt;
  string str;
  getline(cin, str);
  stringstream ss;
  ss << str;
  ss >> vec[0] >> vec[1] >> vec[2];
}

int main(int argc, char* argv[])
{
  Scene scene;

  Vector position;
  Vector orientation;
  Vector scale;

  getVector("Enter position: ", position);
  // Vector scale;
  // getVector("Enter scale: ", scale);

  Sphere sphere;
  CSGObject* obj1 = sphere.getObject();
  obj1->name = "Sphere";
  position.print();
  obj1->setPosition(position);

  getVector("Enter position: ", position);

  Sphere sphere2;
  CSGObject* obj2 = sphere2.getObject();
  obj2->name = "Sphere";
  obj2->setPosition(position);

  /*
  getVector("Enter orientation: ", orientation);
  getVector("Enter scale: ", scale);

  Cylinder cylinder;
  CSGObject* obj3 = cylinder.getObject();
  obj3->name = "Cylinder";
  obj3->setOrientation(orientation);
  obj3->setScale(scale);
  */

  CSGObject* obj4 = new CSGObject(OP_TYPE::EXCLUSION, obj2, obj1);
  // CSGObject* obj5 = new CSGObject(OP_TYPE::EXCLUSION, obj4, obj2);
  // CSGObject* obj5 = new CSGObject(OP_TYPE::EXCLUSION, obj1, obj3);
  scene.addObject(obj4);

  // CSGObject combined(OP_TYPE::INTERSECTION, obj, obj2);

  Camera camera;
  camera.dx = 0.005;
  camera.dy = 0.005;
  scene.setCamera(camera);

  scene.draw(720, 720);
}
