#ifndef TELESCOPEMC
#define TELESCOPEMC

#include <iostream>
#include <vector>

class Telescope{
  private:
    int NumberOfPlanes_;
    std::vector< double > PlanePositions_;
    double DUTPosition_;
    double Resolution_;

    int NumberOfPixelsX_;
    int NumberOfPixelsY_;
    double PixelSizeX_;
    double PixelSizeY_;
  public:
    Telescope();
    ~Telescope();
    void init(int numberofplanes, std::vector< double > planepositions,
              double dutposition, double resolution, int numberofpixelsx,
              int numberofpixelsy, double pixelsizex, double pixelsizey);

    void resetPixels();

    int getNumberOfPlanes(){return NumberOfPlanes_;}
    std::vector< double > getPlanePositions(){return PlanePositions_;}
    double getDUTPosition(){return DUTPosition_;}
    double getResolution(){return Resolution_;}
    int getNumberOfPixelsX(){return NumberOfPixelsX_;}
    int getNumberOfPixelsY(){return NumberOfPixelsY_;}
    double getPixelSizeX(){return PixelSizeX_;}
    double getPixelSizeY(){return PixelSizeY_;}
};

#endif
