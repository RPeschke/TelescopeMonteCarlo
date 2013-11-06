#ifndef Planes_h__
#define Planes_h__
#include "rapidxml.hpp"
#include <string>
#include <vector>


/*! this Class should contain all informations aboud DUT and Telescops
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
	struct Positions
	{
		double x,y,z;
		Positions():x(0),y(0),z(0){}
		Positions(double X,double Y,double Z):x(X),y(Y),z(Z){}
	};

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
		bool setPosition(const int coordinate,const Positions &p){};
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
			
			double getZEndPos() const{return -(FrontPlane.D/FrontPlane.C);};// positionat x=0 y=0
			std::string name_;
			bool write2file;
			int hit_x,hit_y;

	private:


		struct hyperPlane{
			double A,B,C,D,LengthOfNormVec; // A*x + B*y + C*z +D =0
		 double normalDistanceToPLane(const Particle& p);
		 void MetsPlaneAt(const Particle& Par,Positions& pos);
		 void PropagateToPlane(Particle& par);
		 void ShiftPositionNormalToPlane(Positions& pos,double distnace);
		 void makeHyperPlane(const Positions& pos1,const Positions& pos2, const Positions& pos3);
		 hyperPlane(const Positions& pos1,const Positions& pos2,const Positions& pos3);
		 hyperPlane():A(0),B(0),C(0),D(0),LengthOfNormVec(0){}
		}FrontPlane, BackPlane;


		struct BondaryLine{
			double A,B,C;// A*x +B*y+C=0
		    	
			bool isInsideBoundary(const Positions& pos);
			bool isInsideBoundary(const Particle& par);
			double normalDistanceToLine(const Particle& p);// z coordinate gets ignored
			BondaryLine(const Positions &p1,const Positions&p2);
			BondaryLine():A(0),B(0),C(0){}
		};
		
		//Positions p1_,p2_,p3_,p4_;
		std::vector<Positions> positions_;
		std::vector<BondaryLine> Bondary_;
		BondaryLine yZeroLine,xZeroLine;
		bool Vec_isInsideBoundaries(const Particle& par);
		double radiationLength_,thickness;
		double pixelSizeX_,pixelSizeY_; /*!< the size of the Pixel in mm*/
		
	};

	Planes makeTelescopePlane(double zPosition);
	Planes makeTelescopePlane(const Planes& PreviousPlane);
	Planes makeAirPlane(double zStart,double ZEnd);
	Planes makeAirPlane(const Planes& PreviousPlane,double ZEnd);	
}
#endif // Planes_h__
