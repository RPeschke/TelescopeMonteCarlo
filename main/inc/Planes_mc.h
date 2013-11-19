#ifndef Planes_h__
#define Planes_h__
#include "rapidxml.hpp"
#include <string>
#include <vector>
#include "Positions_mc.h"
#include "geometric.h"



/*! this Class should contain all informations about DUT and Telescopes
	The Telescopes and the DUT will just be planes with different values 


	it will be created with 4 position as inputs and radiation length. 
	it is designed that in the end it will read a gear file from the computer 
	but for now the information needs to be hard coded

*/

class Particle;

namespace mcEUTEL{
	
	// 	struct particle
// 	{
// 		double x,y,z,phi,theta,energy;
// 	};

	void Disp(const Particle& p);
	class Planes{
	public:

		Planes(rapidxml::xml_node<> *node);
		Planes():hit_x(0),hit_y(0),name_(""){}  
// 		Planes(const Positions &p1,const Positions &p2,const Positions &p3,const Positions &p4):hit_x(0),hit_y(0),name_(""){
// 			p1_=p1;
// 			p2_=p2;
// 			p3_=p3;
// 			p4_=p4;
// 			thickness=p4_.z-p1_.z;
// 		};
		int ProcessXMLNode(rapidxml::xml_node<> *node);

		int fitPlaneBetweenOtherPlanes(const Planes& beforePlane,const Planes& afterPlane);

		void setRadiationLength(double radiationLength){
			radiationLength_=radiationLength;
		}
		bool setPosition(const int coordinate,const vector3 &p){};
			/**
			* @param orientation where 1 means x direction and 2 means y direction.
			* @param PixelSize is the pixel size of the device in this direction 
			* @return returns 0 if ok 
			*/
			bool setPixelSize(const int oriantaion,const double PixelSize); //!< Sets the pixel size it will return 0 if it was ok 1 

			/**
			* describes the Propagation of a particle through the material. 
			* for now it will kink the angle at the beginning (at the current particle position) and then 
			* Propagate in the new direction as a straight line.  
			* @param p gives the information about the particle. This function take the 
			* input by ref and changes the value of the input. 
			* @return void 
			*/
			void propagate(Particle& p);
			/**
			* checks where on the Sensor the hit was. records original position an Sensor response.  
			* @param p gives the information about the particle.  
			* @return void 
			*/
			void getHit(const Particle& p);;
			
			double getZEndPos() const{return -(FrontPlane.D/FrontPlane.NormVector.z);};// positionat x=0 y=0
			std::string name_;
			bool write2file;
			int hit_x,hit_y;
			vector3 BField;
			bool inBFiel;
			mcGeometric::helix trajectory_;
	private:


		mcGeometric::hyperPlane FrontPlane, BackPlane;


		struct BoundaryLine{
			double A,B,C;// A*x +B*y+C=0
		    	
			bool isInsideBoundary(const vector3& pos);
			bool isInsideBoundary(const Particle& par);
			double normalDistanceToLine(const Particle& p);// z coordinate gets ignored
			BoundaryLine(const vector3 &p1,const vector3&p2);
			BoundaryLine():A(0),B(0),C(0){}
		};
		
		//Positions p1_,p2_,p3_,p4_;
		std::vector<vector3> positions_;
		std::vector<BoundaryLine> Boundary_;
		BoundaryLine yZeroLine,xZeroLine;
		bool Vec_isInsideBoundaries(const Particle& par);
		double radiationLength_,thickness;
		double pixelSizeX_,pixelSizeY_; /*!< the size of the Pixel in m*/
		
	};

	Planes makeTelescopePlane(double zPosition);
	Planes makeTelescopePlane(const Planes& PreviousPlane);
	Planes makeAirPlane(double zStart,double ZEnd);
	Planes makeAirPlane(const Planes& PreviousPlane,double ZEnd);	
}
#endif // Planes_h__
