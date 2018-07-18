//Bradley Manning
//Custom Header Files
#include "readPart_1_7b.h" //From S.Saffi -modified to mathematical convention 

//C++ Headers
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <math.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//Root headers
#include "TVector2.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3I.h"
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
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TMath.h"
//BOOST headers
#include <boost/random/poisson_distribution.hpp>
using boost::poisson_distribution;
#include <boost/random/variate_generator.hpp>
using boost::variate_generator;
#include <boost/random/mersenne_twister.hpp>
using boost::mt19937;
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>


int main()
{
  TFile* f_26deg = new TFile("12-7-2018-1e19-26deg-componentLDFs.root");
  TFile* f_0deg = new TFile("12-7-2018-1e19-0deg-componentLDFs.root");
  TFile* test = new TFile("test.root", "RECREATE");
  
  TCanvas* multipleLDFs_a_26 = new TCanvas("MultipleLDFs_a_26", "MultipleLDFs_a_26", 800, 600);
  TCanvas* multipleLDFs_a_0 = new TCanvas("MultipleLDFs_a_0", "MultipleLDFs_a_0", 800, 600);
  
  multipleLDFs_a_26 = (TCanvas*)f_26deg->Get("MultipleLDFs_a;50");
  multipleLDFs_a_0 = (TCanvas*)f_0deg->Get("MultipleLDFs_a;50");

  TCanvas* master = new TCanvas("master", "master", 800, 600);
  //  master->cd();
  multipleLDFs_a_26->Draw();
  multipleLDFs_a_0->Draw("SAME");
  multipleLDFs_a_26->Write();
  //multipleLDFs_a_0->Write();
  master->Write();
  test->Write();
  

  return 0;
}
