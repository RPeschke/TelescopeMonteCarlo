#include <vector>

#include "telescope_mc.h"

using namespace std;

Telescope::Telescope()
: NumberOfPlanes_(0),
  PlanePositions_(),
  DUTPosition_(0.0),
  Resolution_(0.0),
  NumberOfPixelsX_(0),
  NumberOfPixelsY_(0),
  PixelSizeX_(0.0),
  PixelSizeY_(0.0)
{}

void Telescope::init(int numberofplanes, vector< double > planepositions, double dutposition, double resolution, int numberofpixelsx, int numberofpixelsy, double pixelsizex, double pixelsizey){
  NumberOfPlanes_ = numberofplanes;
  PlanePositions_ = planepositions;
  DUTPosition_ = dutposition;
  Resolution_ = resolution;
  NumberOfPixelsX_ = numberofpixelsx;
  NumberOfPixelsY_ = numberofpixelsy;
  PixelSizeX_ = pixelsizex;
  PixelSizeY_ = pixelsizey;
}

void Telescope::resetPixels(){

}
