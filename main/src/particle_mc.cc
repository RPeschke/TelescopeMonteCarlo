#include <string>

#include "particle_mc.h"

using namespace std;

Particle::Particle()
: Name_(""),
  Mass_(0.0),
  Charge_(0.0),
  Energy_(0.0),
  BeamSize_(0.0),
  BeamSpread_(0.0),
  BeamCentreX_(0.0),
  BeamCentreY_(0.0)
{}

void Particle::init(string name, double mass, double charge, double energy, double beamsize, double beamspread, double beamcentrex, double beamcentrey){
  Name_ = name;
  Mass_ = mass;
  Charge_ = charge;
  Energy_ = energy;
  BeamSize_ = beamsize;
  BeamSpread_ = beamspread;
  BeamCentreX_ = beamcentrex;
  BeamCentreY_ = beamcentrey;
}
