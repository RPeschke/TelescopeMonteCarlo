
#include <vector>
#include "geometric.h"
#include "TF1.h"
#include <iostream>
using namespace mcEUTEL;


#define SPEED_OF_LIGHT (299792458)
#define GeV (1e9)
#define keV (1e3)
#define MeV (1e6)
#define electronMass (511*keV)
#define PI 3.14


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
	
	return (NormVector.x*p.position_.x+NormVector.y*p.position_.y+NormVector.z*p.position_.z+D)/LengthOfNormVec;
}

void mcGeometric::hyperPlane::MeetsPlaneAt( const Particle& Par,vector3& pos )
{
	double t=-((NormVector*Par.position_)+D)/(NormVector*Par.directions_);

	pos.x=Par.position_.x+t*Par.directions_.x;//tan(Par.phi);
	pos.y=Par.position_.y+t*Par.directions_.y;//*tan(Par.theta);
	pos.z=Par.position_.z+t*Par.directions_.z;//*1;

}

void mcGeometric::hyperPlane::PropagateToPlane( Particle& Par )
{
	double t=-((NormVector*Par.position_)+D)/(NormVector*Par.directions_);
	Par.position_.x+=t*Par.directions_.x;
	Par.position_.y+=t*Par.directions_.y;
	Par.position_.z+=t*Par.directions_.z;
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

p.position_=p1;


// p.phi=tan(p1.x/p1.z);  // is wrong but the function will be removed in total
// p.theta=tan(p1.y/p1.z);


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

double mcGeometric::intersectionHelixPlaneApprox( const helix& h,const hyperPlane& plane)
{
	// o= a* lambda^2 + p* lambda +q

	double a=-h.radius_*(plane.NormVector*h.normal_)*h.angleVelocity_;
	double p=plane.NormVector*h.parallel_
		      +h.radius_*h.angleVelocity_*(h.tangential_*plane.NormVector);
	
	double q=h.startCenter_*plane.NormVector
		     +h.radius_*plane.NormVector*h.normal_
			 +plane.D;

	if (abs(a/p)<1e-3) // it seems to be numerically unstable to divide if a is much smaller than q. 
		             // and if a is small compared to q than it is almost a straight line.
				    // and since this is only the starting point for the newton approximation
				    //it is ok to make this approximation
	{
		a=0;
	}
	if (a==0)
	{
		double lamda1=-q/p;
		
		return lamda1;
	}else{
		p/=a;
		q/=a;
		double lamda1=-p/2+sqrt((p/2)*(p/2)-q);


		double lamda2=-p/2-sqrt((p/2)*(p/2)-q);

		if (lamda1>0&&(lamda1<lamda2||lamda2<0))
		{
			intersectionHelixPlaneApproxNowton(h,plane,lamda1);
			
			return lamda1;
		}else if (lamda2>0&&(lamda2<lamda1||lamda1<0))
		{
			intersectionHelixPlaneApproxNowton(h,plane,lamda2);
			
			return lamda2;
		}

		
		
	}

	
	return -1;


}

int mcGeometric::intersectionHelixPlaneApproxNowton( const helix& h,const hyperPlane& plane,double& lambda )
{
	// F(lambda) = A + B*lambda +C*cos(angleVel*lambda) +D*sin(angleVel*lambda);
	// F'(lambda)= B-C*angleVel*sin(angelVel*lambda) + D*cos(angleVel*Lambda);
	// lambda_{n+1} = lambda_{n}- F(lambda)/F'(lambda);
	lambda*=h.angleVelocity_;
	double A=plane.NormVector*h.startCenter_+plane.D;
	double B=plane.NormVector*h.parallel_/h.angleVelocity_;
	double C=h.radius_*(plane.NormVector*h.normal_);
	double D=h.radius_*(plane.NormVector*h.tangential_);
	double lambdaOld=-1,deltaLamda=1;
	int n=0;
	while(deltaLamda>1e-3&&++n<100)
	{
		lambdaOld=lambda;
		lambda-=(A+B*lambda+C*cos(lambda)+D*sin(lambda))
			    /(B-C*sin(lambda)+D*cos(lambda));
		deltaLamda=abs(lambda-lambdaOld);

	}
		lambda/=h.angleVelocity_;
	return 0;
}

int mcGeometric::intersectionHelixPlaneApproxNumeric( const helix& h,const hyperPlane& plane,mcEUTEL::vector3& p )
{
		// F(lambda) = A + B*lambda +C*cos(angleVel*lambda) +D*sin(angleVel*lambda);

	double A=plane.NormVector*h.startCenter_+plane.D;
	double B=plane.NormVector*h.parallel_;
	double C=h.radius_*(plane.NormVector*h.normal_);
	double D=h.radius_*(plane.NormVector*h.tangential_);


	double parameter[5];
	parameter[0]=A;
	parameter[1]=B;
	parameter[2]=C;
	parameter[3]=D;
	parameter[4]=h.angleVelocity_;
	 TF1 *fa = new TF1("fa","[0] + [1]/[4]*x +[2]*cos(x) +[3]*sin(x)");


	 fa->SetParameters(parameter);
	 fa->SetNpx(100);
	 auto y=fa->Eval(590.01226738478792190416749513406);
	 y=fa->Eval(0.78500000000000003);
	 auto y1=1;
	 double lambda;
	 int i=0;
	 while (y1>1e-4)
	 {
		  lambda=fa->GetX(0,i*PI/4,(i+1)*PI/4);
		  if (lambda<(i+1)*PI/4)
		  {
			   y1=fa->Eval(lambda);
		  }
		 ++i;
	 }
	 lambda/=h.angleVelocity_;
	 intersectionHelixPlaneApproxNowton(h,plane,lambda);
	 p=h.getPosition(lambda);
	
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

mcGeometric::helix::helix( const vector3& StartPoint,const vector3& direction,const vector3& Bfield, const double& energy)
{
	
	auto d=direction;
	d.normalize();
	d=d*SPEED_OF_LIGHT;
	 parallel_=(Bfield*d)*Bfield/Bfield.length();
	 normal_=crossProduct(d,Bfield);
	 normal_.normalize();
	 tangential_=crossProduct(normal_,Bfield);
	 tangential_.normalize();

	 angleVelocity_=Bfield.length()*SPEED_OF_LIGHT*SPEED_OF_LIGHT/electronMass;
	 
	 vector3 rad=energy*GeV/SPEED_OF_LIGHT*crossProduct(d,Bfield)/(Bfield.length()*Bfield.length()*d.length());
	 radius_=rad.length();
	 startCenter_=StartPoint-rad;
	 
	 
}

mcGeometric::helix::helix( const Particle& p, const mcEUTEL::vector3& Bfield )
{
	MakeNewHelix(p,Bfield);
}
mcGeometric::helix::helix(): angleVelocity_(0.0),radius_(0.0){}

void mcGeometric::helix::MakeNewHelix( const Particle& p, const mcEUTEL::vector3& Bfield )
{
	auto velocity=p.directions_;
	velocity.normalize();
	velocity=velocity*SPEED_OF_LIGHT;
	parallel_=(Bfield*velocity)*Bfield/(Bfield.length()*Bfield.length());
	normal_=crossProduct(velocity,Bfield)*p.Charge_;
	normal_.normalize();
	tangential_=crossProduct(normal_,Bfield);
	tangential_.normalize();
	if (tangential_.z<0)
	{
		tangential_=(-1)*tangential_;
	}
	angleVelocity_=Bfield.length()*SPEED_OF_LIGHT*SPEED_OF_LIGHT/(p.particleEnergy_*GeV);

	vector3 rad=p.particleEnergy_*GeV/SPEED_OF_LIGHT*crossProduct(velocity,Bfield)/(Bfield.length()*Bfield.length()*velocity.length()*p.Charge_);
	radius_=rad.length();
	startCenter_=p.position_-rad;
}

mcEUTEL::vector3 mcGeometric::helix::getPosition( double lambda ) const
{
	mcEUTEL::vector3 ret=startCenter_
						+parallel_*lambda
						+radius_*normal_*cos(angleVelocity_*lambda)
						+radius_*tangential_*sin(angleVelocity_*lambda);
	return ret;
}

mcEUTEL::vector3 mcGeometric::helix::getDirection( double lambda ) const
{
	mcEUTEL::vector3 ret=parallel_
						-radius_*normal_*sin(angleVelocity_*lambda)*angleVelocity_
						+radius_*tangential_*cos(angleVelocity_*lambda)*angleVelocity_;
	return ret;
}
