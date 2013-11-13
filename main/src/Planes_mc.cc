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
	BackPlane.PropagateToPlane(p);
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
	Positions py2; // the vector from PixelAxis.at(0) to py2 is orthogonal (in 2 dimensions) 
				   // to the vector PixelAxis.at(0)  ---> PixelAxis.at(1)
	py2.x=PixelAxis.at(1).y-PixelAxis.at(0).y+PixelAxis.at(0).x;
	py2.y=PixelAxis.at(0).x-PixelAxis.at(1).x+PixelAxis.at(1).y;
	py2.z=PixelAxis.at(0).z;
	xZeroLine=BoundaryLine(PixelAxis.at(0),py2);


	//////////////////////////////////////////////////////////////////////////
	// Extracts the Positions for the Boundary lines
	auto BoundaryPositions=XMLExtractPositions(node->first_node("ActiveArea")->first_node("position"));
	BoundaryPositions.push_back(Positions(BoundaryPositions.at(0))); // close the circle 
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
	double cosAlpha=(xZeroLine.A*FrontPlane.A+xZeroLine.B*FrontPlane.B)/sqrt(FrontPlane.A*FrontPlane.A + FrontPlane.B*FrontPlane.B+FrontPlane.C*FrontPlane.C);
	double cosBeta=(yZeroLine.A*FrontPlane.A+yZeroLine.B*FrontPlane.B)/sqrt(FrontPlane.A*FrontPlane.A + FrontPlane.B*FrontPlane.B+FrontPlane.C*FrontPlane.C);
	pixelSizeX_*=sqrt(1-cosAlpha*cosAlpha);
	pixelSizeY_*=sqrt(1-cosBeta*cosBeta);




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

mcEUTEL::Planes::BoundaryLine::BoundaryLine( const Positions &p1,const Positions&p2 )
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

bool mcEUTEL::Planes::BoundaryLine::isInsideBoundary( const Positions& pos )
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
return B*par.y+A*par.x +C<=0;
}

double mcEUTEL::Planes::BoundaryLine::normalDistanceToLine( const Particle& p )
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

void mcEUTEL::Planes::hyperPlane::MeetsPlaneAt( const Particle& Par,Positions& pos )
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


void mcEUTEL::Planes::hyperPlane::fitHyperPlane( std::vector<Positions>& pos )
{
	// making this fit with least square estimator is in principle just solving a linear equation system:
	// chi^2= sum (z_i - A*x_i -B*y_i-D)^2
	// I: d chi^2/dA= sum (x_i* (z_i - A*x_i -B*y_i-D))=0
	// II: d chi^2/dB= sum (y_i* (z_i - A*x_i -B*y_i-D))=0
	// III: d chi^2/dD= sum (z_i - A*x_i -B*y_i-D)=0
	// rewritten to 
	// I: 0=sumXZ - A*sumXX - B*sumXY -D*sumX
	// II: 0=sumYZ - A*sumXY - B * sumYY -D*sumY
	// III: 0=sumZ- A*sumX - B* sumY - D*N
	// with N number of entries

	
	double sumXZ=0,sumXX=0,sumXY=0,sumX=0,sumYZ=0,sumYY=0,sumY=0,sumZ=0,N=pos.size();


	for (auto p:pos)
	{
		sumXZ+=p.x*p.z;
		sumXY+=p.x*p.y;
		sumXX+=p.x*p.x;
		sumX+=p.x;
		sumYZ+=p.y*p.z;
		sumYY+=p.y*p.y;
		sumY+=p.y;
		sumZ+=p.z;

	}

	auto det =[](double a11,double a12,double a13, 
		         double a21,double a22,double a23,
				 double a31,double a32,double a33) -> double 
	{return (a11*a22*a33+ a13*a21*a32+a12*a23*a31)-(a31*a22*a13+a21*a12*a33+a11*a23*a32);};

	double denominator=det(sumXX,sumXY,sumX,
		                   sumXY,sumYY,sumY,
						   sumX,sumY,N);

	if (denominator==0)
	{
		// don't divide by zero
		std::cout<<"could note make hyperplane \n";
		return;

	}
	double ANumerator =det(sumXZ,sumXY,sumX,
		                   sumYZ,sumYY,sumY,
						   sumZ,sumY  ,N     );

	double BNumerator =det(sumXX,sumXZ,sumX,
		                   sumXY,sumYZ,sumY,
						   sumX,sumZ  ,N     );


	double DNumerator =det(sumXX,sumXY,sumXZ,
		                   sumXY,sumYY,sumYZ,
						   sumX,sumY  ,sumZ    );

	A=-ANumerator/denominator;
	B=-BNumerator/denominator;
	C=1;
	D=-DNumerator/denominator;
	LengthOfNormVec=sqrt(A*A +B*B+C*C);
}
