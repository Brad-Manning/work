// Bradley Manning 4/2018

#include <numeric>
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
#include "TPad.h"
#include "TCut.h"
#include "TLatex.h"
#include "TColor.h"
#include "TFile.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TGraph.h"
#include <cstdlib>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;

//Root Variables (GLOBAL)
TGraph* simLDFSSD = new TGraph();
TGraph* simLDFSD = new TGraph();
TGraphErrors* simLDFSDerrors = new TGraphErrors();
TMultiGraph *mg = new TMultiGraph();
TGraph* eSimLDFSD = new TGraph();
TGraph* mSimLDFSD = new TGraph();
TGraph* pSimLDFSD = new TGraph();
TGraph* eSimLDFSSD = new TGraph();
TGraph* mSimLDFSSD = new TGraph();
TGraph* pSimLDFSSD = new TGraph();

TGraph* simLDFSSDnum = new TGraph();
TGraph* simLDFSDnum = new TGraph();

TGraph* eSimLDFSDnum = new TGraph();
TGraph* mSimLDFSDnum = new TGraph();
TGraph* pSimLDFSDnum = new TGraph();
TGraph* eSimLDFSSDnum = new TGraph();
TGraph* mSimLDFSSDnum = new TGraph();
TGraph* pSimLDFSSDnum = new TGraph();

// TH1* eSimLDFSDnum = new TH1F("eSimLDFSDnum", "eSimLDFSDnum", 1000, 0, 4000);
// TH1* mSimLDFSDnum = new TH1F("mSimLDFSDnum", "mSimLDFSDnum", 1000, 0, 4000);
// TH1* pSimLDFSDnum = new TH1F("pSimLDFSDnum", "pSimLDFSDnum", 1000, 0, 4000);
// TH1* eSimLDFSSDnum = new TH1F("eSimLDFSSDnum", "eSimLDFSSDnum", 1000, 0, 4000);
// TH1* mSimLDFSDnum = new TH1F("mSimLDFSDnum", "mSimLDFSDnum", 1000, 0, 4000);
// TH1* pSimLDFSDnum = new TH1F("pSimLDFSDnum", "pSimLDFSDnum", 1000, 0, 4000);

auto eSimLDFSDnumprof = new TProfile("eSimLDFSDumprof", "Profile of e number", 1000, 0, 4000);
auto mSimLDFSDnumprof = new TProfile("mSimLDFSDumprof", "Profile of m number", 1000, 0, 4000);
auto pSimLDFSDnumprof = new TProfile("pSimLDFSDumprof", "Profile of p number", 1000, 0, 4000);
auto eSimLDFSSDnumprof = new TProfile("eSimLDFSDumprof", "Profile of e number", 1000, 0, 4000);
auto mSimLDFSSDnumprof = new TProfile("mSimLDFSDumprof", "Profile of m number", 1000, 0, 4000);
auto pSimLDFSSDnumprof = new TProfile("pSimLDFSDumprof", "Profile of p number", 1000, 0, 4000);

auto eSimLDFSDprof = new TProfile("eSimLDFSDprof", "Profile of e number", 1000, 0, 4000);
auto mSimLDFSDprof = new TProfile("mSimLDFSDprof", "Profile of m number", 1000, 0, 4000);
auto pSimLDFSDprof = new TProfile("pSimLDFSDprof", "Profile of p number", 1000, 0, 4000);
auto eSimLDFSSDprof = new TProfile("eSimLDFSDprof", "Profile of e number", 1000, 0, 4000);
auto mSimLDFSSDprof = new TProfile("mSimLDFSDprof", "Profile of m number", 1000, 0, 4000);
auto pSimLDFSSDprof = new TProfile("pSimLDFSDprof", "Profile of p number", 1000, 0, 4000);

