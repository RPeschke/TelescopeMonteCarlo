#include "Planes_mc.h"
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include "particle_mc.h"
#include "randomGen.h"
#include "rapidxml.hpp"
#include "Positions_mc.h"
#include "XMLHelpers.h"
#include "geometric.h"


void higlandFormular(Particle& p,double relRadiationLength );
void straigthForward(Particle& p,double distance );
int getPosition(rapidxml::xml_node<> *node,mcEUTEL::vector3 & p);


mcEUTEL::Planes::Planes(rapidxml::xml_node<> *node):hit_x(0),hit_y(0),write2file(true),inBFiel(false){
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
	if (inBFiel)
	{
		//intersectionHelixPlaneApprox(p.helix_,*this,p.position_)
		trajectory_.MakeNewHelix(p,BField);
		double lambda=0;
		intersectionHelixPlaneApproxNowton(trajectory_,BackPlane,lambda);
		p.position_=trajectory_.getPosition(lambda);
		p.directions_=trajectory_.getDirection(lambda);

	}else {
		BackPlane.PropagateToPlane(p);
	}
}

void mcEUTEL::Planes::getHit( const Particle& p )
{
	if (Vec_isInsideBoundaries(p))
	{

		hit_y=static_cast<int>(yZeroLine.normalDistanceToLine(p)/pixelSizeY_+1);
		hit_x=static_cast<int>(xZeroLine.normalDistanceToLine(p)/pixelSizeX_+1);


	}else				{
		hit_x=0;
		hit_x=0;
	}
}




int mcEUTEL::Planes::ProcessXMLNode( rapidxml::xml_node<> *node )
{
	if(strcmp(node->name(),"plane")) return -1; // to make sure that the correct node is used
	

	name_=node->first_attribute("Type")->value();
	radiationLength_=std::atof(node->first_node("radiationLength")->first_attribute("value")->value());
	thickness=std::atof(node->first_node("Thickness")->first_attribute("value")->value());

	pixelSizeX_=std::atof(node->first_node("PixelSize")->first_attribute("x")->value());
	pixelSizeY_=std::atof(node->first_node("PixelSize")->first_attribute("y")->value());
	

	//////////////////////////////////////////////////////////////////////////
	// extracts the information where to put the x,y Axis for the pixel
	auto PixelAxis=XMLExtractPositions(node->first_node("pixel")->first_node("position"));// x-Axis of the Pixel coordinate system
	XMLModifyPositions(PixelAxis,node->first_node("modifyPlane"));
//	std::cout<<node->first_node("modifyActiveArea")->value()<<std::endl;

	yZeroLine=BoundaryLine(PixelAxis.at(0),PixelAxis.at(1)); //y is zero at the x-Axis sorry for the notation 

	// create a vector that is orthogonal on the vector PixelAxis.at(0)  ---> PixelAxis.at(1)
	vector3 py2; // the vector from PixelAxis.at(0) to py2 is orthogonal (in 2 dimensions) 
				   // to the vector PixelAxis.at(0)  ---> PixelAxis.at(1)
	py2.x=PixelAxis.at(1).y-PixelAxis.at(0).y+PixelAxis.at(0).x;
	py2.y=PixelAxis.at(0).x-PixelAxis.at(1).x+PixelAxis.at(1).y;
	py2.z=PixelAxis.at(0).z;
	xZeroLine=BoundaryLine(PixelAxis.at(0),py2);


	//////////////////////////////////////////////////////////////////////////
	// Extracts the Positions for the Boundary lines
	auto BoundaryPositions=XMLExtractPositions(node->first_node("ActiveArea")->first_node("position"));
	BoundaryPositions.push_back(vector3(BoundaryPositions.at(0))); // close the circle 
	XMLModifyPositions(BoundaryPositions,node->first_node("modifyPlane"));
	for (int i=1;i<BoundaryPositions.size();++i)
	{
		Boundary_.push_back(BoundaryLine(BoundaryPositions.at(i-1),BoundaryPositions.at(i)));
	}


	//////////////////////////////////////////////////////////////////////////
	// fits Position to a plane
	FrontPlane.fitHyperPlane(BoundaryPositions);

	for (auto&  p:BoundaryPositions)
	{
		FrontPlane.ShiftPositionNormalToPlane(p,thickness);
	}


	BackPlane.fitHyperPlane(BoundaryPositions);
	
	//////////////////////////////////////////////////////////////////////////
	// shrink pixel size for the projection
	// since the hyper plane can be tilted against the x,y plane the "Projected Pixel size"  shrinks
	// 
	double cosAlpha=(xZeroLine.A*FrontPlane.NormVector.x+xZeroLine.B*FrontPlane.NormVector.y)/length(FrontPlane.NormVector);
	double cosBeta=(yZeroLine.A*FrontPlane.NormVector.x+yZeroLine.B*FrontPlane.NormVector.y)/length(FrontPlane.NormVector);
	pixelSizeX_*=sqrt(1-cosAlpha*cosAlpha);
	pixelSizeY_*=sqrt(1-cosBeta*cosBeta);




	return 1;
}

