#ifndef PARTICLEMC
#define PARTICLEMC

#include <string>

class Particle{
  private:
    std::string Name_;
    double Mass_;
    double Charge_;
    double Energy_;
    
    double BeamSize_;  //Size of the beam spot
    double BeamSpread_;  //Error on the energy, measured as one sigma of a guassianly distributed energy
    double BeamCentreX_;
    double BeamCentreY_;
  public:
    Particle();
    ~Particle();
    void init(std::string name, double mass, double charge, 
              double energy, double beamsize, double beamspread,
              double beamcentrex, double beamcentrey);

    std::string getName(){return Name_;}
    double getMass(){return Mass_;}
    double getCharge(){return Charge_;}
    double getEnergy(){return Energy_;}
    double getBeamSize(){return BeamSize_;}
    double getBeamSpread(){return BeamSpread_;}
    double getBeamCentreX(){return BeamCentreX_;}
    double getBeamCentreY(){return BeamCentreY_;}

};

#endif
