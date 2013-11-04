#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector3.h"

#include "histograms_mc.h"
#include "montecarlo.h"
#include "particle_mc.h"
#include "material_mc.h"
#include "telescope_mc.h"
#include "SimpleRNG.h"

using namespace std;

double GaussianRand(double mean, double sigma);

SimpleRNG *RANDOMNUMBERGEN = new SimpleRNG();

//Temp Hack for test purposes:
TH1D *XDistribution = new TH1D("XDistribution", "Distribution of tracks around the x position of the sensor; X position (metres); Count", 1174, -0.022, 0.022);
TH1D *YDistribution = new TH1D("YDistribution", "Distribution of tracks around the y position of the sensor; Y position (metres); Count", 578, -0.011, 0.011);
TH1D *RandomHistogram = new TH1D("RandomHistogram", "Test of the gaussian-ness of a random number generator", 578, -0.1, 0.1);
TH1D *AngleX = new TH1D("AngleX", "Distribution of angles in X for all planes; \\theta_{x} (rads); Count", 578, -0.011, 0.011);
TH1D *AngleY = new TH1D("AngleY", "Distribution of angles in Y for all planes; \\theta_{y} (rads); Count", 578, -0.011, 0.011);

bool DEBUG(false);

Montecarlo::Montecarlo()
: Graphs_(),
  DUT_(NULL),
  Beam_(NULL),
  Setup_(NULL),
  NumberOfEvents_(0)
{}

void Montecarlo::init(Particle *particle, Telescope *telescope, Material *material, int numberofevents){
  DUT_ = material;
  Beam_ = particle;
  Setup_ = telescope;
  NumberOfEvents_ = numberofevents;
  Graphs_ = new Histograms();
  for(int i = 0; i < Setup_->getNumberOfPlanes(); ++i){
    stringstream hitmapname;
    stringstream hitmaptitle;
    hitmapname << "HitmapPlane" << i;
    hitmaptitle << "Hitmap for plane" << i << "; x (m); y(m)";
    TH2D *hitmap = new TH2D(hitmapname.str().c_str(),hitmaptitle.str().c_str(), 1172, -0.001, 0.023, 578, -0.001, 0.013); 
    Graphs_->getHitmapPlane()->push_back(hitmap);
  }
  
  for(int i = 0; i < Setup_->getNumberOfPlanes() + 1; ++i){
    stringstream anglexname;
    stringstream anglextitle;
    stringstream angleyname;
    stringstream angleytitle;
    stringstream scatteringanglexname;
    stringstream scatteringanglextitle;    
    stringstream scatteringangleyname;
    stringstream scatteringangleytitle;

    if(i != Setup_->getNumberOfPlanes() - 1 &&
      Setup_->getDUTPosition() <= Setup_->getPlanePositions()[i+1] &&
      Setup_->getDUTPosition() >= Setup_->getPlanePositions()[i]){

      anglexname << "AngleXPlaneDUT";
      angleyname << "AngleYPlaneDUT";
      scatteringanglexname << "ScatteringAngleXPlaneDUT";
      scatteringangleyname << "ScatteringAngleYPlaneDUT";

      anglextitle << "Track angle in X on plane DUT; \\theta_{x} (rads); Count";
      angleytitle << "Track angle in Y on plane DUT; \\theta_{y} (rads); Count";
      scatteringanglextitle << "Track scattering angle in X on plane DUT; \\theta_{x} (rads); Count";
      scatteringangleytitle << "Track scattering angle in Y on plane DUT; \\theta_{y} (rads); Count";
    } else{
      anglexname << "AngleXPlane" << i;
      angleyname << "AngleYPlane" << i;
      scatteringanglexname << "ScatteringAngleXPlane" << i;
      scatteringangleyname << "ScatteringAngleYPlane" << i;

      anglextitle << "Track angle in X on plane " << i << "; \\theta_{x} (rads); Count";
      angleytitle << "Track angle in Y on plane " << i << "; \\theta_{y} (rads); Count";
      scatteringanglextitle << "Track scattering angle in X on plane " << i << "; \\theta_{x} (rads); Count";
      scatteringangleytitle << "Track scattering angle in Y on plane " << i << "; \\theta_{y} (rads); Count";
    }

    TH1D *anglex = new TH1D(anglexname.str().c_str(),anglextitle.str().c_str(),1000,-0.001,0.001); 
    Graphs_->getAngleXPlane()->push_back(anglex);
    TH1D *angley = new TH1D(angleyname.str().c_str(),angleytitle.str().c_str(),1000,-0.001,0.001); 
    Graphs_->getAngleYPlane()->push_back(angley);
    TH1D *scatteringanglex = new TH1D(scatteringanglexname.str().c_str(),scatteringanglextitle.str().c_str(),1000,-0.001,0.001); 
    Graphs_->getScatteringAngleXPlane()->push_back(scatteringanglex);
    TH1D *scatteringangley = new TH1D(scatteringangleyname.str().c_str(),scatteringangleytitle.str().c_str(),1000,-0.001,0.001); 
    Graphs_->getScatteringAngleYPlane()->push_back(scatteringangley);
  }
}

