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


 double count,r,phi,px, py, pz, e, type, weight;
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
 ofstream weightAll;
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
   int maxWeight = 1000;

   G4name.erase(remove_if(G4name.begin(), G4name.end(), &IsForwardSlash), G4name.end());
  
   string weightName = G4name+"weight.txt";
   G4name = G4name+".mac";
   const char * G4 = G4name.c_str();
   const char * wei = weightName.c_str();
   
   
   cout << weightName << endl;
   G4file.open (G4, ios_base::out | ios_base::app);
   G4file << "/run/initialize" << "\n";
   G4file << "/gun/energy 0" << "\n";
   G4file << "/run/beamOn 1" << "\n";
   weightAll.open (wei, ios_base::out | ios_base::app);
   weightAll << 0 <<"\n";
   rValues.open ("rValues.txt", ios_base::out | ios_base::app);
   rValues << 0 << "\n";

   //Request Input
   int min;
   int max;
   cout << "Select starting radius of annulus" << endl;
   cin >> min;
   cout << "Select ending radius of annulus" << endl;
   cin >> max;
   int thin = 0;
 while (file.findnextrun())
    {
      
      run theRun=file.getnextrun();
      cout << "getting run" << endl;
      while (theRun.findnextevent())
	{
	  event theEvent=theRun.getnextevent();
	  cout << "getting event" << endl;
	  while (theEvent.findnextparticle())
	    {
	     particle Part=theEvent.getnextparticle();
	     r=(sqrt(pow((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)),2)+pow(((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)),2)))/100.0;//core dist in shower plane (m)
	     phi=atan2((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)),((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)));//azimuth in shower plane
	     //time=Part.t-(Part.zstart-theRun.OBSLEVELS[Part.obslev-1])*(1.0E+07/cos(Part.primtheta))/c-Part.x*1.0E+07*sin(Part.primtheta)*cos(Part.primphi)/c-Part.y*1.0E+07*sin(Part.primtheta)*sin(Part.primphi)/c;//time in shower front
	     px = Part.px;
	     py = Part.py;
	     pz = Part.pz;
	     e = Part.energy;
	     type = Part.type;
	     //cout << type << "\n";
	     //cout << r << "\n";
	     //cout << Part.x << "\n";
	     weight = Part.weight;
	     double weight_loss  = 0.15*weight;
	     if (thin %100 == 0)// ||)// r > 2500)
	       {
		 if ((r > min && r < max) )
		   {

		     if (type < 16)
		       {
			 while ( weight > maxWeight )
			   {
			    
			     G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			     G4file << "/gun/particle " << vecName[type-1] << "\n";
			     G4file << "/run/beamOn 1" << "\n";
			     weightAll << weight_loss << "\n";
			     // cout << "weight " << weight << endl;
			     weight = weight - weight_loss;
		
			     rValues << r << "\n";
			   }
			 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			 G4file << "/gun/particle " << vecName[type-1] << "\n";
			 G4file << "/run/beamOn 1" << "\n";
			 weightAll << weight << "\n";
			 // cout << "WEIGHT " << weight << endl;
			 rValues << r << "\n";
		       }
		     if (type >= 16 )
		       {
			 while ( weight > maxWeight )
			   {
			     G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			     G4file << "/gun/particle " << vecName[type-1] << "\n";
			     G4file << "/run/beamOn 1" << "\n";
			     weightAll << weight_loss << "\n";
			     weight = weight - weight_loss;
			     rValues << r << "\n";
			   }
			 G4file << "/gun/momentum " << pz << " " << py << " " << pz << "\n";
			 G4file << "/gun/particle ion " << "\n";
			 G4file << "/gun/ion " << type << " " << type*2 << "\n";
			 G4file << "/run/beamOn 1" << "\n";
			 weightAll << weight << "\n";
			 rValues << r << "\n";
			 
		       }
		   }
	       }
	     thin++;
	     //for (int i = 1; i <= weight; i++) 
	     // if ( r < rmax ) {
	     //   {
	     // 	 if (type == 1)
	     // 	   {
	     // 	     gammaEvents.Fill(r,weight);
	     // 	   }
	     // 	 if (type == 2 || type == 3)
	     // 	   {
	     // 	     eEvents.Fill(r,weight);
	     // 	   }
	     // 	 if (type == 75 || type == 76)
	     //   {
	     // 	 muEvents.Fill(r,weight);
	     //  }
	     //Muons
	       // 	 if (type == 5 || type == 6)
		 
	       // 	   {
	       // 	     // cout << "Hello" << endl;
	       // 	     if ((r > 995 && r < 1005) )
	       // 	       {
	       // 	 	 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
	       // 	 	 G4file << "/gun/particle " << vecName[type-1] << "\n";
	       // 		 // G4file << "/gun/direction " << px << " " << py << " " << -pz << "\n";
	       // 	 	 //G4file << "/gun/energy " << e << " eV " << "\n";
	       // 	 	 G4file << "/run/beamOn 1" << "\n";
	       // 		 weightAll << weight << "\n";

	       // 	 	 //rValuesMuonAll << r << "\n";
	       // 	       }
	       // 	   }
		 
	       // // 	 //Gamma
	       // 	if (type ==1)
	       // 	    {
	       // 	      if ((r > 995 && r < 1005) )//&& (Part.y > -50 && Part.y < 50))
		       
	       // 	       {
	       // 		 //cout << Part.y << "\n";
	       // 		 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
	       // 		 G4file << "/gun/particle " << vecName[type-1] << "\n";
	       // 		 // G4file << "/gun/direction " << px << " " << py << " " << -pz << "\n";
	       // 		 // G4file << "/gun/energy " << e << " eV " << "\n";
	       // 		 G4file << "/run/beamOn 1" << "\n";
	       // 		 weightAll << weight << "\n";
	       // 		 //rValuesGammaAll << r << "\n";
	       // 	       }
	       // 	   }
	       // //Electron
	       // 	if (type == 2 || type == 3)
	       // 	  {
	       // 	    if ((r > 995 && r < 1005) )//&& (Part.y > -50 && Part.y < 50))
		       
	       // 	       {
	       // 		 G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
	       // 		 G4file << "/gun/particle " << vecName[type-1] << "\n";
	       // 		 // G4file << "/gun/direction " << px << " " << py << " " << -pz << "\n";
	       // 		 // G4file << "/gun/energy " << e << " eV " << "\n";
	       // 		 G4file << "/run/beamOn 1" << "\n";
	       // 		 weightAll << weight << "\n";
	       // 		 //rValuesElectronAll << r << "\n";
	       // 	       }
	       // 	  }
	       	 
	
		   
	    }
	}
    }
    G4file.close();
    weightAll.close();
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

