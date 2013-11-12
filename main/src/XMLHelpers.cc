#include "XMLHelpers.h"
#include <vector>
#include "rapidxml_utils.hpp"
#include <iostream>
#include "Positions_mc.h"
#include "rapidxml_print.hpp"


void XMLModifyPositions(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *modifyPlaneGroup ){

	if (modifyPlaneGroup)
	{
	
	for (auto modifyPlane=modifyPlaneGroup;
		modifyPlane;
		modifyPlane=modifyPlane->next_sibling("modifyPlane"))
	{
	

		for (auto moveOrRotate=modifyPlane->first_node();moveOrRotate;moveOrRotate=moveOrRotate->next_sibling())
		{
			//std::cout<<moveOrRotate->name()<<std::endl;
			if(strcmp(moveOrRotate->name(),"move")==0) XMLMovePositions(pos,moveOrRotate);
			if(strcmp(moveOrRotate->name(),"rotate")==0) XMLRotateAroundAxis(pos,moveOrRotate);
		}
	}
	}
}

int XMLRotateAroundAxis(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *node ){
	if(strcmp(node->name(),"rotate")) return -1; // to make sure that the correct node is used

	auto axis=node->first_attribute("axis")->value();
	double rotaionAngle=std::atof(node->first_attribute("angle")->value());




	for (auto& p:pos)
	{
		if (strcmp(axis,"x")==0)
		{
			p.rotateX(rotaionAngle);
		}else if (strcmp(axis,"y")==0)
		{
			p.rotateY(rotaionAngle);
		}else if (strcmp(axis,"z")==0)
		{
			p.rotateZ(rotaionAngle);
		}else
		{
			return -1; 
		}

	}

	return 1;
}


int XMLMovePositions(std::vector<mcEUTEL::Positions>& pos,rapidxml::xml_node<> *node ){
	if(strcmp(node->name(),"move")) return -1; // to make sure that the correct node is used

	double moveX=std::atof(node->first_attribute("x")->value());
	double moveY=std::atof(node->first_attribute("y")->value());
	double moveZ=std::atof(node->first_attribute("z")->value());
	for (auto& p:pos)
	{
		p.x+=moveX;
		p.y+=moveY;
		p.z+=moveZ;
	}


	return 1;
}

std::vector<mcEUTEL::Positions> XMLExtractPositions( rapidxml::xml_node<> *position ){

	std::vector<mcEUTEL::Positions> ReturnValue;
	mcEUTEL::Positions p;
	// 	auto position= node->first_node("ActiveArea")->first_node("position");
	// 	getPosition(position,oldP);


	for (;position;position=position->next_sibling())
	{


		getPosition(position,p);
		ReturnValue.push_back(mcEUTEL::Positions(p));
	}
	return ReturnValue;
}

int getPosition(rapidxml::xml_node<> *node,mcEUTEL::Positions & p){
	if(strcmp(node->name(),"position")) return -1; // to make sure that the correct node is used


	p.x=std::atof(node->first_node("x")->first_attribute()->value());

	p.y=std::atof(node->first_node("y")->first_attribute()->value());

	p.z=std::atof(node->first_node("z")->first_attribute()->value());
	return 1;
}

void XMLPreparser( rapidxml::xml_document<>& doc )
{
	auto predef=doc.first_node("ConfigurationFile")->first_node("PredefinedPlanes");

for (auto plane = doc.first_node("ConfigurationFile")->first_node("Planes")->first_node("plane");
	 plane;
	 plane=plane->next_sibling()){
	
	if (plane->first_node("usePredifinedPlane"))
	{
		auto use=plane->first_node("usePredifinedPlane")->first_attribute("name")->value();
	//std::cout << use<<std::endl;

	auto correspondingPredefinedPlane=XMLFindPredefinedPlane(predef,use);


	// copies all sub nodes to this position
	auto dummyNote=doc.clone_node(plane);
	plane->remove_all_nodes();
	//print(std::cout, doc, 0);
	for (auto subNode=correspondingPredefinedPlane->first_node();subNode;subNode=subNode->next_sibling())
	{
		
		plane->append_node(doc.clone_node(subNode)); 
	}
	for (auto subNode=dummyNote->first_node();subNode;subNode=subNode->next_sibling())
	{

		plane->append_node(doc.clone_node(subNode)); 
	}
	//print(std::cout, doc, 0);

	}

}
}

rapidxml::xml_node<> * XMLFindPredefinedPlane( rapidxml::xml_node<> *node,const char* name )
{	if(strcmp(node->name(),"PredefinedPlanes")) return nullptr; // to make sure that the correct node is used
	for(node=node->first_node();node;node=node->next_sibling())
	{
		//std::cout<<node->first_attribute("name")->value()<<std::endl;
		if (strcmp(node->first_attribute("name")->value(),name)==0)
		{
			return node;
		}

	}
	return nullptr;
}