#include "readPart_1_7b.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include "TVector2.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TMarker.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TLatex.h"
#include "TColor.h"
#include "TFile.h"
#include <algorithm>
//#include <random>
#include <boost/random/poisson_distribution.hpp>
using boost::poisson_distribution;
#include <boost/random/variate_generator.hpp>
using boost::variate_generator;
#include <boost/random/mersenne_twister.hpp>
using boost::mt19937;

//Place Functions here:
void usage();
bool IsForwardSlash(char c)
{
  switch(c)
    {
    case '/':
      return true;
    default:
      return false;
    }
}

	

using namespace std; //don't forget this!

int main (int argc, char **argv){

  //See if use specified what files to analyse
  if (argc < 2) {
    usage();
    return 1;
  }
  mt19937 gen;
  gen.seed(time(NULL));
  

  double count,r,phi,px, py, pz, e, type, weight, zenith;
 char* strName[] = {"gamma", "e+", "e-", "?" , "mu+" , "mu-" , "pi0" , "pi+" , "pi-" , "kaon0" , "kaon+" , "kaon-" , "neutron" , "proton" , "anti_proton"};
 vector<string> vecName(strName, strName+15);
 count = 0;
 
 //ofstream rValuesElectronAll;
 //ofstream rValuesGammaAll;
 //ofstream rValuesMuonAll;
 //rValuesElectronAll.open ("rValuesElectronAll.txt");
 //rValuesGammaAll.open ("rValuesGammaAll.txt");
 //rValuesMuonAll.open ("rValuesMuonAll.txt");
 
 
 //TFile *showerEvents = new TFile("showerEvents.root", "RECREATE");
 //if ( showerEvents->IsOpen() ) printf("File opened successfully\n");
 //double nBins = 1000;
 //double rmax = 2000;
 //TH1F gammaEvents("Gamma Events", "Gamma Events", nBins,0,rmax);
 //TH1F eEvents("eEvents", "Electron/Positron Events", nBins, 0, rmax);
 //TH1F muEvents("Muon+,Muon- Events", "Muon+,Muon- Events",nBins, 0, rmax);
 ofstream G4file;
 ofstream RunInfo;
 ofstream rValues;

 // ofstream Gamma;
 // Gamma.open ("gamma.root");

 // ofstream electrons;
 // electrons.open ("electrons.root")

 //Loop over files, and read the files
  for (int iFile = 1; iFile <= argc - 1; iFile++) {
   corsikafile file(argv[iFile]);
   cout << "Current File: " << argv[iFile] << "\n";
   string fileName = argv[iFile];
   size_t pos_start = fileName.find("qgs");
   size_t pos_end = fileName.find("/DAT");
   cout << fileName << endl;
   string G4name = fileName.substr(pos_start,pos_end-pos_start);
   
   //Maximum weight value::
   int maxWeight = 100000000000;

   G4name.erase(remove_if(G4name.begin(), G4name.end(), &IsForwardSlash), G4name.end());
  
   string infoName = "runInfo.txt";
   G4name = G4name+".mac";
   const char * G4 = G4name.c_str();
   const char * runInfo = infoName.c_str();
   
   
   G4file.open (G4, ios_base::out | ios_base::app);
   G4file << "/run/initialize" << "\n";
   G4file << "/gun/energy 0" << "\n";
   G4file << "/run/beamOn 1" << "\n";
   RunInfo.open (runInfo);
   rValues.open ("rValues.txt", ios_base::out | ios_base::app);
   rValues << 0 << "\n";
  
   //Request Input
   int min;
   int max;
   //cout << "Select starting radius of annulus" << endl;
   cin >> min;
   // cout << "Select ending radius of annulus" << endl;
   max = min*(1.1/0.9);
   //"Place" tank in the centre of this area.
   double tank_pos = (max-min)/2.0 + min;
   RunInfo << tank_pos << "\n";

   double delta = 0.1;
   // double alpha = 0.15;
   double minPhi = 0;
   double maxPhi = 0.15;
   int thin = 0;
   //Tank radius given in metres
   double r_tank = 1.8;
 while (file.findnextrun())
    {
      
      run theRun=file.getnextrun();
      //cout << "getting run" << endl;
      while (theRun.findnextevent())
	{
	  event theEvent=theRun.getnextevent();
	  // cout << "getting event" << endl;
	  while (theEvent.findnextparticle())
	    {
	     particle Part=theEvent.getnextparticle();
	     r=(sqrt(pow((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)),2)+pow(((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)),2)))/100.0;//core dist in shower plane (m)
	     phi=atan2((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)) ,((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)));//azimuth in shower plane
	     phi += 0;
	     //time=Part.t-(Part.zstart-theRun.OBSLEVELS[Part.obslev-1])*(1.0E+07/cos(Part.primtheta))/c-Part.x*1.0E+07*sin(Part.primtheta)*cos(Part.primphi)/c-Part.y*1.0E+07*sin(Part.primtheta)*sin(Part.primphi)/c;//time in shower front
	     px = Part.px;
	     py = Part.py;
	     pz = Part.pz;
	     //Zenith is in RADIANS
	     zenith = fabs (atan2 ( py , pz  ) );
	     //cout << zenith << endl;
	     pz = -pz;

	  
	     e = Part.energy;
	     type = Part.type;
	     //cout << type << "\n";
	     //cout << r << "\n";
	     //cout << Part.x << "\n";
	     weight = Part.weight;
	     
	     //1.2 = height of tank (m)
	     float area_tank = 1.3*3.8*cos( zenith );
	     float area_section = M_PI * ( pow(tank_pos+delta*tank_pos,2) - pow(tank_pos-delta*tank_pos,2) ) * ( (maxPhi-minPhi) / ( 2*M_PI ) );// * cos(zenith);

	     double average_n = weight * ( area_tank / area_section );
	     //cout << weight <<  " " << area_tank << " " << zenith <<  " " << py << " " << pz << endl;
	    
	     //cout << average_n << endl;
	     //std::default_random_engine generator;
	     boost::poisson_distribution<int> distribution(average_n);
	     int n = distribution(gen);
	     // cout << n << endl;
	     double weight_loss  = 0.001*weight;// 0.15*weight;
	      if (n != 0)// ||)// r > 2500)
	       {
		 //if ((r > min && r < max) )
		 if ( (phi >= minPhi && phi <= maxPhi)  && (r > min) && (r < max)) 
		   {
		     //  cout << "phi = " << phi << " r = " << r << endl;
		     if (type < 16)
		       {
			
			 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			 G4file << "/gun/particle " << vecName[type-1] << "\n";
			 G4file << "/run/beamOn " << (int) n <<"\n";
			 rValues << r << "\n";
			 count = count + n;
		       }
		     if (type >= 16 )
		       {
		
			 G4file << "/gun/momentum " << pz << " " << py << " " << pz << "\n";
			 G4file << "/gun/particle ion " << "\n";
			 G4file << "/gun/ion " << type << " " << type*2 << "\n";
			 G4file << "/run/beamOn " << (int) n << "\n";
			 rValues << r << "\n";
			 count = count + n;
			 
		       }
		   }
		 }
	     thin++;
	    }
	}
    }
						     RunInfo << count << "\n";						     
    G4file.close();
    RunInfo.close();
 }
    
 // showerEvents->Write();
}


void usage()
{
  printf("Usage:\n"
	 "./usereadpart $filename(s)\n"
	 "where $filename is the CORSIKA file(s) you want to analyse.\n"
	 "For example, to run over all data at 10e20 eV:\n"
	 "./usereadpart /remote/riposte/bmanning/data/qgsII4/proton/10E20/*\n"
	 );
}

