#include <cmath>
#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include "Planes_mc.h"
#include <vector>
#include "particle_mc.h"
#include "..\inc\randomGen.h"

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
	int plane_id=0;
	double true_x,true_y;
	int hit_x,hit_y;
	double randomNr;
	TFile f("test.root","recreate");
	TTree *tree=new TTree("tree","a simple tree");

	tree->Branch("plane_id",&plane_id,"plane_id/I");
	tree->Branch("true_x",&true_x,"true_x/D");
	tree->Branch("true_y",&true_y,"true_y/D");
	tree->Branch("hit_x",&hit_x,"hit_x/I");
	tree->Branch("hit_y",&hit_y,"hit_y/I");
	tree->Branch("rand",&randomNr,"rand/D");
	std::vector<Planes> pl;
	pl.push_back(makeTelescopePlane(0));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),150));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));
	pl.push_back(makeAirPlane(pl.back().getZEndPos(),300));
	pl.push_back(makeTelescopePlane(pl.back().getZEndPos()));

	// enter DUT here
	pl.push_back(makeAirPlane(pl.back(),450));
	pl.push_back(makeTelescopePlane(pl.back()));
	pl.push_back(makeAirPlane(pl.back(),600));
	pl.push_back(makeTelescopePlane(pl.back()));
	pl.push_back(makeAirPlane(pl.back(),750));
	pl.push_back(makeTelescopePlane(pl.back()));

Particle t;
t.init("electron",0.511, //mass
	   -1,               //charge
	   5,                //energy
	   5,                //beam Size
	   00,              // beam spread
	   10,               // beam center X 
	   5);               // beam center Y

for (int i=1;i<1000000;++i)
{
	plane_id=0;
t.newParticle();	
//	Disp(t);
	if (i%1000==0)
	{
		cout<<i<<endl;

	}
		for (auto p:pl)
	{
		p.getHit(t);

		//cout<<"telescope: "<<endl;
		//Disp(t);
		if (p.write2file)
		{
		//	Disp(t);
			true_x=t.x;
			true_y=t.y;
			hit_x=p.hit_x;
			hit_y=p.hit_y;
			randomNr=getNormRandom();
			tree->Fill();
			++plane_id;
		}
				p.propagate(t);
	}
	
}	
f.Write();
	return 0;
}
