namespace mcGeometric{

int intersectionPlanePlane(const hyperPlane& plane1, const hyperPlane& plane2, line& intersectionLine);

int intersectionPlanePlaneSphere(const hyperPlane& particlePlane, const hyperPlane& TelescopePlane, const Sphere& sphere, Particle& p);

int intersectionLineSphere(const line& l,const Sphere& sphere,mcEUTEL::vector3& p);

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

int mcGeometric::intersectionPlanePlaneSphere( const hyperPlane& particlePlane, const hyperPlane& TelescopePlane, const Sphere& sphere ,Particle& p)
{
	


line intersectionLine;
mcEUTEL::vector3 p1;
if(intersectionPlanePlane(particlePlane,TelescopePlane,intersectionLine)) return -1;
if(intersectionLineSphere(intersectionLine,sphere,p1)) return -1;

mcEUTEL::vector3 r=p1-sphere.Center_;
mcEUTEL::vector3 direction;
direction=crossProduct(r,particlePlane.NormVector);

p.position_=p1;


// p.phi=tan(p1.x/p1.z);  // is wrong but the function will be removed in total
// p.theta=tan(p1.y/p1.z);


return 0;

}

int mcGeometric::intersectionPlanePlane( const hyperPlane& plane1, const hyperPlane& plane2, line& intersectionLine )
{
	intersectionLine.Direction_=crossProduct(plane1.NormVector,plane2.NormVector);


	if (length(intersectionLine.Direction_)==0)
	{
		return -1; // the planes are parallel
	}


	if (intersectionLine.Direction_.z!=0)
	{

		intersectionLine.StartPoint_.z=0;

		vector3 dummy(0,0,1);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.y=(plane1.NormVector.x*plane2.D-plane1.D*plane2.NormVector.x)/denominator;
		intersectionLine.StartPoint_.x=(plane1.NormVector.y*plane2.D-plane1.D*plane2.NormVector.y)/denominator;

	}else if (intersectionLine.Direction_.x!=0)
	{
		intersectionLine.StartPoint_.x=0;

		vector3 dummy(1,0,0);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.y=(plane1.NormVector.z*plane2.D-plane1.D*plane2.NormVector.z)/denominator;
		intersectionLine.StartPoint_.z=(plane1.D*plane2.NormVector.y-plane1.NormVector.y*plane2.D)/denominator;


	} else{

		intersectionLine.StartPoint_.y=0;

		vector3 dummy(0,1,0);
		double denominator=det(dummy,plane1.NormVector,plane2.NormVector);

		intersectionLine.StartPoint_.x=(plane1.NormVector.z*plane2.D-plane1.D*plane2.NormVector.z)/denominator;
		intersectionLine.StartPoint_.z=(plane1.D*plane2.NormVector.x-plane1.NormVector.x*plane2.D)/denominator;
	}

	return 0;

}

int mcGeometric::intersectionLineSphere( const line& l,const Sphere& sphere,mcEUTEL::vector3& p )
{
	vector3 delta=sphere.Center_-l.StartPoint_;
	double directionSquare=(l.Direction_*l.Direction_);
	double sq= ((l.Direction_*delta)/directionSquare)*(l.Direction_*delta)/directionSquare  
		       -((delta*delta)/directionSquare  - (sphere.Radius_*sphere.Radius_)/directionSquare)    ;
	if (sq<0)
	{
		return -1;
	}
	double lamda1=(l.Direction_*delta)/directionSquare+sqrt(sq);
	double lamda2=(l.Direction_*delta)/directionSquare-sqrt(sq);

	vector3 pos1=l.getPosition(lamda1);
	vector3 pos2=l.getPosition(lamda2);

    double distance1=(pos1-p)*(pos1-p);
	double distance2=(pos2-p)*(pos2-p);

	if (distance1<distance2)
	{
		p.x=pos1.x;
		p.y=pos1.y;
		p.z=pos1.z;
	}
	else
	{
		p.x=pos2.x;
		p.y=pos2.y;
		p.z=pos2.z;
	}

		return 0;
}