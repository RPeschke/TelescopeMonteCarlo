#include <iostream>
#include "TelescopeSetup_mc.h"
#include <string>
#include <time.h>
#include "geometric.h"
using namespace std;


void testintersectins();
int main(int argc, char *argv[]){


	if (argc!=4)
	{
		cout<<"wrong Syntax \n use:\n .exe Configuration.xml test.root NumberOfEvents \n";
		return -1;

	}
//	testintersectins();
	


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
void testintersectins(){

	mcEUTEL::vector3 B(0,0,1);
	mcEUTEL::vector3 V(0.1,0.0,1);
	mcEUTEL::vector3 X0(0,0,0);
	mcEUTEL::vector3 X1(0,0,0);

	auto Vp=(B*V)*B/B.length();
	auto Vn=crossProduct(V,B);
	Vn.normalize();
	auto Vt=crossProduct(Vp,Vn);
	Vt.normalize();

	mcGeometric::helix h(X0,V,B,5);


	auto p2=h.getPosition(3.57*(2*3.14)/360/h.angleVelocity_);
	std::vector<mcEUTEL::vector3> p;

	p.push_back(mcEUTEL::vector3(0,0,0));
	p.push_back(mcEUTEL::vector3(0,1,0));
	p.push_back(mcEUTEL::vector3(0,0,1));

	mcGeometric::hyperPlane particlePlane(mcEUTEL::vector3(0,0,1),mcEUTEL::vector3(1,0,0),mcEUTEL::vector3(0,0,0));
	mcGeometric::hyperPlane telescopePlane(mcEUTEL::vector3(1,0,1),mcEUTEL::vector3(0,1,1),mcEUTEL::vector3(0,0,1));

	mcGeometric::line l;


	clock_t start, finish;
	double  duration;


	start = clock();
	for (int i=0;i<100;++i)
	{
			mcGeometric::intersectionHelixPlaneApproxNumeric(h,telescopePlane,X1);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "%2.1f seconds for mcGeometric::intersectionHelixPlaneApproxNumeric(h,telescopePlane,X1)\n", duration );

		start = clock();

		for (int i=0;i<1000000;++i)
		{
				auto lambda=mcGeometric::intersectionHelixPlaneApprox(h,telescopePlane);
		}


	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf( "%2.1f seconds for mcGeometric::intersectionHelixPlaneApprox(h,telescopePlane,X0);\n", duration );

	mcGeometric::intersectionPlanePlane(particlePlane,telescopePlane,l);

	mcGeometric::Sphere s(5,mcEUTEL::vector3(-5,0,0));
	mcEUTEL::vector3 p1;
	intersectionLineSphere(l,s,p1);
	Particle par;
	intersectionPlanePlaneSphere(particlePlane,telescopePlane,s,par);



}