#ifndef XMLHelpers_h__
#define XMLHelpers_h__
#include <vector>
#include "rapidxml_utils.hpp"
#include "Positions_mc.h"



void XMLModifyPositions(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *node );
int XMLRotateAroundAxis(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *node );
int XMLMovePositions(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *node );
std::vector<mcEUTEL::Positions> XMLExtractPositions( rapidxml::xml_node<> *position );
int getPosition(rapidxml::xml_node<> *node,mcEUTEL::Positions & p);

void XMLPreparser( rapidxml::xml_document<>& doc );
rapidxml::xml_node<> *XMLFindPredefinedPlane(rapidxml::xml_node<> *node,const char* name);


#endif // XMLHelpers_h__
