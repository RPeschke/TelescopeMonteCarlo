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
	//straigthForward(p,thickness);
	BackPlane.PropagateToPlane(p);
}

void mcEUTEL::Planes::getHit( const Particle& p )
{
	if (Vec_isInsideBoundaries(p))
	{

		hit_y=static_cast<int>(yZeroLine.normalDistanceToLine(p)/pixelSizeY_);
		hit_x=static_cast<int>(xZeroLine.normalDistanceToLine(p)/pixelSizeX_);

		//hit_x=static_cast<int>((p.x-p1_.x)/pixelSizeX_);
		//hit_y=static_cast<int>((p.y-p1_.y)/pixelSizeY_);
	}else				{
		hit_x=0;
		hit_x=0;
	}
}

int mcEUTEL::Planes::ProcessXMLNode( rapidxml::xml_node<> *node )
{
	if(strcmp(node->name(),"plane")) return -1; // to make sure that the correct node is used
	
	name_=node->first_attribute("Type")->value();
	//std::cout<<name_<<std::endl;
	radiationLength_=std::atof(node->first_node("radiationLength")->first_attribute("value")->value());
	pixelSizeX_=std::atof(node->first_node("PixelSize")->first_attribute("x")->value());

	pixelSizeY_=std::atof(node->first_node("PixelSize")->first_attribute("y")->value());
	thickness=std::atof(node->first_node("Thickness")->first_attribute("value")->value());

	auto PixelPos1= node->first_node("pixel")->first_node("position");
	auto PixelPos2= node->first_node("pixel")->first_node("position")->next_sibling();

	Positions px1,px2,py2;
	getPosition(PixelPos1,px1);
	getPosition(PixelPos2,px2);
	yZeroLine=BondaryLine(px1,px2);
	py2.x=px2.y-px1.y+px1.x;
	py2.y=px1.x-px2.x+px1.y;
	py2.z=px1.z;
	xZeroLine=BondaryLine(px1,py2);


	//auto position= node->first_node("positions");
	
	int posNr=0;
	Positions newP,oldP,firstP;
	auto position= node->first_node("ActiveArea")->first_node("position");
	getPosition(position,oldP);
	firstP=oldP;
	int j=0;
	for (position=position->next_sibling();position;position=position->next_sibling())
	{
		
		j++;
			getPosition(position,newP);
			Bondary_.push_back(BondaryLine(oldP,newP));
			if (j==2)
			{
				Positions p1=firstP,p2=oldP,p3=newP;
				FrontPlane.makeHyperPlane(p1,p2,p3);
				FrontPlane.ShiftPositionNormalToPlane(p1,thickness);
				FrontPlane.ShiftPositionNormalToPlane(p2,thickness);
				FrontPlane.ShiftPositionNormalToPlane(p3,thickness);
				
				BackPlane.makeHyperPlane(p1,p2,p3);

			}
			oldP=newP;

	}
			Bondary_.push_back(BondaryLine(newP,firstP));


	return 1;
}

int mcEUTEL::Planes::fitPlaneBetweenOtherPlanes( const Planes& beforePlane,const Planes& afterPlane )
{
FrontPlane=beforePlane.BackPlane;
BackPlane=afterPlane.FrontPlane;

	thickness=FrontPlane.D/FrontPlane.C-BackPlane.D/BackPlane.C;// just a rough estimation.

	return 1;
}

bool mcEUTEL::Planes::Vec_isInsideBoundaries( const Particle& par )
{
	
	for (auto e:Bondary_)
	{
	
		if (!e.isInsideBoundary(par))
		{
			return false;
		}
	}
	return true;
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
	
	Planes p; //dummy telescope plane without position information 
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
	
	Planes p; // dummy plane without position information needs to be implemented afterwards
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




	A=(p2.y-p1.y);
	B=(p1.x-p2.x);
	assert(B*p1.y+A*p1.x==B*p2.y+A*p2.x);
	auto n=sqrt(A*A+B*B);
	B/=n;
	A/=n;
	//A*y + B*x+C=0
	// C= -A*y-B*x


	C=-B*p1.y-A*p1.x;

	

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

	return B*pos.y+A*pos.x +C<=0;

	

}

bool mcEUTEL::Planes::BondaryLine::isInsideBoundary( const Particle& par )
{
return B*par.y+A*par.x +C<=0;
}

double mcEUTEL::Planes::BondaryLine::normalDistanceToLine( const Particle& p )
{
	return -(A*p.x+B*p.y+C);
}


mcEUTEL::Planes::hyperPlane::hyperPlane( const Positions& pos1,const Positions& pos2,const Positions& pos3 )
{
	makeHyperPlane(pos1,pos2,pos3);
}

void mcEUTEL::Planes::hyperPlane::makeHyperPlane( const Positions& pos1,const Positions& pos2, const Positions& pos3 )
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

	LengthOfNormVec=sqrt(A*A +B*B+C*C);
}

double mcEUTEL::Planes::hyperPlane::normalDistanceToPLane( const Particle& p )
{
	return (A*p.x+B*p.y+C*p.z+D)/LengthOfNormVec;
}

void mcEUTEL::Planes::hyperPlane::MetsPlaneAt( const Particle& Par,Positions& pos )
{
	double t=-((A*Par.x+B*Par.y+C*Par.z+D)/(A*tan(Par.phi)+B*tan(Par.theta)+C));

	pos.x=Par.x+t*tan(Par.phi);
	pos.y=Par.y+t*tan(Par.theta);
	pos.z=Par.z+t;

}

void mcEUTEL::Planes::hyperPlane::PropagateToPlane( Particle& Par )
{
	double t=-((A*Par.x+B*Par.y+C*Par.z+D)/(A*tan(Par.phi)+B*tan(Par.theta)+C));

	Par.x=Par.x+t*tan(Par.phi);
	Par.y=Par.y+t*tan(Par.theta);
	Par.z=Par.z+t;
}

void mcEUTEL::Planes::hyperPlane::ShiftPositionNormalToPlane( Positions& pos,double distnace )
{
	pos.x+=A*distnace/LengthOfNormVec;
	pos.y+=B*distnace/LengthOfNormVec;
	pos.z+=C*distnace/LengthOfNormVec;
}
