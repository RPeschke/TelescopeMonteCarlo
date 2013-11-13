#ifndef geometric_h__
#define geometric_h__

#include <vector>
#include "Positions_mc.h"
#include "particle_mc.h"

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

int intersectionPlanePlane(const hyperPlane& plane1, const hyperPlane& plane2, line& intersectionLine);

int intersectionPlanePlaneSphere(const hyperPlane& particlePlane, const hyperPlane& TelescopePlane, const Sphere& sphere, Particle& p);

int intersectionLineSphere(const line& l,const Sphere& sphere,mcEUTEL::vector3& p);
}

#endif // geometric_h__
