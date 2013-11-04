#include <iostream>
#include "TelescopeSetup_mc.h"
#include <string>
#include <time.h>
using namespace std;



int main(int argc, char *argv[]){

	if (argc!=4)
	{
		cout<<"wrong Syntax \n use:\n .exe Configuration.xml test.root NumberOfEvents \n";
		return -1;

	}

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
