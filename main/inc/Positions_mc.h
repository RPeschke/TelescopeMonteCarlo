#ifndef Positions_mc_h__
#define Positions_mc_h__
#include <cmath>
namespace mcEUTEL{

class Positions
{
public:
	double x,y,z;
	Positions():x(0),y(0),z(0){}
	Positions(double X,double Y,double Z):x(X),y(Y),z(Z){}
	void rotateX(double Angle){
		double dummyY=y*cos(Angle)-z*sin(Angle);
		z=y*sin(Angle)+z*cos(Angle);
		y=dummyY;
	}
	void rotateY(double angle){
		double dummyX=x*cos(angle)+z*sin(angle);
		z=-x*sin(angle)+z*cos(angle);
		x=dummyX;
	}

	void rotateZ(double angle){
		double dummyX=x*cos(angle)-y*sin(angle);
		y=x*sin(angle)+y*cos(angle);

		x=dummyX;
	}
};
}
#endif // Positions_mc_h__
