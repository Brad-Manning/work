// Bradley Manning

#include <vector>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
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
#include "TApplication.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TPad.h"

using namespace std;

vector < double >  getDataFromDirectory(string directory)
{
  ifstream fin;
  string dir, filepath, file;
  string nameOfLine;
  double num1; DIR *dp;
  double num2; double num3;
  struct dirent *dirp;
  struct stat filestat;
  double nFiles = 0;
  double sumVEM = 0;
  double finalVEM;
  vector < double > xmax;
  //cout << "dir to get files of: " << flush;
  dir = directory;
 
  dp = opendir( dir.c_str() );
  if (dp == NULL)
    {
      cout << "Error(" << errno << ") opening " << dir << endl;
    }
  while ((dirp = readdir( dp )))
    {
      filepath = dir + "/" + dirp->d_name;
      file = dirp->d_name;
      int found=file.find(".long");
      // If the file is a directory (or is in some way invalid) we'll skip it
      if (stat( filepath.c_str(), &filestat )) continue;
      if (S_ISDIR( filestat.st_mode )) continue;

      // Endeavor to read a single number from the file and display it
      fin.open( filepath.c_str() );
      //      cout << file << endl;
      //File is opened

       if ( found!=std::string::npos)
       	{
	  getline(fin,nameOfLine);
	  //  cout << nameOfLine << endl;
	  //cout << file << endl;
      	  while (getline(fin, nameOfLine)) 
	    {
	      //  cout << nameOfLine << " from while loop" <<  endl;
	      string word;
	      int found2 = nameOfLine.find("PARAMETERS");
	      if  ( found2!=std::string::npos)
		{
		  istringstream iss(nameOfLine, istringstream::in);
		  int count = 0;
		  while( iss >> word )
		    {
		      // cout << word << endl; //XMAX is 5th "word"
		      count++;
		      if (count == 5)
			{
			  cout << word << endl;
			  xmax.push_back(stod(word));
			}
		    }
		}
	    }
	}
       fin.close();
    }
  return xmax;
}



int main()
{
  TFile* xmaxFile = new TFile("xmax.root" , "RECREATE");
  vector < double > xmaxValues;
  xmaxValues =  getDataFromDirectory("/remote/tesla/bmanning/data/qgsII4/proton/19/0deg/");

  TH1* xmax_hist0 = new TH1F("xmax0", "xmax values", 50, 500, 1100);
  for (int i = 0; i < xmaxValues.size(); i++)
    {
      xmax_hist0->Fill(xmaxValues[i]);
    }

  xmaxValues =  getDataFromDirectory("/remote/tesla/bmanning/data/qgsII4/proton/19/26deg/");
  TH1* xmax_hist26 = new TH1F("xmax26", "", 50, 500, 1100);
  for (int i = 0; i < xmaxValues.size(); i++)
    {
      xmax_hist26->Fill(xmaxValues[i]);
    }
  xmaxValues =  getDataFromDirectory("/remote/tesla/bmanning/data/qgsII4/proton/19/38deg/");
  TH1* xmax_hist38 = new TH1F("xmax38", "", 50, 500, 1100);
  for (int i = 0; i < xmaxValues.size(); i++)
    {
      xmax_hist38->Fill(xmaxValues[i]);
    }
  xmaxValues =  getDataFromDirectory("/remote/tesla/bmanning/data/qgsII4/proton/19/49deg/");
  TH1* xmax_hist49 = new TH1F("xmax49", "", 50, 500, 1100);
  for (int i = 0; i < xmaxValues.size(); i++)
    {
      xmax_hist49->Fill(xmaxValues[i]);
    }
  xmaxValues =  getDataFromDirectory("/remote/tesla/bmanning/data/qgsII4/proton/19/60deg/");
  TH1* xmax_hist60 = new TH1F("xmax60", "", 50, 500, 1100);
  for (int i = 0; i < xmaxValues.size(); i++)
    {
      xmax_hist60->Fill(xmaxValues[i]);
    }



  
  xmax_hist0->Write();
  xmax_hist26->Write();
  xmax_hist38->Write();
  xmax_hist49->Write();
  xmax_hist60->Write();

  TCanvas* xmaxAllAngles = new TCanvas("xmaxAllAngles", "xmaxAllAngles", 800,600);
  TPad* p0deg_pad = new TPad("0deg", "0deg", 0 , 1 , 1, 0.8);
  TPad* p26deg_pad = new TPad("26deg", "26deg", 0, 0.8, 1, 0.6);
  TPad* p38deg_pad = new TPad("38deg", "38deg", 0, 0.6, 1, 0.4);
  TPad* p49deg_pad = new TPad("49deg", "49deg", 0, 0.4, 1, 0.2);
  TPad* p60deg_pad = new TPad("60deg", "60deg", 0, 0.2, 1, 0);

  xmaxAllAngles->cd();
  p0deg_pad->Draw();
  p0deg_pad->cd();
  xmax_hist0->Draw();

  xmaxAllAngles->cd();
  p26deg_pad->Draw();
  p26deg_pad->cd();
  xmax_hist26->Draw();

  xmaxAllAngles->cd();
  p38deg_pad->Draw();
  p38deg_pad->cd();
  xmax_hist38->Draw();

  xmaxAllAngles->cd();
  p49deg_pad->Draw();
  p49deg_pad->cd();
  xmax_hist49->Draw();

  xmaxAllAngles->cd();
  p60deg_pad->Draw();
  p60deg_pad->cd();
  xmax_hist60->Draw();

  xmaxAllAngles->Write();





  
  xmaxFile->Write();
  

  return 1;
}
