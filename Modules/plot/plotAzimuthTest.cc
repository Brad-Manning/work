// Bradley Manning 5/2018


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <vector>
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
#include "TGaxis.h"
#include <cstdlib>
using namespace std;

struct point
{
  double pos;
  double value;
};
  

int main(){

  ifstream AzimuthTestFile ("/remote/tesla/bmanning/work/Scripts/AzimuthTest/test.txt");
  ifstream AzimuthPrimTestFile ("/remote/tesla/bmanning/work/Scripts/AzimuthTest/primtest.txt");

  double zenith;
  double azimuth;
  //  std::vector<int> nParticles;
  int nP;
  //  std::vector<double> minPhi;
  double minP;
  // std::vector<double> maxPhi; //Min and Max phi are in radians
  double maxP;
  int i = 0;
  TFile *AzimuthTest = new TFile("AzimuthTest.root", "RECREATE");
  TCanvas * AzimuthTestCanvas = new TCanvas( "AzimuthTest", "AzimuthTest", 1920, 1000 );
  TGraph* AzimuthTestGraph = new TGraph();
  std::vector< vector < point > > vectorOfPoints;
  bool firstPoint = true;
  vectorOfPoints.push_back(vector < point >() );
  if ( AzimuthTestFile.is_open() )
    {
      while (AzimuthTestFile >> nP >> minP >> maxP)
	{
	
	  double pos = (maxP-minP)/2.;
	  point p;
	  if (maxP < minP) pos = 0.15;
	  if (firstPoint)
	    {
	      minP = -minP;
	      pos = -pos;
	    }
	  firstPoint = false;
	 //" " << pos << " " << minP << endl;
	  p.pos = (minP+pos)*180/M_PI;
	  p.value = nP;
	  //cout << nP << " " << minP << " " << maxP << endl;
	  if (nP == -1 && minP == -1 && maxP == -1) //end of run for that shower
	    {
	      firstPoint = true;
	      // cout << i << "i" <<  endl;
	      i++;
	      vectorOfPoints.push_back(vector< point >() );
	    }
	  else
	    {
	      vectorOfPoints[i].push_back(p);
	      // cout << minP+pos << endl;
	    }
	  // AzimuthTestGraph->SetPoint(AzimuthTestGraph->GetN(), (minP+pos)*180/M_PI, nP);
	}
    }
  int nShowers = i;
  vector < point > vectorOfSums;
  double sum;
  point sumPoint;
  cout << " dfdf " << endl;
  cout << nShowers << " " << vectorOfPoints[0].size() << endl;
  for (int j = 0; j < vectorOfPoints[0].size(); j++)
    {
      sum = 0;
      for (int k = 0; k <  nShowers; k++)
	{

	  sumPoint.pos = vectorOfPoints[k][j].pos;
	  sum = sum + vectorOfPoints[k][j].value;
	  sumPoint.value = sum;
	  
	  //cout << vectorOfPoints[j][k].value << endl;
	}
      vectorOfSums.push_back(sumPoint);
      //cout << sum << " " << sumPoint.pos << endl;
    }
  

  for (int k = 0; k < vectorOfPoints[0].size(); k++)
    {
      AzimuthTestGraph->SetPoint(k,vectorOfSums[k].pos,vectorOfSums[k].value/nShowers);
    }


  
  if ( AzimuthPrimTestFile.is_open() )
    {
      while ( AzimuthPrimTestFile >> zenith >> azimuth )
	{
	}
    }
  
  ostringstream primHeading;
  ostringstream primZenith;
  ostringstream primAzimuth;
  primHeading << "--Primary Particle--";
  primZenith << "Zenith: " << (int) (zenith*180/M_PI);
  primAzimuth << "Azimuth: " << (int) (azimuth*180/M_PI);
  TLegend* AzimuthTestLegend = new TLegend(0.1, 0.7, 0.48, 0.9);
  AzimuthTestLegend->AddEntry(AzimuthTestGraph, primHeading.str().c_str(),"");
  AzimuthTestLegend->AddEntry(AzimuthTestGraph, primZenith.str().c_str(),"");
  AzimuthTestLegend->AddEntry(AzimuthTestGraph, primAzimuth.str().c_str(),"");

  AzimuthTestGraph->GetXaxis()->SetRangeUser(-180,180);
  AzimuthTestGraph->GetXaxis()->SetTitle("deg");
  AzimuthTestGraph->GetYaxis()->SetTitle("number of particles");
  AzimuthTestGraph->Draw("AP*");
  // AzimuthTestLegend->Draw();
  AzimuthTestGraph->Write();
  AzimuthTestCanvas->Write();
  AzimuthTest->Write();
  return 0;
}

