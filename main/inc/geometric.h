#ifndef geometric_h__
#define geometric_h__

#include <vector>
#include "Positions_mc.h"
#include "particle_mc.h"


class TF1;
namespace mcGeometric{

struct hyperPlane{
	double D,LengthOfNormVec; // A*x + B*y + C*z +D =0
	mcEUTEL::vector3 NormVector;
	double normalDistanceToPLane(const Particle& p);
	void MeetsPlaneAt(const Particle& Par,mcEUTEL::vector3& pos);
	void PropagateToPlane(Particle& par);
	void ShiftPositionNormalToPlane(mcEUTEL::vector3& pos,double distnace);
	void makeHyperPlane(const mcEUTEL::vector3& pos1,const mcEUTEL::vector3& pos2, const mcEUTEL::vector3& pos3);
	void fitHyperPlane(std::vector<mcEUTEL::vector3>& pos);
	hyperPlane(const mcEUTEL::vector3& pos1,const mcEUTEL::vector3& pos2,const mcEUTEL::vector3& pos3);
	hyperPlane():D(0),LengthOfNormVec(0){}
};

class Sphere{
public:
	mcEUTEL::vector3 Center_;
	double Radius_;

	Sphere(double radius,const mcEUTEL::vector3& center){Center_=center;Radius_=radius;};
	Sphere(double radius){Radius_=radius;}

	void newSphere(double radius,const mcEUTEL::vector3& center);
};

class line{
public:
	mcEUTEL::vector3 Direction_,StartPoint_;
	
	mcEUTEL::vector3 getPosition(double lambda) const;
	line(const mcEUTEL::vector3& p1,const mcEUTEL::vector3& p2); // two points on the line;
	line(){};

};

class helix{
public:
	mcEUTEL::vector3 startCenter_,
						parallel_,// is parallel to the magnetic field
						tangential_,
						normal_;
	double angleVelocity_,radius_;
	mcEUTEL::vector3 getPosition(double lambda) const;
	mcEUTEL::vector3 getDirection(double lambda) const;
	helix(const mcEUTEL::vector3& StartPoint,const mcEUTEL::vector3& direction,const mcEUTEL::vector3& Bfiel, const double& energy);
	helix(const Particle& p, const mcEUTEL::vector3& Bfield);
	helix();
	void MakeNewHelix(const Particle& p, const mcEUTEL::vector3& Bfield);


};


int intersectionHelixPlaneApproxNumeric(const helix& h,const hyperPlane& plane,mcEUTEL::vector3& p);
double intersectionHelixPlaneApprox( const helix& h,const hyperPlane& plane);
int intersectionHelixPlaneApproxNowton(const helix& h,const hyperPlane& plane,double& lambda);

}


#endif // geometric_h__
