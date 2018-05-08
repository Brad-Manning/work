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
  

  double count,r,phi,px, py, pz, type, weight, zenith;
 char* strName[] = {"gamma", "e+", "e-", "?" , "mu+" , "mu-" , "pi0" , "pi+" , "pi-" , "kaon0" , "kaon+" , "kaon-" , "neutron" , "proton" , "anti_proton"};
 vector<string> vecName(strName, strName+15);
 count = 0;

 ofstream G4file;
 ofstream RunInfo;
 ofstream rValues;

 //Loop over files, and read the files
  for (int iFile = 1; iFile <= argc - 1; iFile++) {
   corsikafile file(argv[iFile]);
   cout << "Current File: " << argv[iFile] << "\n";
   string fileName = argv[iFile];
   size_t pos_start = fileName.find("qgs");
   size_t pos_end = fileName.find("/DAT");
   cout << fileName << endl;
   string G4name = fileName.substr(pos_start,pos_end-pos_start);
 
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
   double max;
   double tank_pos;
   cout << "Type detector position, eg. 100 (m)" << endl;
   cin >> tank_pos;
   max = 1.1*tank_pos;
   min = 0.9*tank_pos;
   RunInfo << tank_pos << "\n";

   double delta = 0.1;
   double minPhi = 0;
   double maxPhi = 0.15;
   int thin = 0;
   //Tank radius given in metres
   double r_tank = 1.8;
 while (file.findnextrun())
    {
      
      run theRun=file.getnextrun();

      while (theRun.findnextevent())
	{
	  event theEvent=theRun.getnextevent();
	
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
	     pz = -pz;

	  
	    type = Part.type;
	     weight = Part.weight;
	     
	     //1.2 = height of tank (m)
	     double area_tank = M_PI * pow(r_tank,2) * cos( zenith ) + 2*r_tank*1.2*sin( zenith );
	     double area_section = M_PI * ( pow(tank_pos+delta*tank_pos,2) - pow(tank_pos-delta*tank_pos,2) ) * ( (maxPhi-minPhi) / ( 2*M_PI ) );// * cos(zenith);

	   
	     double average_n = weight * ( area_tank / area_section );

	     //    cout << "Area Of Tank: " << area_tank << " Area of Section: " << area_section << " Average N: " << average_n << endl; 
	    
	     boost::poisson_distribution<int> distribution(average_n);
	     int n = distribution(gen);
	         
	      if (n != 0)
	       {
		 if ( (phi >= minPhi && phi <= maxPhi)  && (r >= min) && (r <= max)) 
		   {
		     
		     if (type == 1)
		       {
			
			 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			 G4file << "/gun/particle " << vecName[type-1] << "\n";
			 G4file << "/run/beamOn " << (int) n <<"\n";
			 rValues << r << "\n";
			 count = count + n;
		       }
		     // if (type >= 16 )
		     //   {
		
		     // 	 G4file << "/gun/momentum " << pz << " " << py << " " << pz << "\n";
		     // 	 G4file << "/gun/particle ion " << "\n";
		     // 	 G4file << "/gun/ion " << type << " " << type*2 << "\n";
		     // 	 G4file << "/run/beamOn " << (int) n << "\n";
		     // 	 rValues << r << "\n";
		     // 	 count = count + n;
			 
		     //   }
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