void Montecarlo::run(){
if(DEBUG){
  cout << "Started the run" << endl;
}
  for(int i = 0; i < NumberOfEvents_; ++i){
    makeHits();
    createTwoTracks();
    calculateScatteringAngle();
  }
  discoverRadiationLength();
  printResults();
  Graphs_->getRootFile()->Write();
}

void Montecarlo::makeHits(){
  vector< TVector3* > realhits;
  vector< TVector3* > pixelhits;

  double x0 = (rand()%Setup_->getNumberOfPixelsX())*Setup_->getPixelSizeX();
if(DEBUG){
  cout << "x0 = " << x0 << endl;
}
  double y0 = (rand()%Setup_->getNumberOfPixelsY())*Setup_->getPixelSizeY();
if(DEBUG){
  cout << "y0 = " << y0 << endl;
}
if(DEBUG){
  cout << "HitMapPlane Size = " << Graphs_->getHitmapPlane()->size() << endl;
}
  Graphs_->getHitmapPlane()->at(0)->Fill(x0,y0);
  double z0 = Setup_->getPlanePositions()[0];
if(DEBUG){
  cout << "z0 = " << z0 << endl;
}
  TVector3 *initial = new TVector3(x0, y0, z0);
  realhits.push_back(initial);

  int x0pix = static_cast<int>(x0/Setup_->getPixelSizeX());//Should auto round down to the nearest pixel
if(DEBUG){
  cout << "x0pix = " << x0pix << endl;
}
  int y0pix = static_cast<int>(y0/Setup_->getPixelSizeY());//Should auto round down to the nearest pixel
if(DEBUG){
  cout << "y0pix = " << y0pix << endl;
}
  TVector3 *initialpix = new TVector3(x0pix, y0pix, z0);
  pixelhits.push_back(initialpix);

  double previouspositionx(x0);
  double previouspositiony(y0);
  double previouspositionz(z0);
  double previousanglex(0.0);//Relative to Z axis
  double previousangley(0.0);//Relative to Z axis

  for(int i = 1; i < Setup_->getNumberOfPlanes(); ++i){
    double sensorradlength = 0.000533842;//The relative radiation length of the mimosa sensor
    double sigmaangle = (13.6/Beam_->getEnergy())*sqrt(sensorradlength)*(1 + 0.038*log(sensorradlength));
    double anglex = GaussianRand(previousanglex,sigmaangle);
    Graphs_->getAngleXPlane()->at(i-1)->Fill(anglex);
    double angley = GaussianRand(previousangley,sigmaangle);
    Graphs_->getAngleYPlane()->at(i-1)->Fill(angley);
    Graphs_->getScatteringAngleXPlane()->at(i-1)->Fill(anglex-previousanglex);
    Graphs_->getScatteringAngleYPlane()->at(i-1)->Fill(angley-previousangley);
    previousanglex = anglex;
    previousangley = angley;
    double z = Setup_->getPlanePositions()[i];
    double x = previouspositionx + (z-previouspositionz)*tan(anglex);
    double y = previouspositiony + (z-previouspositionz)*tan(angley);
    Graphs_->getHitmapPlane()->at(i)->Fill(x,y);
    double xpix = static_cast<int>(x/Setup_->getPixelSizeX());
    double ypix = static_cast<int>(y/Setup_->getPixelSizeY());
if(DEBUG){
    cout << "sigmaangle = " << sigmaangle << endl;
}
if(DEBUG){
    cout << "anglex = " << anglex << endl;
}
if(DEBUG){
    cout << "angley = " << angley << endl;
}
if(DEBUG){
    cout << "z = " << z << endl;
}
if(DEBUG){
    cout << "x = " << x << endl;
}
if(DEBUG){
    cout << "y = " << y << endl;
}
if(DEBUG){
    cout << "xpix = " << xpix << endl;
}
if(DEBUG){
    cout << "ypix = " << ypix << endl;
}
    previouspositionx = x;
    previouspositiony = y;
    previouspositionz = z;

    if(i != Setup_->getNumberOfPlanes() - 1){
      if(Setup_->getDUTPosition() <= Setup_->getPlanePositions()[i+1] && Setup_->getDUTPosition() >= z){
        double dutradlength = DUT_->getThickness()/DUT_->getIntrinsicRadiationLength();
        dutradlength = 1.0;
        double sigmaangledut = (13.6/Beam_->getEnergy())*sqrt(dutradlength)*(1 + 0.038*log(dutradlength));
        double anglexdut = GaussianRand(previousanglex,sigmaangledut);
        AngleX->Fill(anglexdut - previousanglex);
        double angleydut = GaussianRand(previousangley,sigmaangledut);
        AngleY->Fill(angleydut - previousangley);
        Graphs_->getAngleXPlane()->at(i-1)->Fill(anglexdut);
        Graphs_->getAngleYPlane()->at(i-1)->Fill(angleydut);
        Graphs_->getScatteringAngleXPlane()->at(i-1)->Fill(anglexdut-previousanglex);
        Graphs_->getScatteringAngleYPlane()->at(i-1)->Fill(angleydut-previousangley);
        previousanglex = anglexdut;
        previousangley = angleydut;
        double zdut = Setup_->getDUTPosition();
        double xdut = previouspositionx + (zdut-previouspositionz)*tan(anglexdut);
        double ydut = previouspositiony + (zdut-previouspositionz)*tan(angleydut);
        previouspositionx = xdut;
        previouspositiony = ydut;
        previouspositionz = zdut;
      }
    }    

    TVector3 *nexthit = new TVector3(x,y,z);
    TVector3 *nexthitpix = new TVector3(xpix,ypix,z);
    realhits.push_back(nexthit);
    pixelhits.push_back(nexthitpix);
  }
if(DEBUG){
  cout << "Real positions are:" << endl;
}
  for(size_t i = 0; i < realhits.size(); ++i){
if(DEBUG){
    cout << realhits[i]->x() << "\t\t" << realhits[i]->y() << "\t\t" << realhits[i]->z() << endl;
}
  }
if(DEBUG){
  cout << "Pixel positions are:" << endl;
}
  for(size_t i = 0; i < pixelhits.size(); ++i){
if(DEBUG){
    cout << pixelhits[i]->x() << "\t\t" << pixelhits[i]->y() << "\t\t" << pixelhits[i]->z() << endl;
}
  }
if(DEBUG){
  cout << endl;
}
}

void Montecarlo::createTwoTracks(){

}

void Montecarlo::calculateScatteringAngle(){

}

void Montecarlo::discoverRadiationLength(){

}

void Montecarlo::printResults(){

}

double GaussianRand(double mean, double sigma){
  double number = RANDOMNUMBERGEN->GetNormal(mean,sigma);
  RandomHistogram->Fill(number);
  return number;
}
