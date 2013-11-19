#ifndef PARTICLEMC
#define PARTICLEMC

#include <string>
#include "rapidxml.hpp"
#include "Positions_mc.h"

class Particle{
  private:
    std::string Name_;
    double Mass_;
    
    double BeamEnergy_;  // Nomal energy of the beam
    
    double BeamSize_;  //Size of the beam spot
    double BeamSpread_;  //Error on the energy, measured as one sigma of a guassianly distributed energy
    double BeamCentreX_;
    double BeamCentreY_;
	
  public:
	  double Charge_;
	  double InitialBeamDirectionX,InitialBeamDirectionY;
	  //double x,y,z;
	  mcEUTEL::vector3 position_,directions_;

	  //double phi,theta;
	  double particleEnergy_; // momentum
	  void newParticle(); //this function starts the random generator.
    Particle();
    
    void init(std::string name, double mass, double charge, 
              double energy, double beamsize, double beamspread,
              double beamcentrex, double beamcentrey);
	int ProcessXMLNode(rapidxml::xml_node<> *node);

    std::string getName(){return Name_;}
    double getMass(){return Mass_;}
    double getCharge(){return Charge_;}
    double getEnergy(){return particleEnergy_;}
    double getBeamSize(){return BeamSize_;}
    double getBeamSpread(){return BeamSpread_;}
    double getBeamCentreX(){return BeamCentreX_;}
    double getBeamCentreY(){return BeamCentreY_;}

};


#endif
