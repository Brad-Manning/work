// Bradley Manning 3/2018


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
#include <cstdlib>
using namespace std;


int main(){
  int count;
  int binSize;
  int thinning;
  cout << "What is binSize?" << endl;
  cin >>  binSize;
  cout << "How many particles did you thin by? eg. Every 50 particles?" << endl;
  cin >> thinning;
  std::vector<int> r_bins;
  double MIP, VEM;

  ifstream MIP_r_bins ("MIP_r_bins.txt");
  ifstream VEM_r_bins ("VEM_r_bins.txt");
  for (int i = 0; i < 3500; i=i+binSize)
    {
      r_bins.push_back(i+0.5*binSize);
    }
  

  TFile *LDF = new TFile("LDF.root", "RECREATE");
  if (LDF->IsOpen() ) printf("File opened successfully\n");
  TH1F *LDF1 = new TH1F("LDF1", "LDF - MIP", 3500/binSize, 0, 3500);
  TH1F *LDF2 = new TH1F("LDF2", "LDF - VEM", 3500/binSize, 0, 3500);
  TCanvas * c1 = new TCanvas ( "c", "c", 800, 600);

  TGraph* LDF3 = new TGraph();
  


  c1->SetLogy();

  if (MIP_r_bins.is_open() )
    {
      count = 0;
      while (MIP_r_bins >> MIP)
	{
	  LDF1->Fill(r_bins[count],((MIP/2.41)/(2*M_PI*binSize*r_bins[count]))/(4));
	  count++;
	}
    }
  if (VEM_r_bins.is_open() )
    {
      count = 0;
      while (VEM_r_bins >> VEM)
	{
	  if (r_bins[count] < 2500) {
	    LDF2->Fill(r_bins[count],VEM);
	    LDF3->SetPoint(LDF3->GetN(), r_bins[count], VEM);
	  count++;
	  } else {
	    LDF2->Fill(r_bins[count],VEM);
	    LDF3->SetPoint(LDF3->GetN(), r_bins[count], VEM);
	    count++;
	  }
	  
	}
    }

  LDF2->SetStats(kFALSE);
  LDF2->GetXaxis()->SetTitle("r [m]");
  LDF2->GetYaxis()->SetTitle("Signal [VEM per tank]");
  LDF1->Write("LDF1");
  LDF2->Write("LDF2");

  LDF3->Draw("AP*");
  LDF3->GetYaxis()->SetRangeUser(0.2,7000);
  LDF3->Write();
 
  
  c1->SetLogy();
  c1->Write("c");
  
  LDF->Write();
  return 0;
}
