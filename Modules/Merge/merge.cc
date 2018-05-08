// Bradley Manning 3/2018


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <iomanip>
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
  string MIP;
  string VEM;
  ifstream MIP_in ("finalMIP.txt");
  ofstream MIP_out ("MIP_r_bins.txt", std::ios_base::app);
  ifstream VEM_in ("finalVEM.txt");
  ofstream VEM_out ("VEM_r_bins.txt", std::ios_base::app);
  if (MIP_in.is_open())
    {
      while ( getline (MIP_in,MIP) )
	{
	  MIP_out << MIP << "\n";
	}
      MIP_in.close();
      MIP_out.close();
    }
  if (VEM_in.is_open())
    {
      while ( getline (VEM_in,VEM) )
	{
	  VEM_out << VEM << "\n";
	}
      VEM_in.close();
      VEM_out.close();
    }
  return 0;
}
