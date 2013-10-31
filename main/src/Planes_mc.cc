#include "Planes_mc.h"
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include "particle_mc.h"
#include "randomGen.h"
void higlandFormular(Particle& p,double relRadiationLength );
void straigthForward(Particle& p,double distance );


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

void mcEUTEL::Planes::propagate( Particle& p )
{
	higlandFormular(p,thickness/radiationLength_);
	straigthForward(p,thickness);

}

void mcEUTEL::Planes::getHit( const Particle& p )
{
	if (p.x>p1_.x&&p.x<p2_.x
		&& 
		p.y>p1_.y&&p.y<p3_.y)
	{
		hit_x=static_cast<int>((p.x-p1_.x)/pixelSizeX_);
		hit_y=static_cast<int>((p.y-p1_.y)/pixelSizeY_);
	}else				{
		hit_x=0;
		hit_x=0;
	}
}


void higlandFormular(Particle& p,double relRadiationLength ){


	double sigmaHl=0.0136/p.energy_*sqrt(relRadiationLength)*(1+0.038*log(relRadiationLength));
	if (sigmaHl>0)
	{
		p.phi+=getNormRandom()*sigmaHl;
		p.theta+=getNormRandom()*sigmaHl;

	}
	
	
}

void straigthForward( Particle& p,double distance )
{
	p.x+=distance*tan(p.phi);
	p.y+=distance*tan(p.theta);
	p.z+=distance;
}


void mcEUTEL::Disp( const Particle& p )
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
	p.setPixelSize(1,0.045);
	p.setPixelSize(2,0.045);
	p.write2file=true;
	return p;
}

mcEUTEL::Planes mcEUTEL::makeTelescopePlane( const mcEUTEL::Planes& PreviousPlane )
{
	return makeTelescopePlane(PreviousPlane.getZEndPos());
}

mcEUTEL::Planes mcEUTEL::makeAirPlane( double zStart,double ZEnd )
{
	Positions p1={0.0,0.0,zStart};
	Positions p2={21,0,zStart};
	Positions p3={21,10,zStart};
	Positions p4={0,0,ZEnd}; 

	Planes p(p1,p2,p3,p4);
	p.setPixelSize(1,0.045);
	p.setPixelSize(2,0.045);
	p.setRadiationLength(304000);//Air (Si). 304 m
	p.write2file=false;
	return p;
}

mcEUTEL::Planes mcEUTEL::makeAirPlane( const Planes& PreviousPlane,double ZEnd )
{
	return makeAirPlane(PreviousPlane.getZEndPos(),ZEnd);
}
