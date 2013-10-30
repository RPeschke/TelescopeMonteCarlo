#include "Planes_mc.h"
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
void higlandFormular(mcEUTEL::particle& p,double relRadiationLength );
void straigthForward(mcEUTEL::particle& p,double distance );


bool mcEUTEL::Planes::setPixelSize( const int oriantaion,const double PixelSize )
{
	if (oriantaion == 1)
	{
		pixelSizeX_=PixelSize;
	}else if (oriantaion==2)
	{
		pixelSizeY_=PixelSize;
	}
	return false;
}

void mcEUTEL::Planes::propagate( particle& p )
{
	higlandFormular(p,thickness/radiationLength_);
	straigthForward(p,thickness/cos(sqrt(p.phi*p.phi+p.theta*p.theta)));

}


void higlandFormular(mcEUTEL::particle& p,double relRadiationLength ){

	std::mt19937 mt;
	mt.seed( time(NULL) );
	std::normal_distribution<double> normalDist(0,1);
	double sigmaHl=0.0136/p.energy*sqrt(relRadiationLength)*(1+0.038*log(relRadiationLength));
	if (sigmaHl>0)
	{
		p.phi+=normalDist(mt)*sigmaHl;
		p.theta+=normalDist(mt)*sigmaHl;

	}
	
	
}

void straigthForward( mcEUTEL::particle& p,double distance )
{
	p.x+=distance*tan(p.phi);
	p.y+=distance*tan(p.theta);
	p.z+=distance;
}


void mcEUTEL::Disp( const mcEUTEL::particle& p )
{
	std::cout<<"p.x: "<<p.x <<"  p.y: "<<p.y<< " p.z: "<< p.z<<std::endl;

}

mcEUTEL::Planes mcEUTEL::makeTelescopePlane( double zPosition )
{
	Positions p1={0.0,0.0,zPosition};
	Positions p2={21,0,zPosition};
	Positions p3={21,10,zPosition};
	Positions p4={0,0,zPosition+0.05}; 

	Planes p(p1,p2,p3,p4);
	p.setRadiationLength(93.6);//Silicon (Si). 9.36 cm
	return p;
}

mcEUTEL::Planes mcEUTEL::makeAirPlane( double zStart,double ZEnd )
{
	Positions p1={0.0,0.0,zStart};
	Positions p2={21,0,zStart};
	Positions p3={21,10,zStart};
	Positions p4={0,0,ZEnd}; 

	Planes p(p1,p2,p3,p4);
	p.setRadiationLength(304000);//Air (Si). 304 m
	return p;
}
