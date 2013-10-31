#include <string>
#include <random>
#include "particle_mc.h"
#include "randomGen.h"
using namespace std;


Particle::Particle()
: Name_(""),
  Mass_(0.0),
  Charge_(0.0),
  Energy_(0.0),
  BeamSize_(0.0),
  BeamSpread_(0.0),
  BeamCentreX_(0.0),
  BeamCentreY_(0.0),
  x(0.0),y(0.0),z(0.0),
  phi(0.0),theta(0.0),energy_(0.0)
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

void Particle::newParticle()
{
	y=BeamCentreY_+BeamSize_*getNormRandom();
	x=BeamCentreX_+BeamSize_*getNormRandom();
	energy_=Energy_+BeamSpread_*getNormRandom();
	phi=0;
	theta=0;
	z=0;
}
