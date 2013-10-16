#ifndef MONTECARLO
#define MONTECARLO

#include <string>

class Histograms;
class Material;
class Particle;
class Telescope;

class Montecarlo{
  private:
    Histograms *Graphs_;   

    Material *DUT_;
    Particle *Beam_;
    Telescope *Setup_;

    int NumberOfEvents_;

  //Implementation
    void makeHits();
    void createTwoTracks();
    void calculateScatteringAngle();
    void discoverRadiationLength();
    void printResults();

  public:
    Montecarlo();
    void init(Particle *beam, Telescope *setup, Material *dut, int numberofevents);
    void run();
};

#endif
