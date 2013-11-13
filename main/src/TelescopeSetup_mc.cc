
#include "TelescopeSetup_mc.h"
#include "rapidxml_utils.hpp"
#include "Planes_mc.h"
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "particle_mc.h"
#include "randomGen.h"
#include "rapidxml_print.hpp"
#include <string>
#include <map>
#include "XMLHelpers.h"
using namespace std;
using namespace mcEUTEL;
using namespace rapidxml;


class TelescopeSetup::PImpl{
public:
	PImpl():froot(nullptr),tree(nullptr),plane_id(0),hit_x(0),hit_y(0),Configured(0),SaveFileSet(0){}
	~PImpl(){

		if (froot!=nullptr)
		{
			froot->Close();
			delete froot;
		}
	}
	void run(int NumberOfEvents){

		//check if it is configured or not
		if (!SaveFileSet||  
			!Configured)
		{
			return;
		}
	


		for (int i=1;i<NumberOfEvents;++i)
		{
			plane_id=0;
			t.newParticle();	
			//	Disp(t);
			if (i%10000==0)
			{
				cout<<i<<endl;

			}
			for (auto p:pl)
			{


				//cout<<"telescope: "<<endl;
				//Disp(t);
				if (p.write2file)
				{
					p.getHit(t);
					//	Disp(t);
					hit_x=p.hit_x;
					hit_y=p.hit_y;
					
					tree->Fill();
					++plane_id;
				}
				p.propagate(t);
			}

		}	
		froot->Write();
	}
	void setSaveFile(const char* fileName){
		
		
		if (froot!=nullptr)
		{
			froot->Close();
			delete froot;
		}

		froot=new TFile(fileName,"recreate");
		tree=new TTree("tree","a simple tree");
		tree->Branch("plane_id",&plane_id,"plane_id/I");
		tree->Branch("true_x",&t.x,"true_x/D");
		tree->Branch("true_y",&t.y,"true_y/D");
		tree->Branch("true_z",&t.z,"true_z/D");
		tree->Branch("hit_x",&hit_x,"hit_x/I");
		tree->Branch("hit_y",&hit_y,"hit_y/I");
		
		SaveFileSet=true;
	}
	void loadXMLConfiguration(const char* FileName){
		rapidxml::file<> f(FileName);
		rapidxml::xml_document<> doc; 

		doc.parse<0>(f.data());



		//xml_node<> *node = doc.first_node()->first_node();
		XMLPreparser(doc);
			
	//	print(std::cout, doc, 0);
		//doc.allocate_node("newName");
		
	//	node->append_node(node1);

		auto node = doc.first_node("ConfigurationFile")->first_node("Planes")->first_node("plane");
		pl.push_back(Planes(node));
		for (node = node->next_sibling("plane");node;node=node->next_sibling("plane"))
		{
			pl.push_back(makeAirPlane(0,0));
			pl.push_back(Planes(node));
			pl.at(pl.size()-2).fitPlaneBetweenOtherPlanes(pl.at(pl.size()-3),pl.at(pl.size()-1));
		}
		node=doc.first_node("ConfigurationFile")->first_node("Beam");
		t.ProcessXMLNode(node);

		Configured=true;

	}
private:
	std::vector<Planes> pl;
	TFile *froot;
	TTree *tree;
	int plane_id;
	int hit_x,hit_y;
	
	Particle t;
	bool Configured,SaveFileSet;
};


TelescopeSetup::TelescopeSetup()
{
	privatImpl=new PImpl();
}

void TelescopeSetup::setSaveFile( const char* fileName )
{
	privatImpl->setSaveFile(fileName);
}

void TelescopeSetup::readXMLConfig( const char* fileName )
{
	privatImpl->loadXMLConfiguration(fileName);
}

void TelescopeSetup::Run( int NumberOfEvents )
{
	privatImpl->run(NumberOfEvents);
}

TelescopeSetup::~TelescopeSetup()
{
delete privatImpl;
}