int main(){
  ifstream fin;
  string dir, filepath,file;
  double num, nParticles, tank_pos;
  DIR *dp;
  struct dirent *dirp;
  struct stat filestat;
  int nEvents = 16;
  int nFiles = 20;
  int idx = 0;

  double_t  sum[nEvents] = {0};
  double_t  error[nEvents] = {0};

  vector < double > pSDSum, eSDSum, mSDSum, pSSDSum, eSSDSum, mSSDSum;
  
  cout << "dir to get files of: " << flush;
  //getline( cin, dir );  // gets everything the user ENTERs
  dir =  "/remote/tesla/bmanning/data/SimulationData/proton/19/38deg";
  dp = opendir( dir.c_str() );
  if (dp == NULL)
    {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
    }

  while ((dirp = readdir( dp )))
    {
    filepath = dir + "/" + dirp->d_name;
    file = dirp->d_name;
    // If the file is a directory (or is in some way invalid) we'll skip it 
    if (stat( filepath.c_str(), &filestat )) continue;
    if (S_ISDIR( filestat.st_mode ))         continue;

    // Endeavor to read a single number from the file and display it
    fin.open( filepath.c_str() );

    // Dynamic Bin Size 12 April 2018


    
   
    while ( fin >> num >> nParticles >> tank_pos) 
      {
       	if ( ( file.find("MIP") != std::string::npos  ) ) { //MIP file
	  //Define R start point and bin
	  if ( file.find("gamma") != std::string::npos) {
	    pSimLDFSSD->SetPoint(pSimLDFSSD->GetN(), tank_pos, num/2);
	    pSimLDFSSDnum->SetPoint(pSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    pSSDSum.push_back(num/2);
	    pSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    pSimLDFSSDprof->Fill(tank_pos, num/2.);
	    
	  } else if (file.find("muon") != std::string::npos) {
	    mSimLDFSSD->SetPoint(mSimLDFSSD->GetN(), tank_pos, num/2);
	    mSimLDFSSDnum->SetPoint(mSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    mSSDSum.push_back(num);
	    mSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    mSimLDFSSDprof->Fill(tank_pos, num/2.);
	  } else if (file.find("electron") != std::string::npos) {
	    eSimLDFSSD->SetPoint(eSimLDFSSD->GetN(), tank_pos, num/2);
	    eSimLDFSSDnum->SetPoint(eSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    eSSDSum.push_back(num);
	    eSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    cout << nParticles+1 << " " << tank_pos << " " << num/2. << endl;
	    eSimLDFSSDprof->Fill(tank_pos, num/2.);

	    //  cout << tank_pos << " " << nParticles << endl;
	  } else
	    {
	      simLDFSSD->SetPoint(simLDFSSD->GetN(), tank_pos, num/2);
	      simLDFSSDnum->SetPoint(simLDFSSDnum->GetN(), tank_pos, nParticles);
	    }
	
	 	  
	  //cout << dirp->d_name << ": " << num << endl;
	  //Note MIP was not calibrated to MIP in GEANT4 (hence 2)
	  //simLDFSSD->SetPoint(simLDFSSD->GetN(), rMIP, num/2);
	  //rMIP = rMIP + rStep;
	  
	} else if ( ( file.find("VEM") != std::string::npos ) ) { //VEM file

	    
	  if ( file.find("gamma") != std::string::npos) {
	    pSimLDFSD->SetPoint(pSimLDFSD->GetN(), tank_pos, num);
	    pSimLDFSDnum->SetPoint(pSimLDFSDnum->GetN(), tank_pos, nParticles);
	    pSDSum.push_back(num);
	    pSimLDFSDnumprof->Fill(tank_pos, nParticles);
	    pSimLDFSDprof->Fill(tank_pos, num);
	  } else if (file.find("muon") != std::string::npos) {
	    mSimLDFSD->SetPoint(mSimLDFSD->GetN(), tank_pos, num);
	    mSimLDFSDnum->SetPoint(mSimLDFSDnum->GetN(), tank_pos, nParticles);
	    mSDSum.push_back(num);
	    mSimLDFSDnumprof->Fill(tank_pos, nParticles);
	    mSimLDFSDprof->Fill(tank_pos, num);
	  } else if (file.find("electron") != std::string::npos) {
	    eSimLDFSD->SetPoint(eSimLDFSD->GetN(), tank_pos, num);
	    eSimLDFSDnum->SetPoint(eSimLDFSDnum->GetN(), tank_pos, nParticles);
	    eSDSum.push_back(num);
	    eSimLDFSDnumprof->Fill(tank_pos, nParticles);
	    eSimLDFSDprof->Fill(tank_pos, num);
	  } else {
	  simLDFSD->SetPoint(simLDFSD->GetN(), tank_pos, num);
	  simLDFSDnum->SetPoint(simLDFSDnum->GetN(), tank_pos, nParticles);
	  sum[idx % nEvents] += num;
	  }
	  
	 
	    // cout << sum[idx % nEvents] << endl;
	   
	  
	}
	idx++;
      } 
    fin.close();
    }

  closedir( dp );
  double rVEM = 164;
  double rVEMStep = 0;
  const double factor = 0.1;
  double_t rVEM_vector[nEvents] = {0};
  double_t mean[nEvents] = {0};

  for (int i = 0; i < nEvents; i++) {
    error[i] = 1.06*sqrt(sum[i]/ nFiles);
    mean[i] = (sum[i]/nFiles);
    rVEM_vector[i] = rVEM;
    rVEMStep = (1+factor)*rVEM;
    rVEM = rVEMStep / (1-factor);
    
    //cout << error[i] << " " << sum[i] << endl;
  }
  simLDFSDerrors = new TGraphErrors(nEvents, rVEM_vector, mean, 0, error);

  
  // double pSDMean = accumulate( pSDSum.begin(), pSDSum.end(), 0.0)/pSDSum.size();
  // double eSDMean = accumulate( eSDSum.begin(), eSDSum.end(), 0.0)/eSDSum.size();
  // double mSDMean = accumulate( mSDSum.begin(), mSDSum.end(), 0.0)/mSDSum.size();
  // double pSSDMean = accumulate( pSSDSum.begin(), pSSDSum.end(), 0.0)/pSSDSum.size();
  // double eSSDMean = accumulate( eSSDSum.begin(), eSSDSum.end(), 0.0)/eSSDSum.size();
  // double mSSDMean = accumulate( mSSDSum.begin(), mSSDSum.end(), 0.0)/mSSDSum.size();
      
  simLDFSSD->SetMarkerColor(kBlue); simLDFSSD->SetMarkerStyle(20);
  simLDFSD->SetMarkerColor(kGreen); simLDFSD->SetMarkerStyle(20);
  eSimLDFSD->SetMarkerColor(kRed);  eSimLDFSD->SetMarkerStyle(21);
  eSimLDFSSD->SetMarkerColor(kRed); eSimLDFSSD->SetMarkerStyle(21);
  pSimLDFSD->SetMarkerColor(kGreen); pSimLDFSD->SetMarkerStyle(22);
  pSimLDFSSD->SetMarkerColor(kGreen); pSimLDFSSD->SetMarkerStyle(22);
  mSimLDFSD->SetMarkerColor(kBlue); mSimLDFSD->SetMarkerStyle(23);
  mSimLDFSSD->SetMarkerColor(kBlue); mSimLDFSSD->SetMarkerStyle(23);
  simLDFSSDnum->SetMarkerColor(kBlue); simLDFSSDnum->SetMarkerStyle(20);
  simLDFSDnum->SetMarkerColor(kGreen); simLDFSDnum->SetMarkerStyle(20);
  eSimLDFSDnum->SetMarkerColor(kRed); eSimLDFSDnum->SetMarkerStyle(21);
  eSimLDFSSDnum->SetMarkerColor(kRed); eSimLDFSSDnum->SetMarkerStyle(21);
  pSimLDFSDnum->SetMarkerColor(kGreen); pSimLDFSDnum->SetMarkerStyle(22);
  pSimLDFSSDnum->SetMarkerColor(kGreen); pSimLDFSSDnum->SetMarkerStyle(22);
  mSimLDFSDnum->SetMarkerColor(kBlue); mSimLDFSDnum->SetMarkerStyle(23);
  mSimLDFSSDnum->SetMarkerColor(kBlue); mSimLDFSSDnum->SetMarkerStyle(23);

  eSimLDFSDprof->SetMarkerColor(kRed);  eSimLDFSDprof->SetMarkerStyle(21);
  eSimLDFSSDprof->SetMarkerColor(kRed); eSimLDFSSDprof->SetMarkerStyle(21);
  pSimLDFSDprof->SetMarkerColor(kGreen); pSimLDFSDprof->SetMarkerStyle(22);
  pSimLDFSSDprof->SetMarkerColor(kGreen); pSimLDFSSDprof->SetMarkerStyle(22);
  mSimLDFSDprof->SetMarkerColor(kBlue); mSimLDFSDprof->SetMarkerStyle(23);
  mSimLDFSSDprof->SetMarkerColor(kBlue); mSimLDFSSDprof->SetMarkerStyle(23);
  
  eSimLDFSDnumprof->SetMarkerColor(kRed); eSimLDFSDnumprof->SetMarkerStyle(21);
  eSimLDFSSDnumprof->SetMarkerColor(kRed); eSimLDFSSDnumprof->SetMarkerStyle(21);
  pSimLDFSDnumprof->SetMarkerColor(kGreen); pSimLDFSDnumprof->SetMarkerStyle(22);
  pSimLDFSSDnumprof->SetMarkerColor(kGreen); pSimLDFSSDnumprof->SetMarkerStyle(22);
  mSimLDFSDnumprof->SetMarkerColor(kBlue); mSimLDFSDnumprof->SetMarkerStyle(23);
  mSimLDFSSDnumprof->SetMarkerColor(kBlue); mSimLDFSSDnumprof->SetMarkerStyle(23);
  
  simLDFSDerrors->SetMarkerColor(kRed);
  TFile *Trace = new TFile("/remote/tesla/bmanning/work/Modules/ADST_Analysis/Trace.root");
  TFile *simTrace = new TFile("SimTrace.root", "RECREATE");

  //Custom NKG function to fit
  
  TF1 *NKG = new TF1("NKG","(x/[0])^-[1]*(1+(x/[0]))^-[2]");
  NKG->SetParName(0,"r_opt");
  NKG->SetParName(1,"beta");
  NKG->SetParName(2,"gamma");
  NKG->SetParameter(0,1000);
  NKG->SetParameter(1,2);
  NKG->SetParameter(2,0.2);


  
  TCanvas *errorsC = new TCanvas("Errors", "Errors", 1920,1000);


  simLDFSDerrors->Fit("NKG");



  double NKG_0 = NKG->GetParameter(0);
  double NKG_1 = NKG->GetParameter(1);
  double NKG_2 = NKG->GetParameter(2);
  //Evaluate NKG
  Double_t y[5000] = {0};
  Double_t x[5000] = {0};
  Double_t errorTest[5000] = {0};
  
  for (int i = 0 ; i < 5000; i++)
    {
      y[i] = pow(i/NKG_0,-NKG_1)*pow(1+(i/NKG_0),-NKG_2);
      x[i] = i;
      error[i] = 1.06*sqrt(y[i]);
      // cout << x[i] << endl;
    }  
  TGraphErrors *simLDFSDfunc = new TGraphErrors(5000,x,y,0,errorTest);
  errorsC->cd();
  simLDFSDfunc->Draw("*e3");
  simLDFSDfunc->SetMinimum(0.1);
  simLDFSDfunc->SetMaximum(15000);
  simLDFSDfunc->GetXaxis()->SetLimits(0,4000);
  simLDFSDfunc->GetXaxis()->SetTitle("r (m)");
  simLDFSDfunc->SetMarkerColor(34);
  simLDFSDfunc->SetLineColor(34);
  simLDFSDfunc->SetFillColor(34);

  simLDFSDfunc->Write();
  errorsC->SetTitle("Lateral Distribution Function");
  errorsC->SetLogy();
  errorsC->Write();
  
  if (simTrace->IsOpen() ) printf("My sim file opened successfully\n");
  if (Trace->IsOpen() ) printf("Offline sim file opened successfully\n");
  TGraph *LDFSD = (TGraph*)Trace->Get("LDFSD");
  TGraph *LDFSSD = (TGraph*)Trace->Get("LDFSSD");
  
  TCanvas *c = new TCanvas("All4", "All4", 1920,1000);
  mg->Add(simLDFSSD);
  mg->Add(simLDFSD);
  mg->Add(LDFSD);
  mg->Add(LDFSSD);
  mg->Draw("AP*");
  mg->SetMinimum(0.1);
  mg->SetMaximum(15000);
  mg->GetXaxis()->SetLimits(0,4000);
  mg->GetXaxis()->SetTitle("r (m)");
  mg->SetTitle("Lateral Distribution Function");

  TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->AddEntry(LDFSD, "OffLine - Signal (VEM PEAK)", "p");
  leg->AddEntry(LDFSSD, "OffLine - Signal (MIP PEAK)", "p");
  leg->AddEntry(simLDFSD, "My G4 - Signal (VEM PEAK)", "p");
  leg->AddEntry(simLDFSSD, "My G4 - Signal (MIP PEAK)", "p");
  leg->Draw();

  
  mg->Write();
  simLDFSSD->Write();
  simLDFSD->Write();
  c->SetTitle("Lateral Distribution Function");
  c->SetLogy();
  c->Write();

  TCanvas *c2 = new TCanvas("MIP-2", "MIP-2", 1920, 1000);
  TMultiGraph *mg2 = new TMultiGraph();
  c2->cd();
  mg2->Add(simLDFSSD);
  mg2->Add(LDFSSD);
  mg2->Draw("AP*");
  mg2->SetMinimum(0.1);
  mg2->SetMaximum(15000);
  mg2->GetXaxis()->SetLimits(0,4000);
  mg2->GetXaxis()->SetTitle("r (m)");
  mg2->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");

  TLegend* leg2 = new TLegend(0.1,0.7,0.48,0.9);
  leg2->AddEntry(LDFSSD, "OffLine - Signal (MIP PEAK)", "p");
  leg2->AddEntry(simLDFSSD, "My G4 - Signal (MIP PEAK)", "p");
  leg2->Draw();

  c2->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  c2->SetLogy();
  c2->Write();

  TCanvas *c3 = new TCanvas("VEM-2", "VEM-2", 1920, 1000);
  TMultiGraph *mg3 = new TMultiGraph();
  c3->cd();
  mg3->Add(simLDFSD);
  mg3->Add(LDFSD);
  // mg3->Add(simLDFSDerrors);
  mg3->Draw("AP*");
  mg3->SetMinimum(0.1);
  mg3->SetMaximum(15000);
  mg3->GetXaxis()->SetLimits(0,4000);
  mg3->GetXaxis()->SetTitle("r (m)");
  mg3->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");

  TLegend* leg3 = new TLegend(0.1, 0.7, 0.48, 0.9);
  leg3->AddEntry(LDFSD, "OffLine - Signal (VEM PEAK)", "p");
  leg3->AddEntry(simLDFSD, "My G4 - Signal (VEM PEAK)", "p");
  leg3->Draw();

  c3->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  c3->SetLogy();
  c3->Write();
  
  TCanvas *simComponentsSD = new TCanvas("ComponentsSD", "ComponentsSD-Sim", 1920, 1000);
  TMultiGraph *SDmg = new TMultiGraph();
  simComponentsSD->cd();
  simComponentsSD->SetLogy();
  // SDmg->Add(eSimLDFSD);
  // SDmg->Add(pSimLDFSD);
  // SDmg->Add(mSimLDFSD);
  SDmg->Add(eSimLDFSD);
  SDmg->Add(pSimLDFSD);
  SDmg->Add(mSimLDFSD);
  SDmg->Draw("AP");
  SDmg->SetMinimum(0.1);
  SDmg->SetMaximum(15000);
  SDmg->GetXaxis()->SetLimits(0,4000);
  SDmg->GetXaxis()->SetTitle("r (m)");
  SDmg->SetTitle("Lateral Distribution Function for components (SD Sim)");
  SDmg->Write();
  simComponentsSD->Write();

  TCanvas *simComponentsSDnum = new TCanvas("ComponentsSDnum", "ComponentsSD-Simnum", 1920, 1000);
  TMultiGraph *SDmgnum = new TMultiGraph();
  simComponentsSDnum->cd();
  simComponentsSDnum->SetLogy();
  SDmgnum->Add(eSimLDFSDnum);
  SDmgnum->Add(pSimLDFSDnum);
  SDmgnum->Add(mSimLDFSDnum);
  // SDmgnum->Add(eSimLDFSDnumprof);
  // SDmgnum->Add(pSimLDFSDnumprof);
  // SDmgnum->Add(mSimLDFSDnumprof);
  SDmgnum->Draw("AP");
  SDmgnum->SetMinimum(0.1);
  SDmgnum->SetMaximum(15000);
  SDmgnum->GetXaxis()->SetLimits(0,4000);
  SDmgnum->GetXaxis()->SetTitle("r (m)");
  SDmgnum->SetTitle("Lateral Distribution Function for num components (SD Sim)");
  SDmgnum->Write();
  simComponentsSDnum->Write();

  TCanvas *simComponentsSSD = new TCanvas("ComponentsSSD", "ComponentsSSD-Sim", 1920, 1000);
  TMultiGraph *SSDmg = new TMultiGraph();
  simComponentsSSD->cd();
  simComponentsSSD->SetLogy();
  // SSDmg->Add(eSimLDFSSD);
  // SSDmg->Add(pSimLDFSSD);
  // SSDmg->Add(mSimLDFSSD);
  SSDmg->Add(eSimLDFSSD);
  SSDmg->Add(pSimLDFSSD);
  SSDmg->Add(mSimLDFSSD);
  SSDmg->Draw("AP");
  SSDmg->SetMinimum(0.1);
  SSDmg->SetMaximum(15000);
  SSDmg->GetXaxis()->SetLimits(0,4000);
  SSDmg->GetXaxis()->SetTitle("r (m)");
  SSDmg->SetTitle("Lateral Distribution Function for components (SD Sim)");
  SSDmg->Write();
  simComponentsSSD->Write();
  
  TCanvas *simComponentsSSDnum = new TCanvas("ComponentsSSDnum", "ComponentsSSD-Simnum", 1920, 1000);
  TMultiGraph *SSDmgnum = new TMultiGraph();
  simComponentsSSDnum->cd();
  simComponentsSSDnum->SetLogy();
  SSDmgnum->Add(eSimLDFSSDnum);
  SSDmgnum->Add(pSimLDFSSDnum);
  SSDmgnum->Add(mSimLDFSSDnum);
  SSDmgnum->Draw("AP");
  SSDmgnum->SetMinimum(0.1);
  SSDmgnum->SetMaximum(15000);
  SSDmgnum->GetXaxis()->SetLimits(0,4000);
  SSDmgnum->GetXaxis()->SetTitle("r (m)");
  SSDmgnum->SetTitle("Lateral Distribution Function for components (SD Sim)");
  SSDmgnum->Write();
  simComponentsSSDnum->Write();

  
  TCanvas *simComponentsSSDnumprof = new TCanvas("ComponentsSSDnumprof", "ComponentsSSD-SimNumProf", 1920, 1000);
  simComponentsSSDnumprof->cd();
  simComponentsSSDnumprof->SetLogy();
  eSimLDFSSDnumprof->Draw("LP");
  pSimLDFSSDnumprof->Draw("SAME");
  mSimLDFSSDnumprof->Draw("SAME");
  eSimLDFSSDnumprof->SetMinimum(0.001);
  eSimLDFSSDnumprof->SetMaximum(1500000);
  eSimLDFSSDnumprof->SetStats(false);
  eSimLDFSSDnumprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSSDnumprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSSDnumprof->GetYaxis()->SetTitle("Number of Particles");
  eSimLDFSSDnumprof->SetTitle("Lateral Distribution Function for components (SSD Sim)");

  TLegend* legsimComponentsSSDnumprof = new TLegend(0.1, 0.7, 0.48, 0.9);
  legsimComponentsSSDnumprof->AddEntry(eSimLDFSSDnumprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSSDnumprof->AddEntry(pSimLDFSSDnumprof, "#gamma", "p");
  legsimComponentsSSDnumprof->AddEntry(mSimLDFSSDnumprof, "#mu", "p");
  legsimComponentsSSDnumprof->SetBorderSize(0);
  legsimComponentsSSDnumprof->Draw("SAME");
  
  simComponentsSSDnumprof->Write();

  TCanvas *simComponentsSSDprof = new TCanvas("ComponentsSSDprof", "ComponentsSSD-SimProf", 1920, 1000);
  simComponentsSSDprof->cd();
  simComponentsSSDprof->SetLogy();
  eSimLDFSSDprof->Draw("LP");
  pSimLDFSSDprof->Draw("SAME");
  mSimLDFSSDprof->Draw("SAME");
  eSimLDFSSDprof->SetMinimum(0.001);
  eSimLDFSSDprof->SetMaximum(15000);
  eSimLDFSSDprof->SetStats(false);
  eSimLDFSSDprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSSDprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSSDprof->GetYaxis()->SetTitle("SSD signal (MIP)");
  eSimLDFSSDprof->SetTitle("Lateral Distribution Function for components (SSD Sim)");

  TLegend* legsimComponentsSSDprof = new TLegend(0.1, 0.7, 0.48, 0.9);
  legsimComponentsSSDprof->AddEntry(eSimLDFSSDprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSSDprof->AddEntry(pSimLDFSSDprof, "#gamma", "p");
  legsimComponentsSSDprof->AddEntry(mSimLDFSSDprof, "#mu", "p");
  legsimComponentsSSDprof->SetBorderSize(0);
  legsimComponentsSSDprof->Draw("SAME");

  simComponentsSSDprof->Write();
  
  TCanvas *simComponentsSDnumprof = new TCanvas("ComponentsSDnumprof", "ComponentsSD-SimNumProf", 1920, 1000);
  simComponentsSDnumprof->cd();
  simComponentsSDnumprof->SetLogy();
  eSimLDFSDnumprof->Draw("LP");
  pSimLDFSDnumprof->Draw("SAME");
  mSimLDFSDnumprof->Draw("SAME");
  eSimLDFSDnumprof->SetMinimum(0.001);
  eSimLDFSDnumprof->SetMaximum(1500000);
  eSimLDFSDnumprof->SetStats(false);
  eSimLDFSDnumprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSDnumprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSDnumprof->GetYaxis()->SetTitle("Number of Particles");
  eSimLDFSDnumprof->SetTitle("Lateral Distribution Function for components (WCD Sim)");

  TLegend* legsimComponentsSDnumprof = new TLegend(0.1, 0.7, 0.48, 0.9);
  legsimComponentsSDnumprof->AddEntry(eSimLDFSDnumprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSDnumprof->AddEntry(pSimLDFSDnumprof, "#gamma", "p");
  legsimComponentsSDnumprof->AddEntry(mSimLDFSDnumprof, "#mu", "p");
  legsimComponentsSDnumprof->SetBorderSize(0);
  legsimComponentsSDnumprof->Draw("SAME");
  
  simComponentsSDnumprof->Write();

  TCanvas *simComponentsSDprof = new TCanvas("ComponentsSDprof", "ComponentsSD-SimProf", 1920, 1000);
  simComponentsSDprof->cd();
  simComponentsSDprof->SetLogy();
  eSimLDFSDprof->Draw("LP");
  pSimLDFSDprof->Draw("SAME");
  mSimLDFSDprof->Draw("SAME");
  eSimLDFSDprof->SetMinimum(0.001);
  eSimLDFSDprof->SetMaximum(15000);
  eSimLDFSDprof->SetStats(false);
  eSimLDFSDprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSDprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSDprof->GetYaxis()->SetTitle("WCD signal (VEM)");
  eSimLDFSDprof->SetTitle("Lateral Distribution Function for components (WCD Sim)");


  TLegend* legsimComponentsSDprof = new TLegend(0.1, 0.7, 0.48, 0.9);
  legsimComponentsSDprof->AddEntry(eSimLDFSDprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSDprof->AddEntry(pSimLDFSDprof, "#gamma", "p");
  legsimComponentsSDprof->AddEntry(mSimLDFSDprof, "#mu", "p");
  legsimComponentsSDprof->SetBorderSize(0);
  legsimComponentsSDprof->Draw("SAME");

  simComponentsSDprof->Write();
  simTrace->Write();



  TFile *Hists2D = new TFile("/remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/2Dhist.root");
  if (Hists2D->IsOpen() )
    {
      printf("2DHIST file opened successfully\n");

      TH2* h2 = (TH2F*)Hists2D->Get("h2");
      TH2* h2_1 = (TH2F*)Hists2D->Get("h2_1");
      TH2* h2_2 = (TH2F*)Hists2D->Get("h2_2");
      TH2* h1 = (TH2F*)Hists2D->Get("h1");
      TH2* h1_1 = (TH2F*)Hists2D->Get("h1_1");
      TGraph* showerDirection = (TGraph*)Hists2D->Get("");

	//	TGraph *LDFSD = (TGraph*)Trace->Get("LDFSD");

      TCanvas *canvas = new TCanvas("2DHists", "2DHists", 1000,1000);
      TFile *PlanesLDF = new TFile("PlanesLDF.root","RECREATE");
      TPad pad1("Surface Plot","Ground Particles", 0.01, 0.99,0.5 ,0.5);
      TPad pad2("Surface Plot2","Shower plane particles", 0.51, 0.99, 0.99, 0.5);
      TPad *pad3 = new TPad("LDF", "SSD", 0.01, 0.01, 0.99, 0.5);
      pad1.Draw();
      pad1.cd();
      h2_1->Draw("");
      h2_1->SetStats(false);
      h2->Draw("SAME");
      h2_2->Draw("SAME");
      showerDirection->Draw("SAME");
      showerDirection->SetLineColor(kGreen);
      h2_1->SetMarkerColor(kBlue);
      h2_2->SetMarkerColor(kRed);
      canvas->cd();
      pad2.Draw();
      pad2.cd();
      h1->Draw("");
      h1->SetStats(false);
      h1_1->Draw("SAME");
      h1_1->SetMarkerColor(kRed);
      canvas->cd();
      pad3->Draw();
      pad3->cd();
      pad3->SetLogy();
      pad3->SetTitle("LDF-SSD");
      mg2->Draw("AP*");
      leg2->Draw();

      canvas->Write();
      PlanesLDF->Write();


      
    }
  
  return 0;
}
