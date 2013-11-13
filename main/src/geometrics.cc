
#include <vector>
#include "geometric.h"

#include <iostream>
using namespace mcEUTEL;

mcGeometric::hyperPlane::hyperPlane( const vector3& pos1,const vector3& pos2,const vector3& pos3 )
{
	makeHyperPlane(pos1,pos2,pos3);
}

void mcGeometric::hyperPlane::makeHyperPlane( const vector3& pos1,const vector3& pos2, const vector3& pos3 )
{
// 	double a1=pos2.x-pos1.x;
// 	double a2=pos2.y-pos1.y;
// 	double a3=pos2.z-pos1.z;
// 
// 	double b1=pos3.x-pos1.x;
// 	double b2=pos3.y-pos1.y;
// 	double b3=pos3.z-pos1.z;

	vector3 p1=pos2-pos1;
	vector3 p2=pos3-pos1;
	NormVector=crossProduct(p1,p2);
	D=-(NormVector*pos1);

// 	A=a2*b3-a3*b2;
// 	B=a3*b1-a1*b3;
// 	C=a1*b2-a2*b1;
// 
// 	D=-(A*pos1.x+B*pos1.y+C*pos1.z);

	LengthOfNormVec=length(NormVector);
}

double mcGeometric::hyperPlane::normalDistanceToPLane( const Particle& p )
{
	
	return (NormVector.x*p.x+NormVector.y*p.y+NormVector.z*p.z+D)/LengthOfNormVec;
}

void mcGeometric::hyperPlane::MeetsPlaneAt( const Particle& Par,vector3& pos )
{
	double t=-((NormVector.x*Par.x+NormVector.y*Par.y+NormVector.z*Par.z+D)/(NormVector.x*tan(Par.phi)+NormVector.y*tan(Par.theta)+NormVector.z));

	pos.x=Par.x+t*tan(Par.phi);
	pos.y=Par.y+t*tan(Par.theta);
	pos.z=Par.z+t;

}

void mcGeometric::hyperPlane::PropagateToPlane( Particle& Par )
{
	double t=-((NormVector.x*Par.x+NormVector.y*Par.y+NormVector.z*Par.z+D)/(NormVector.x*tan(Par.phi)+NormVector.y*tan(Par.theta)+NormVector.z));

	Par.x=Par.x+t*tan(Par.phi);
	Par.y=Par.y+t*tan(Par.theta);
	Par.z=Par.z+t;
}

void mcGeometric::hyperPlane::ShiftPositionNormalToPlane( vector3& pos,double distnace )
{
	pos.x+=NormVector.x*distnace/LengthOfNormVec;
	pos.y+=NormVector.y*distnace/LengthOfNormVec;
	pos.z+=NormVector.z*distnace/LengthOfNormVec;
}


void mcGeometric::hyperPlane::fitHyperPlane( std::vector<vector3>& pos )
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

	if (pos.size()==3)
	{
		makeHyperPlane(pos.at(0),pos.at(1),pos.at(2));
		return;
	}

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

	NormVector.x=-ANumerator/denominator;
	NormVector.y=-BNumerator/denominator;
	NormVector.z=1;
	D=-DNumerator/denominator;
	LengthOfNormVec=length(NormVector);
}

int mcGeometric::intersectionPlanePlaneSphere( const hyperPlane& particlePlane, const hyperPlane& TelescopePlane, const Sphere& sphere ,Particle& p)
{
	


line intersectionLine;
mcEUTEL::vector3 p1;
if(intersectionPlanePlane(particlePlane,TelescopePlane,intersectionLine)) return -1;
if(intersectionLineSphere(intersectionLine,sphere,p1)) return -1;

mcEUTEL::vector3 r=p1-sphere.Center_;
mcEUTEL::vector3 direction;
direction=crossProduct(r,particlePlane.NormVector);

p.x=p1.x;
p.y=p1.y;
p.z=p1.z;

p.phi=tan(p1.x/p1.z);
p.theta=tan(p1.y/p1.z);


return 0;

}

int mcGeometric::intersectionPlanePlane( const hyperPlane& plane1, const hyperPlane& plane2, line& intersectionLine )
{
	intersectionLine.Direction_=crossProduct(plane1.NormVector,plane2.NormVector);


	if (length(intersectionLine.Direction_)==0)
	{
		return -1; // the planes are parallel
	}


	if (intersectionLine.Direction_.z!=0)
	{

		intersectionLine.StartPoint_.z=0;

		vector3 dummy(0,0,1);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.y=(plane1.NormVector.x*plane2.D-plane1.D*plane2.NormVector.x)/denominator;
		intersectionLine.StartPoint_.x=(plane1.NormVector.y*plane2.D-plane1.D*plane2.NormVector.y)/denominator;

	}else if (intersectionLine.Direction_.x!=0)
	{
		intersectionLine.StartPoint_.x=0;

		vector3 dummy(1,0,0);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.y=(plane1.NormVector.z*plane2.D-plane1.D*plane2.NormVector.z)/denominator;
		intersectionLine.StartPoint_.z=(plane1.D*plane2.NormVector.y-plane1.NormVector.y*plane2.D)/denominator;


	} else{

		intersectionLine.StartPoint_.y=0;

		vector3 dummy(0,1,0);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.x=(plane1.NormVector.z*plane2.D-plane1.D*plane2.NormVector.z)/denominator;
		intersectionLine.StartPoint_.z=(plane1.D*plane2.NormVector.x-plane1.NormVector.x*plane2.D)/denominator;
	}

	return 0;

}

int mcGeometric::intersectionLineSphere( const line& l,const Sphere& sphere,mcEUTEL::vector3& p )
{
	vector3 delta=sphere.Center_-l.StartPoint_;
	double directionSquare=(l.Direction_*l.Direction_);
	double sq= ((l.Direction_*delta)/directionSquare)*(l.Direction_*delta)/directionSquare  
		       -((delta*delta)/directionSquare  - (sphere.Radius_*sphere.Radius_)/directionSquare)    ;
	if (sq<0)
	{
		return -1;
	}
	double lamda1=(l.Direction_*delta)/directionSquare+sqrt(sq);
	double lamda2=(l.Direction_*delta)/directionSquare-sqrt(sq);

	vector3 pos1=l.getPosition(lamda1);
	vector3 pos2=l.getPosition(lamda2);

    double distance1=(pos1-p)*(pos1-p);
	double distance2=(pos2-p)*(pos2-p);

	if (distance1<distance2)
	{
		p.x=pos1.x;
		p.y=pos1.y;
		p.z=pos1.z;
	}
	else
	{
		p.x=pos2.x;
		p.y=pos2.y;
		p.z=pos2.z;
	}

		return 0;
}

mcEUTEL::vector3 mcGeometric::line::getPosition( double lambda ) const
{
	vector3 ret;
	ret.x=Direction_.x*lambda+StartPoint_.x;
	ret.y=Direction_.y*lambda+StartPoint_.y;
	ret.z=Direction_.z*lambda+StartPoint_.z;
	return ret;
}

void mcGeometric::Sphere::newSphere( double radius,const mcEUTEL::vector3& center )
{
	Radius_=radius;
	Center_=center;

}
