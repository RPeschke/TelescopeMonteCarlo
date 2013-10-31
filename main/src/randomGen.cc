#include "randomGen.h"
#include <random>
std::mt19937 mt;
std::normal_distribution<double> normalDist(0,1);

double getNormRandom(){

	 return normalDist(mt);
}