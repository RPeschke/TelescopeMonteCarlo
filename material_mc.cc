#include <string>

#include "material_mc.h"

using namespace std;

Material::Material()
: Name_(""),
  IntrinsicRadiationLength_(0.0),
  Density_(0.0),
  Thickness_(0.0),
  AtomicNumber_(0),
  ProtonNumber_(0)
{}

void Material::init(string name, double intrinsicradiationlength, double density, double thickness, int atomicnumber, int protonnumber){
  Name_ = name;
  IntrinsicRadiationLength_ = intrinsicradiationlength;
  Density_ = density;
  Thickness_ = thickness;
  AtomicNumber_ = atomicnumber;
  ProtonNumber_ = protonnumber;
}