int mcEUTEL::Planes::fitPlaneBetweenOtherPlanes( const Planes& beforePlane,const Planes& afterPlane )
{
FrontPlane=beforePlane.BackPlane;
BackPlane=afterPlane.FrontPlane;

	thickness=FrontPlane.D/FrontPlane.NormVector.z-BackPlane.D/BackPlane.NormVector.z;// just a rough estimation.

	return 1;
}

bool mcEUTEL::Planes::Vec_isInsideBoundaries( const Particle& par )
{
	
	for (auto e:Boundary_)
	{
	
		if (!e.isInsideBoundary(par))
		{
			return false;
		}
	}
	return true;
}



void higlandFormular(Particle& p,double relRadiationLength ){


	double sigmaHl=0.0136/p.particleEnergy_*sqrt(relRadiationLength)*(1+0.038*log(relRadiationLength));
	if (sigmaHl>0)
	{
		p.directions_.x=p.directions_.z*atan(tan(p.directions_.x/p.directions_.z)+getNormRandom()*sigmaHl);
		p.directions_.y=p.directions_.z*atan(tan(p.directions_.y/p.directions_.z)+getNormRandom()*sigmaHl);
			
			
			//p.phi+=getNormRandom()*sigmaHl;
		//p.theta+=getNormRandom()*sigmaHl;

	}
	
	
}

void straigthForward( Particle& p,double distance )
{
	distance=distance/p.directions_.z;
	p.position_.x+=distance*p.directions_.x;
	p.position_.y+=distance*p.directions_.y;
	p.position_.z+=distance*p.directions_.z;
}


void mcEUTEL::Disp( const Particle& p )
{
	std::cout<<"p.x: "<<p.position_.x <<"  p.y: "<<p.position_.y<< " p.z: "<< p.position_.z<<std::endl;

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
	p.setRadiationLength(304);//Air. 304 m
	p.write2file=false;
	p.name_="air";
	return p;
}

mcEUTEL::Planes mcEUTEL::makeAirPlane( const Planes& PreviousPlane,double ZEnd ){
	return makeAirPlane(PreviousPlane.getZEndPos(),ZEnd);
}

mcEUTEL::Planes::BoundaryLine::BoundaryLine( const vector3 &p1,const vector3&p2 )
{




	A=(p2.y-p1.y);
	B=(p1.x-p2.x);
	//assert(B*p1.y+A*p1.x-(B*p2.y+A*p2.x)<0.01); // true besides some rounding issues
	auto n=sqrt(A*A+B*B);
	B/=n;
	A/=n;
	//A*y + B*x+C=0
	// C= -A*y-B*x


	C=-B*p1.y-A*p1.x;

	

}

bool mcEUTEL::Planes::BoundaryLine::isInsideBoundary( const vector3& pos )
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

bool mcEUTEL::Planes::BoundaryLine::isInsideBoundary( const Particle& par )
{
return B*par.position_.y+A*par.position_.x +C<=0;
}

double mcEUTEL::Planes::BoundaryLine::normalDistanceToLine( const Particle& p )
{
	return -(A*p.position_.x+B*p.position_.y+C);
}


