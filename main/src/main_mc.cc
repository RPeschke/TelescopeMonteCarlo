#include <iostream>
#include "TelescopeSetup_mc.h"
#include <string>
#include <time.h>
#include "geometric.h"
using namespace std;



int main(int argc, char *argv[]){

	if (argc!=4)
	{
		cout<<"wrong Syntax \n use:\n .exe Configuration.xml test.root NumberOfEvents \n";
		return -1;

	}



 std::vector<mcEUTEL::vector3> p;

 p.push_back(mcEUTEL::vector3(0,0,0));
 p.push_back(mcEUTEL::vector3(0,1,0));
 p.push_back(mcEUTEL::vector3(0,0,1));

  mcGeometric::hyperPlane particlePlane(mcEUTEL::vector3(0,0,1),mcEUTEL::vector3(1,0,0),mcEUTEL::vector3(0,0,0));
  mcGeometric::hyperPlane telescopePlane(mcEUTEL::vector3(1,0,1),mcEUTEL::vector3(0,1,1),mcEUTEL::vector3(0,0,1));

  mcGeometric::line l;

  mcGeometric::intersectionPlanePlane(particlePlane,telescopePlane,l);

  mcGeometric::Sphere s(5,mcEUTEL::vector3(-5,0,0));
  mcEUTEL::vector3 p1;
  intersectionLineSphere(l,s,p1);
  Particle par;
  intersectionPlanePlaneSphere(particlePlane,telescopePlane,s,par);


	 clock_t start, finish;
	  double  duration;
	  int numberOfEvents=std::stoi(argv[3]);
	  cout<<"processing "<<numberOfEvents<< " events"<<endl;
	   start = clock();
	TelescopeSetup t;

	t.readXMLConfig(argv[1]);
	t.setSaveFile(argv[2]);
	t.Run(numberOfEvents);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	 printf( "%2.1f seconds\n", duration );
	return 0;
}
