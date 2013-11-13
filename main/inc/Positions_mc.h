#ifndef Positions_mc_h__
#define Positions_mc_h__
#include <cmath>
namespace mcEUTEL{

class vector3
{
public:
	double x,y,z;
	vector3():x(0),y(0),z(0){}
	vector3(double X,double Y,double Z):x(X),y(Y),z(Z){}
inline 	void rotateX(double Angle){
		double dummyY=y*cos(Angle)-z*sin(Angle);
		z=y*sin(Angle)+z*cos(Angle);
		y=dummyY;
	}
	inline void rotateY(double angle){
		double dummyX=x*cos(angle)+z*sin(angle);
		z=-x*sin(angle)+z*cos(angle);
		x=dummyX;
	}

	inline void rotateZ(double angle){
		double dummyX=x*cos(angle)-y*sin(angle);
		y=x*sin(angle)+y*cos(angle);

		x=dummyX;
	}

	inline vector3 operator-() const{
		vector3 ret;
		ret.x=-x;
		ret.y=-y;
		ret.z=-z;
		return ret;
	}
};
inline double operator*(const vector3 &A,const vector3& B){
	return A.x*B.x+A.y*B.y+A.z*B.z;

}

inline double length(const vector3& p){
return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
}


inline vector3 operator-(const vector3 &A,const vector3& B){
	vector3 ret;
	ret.x=A.x-B.x;
	ret.y=A.y-B.y;
	ret.z=A.z-B.z;

	return ret;


}

inline vector3 operator+(const vector3 &A,const vector3& B){
	vector3 ret;
	ret.x=A.x+B.x;
	ret.y=A.y+B.y;
	ret.z=A.z+B.z;

	return ret;


}

inline void crossProduct(const vector3 &A,const vector3& B, vector3& ret){

	ret.x=A.y*B.z-A.z*B.y;
	ret.y=A.z*B.x-A.x*B.z;
	ret.z=A.x*B.y-A.y*B.x;



}

inline vector3 crossProduct(const vector3 &A,const vector3& B){
	vector3 ret;
	

	crossProduct(A,B,ret);
	return ret;

}

inline double det(const vector3 &A,const vector3& B,const vector3& C){

	return A.x*B.y*C.z
		  +A.y*B.z*C.x
		  +A.z*B.x*C.y
		  -A.x*B.z*C.y
		  -A.y*B.x*C.z
		  -A.z*B.y*C.z;
}

}
#endif // Positions_mc_h__
