#include <cmath>
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "Planes_mc.h"
#include <vector>

using namespace std;
using namespace mcEUTEL;



int main(int argc, char *argv[]){
//   cout << "Beginning Monte Carlo Simulation" << endl;
//   Telescope *datura = new Telescope();
//   Particle *testbeam = new Particle();
//   Material *aluminium = new Material();
//   
//   int numberofplanes(6);
//   double z0(0.0), z1(0.15), z2(0.30), z3(0.45), z4(0.60), z5(0.75);
//   vector< double > planepositions;
//   planepositions.push_back(z0);
//   planepositions.push_back(z1);
//   planepositions.push_back(z2);
//   planepositions.push_back(z3);
//   planepositions.push_back(z4);
//   planepositions.push_back(z5);
//   double dutposition(z2);
//   double resolution(0.000004);
//   int numberofpixelsx(1172);
//   int numberofpixelsy(576);
//   double pixelsizex(0.0000184);
//   double pixelsizey(0.0000184);
//   string particlename("Positron");
//   double mass(0.511);//MeV
//   double charge(1.0);
//   double energy(1000);//MeV
//   double beamsize(0.02);//metres
//   double beamspread(energy*0.05);//5% of the energy
//   double beamcentrex(0.0);
//   double beamcentrey(0.0);
//   string materialname("Aluminium");
//   double intrinsicradiationlength(100*100*24.01/1000.0);
//   double density(2.7*100.0*100.0*100.0/1000.0);
//   double thickness(0.001);
//   int atomicnumber(27);
//   int protonnumber(13);
//   int numberofevents(1000000);
//   //Initialise values
//   datura->init(numberofplanes,planepositions,dutposition,resolution, numberofpixelsx, numberofpixelsy, pixelsizex, pixelsizey);
//   testbeam->init(particlename, mass, charge, energy, beamsize, beamspread, beamcentrex, beamcentrey);
//   aluminium->init(materialname, intrinsicradiationlength, density, thickness, atomicnumber, protonnumber);
// 
//   Montecarlo *app = new Montecarlo();
//   app->init(testbeam, datura, aluminium, numberofevents);
//   app->run();

	
// 	Positions p1={0,0,0};  // just some fake values 
// 	Positions p2={20,0,0};
// 	Positions p3={20,20,0};
// 	Positions p4={20,20,0.005};

// 	/Planes p(p1,p2,p3,p4);
// 	/p.setRadiationLength(9.5);
	std::vector<Planes> pl;
	pl.push_back(makeTelescopePlane(0));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));

	// enter DUT here
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));

	particle t={0,0,0,0,0,5};
	Disp(t);
	for (auto p:pl)
	{
		p.propagate(t);
		cout<<"telescope: "<<endl;
		Disp(t);
	}

	
	return 0;
}
