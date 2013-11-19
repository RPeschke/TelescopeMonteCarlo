
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
#include <TH1D.h>
#include "TF1.h"
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
	void PrintResults(TTree *tree){

		TF1 *g1    = new TF1("g1","gaus");
		TH1D *h1=new TH1D ("h1","h1",300,0,0.03);

		tree->Draw("true_x>>h1","plane_id=1","");
		h1->Fit(g1);
		
		auto mean=g1->GetParameter(1);// mean
		auto sigma=g1->GetParameter(2); // sigma

		std::cout<<mean<<std::endl;

	}
	void run(int NumberOfEvents){

		//check if it is configured or not
		if (!SaveFileSet||  
			!Configured)
		{
			return;
		}
	


		for (int i=0;i<NumberOfEvents;++i)
		{
			plane_id=0;
			ParticleBeam.newParticle();	
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
					p.getHit(ParticleBeam);
					//	Disp(t);
					hit_x=p.hit_x;
					hit_y=p.hit_y;
					phi=tan(ParticleBeam.directions_.x/ParticleBeam.directions_.z);
					theta=tan(ParticleBeam.directions_.y/ParticleBeam.directions_.z);
					tree->Fill();
					++plane_id;
				}
				p.propagate(ParticleBeam);
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
		tree->Branch("true_x",&ParticleBeam.position_.x,"true_x/D");
		tree->Branch("true_y",&ParticleBeam.position_.y,"true_y/D");
		tree->Branch("true_z",&ParticleBeam.position_.z,"true_z/D");
		tree->Branch("phi",&phi,"phi/D");
		tree->Branch("theta",&theta,"theta/D");
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
		node = doc.first_node("ConfigurationFile")->first_node("BField");

		mcEUTEL::vector3 B;

		B.x=std::atof(node->first_attribute("x")->value());
		B.y=std::atof(node->first_attribute("y")->value());
		B.z=std::atof(node->first_attribute("z")->value());

		if (B.length()>0)
		{
			for (auto& p:pl)
			{
				p.BField=B;
				p.inBFiel=true;
			}
		}


		node=doc.first_node("ConfigurationFile")->first_node("Beam");
		ParticleBeam.ProcessXMLNode(node);



		Configured=true;

	}
private:
	std::vector<Planes> pl;
	TFile *froot;
	TTree *tree;
	int plane_id;
	int hit_x,hit_y;
	double phi,theta;
	Particle ParticleBeam;
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

