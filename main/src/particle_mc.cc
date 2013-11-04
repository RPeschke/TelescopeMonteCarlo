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

int Particle::ProcessXMLNode( rapidxml::xml_node<> *node )
{
	if(strcmp(node->name(),"Beam")) return -1; // to make sure that the correct node is used
	
	auto test=node->first_node("Type");
	auto n=test->first_node()->value();
	Name_=node->first_node("Type")->first_node()->value();
	Mass_=std::atof(node->first_node("mass")->first_node()->value());

	Charge_ = std::atof(node->first_node("charge")->first_node()->value());

	Energy_ =std::atof(node->first_node("energy")->first_node()->value());
	BeamSize_ = std::atof(node->first_node("beamSize")->first_node()->value());
	BeamSpread_ =std::atof(node->first_node("beamEnergySpread")->first_node()->value());
	BeamCentreX_ = std::atof(node->first_node("beamCenterX")->first_node()->value());
	BeamCentreY_ = std::atof(node->first_node("beamCenterY")->first_node()->value());
}
