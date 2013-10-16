#ifndef MATERIALMC
#define MATERIALMC

class Material{
  private:
    std::string Name_;

    //The following should be 'average' values, this code assumes uniformly distributed materials without structure
    double IntrinsicRadiationLength_;
    double Density_;
    double Thickness_;
    int AtomicNumber_;
    int ProtonNumber_;
  public:
    Material();
    ~Material();
    void init(std::string name, double intrinsicradiationlength, 
              double density, double thickness, int atomicnumber, int protonnumber);

    std::string getName(){return Name_;}
    double getIntrinsicRadiationLength(){return IntrinsicRadiationLength_;}
    double getDensity(){return Density_;}
    double getThickness(){return Thickness_;}
    int getAtomicNumber(){return AtomicNumber_;}
    int getProtonNumber(){return ProtonNumber_;}
};

#endif
