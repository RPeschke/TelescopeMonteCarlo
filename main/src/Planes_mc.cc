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
	if (Vec_isInsideBoundaries(p))
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
	
	int posNr=0;
	Positions newP,oldP,firstP;
	auto position= node->first_node("ActiveArea")->first_node("position");
	getPosition(position,oldP);
	firstP=oldP;
	for (position=position->next_sibling();position;position=position->next_sibling())
	{
		
		
			getPosition(position,newP);
			Bondary_.push_back(BondaryLine(oldP,newP));
			oldP=newP;

	}
			Bondary_.push_back(BondaryLine(newP,firstP));

	name_=node->first_attribute("Type")->value();
	//std::cout<<name_<<std::endl;
	radiationLength_=std::atof(node->first_node("radiationLength")->first_attribute("value")->value());
	pixelSizeX_=std::atof(node->first_node("PixelSize")->first_attribute("x")->value());

	pixelSizeY_=std::atof(node->first_node("PixelSize")->first_attribute("y")->value());
	thickness=std::atof(node->first_node("Thickness")->first_attribute("value")->value());
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

bool mcEUTEL::Planes::Vec_isInsideBoundaries( const Particle& par )
{
	bool returnValue=true;
	for (auto e:Bondary_)
	{
		returnValue*=e.isInsideBoundary(par);
		if (!returnValue)
		{
			break;
		}
	}
	return returnValue;
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

mcEUTEL::Planes mcEUTEL::makeAirPlane( const Planes& PreviousPlane,double ZEnd ){
	return makeAirPlane(PreviousPlane.getZEndPos(),ZEnd);
}

mcEUTEL::Planes::BondaryLine::BondaryLine( const Positions &p1,const Positions&p2 )
{




	B=(p2.y-p1.y);
	A=(p1.x-p2.x);
	//A*y + B*x+C=0
	// C= -A*y-B*x


	C=-A*p1.y-B*p1.x;

	assert(A*p1.y+B*p1.x==A*p2.y+B*p2.x);

}

bool mcEUTEL::Planes::BondaryLine::isInsideBoundary( const Positions& pos )
{
/*  defines if A point is "left" or "right" from this line. The orientation of 
	the line is defined from pos1 to pos2. the line is continued till +- inf.
	An Example



	   pos2       *
	             /
	            /
   "inside"    /
	          /    "Outside"
	         /
            /
    pos1   *          


	if pos1 and two would switch then inside and outside would switch too.
	if the point is on the line it counts as "inside".

*/

	return A*pos.y+B*pos.x +C<=0;

	

}

bool mcEUTEL::Planes::BondaryLine::isInsideBoundary( const Particle& par )
{
return A*par.y+B*par.x +C<=0;
}


mcEUTEL::Planes::hyperPlane::hyperPlane( const Positions& pos1,const Positions& pos2,const Positions& pos3 )
{
	double a1=pos2.x-pos1.x;
	double a2=pos2.y-pos1.y;
	double a3=pos2.z-pos1.z;

	double b1=pos3.x-pos1.x;
	double b2=pos3.y-pos1.y;
	double b3=pos3.z-pos1.z;


	A=a2*b3-a3*b2;
	B=a3*b1-a1*b3;
	C=a1*b2-a2*b1;

	D=-(A*pos1.x+B*pos1.y+C*pos1.z);

}
