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
#include "TLegendEntry.h"
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

auto simLDFSDprof = new TProfile("simLDFSDprof", "Profile of SD", 1000, 0, 4000);
auto simLDFSDnumprof = new TProfile("simLDFSDnumprof", "Profile of SD number", 1000, 0, 4000);
auto simLDFSSDprof = new TProfile("simLDFSSDprof", "Profile of SSD", 1000, 0, 4000);
auto simLDFSSDnumprof = new TProfile("simLDFSSDnumprof", "Profile ,of SSD number", 1000, 0, 4000);



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
  dir =  "/remote/tesla/bmanning/data/SimulationData/qgsII4/proton/19/38deg";
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
	    pSimLDFSSD->SetPoint(pSimLDFSSD->GetN(), tank_pos, num);
	    pSimLDFSSDnum->SetPoint(pSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    pSSDSum.push_back(num);
	    pSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    pSimLDFSSDprof->Fill(tank_pos, num);
	    
	  } else if (file.find("muon") != std::string::npos) {
	    mSimLDFSSD->SetPoint(mSimLDFSSD->GetN(), tank_pos, num);
	    mSimLDFSSDnum->SetPoint(mSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    mSSDSum.push_back(num);
	    mSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    mSimLDFSSDprof->Fill(tank_pos, num);
	  } else if (file.find("electron") != std::string::npos) {
	    eSimLDFSSD->SetPoint(eSimLDFSSD->GetN(), tank_pos, num);
	    eSimLDFSSDnum->SetPoint(eSimLDFSSDnum->GetN(), tank_pos, nParticles);
	    eSSDSum.push_back(num);
	    eSimLDFSSDnumprof->Fill(tank_pos, nParticles);
	    cout << nParticles+1 << " " << tank_pos << " " << num << endl;
	    eSimLDFSSDprof->Fill(tank_pos, num);

	    //  cout << tank_pos << " " << nParticles << endl;
	  } else
	    {
	      simLDFSSD->SetPoint(simLDFSSD->GetN(), tank_pos, num);
	      simLDFSSDnum->SetPoint(simLDFSSDnum->GetN(), tank_pos, nParticles);
	      simLDFSSDprof->Fill(tank_pos, num);
	      simLDFSSDnumprof->Fill(tank_pos, nParticles);
	    }
	
	 	  

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
	  simLDFSDprof->Fill(tank_pos, num);
	  simLDFSDnumprof->Fill(tank_pos, nParticles);
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
      
  simLDFSSD->SetMarkerColor(kBlack); simLDFSSD->SetMarkerStyle(24);
  simLDFSD->SetMarkerColor(kBlack); simLDFSD->SetMarkerStyle(20);
  eSimLDFSD->SetMarkerColor(kRed);  eSimLDFSD->SetMarkerStyle(21);
  eSimLDFSSD->SetMarkerColor(kRed); eSimLDFSSD->SetMarkerStyle(25);
  pSimLDFSD->SetMarkerColor(kGreen); pSimLDFSD->SetMarkerStyle(22);
  pSimLDFSSD->SetMarkerColor(kGreen); pSimLDFSSD->SetMarkerStyle(26);
  mSimLDFSD->SetMarkerColor(kBlue); mSimLDFSD->SetMarkerStyle(23);
  mSimLDFSSD->SetMarkerColor(kBlue); mSimLDFSSD->SetMarkerStyle(32);
  simLDFSSDnum->SetMarkerColor(kBlack); simLDFSSDnum->SetMarkerStyle(24);
  simLDFSDnum->SetMarkerColor(kBlack); simLDFSDnum->SetMarkerStyle(20);
  eSimLDFSDnum->SetMarkerColor(kRed); eSimLDFSDnum->SetMarkerStyle(21);
  eSimLDFSSDnum->SetMarkerColor(kRed); eSimLDFSSDnum->SetMarkerStyle(25);
  pSimLDFSDnum->SetMarkerColor(kGreen); pSimLDFSDnum->SetMarkerStyle(22);
  pSimLDFSSDnum->SetMarkerColor(kGreen); pSimLDFSSDnum->SetMarkerStyle(26);
  mSimLDFSDnum->SetMarkerColor(kBlue); mSimLDFSDnum->SetMarkerStyle(23);
  mSimLDFSSDnum->SetMarkerColor(kBlue); mSimLDFSSDnum->SetMarkerStyle(32);

  eSimLDFSDprof->SetMarkerColor(kRed);  eSimLDFSDprof->SetMarkerStyle(21);
  eSimLDFSSDprof->SetMarkerColor(kRed); eSimLDFSSDprof->SetMarkerStyle(25);
  pSimLDFSDprof->SetMarkerColor(kGreen); pSimLDFSDprof->SetMarkerStyle(22);
  pSimLDFSSDprof->SetMarkerColor(kGreen); pSimLDFSSDprof->SetMarkerStyle(26);
  mSimLDFSDprof->SetMarkerColor(kBlue); mSimLDFSDprof->SetMarkerStyle(23);
  mSimLDFSSDprof->SetMarkerColor(kBlue); mSimLDFSSDprof->SetMarkerStyle(32);
  
  eSimLDFSDnumprof->SetMarkerColor(kRed); eSimLDFSDnumprof->SetMarkerStyle(21);
  eSimLDFSSDnumprof->SetMarkerColor(kRed); eSimLDFSSDnumprof->SetMarkerStyle(25);
  pSimLDFSDnumprof->SetMarkerColor(kGreen); pSimLDFSDnumprof->SetMarkerStyle(22);
  pSimLDFSSDnumprof->SetMarkerColor(kGreen); pSimLDFSSDnumprof->SetMarkerStyle(26);
  mSimLDFSDnumprof->SetMarkerColor(kBlue); mSimLDFSDnumprof->SetMarkerStyle(23);
  mSimLDFSSDnumprof->SetMarkerColor(kBlue); mSimLDFSSDnumprof->SetMarkerStyle(32);

  simLDFSDprof->SetMarkerColor(kBlack); simLDFSDprof->SetMarkerStyle(20);
  simLDFSDnumprof->SetMarkerColor(kBlack); simLDFSDnumprof->SetMarkerStyle(20);
  simLDFSSDprof->SetMarkerColor(kBlack); simLDFSSDprof->SetMarkerStyle(24);
  simLDFSSDnumprof->SetMarkerColor(kBlack); simLDFSSDnumprof->SetMarkerStyle(24);
  
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
  simLDFSDfunc->Draw("e3");
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
  TGraph *eLDFSD = (TGraph*)Trace->Get("eLDFSD");
  TGraph *pLDFSD = (TGraph*)Trace->Get("pLDFSD");
  TGraph *mLDFSD = (TGraph*)Trace->Get("mLDFSD");
  TGraph *eLDFSSD = (TGraph*)Trace->Get("eLDF");
  TGraph *pLDFSSD = (TGraph*)Trace->Get("pLDF");
  TGraph *mLDFSSD = (TGraph*)Trace->Get("mLDF");
  
  TCanvas *c = new TCanvas("All4", "All4", 1920,1000);
  mg->Add(simLDFSSD);
  mg->Add(simLDFSD);
  mg->Add(LDFSD);
  mg->Add(LDFSSD);
  mg->Draw("AP");
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
  mg2->Draw("AP");
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
  mg3->Draw("AP");
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
  SDmg->Add(simLDFSD);
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
  SDmgnum->Add(simLDFSDnum);
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
  SSDmg->Add(simLDFSSD);
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
  SSDmgnum->Add(simLDFSSDnum);
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
  simLDFSSDnumprof->Draw("SAME");
  eSimLDFSSDnumprof->SetMinimum(0.001);
  eSimLDFSSDnumprof->SetMaximum(1500000);
  eSimLDFSSDnumprof->SetStats(false);
  eSimLDFSSDnumprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSSDnumprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSSDnumprof->GetYaxis()->SetTitle("Number of Particles");
  eSimLDFSSDnumprof->SetTitle("Lateral Distribution Function for components (SSD Sim)");

  TLegend* legsimComponentsSSDnumprof = new TLegend(0.8, 0.6, 0.9, 0.8);
  legsimComponentsSSDnumprof->AddEntry(eSimLDFSSDnumprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSSDnumprof->AddEntry(pSimLDFSSDnumprof, "#gamma", "p");
  legsimComponentsSSDnumprof->AddEntry(mSimLDFSSDnumprof, "#mu", "p");
  legsimComponentsSSDnumprof->AddEntry(simLDFSSDnumprof, "Total", "p");
  legsimComponentsSSDnumprof->SetBorderSize(0);
  legsimComponentsSSDnumprof->Draw("SAME");
  
  simComponentsSSDnumprof->Write();

  TCanvas *simComponentsSSDprof = new TCanvas("ComponentsSSDprof", "ComponentsSSD-SimProf", 1920, 1000);
  simComponentsSSDprof->cd();
  simComponentsSSDprof->SetLogy();
  eSimLDFSSDprof->Draw("LP");
  pSimLDFSSDprof->Draw("SAME");
  mSimLDFSSDprof->Draw("SAME");
  simLDFSSDprof->Draw("SAME");
  eSimLDFSSDprof->SetMinimum(0.001);
  eSimLDFSSDprof->SetMaximum(15000);
  eSimLDFSSDprof->SetStats(false);
  eSimLDFSSDprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSSDprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSSDprof->GetYaxis()->SetTitle("SSD signal (MIP)");
  eSimLDFSSDprof->SetTitle("Lateral Distribution Function for components (SSD Sim)");

  TLegend* legsimComponentsSSDprof = new TLegend(0.8, 0.6, 0.9, 0.8);
  legsimComponentsSSDprof->AddEntry(eSimLDFSSDprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSSDprof->AddEntry(pSimLDFSSDprof, "#gamma", "p");
  legsimComponentsSSDprof->AddEntry(mSimLDFSSDprof, "#mu", "p");
  legsimComponentsSSDprof->AddEntry(simLDFSSDprof, "Total", "p");
  legsimComponentsSSDprof->SetBorderSize(0);
  legsimComponentsSSDprof->Draw("SAME");

  simComponentsSSDprof->Write();
  
  TCanvas *simComponentsSDnumprof = new TCanvas("ComponentsSDnumprof", "ComponentsSD-SimNumProf", 1920, 1000);
  simComponentsSDnumprof->cd();
  simComponentsSDnumprof->SetLogy();
  eSimLDFSDnumprof->Draw("LP");
  pSimLDFSDnumprof->Draw("SAME");
  mSimLDFSDnumprof->Draw("SAME");
  simLDFSDnumprof->Draw("SAME");
  eSimLDFSDnumprof->SetMinimum(0.001);
  eSimLDFSDnumprof->SetMaximum(1500000);
  eSimLDFSDnumprof->SetStats(false);
  eSimLDFSDnumprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSDnumprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSDnumprof->GetYaxis()->SetTitle("Number of Particles");
  eSimLDFSDnumprof->SetTitle("Lateral Distribution Function for components (WCD Sim)");

  TLegend* legsimComponentsSDnumprof = new TLegend(0.7, 0.6, 0.8, 0.8);
  legsimComponentsSDnumprof->AddEntry(eSimLDFSDnumprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSDnumprof->AddEntry(pSimLDFSDnumprof, "#gamma", "p");
  legsimComponentsSDnumprof->AddEntry(mSimLDFSDnumprof, "#mu", "p");
  legsimComponentsSDnumprof->AddEntry(simLDFSDnumprof, "Total", "p");
  legsimComponentsSDnumprof->SetBorderSize(0);
  legsimComponentsSDnumprof->Draw("SAME");
  
  simComponentsSDnumprof->Write();

  TCanvas *simComponentsSDprof = new TCanvas("ComponentsSDprof", "ComponentsSD-SimProf", 1920, 1000);
  simComponentsSDprof->cd();
  simComponentsSDprof->SetLogy();
  eSimLDFSDprof->Draw("LP");
  pSimLDFSDprof->Draw("SAME");
  mSimLDFSDprof->Draw("SAME");
  simLDFSDprof->Draw("SAME");
  eSimLDFSDprof->SetMinimum(0.001);
  eSimLDFSDprof->SetMaximum(15000);
  eSimLDFSDprof->SetStats(false);
  eSimLDFSDprof->GetXaxis()->SetLimits(0,4000);
  eSimLDFSDprof->GetXaxis()->SetTitle("r (m)");
  eSimLDFSDprof->GetYaxis()->SetTitle("WCD signal (VEM)");
  eSimLDFSDprof->SetTitle("Lateral Distribution Function for components (WCD Sim)");


  TLegend* legsimComponentsSDprof = new TLegend(0.7, 0.6, 0.8, 0.8);
  legsimComponentsSDprof->AddEntry(eSimLDFSDprof, "e^{+} / e^{-}" , "p");
  legsimComponentsSDprof->AddEntry(pSimLDFSDprof, "#gamma", "p");
  legsimComponentsSDprof->AddEntry(mSimLDFSDprof, "#mu", "p");
  legsimComponentsSDprof->AddEntry(simLDFSDprof, "Total", "p");
  legsimComponentsSDprof->SetBorderSize(0);
  legsimComponentsSDprof->Draw("SAME");

  simComponentsSDprof->Write();

  TCanvas* CombinedPlot = new TCanvas("combinedPlot", "combinedPlot", 800,600);
  TPad pad2("WCD", "VEM", 0.5, 0.99, 0.99, 0.5);
  TPad pad4("SSD", "MIP", 0.5, 0.5, 0.99, 0);
  TPad pad1("WCD", "nParticles", 0.01, 0.99, 0.5, 0.5);
  TPad pad3("SSD", "nParticles", 0.01, 0.5, 0.5, 0);
  CombinedPlot->Draw();

  pad1.Draw();
  pad1.cd();

  pad1.SetLogy();
  eSimLDFSDnumprof->Draw("LP");
  pSimLDFSDnumprof->Draw("SAME");
  mSimLDFSDnumprof->Draw("SAME");
  simLDFSDnumprof->Draw("SAME");
  eSimLDFSDnumprof->SetTitle("");
  legsimComponentsSDprof->Draw("SAME");
  CombinedPlot->cd();

  pad2.Draw();
  pad2.cd();
  pad2.SetLogy();
  eSimLDFSDprof->Draw("LP");
  pSimLDFSDprof->Draw("SAME");
  mSimLDFSDprof->Draw("SAME");
  simLDFSDprof->Draw("SAME");
  eSimLDFSDprof->SetTitle("");
  legsimComponentsSDprof->Draw("SAME");
  CombinedPlot->cd();
  pad3.Draw();
  pad3.cd();
  pad3.SetLogy();
  eSimLDFSSDnumprof->Draw("LP");
  pSimLDFSSDnumprof->Draw("SAME");
  mSimLDFSSDnumprof->Draw("SAME");
  simLDFSSDnumprof->Draw("SAME");
  eSimLDFSSDnumprof->SetTitle("");
  legsimComponentsSSDprof->Draw("SAME");
  CombinedPlot->cd();
  pad4.Draw();
  pad4.cd();
  pad4.SetLogy();
  eSimLDFSSDprof->Draw("LP");
  pSimLDFSSDprof->Draw("SAME");
  mSimLDFSSDprof->Draw("SAME");
  simLDFSSDprof->Draw("SAME");
  eSimLDFSSDprof->SetTitle("");

  legsimComponentsSSDprof->Draw("SAME");
  CombinedPlot->Write();

  TCanvas* CombinedPlot2 = new TCanvas("combinedPlot2", "combinedPlot2", 800,600);
  TPad pad2_1("Signal", "VEM/MIP", 0.01, 0.5, 0.99, 0);
  TPad pad1_1("nParticles", "WCD/SSD", 0.01, 0.99, 0.99, 0.5);


  pad1_1.Draw();
  pad1_1.cd();
  pad1_1.SetLogy();
  eSimLDFSDnumprof->Draw("LP");
  pSimLDFSDnumprof->Draw("SAME");
  mSimLDFSDnumprof->Draw("SAME");
  simLDFSDnumprof->Draw("SAME");

  eSimLDFSSDnumprof->Draw("SAME");
  pSimLDFSSDnumprof->Draw("SAME");
  mSimLDFSSDnumprof->Draw("SAME");
  simLDFSSDnumprof->Draw("SAME");

  legsimComponentsSDprof->SetHeader("WCD");
  legsimComponentsSSDprof->SetHeader("SSD");
  TLegendEntry *header = (TLegendEntry*)legsimComponentsSDprof->GetListOfPrimitives()->First();
  header->SetTextAlign(22);
  TLegendEntry *header2 = (TLegendEntry*)legsimComponentsSSDprof->GetListOfPrimitives()->First();
  header2->SetTextAlign(22);
  legsimComponentsSDprof->Draw("SAME");
  legsimComponentsSSDprof->Draw("SAME");
  CombinedPlot2->cd();
  pad2_1.Draw();
  pad2_1.cd();
  pad2_1.SetLogy();
  eSimLDFSSDprof->Draw("LP");
  pSimLDFSSDprof->Draw("SAME");
  mSimLDFSSDprof->Draw("SAME");
  simLDFSSDprof->Draw("SAME");

  eSimLDFSDprof->Draw("SAME");
  pSimLDFSDprof->Draw("SAME");
  mSimLDFSDprof->Draw("SAME");
  simLDFSDprof->Draw("SAME");
  legsimComponentsSDprof->Draw("SAME");
  legsimComponentsSSDprof->Draw("SAME");
  
  CombinedPlot2->Write();

  TCanvas* CombinedPlot3 = new TCanvas("combinedPlot3", "combinedPlot3", 800,600);
  TPad pad2_2("Signal", "MIP", 0.01, 0.5, 0.99, 0);
  TPad pad1_2("Signal", "VEM", 0.01, 0.99, 0.99, 0.5);

  TMultiGraph *mg4 = new TMultiGraph();
  TMultiGraph *mg5 = new TMultiGraph();
  
  CombinedPlot3->cd();
  pad1_2.Draw();
  pad1_2.cd();
  pad1_2.SetLogy();
  mg4->Add(eSimLDFSD);
  mg4->Add(pSimLDFSD);
  mg4->Add(mSimLDFSD);
  //simLDFSDnum->Draw("SAME");
  mg4->Add(eLDFSD);
  mg4->Add(pLDFSD);
  mg4->Add(mLDFSD);

  eLDFSD->SetMarkerSize(1);   pLDFSD->SetMarkerSize(1);   mLDFSD->SetMarkerSize(1); 
  eLDFSD->SetMarkerStyle(29);  pLDFSD->SetMarkerStyle(29);  mLDFSD->SetMarkerStyle(29);
  
  mg4->Draw("AP");
  mg4->SetMinimum(0.1);
  mg4->SetMaximum(15000);
  mg4->GetYaxis()->SetTitle("Signal (VEM)");
  mg4->GetXaxis()->SetLimits(0,4000);
  mg4->GetXaxis()->SetTitle("r (m)");
  mg4->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");


  
  CombinedPlot3->cd();
  pad2_2.Draw();
  pad2_2.cd();
  pad2_2.SetLogy();

  mg5->Add(eSimLDFSSD);
  mg5->Add(pSimLDFSSD);
  mg5->Add(mSimLDFSSD);
  //simLDFSSDnum->Draw("SAME");
  mg5->Add(eLDFSSD);
  mg5->Add(pLDFSSD);
  mg5->Add(mLDFSSD);

  eLDFSSD->SetMarkerSize(1);   pLDFSSD->SetMarkerSize(1);   mLDFSSD->SetMarkerSize(1); 
  eLDFSSD->SetMarkerStyle(30);  pLDFSSD->SetMarkerStyle(30);  mLDFSSD->SetMarkerStyle(30);
  
  mg5->Draw("AP");
  mg5->SetMinimum(0.1);
  mg5->SetMaximum(15000);
  mg5->GetYaxis()->SetTitle("Signal (MIP)");
  mg5->GetXaxis()->SetLimits(0,4000);
  mg5->GetXaxis()->SetTitle("r (m)");
  mg5->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");

  CombinedPlot3->Write();
  
  TCanvas* GammaOffLineMyG4 = new TCanvas("GammaOfflineMyG4", "GammaOfflineMyG4", 800,600);
  TPad pad2_3("Signal", "MIP", 0.01, 0.5, 0.99, 0);
  TPad pad1_3("Signal", "VEM", 0.01, 0.99, 0.99, 0.5);
  TMultiGraph *mg6 = new TMultiGraph();
  TMultiGraph *mg7 = new TMultiGraph();
  pad1_3.Draw();
  pad1_3.cd();
  pad1_3.SetLogy(); 
  mg6->Add(pSimLDFSD);
  mg6->Add(pLDFSD);
  mg6->Draw("AP");
  mg6->SetMinimum(0.001);
  mg6->SetMaximum(15000);
  mg6->GetYaxis()->SetTitle("Signal (VEM)");
  mg6->GetXaxis()->SetLimits(0,4000);
  mg6->GetXaxis()->SetTitle("r (m)");
  mg6->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg6->Write("GammaOffLineMyG4_WCD");

  TLegend* GammaComponentsSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  GammaComponentsSD->AddEntry(pSimLDFSD, "#gamma, My G4", "p");
  GammaComponentsSD->AddEntry(pLDFSD, "#gamma, OffLine", "p");
  GammaComponentsSD->SetBorderSize(0);
  GammaComponentsSD->Draw("SAME");

  
  GammaOffLineMyG4->cd();
  pad2_3.Draw();
  pad2_3.cd();
  pad2_3.SetLogy();

  mg7->Add(pSimLDFSSD);
  mg7->Add(pLDFSSD);
  mg7->Draw("AP");
  mg7->SetMinimum(0.001);
  mg7->SetMaximum(15000);
  mg7->GetYaxis()->SetTitle("Signal (MIP)");
  mg7->GetXaxis()->SetLimits(0,4000);
  mg7->GetXaxis()->SetTitle("r (m)");
  mg7->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg7->Write("GammaOffLineMyG4_SSD");
  TLegend* GammaComponentsSSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  GammaComponentsSSD->AddEntry(pSimLDFSSD, "#gamma, My G4", "p");
  GammaComponentsSSD->AddEntry(pLDFSSD, "#gamma, OffLine", "p");
  GammaComponentsSSD->SetBorderSize(0);
  GammaComponentsSSD->Draw("SAME");
  GammaOffLineMyG4->Write();

  TCanvas* ElectronOffLineMyG4 = new TCanvas("ElectronOffLineMyG4", "ElectronOffLineMyG4", 800,600);
  
  TPad pad2_4("Signal", "MIP", 0.01, 0.5, 0.99, 0);
  TPad pad1_4("Signal", "VEM", 0.01, 0.99, 0.99, 0.5);
  TMultiGraph *mg8 = new TMultiGraph();
  TMultiGraph *mg9 = new TMultiGraph();
  pad1_4.Draw();
  pad1_4.cd();
  pad1_4.SetLogy(); 
  mg8->Add(eSimLDFSD);
  mg8->Add(eLDFSD);
  mg8->Draw("AP");
  mg8->SetMinimum(0.001);
  mg8->SetMaximum(15000);
  mg8->GetYaxis()->SetTitle("Signal (VEM)");
  mg8->GetXaxis()->SetLimits(0,4000);
  mg8->GetXaxis()->SetTitle("r (m)");
  mg8->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg8->Write("ElectronOffLineMyG4_WCD");

  TLegend* ElectronComponentsSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  ElectronComponentsSD->AddEntry(eSimLDFSD, "e^{+} / e^{-}, My G4", "p");
  ElectronComponentsSD->AddEntry(eLDFSD, "e^{+} / e^{-}, OffLine", "p");
  ElectronComponentsSD->SetBorderSize(0);
  ElectronComponentsSD->Draw("SAME");

  ElectronOffLineMyG4->cd();
  pad2_4.Draw();
  pad2_4.cd();
  pad2_4.SetLogy();

  mg9->Add(eSimLDFSSD);
  mg9->Add(eLDFSSD);
  mg9->Draw("AP");
  mg9->SetMinimum(0.001);
  mg9->SetMaximum(15000);
  mg9->GetYaxis()->SetTitle("Signal (MIP)");
  mg9->GetXaxis()->SetLimits(0,4000);
  mg9->GetXaxis()->SetTitle("r (m)");
  mg9->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg9->Write("ElectronOffLineMyG4_SSD");
  TLegend* ElectronComponentsSSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  ElectronComponentsSSD->AddEntry(eSimLDFSSD, "e^{+} / e^{-}, My G4", "p");
  ElectronComponentsSSD->AddEntry(eLDFSSD, "e^{+} / e^{-}, OffLine", "p");
  ElectronComponentsSSD->SetBorderSize(0);
  ElectronComponentsSSD->Draw("SAME");
  ElectronOffLineMyG4->Write();

  TCanvas* MuonOffLineMyG4 = new TCanvas("MuonOffLineMyG4", "MuonOffLineMyG4", 800,600);
  TPad pad2_5("Signal", "MIP", 0.01, 0.5, 0.99, 0);
  TPad pad1_5("Signal", "VEM", 0.01, 0.99, 0.99, 0.5);
  TMultiGraph *mg10 = new TMultiGraph();
  TMultiGraph *mg11 = new TMultiGraph();
  pad1_5.Draw();
  pad1_5.cd();
  pad1_5.SetLogy(); 
  mg10->Add(mSimLDFSD);
  mg10->Add(mLDFSD);
  mg10->Draw("AP");
  mg10->SetMinimum(0.001);
  mg10->SetMaximum(15000);
  mg10->GetYaxis()->SetTitle("Signal (VEM)");
  mg10->GetXaxis()->SetLimits(0,4000);
  mg10->GetXaxis()->SetTitle("r (m)");
  mg10->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg10->Write("MuonOffLineMyG4_WCD");

  TLegend* MuonComponentsSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  MuonComponentsSD->AddEntry(mSimLDFSD, "#mu, My G4", "p");
  MuonComponentsSD->AddEntry(mLDFSD, "#mu, OffLine", "p");
  MuonComponentsSD->SetBorderSize(0);
  MuonComponentsSD->Draw("SAME");
  
  MuonOffLineMyG4->cd();
  pad2_5.Draw();
  pad2_5.cd();
  pad2_5.SetLogy();

  mg11->Add(mSimLDFSSD);
  mg11->Add(mLDFSSD);
  mg11->Draw("AP");
  mg11->SetMinimum(0.001);
  mg11->SetMaximum(15000);
  mg11->GetYaxis()->SetTitle("Signal (MIP)");
  mg11->GetXaxis()->SetLimits(0,4000);
  mg11->GetXaxis()->SetTitle("r (m)");
  mg11->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  mg11->Write("MuonOffLineMyG4_SSD");

  TLegend* MuonComponentsSSD = new TLegend(0.7, 0.6, 0.8, 0.8);
  MuonComponentsSSD->AddEntry(mSimLDFSSD, "#mu, My G4", "p");
  MuonComponentsSSD->AddEntry(mLDFSSD, "#mu, OffLine", "p");
  MuonComponentsSSD->SetBorderSize(0);
  MuonComponentsSSD->Draw("SAME");  

  MuonOffLineMyG4->Write();








  simTrace->Write();





  

  // TFile *Hists2D = new TFile("/remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/2Dhist.root");
  // if (Hists2D->IsOpen() )
  //   {
  //     printf("2DHIST file opened successfully\n");

  //     TH2* h2 = (TH2F*)Hists2D->Get("h2");
  //     TH2* h2_1 = (TH2F*)Hists2D->Get("h2_1");
  //     TH2* h2_2 = (TH2F*)Hists2D->Get("h2_2");
  //     TH2* h1 = (TH2F*)Hists2D->Get("h1");
  //     TH2* h1_1 = (TH2F*)Hists2D->Get("h1_1");
  //     TGraph* showerDirection = (TGraph*)Hists2D->Get("");

  // 	//	TGraph *LDFSD = (TGraph*)Trace->Get("LDFSD");

  //     TCanvas *canvas = new TCanvas("2DHists", "2DHists", 1000,1000);
  //     TFile *PlanesLDF = new TFile("PlanesLDF.root","RECREATE");
  //     TPad pad1("Surface Plot","Ground Particles", 0.01, 0.99,0.5 ,0.5);
  //     TPad pad2("Surface Plot2","Shower plane particles", 0.51, 0.99, 0.99, 0.5);
  //     TPad *pad3 = new TPad("LDF", "SSD", 0.01, 0.01, 0.99, 0.5);
  //     pad1.Draw();
  //     pad1.cd();
  //     h2_1->Draw("");
  //     h2_1->SetStats(false);
  //     h2->Draw("SAME");
  //     h2_2->Draw("SAME");
  //     showerDirection->Draw("SAME");
  //     showerDirection->SetLineColor(kGreen);
  //     h2_1->SetMarkerColor(kBlue);
  //     h2_2->SetMarkerColor(kRed);
  //     canvas->cd();
  //     pad2.Draw();
  //     pad2.cd();
  //     h1->Draw("");
  //     h1->SetStats(false);
  //     h1_1->Draw("SAME");
  //     h1_1->SetMarkerColor(kRed);
  //     canvas->cd();
  //     pad3->Draw();
  //     pad3->cd();
  //     pad3->SetLogy();
  //     pad3->SetTitle("LDF-SSD");
  //     mg2->Draw("AP*");
  //     leg2->Draw();

  //     canvas->Write();
  //     PlanesLDF->Write();


      
  //   }
  
  return 0;
}
