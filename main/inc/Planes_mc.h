#ifndef Planes_h__
#define Planes_h__
/*! this Class should contain all informations aboud DUT and Telescops
	The Telescopes and the DUT will just be planes with different values 


	it will be created with 4 position as inputs and radiation length. 
	it is designed that in the end it will read a gear file from the computer 
	but for now the information needs to be hard coded

*/

namespace mcEUTEL{
	struct particle
	{
		double x,y,z,phi,theta,energy;
	};
	void Disp(const particle& p);
	struct Positions
	{
		double x,y,z;
	};

	class Planes{
	public:

		Planes(){};  
		Planes(const Positions &p1,const Positions &p2,const Positions &p3,const Positions &p4){
			p1_=p1;
			p2_=p2;
			p3_=p3;
			p4_=p4;
			thickness=p4_.z-p1_.z;
		};
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
			void propagate(particle& p);
			/**
			* checks where on the Sensor the hit was. records original position an Sensor response.  
			* @param p gives the information about the particle.  
			* @return void 
			*/
			void getHit(const particle& p){};
			double getZEndPos(){return p4_.z;};
	private:



		Positions p1_,p2_,p3_,p4_;
		double radiationLength_,thickness;
		double pixelSizeX_,pixelSizeY_; /*!< the size of the Pixel in mm*/

	};

	Planes makeTelescopePlane(double zPosition);
	Planes makeAirPlane(double zStart,double ZEnd);
}
#endif // Planes_h__
