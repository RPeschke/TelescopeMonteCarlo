#include "Planes_mc.h"
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include "particle_mc.h"
#include "randomGen.h"
#include "rapidxml.hpp"
void higlandFormular(Particle& p,double relRadiationLength );
void straigthForward(Particle& p,double distance );
int getPosition(rapidxml::xml_node<> *node,mcEUTEL::Positions & p);


mcEUTEL::Planes::Planes(rapidxml::xml_node<> *node):hit_x(0),hit_y(0),write2file(true){
	ProcessXMLNode(node);
}

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

int mcEUTEL::Planes::ProcessXMLNode( rapidxml::xml_node<> *node )
{
	if(strcmp(node->name(),"plane")) return -1; // to make sure that the correct node is used
		
	//auto position= node->first_node("positions");
	Positions p;
	int posNr=0;
	for (auto position= node->first_node("ActiveArea")->first_node("position");position;position=position->next_sibling())
	{
		
		posNr=std::atoi(position->first_attribute("nr")->value());
		switch (posNr)
		{
		case 1:
			getPosition(position,p1_);
			break;
		case 2:
			getPosition(position,p2_);
			break;
		case 3:
			getPosition(position,p3_);
			break;
		case 4:
			getPosition(position,p4_);
			break;
		}


	}
	name_=node->first_attribute("Type")->value();
	//std::cout<<name_<<std::endl;
	radiationLength_=std::atof(node->first_node("radiationLength")->first_attribute("value")->value());
	pixelSizeX_=std::atof(node->first_node("PixelSize")->first_attribute("x")->value());
	pixelSizeY_=std::atof(node->first_node("PixelSize")->first_attribute("y")->value());
	thickness=p4_.z-p1_.z;
	return 1;
}

int mcEUTEL::Planes::fitPlaneBetweenOtherPlanes( const Planes& beforePlane,const Planes& afterPlane )
{
	p1_.x=beforePlane.p1_.x;
	p1_.y=beforePlane.p1_.y;
	p1_.z=beforePlane.p4_.z;


	p2_.x=beforePlane.p2_.x;
	p2_.y=beforePlane.p2_.y;
	p2_.z=beforePlane.p4_.z;

	p3_.x=beforePlane.p3_.x;
	p3_.y=beforePlane.p3_.y;
	p3_.z=beforePlane.p4_.z;


	p4_.x=afterPlane.p1_.x;
	p4_.y=afterPlane.p1_.y;
	p4_.z=afterPlane.p1_.z;


	thickness=p4_.z-p1_.z;
	return 1;
}

int getPosition(rapidxml::xml_node<> *node,mcEUTEL::Positions & p){
if(strcmp(node->name(),"position")) return -1; // to make sure that the correct node is used

//auto x=node->first_node("x")->first_attribute()->value();

p.x=std::atof(node->first_node("x")->first_attribute()->value());

//auto y=node->first_node("y")->first_attribute()->value();

p.y=std::atof(node->first_node("y")->first_attribute()->value());


//auto y=node->first_node("y")->first_attribute()->value();

p.z=std::atof(node->first_node("z")->first_attribute()->value());
return 1;
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
	Positions p1(0.0,0.0,zPosition);
	Positions p2(21,0,zPosition);
	Positions p3(21,21,zPosition);
	Positions p4(0,0,zPosition+0.05); 

	Planes p(p1,p2,p3,p4);
	p.setRadiationLength(93.6);//Silicon (Si). 9.36 cm
	p.setPixelSize(1,0.045);
	p.setPixelSize(2,0.045);
	p.write2file=true;
	p.name_="Telescope";
	return p;
}

mcEUTEL::Planes mcEUTEL::makeTelescopePlane( const mcEUTEL::Planes& PreviousPlane )
{
	return makeTelescopePlane(PreviousPlane.getZEndPos());
}


mcEUTEL::Planes mcEUTEL::makeAirPlane( double zStart,double ZEnd )
{
	Positions p1(0.0,0.0,zStart);
	Positions p2(21,0,zStart);
	Positions p3(21,10,zStart);
	Positions p4(0,0,ZEnd); 

	Planes p(p1,p2,p3,p4);
	p.setPixelSize(1,0.045);
	p.setPixelSize(2,0.045);
	p.setRadiationLength(304000);//Air. 304 m
	p.write2file=false;
	p.name_="air";
	return p;
}

mcEUTEL::Planes mcEUTEL::makeAirPlane( const Planes& PreviousPlane,double ZEnd )
{
	return makeAirPlane(PreviousPlane.getZEndPos(),ZEnd);
}
