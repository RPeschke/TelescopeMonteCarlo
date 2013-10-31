#ifndef Beam_mc_h__
#define Beam_mc_h__


namespace mcEUTEL{

	class ParticleBeam{
		



		void setEnergyInGev(double meanE,double sigmaE){
			meanEnergyInGeV_=meanE;
			sigmaEnergyInGev_=sigmaE;
		}
		void setXInmm(double xMin,double xMax){
			xMax_=xMax;
			xMin_=xMin;
		}
		void setYInmm(double yMin,double yMax){
			yMin_=yMin;
			yMax_=yMax;
		}
		void setAngleInRad(double Phi,double Theta){
			sigmaPhi_=Phi;
			sigmaTheta_=Theta;
		}

		
//		void getNewParticle(mcEUTEL::particle & p);
		double meanEnergyInGeV_,sigmaEnergyInGev_;
		double xMin_,xMax_,yMin_,yMax_;
		double sigmaPhi_,sigmaTheta_;


	};

}

#endif // Beam_mc_h__
