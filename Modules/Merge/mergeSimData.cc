// Bradley Manning 4/2018


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



int main(){
  ifstream fin;
  string dir, filepath,file;
  double num;
  DIR *dp;
  struct dirent *dirp;
  struct stat filestat;
  int nEvents = 16;
  int nFiles = 20;
  int idx = 0;

  double_t  sum[nEvents] = {0};
  double_t  error[nEvents] = {0};
  cout << "dir to get files of: " << flush;
  //getline( cin, dir );  // gets everything the user ENTERs
  dir =  "/remote/tesla/bmanning/data/SimulationData/proton/10E19/0deg";
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
    double rMIPTankPos = 164;
    const double factor = 0.1;
    double rStep = 0;
    double rMIP = rMIPTankPos;
    double prMIP = rMIPTankPos;
    double erMIP = rMIPTankPos;
    double mrMIP = rMIPTankPos;
    double rVEMTankPos = 164;
    double rVEMStep = 0;
    double rVEM = rVEMTankPos;
    double prVEM = rVEMTankPos;
    double erVEM = rVEMTankPos;
    double mrVEM = rVEMTankPos;

   
    while ( fin >> num) 
      {
	if ( file.find("MIP") != std::string::npos ){ //MIP file
	  //Define R start point and bin
	  if ( file.find("Photon") != std::string::npos) {
	    pSimLDFSSD->SetPoint(pSimLDFSSD->GetN(), prMIP, num/2);
	    rStep = (1+factor)*prMIP;
	    prMIP = rStep / (1-factor);

	  } if (file.find("muon") != std::string::npos) {
	    mSimLDFSSD->SetPoint(mSimLDFSSD->GetN(), mrMIP, num/2);
	    rStep = (1+factor)*mrMIP;
	    mrMIP = rStep / (1-factor);
	 
	  } if (file.find("electron") != std::string::npos) {
	    eSimLDFSSD->SetPoint(eSimLDFSSD->GetN(), erMIP, num/2);
	    rStep = (1+factor)*erMIP;
	    erMIP = rStep / (1-factor);

	  } if ( ( file.find("Electron") !=std::string::npos ) || ( file.find("Muon") != std::string::npos) || ( file.find("Photon") != std::string::npos ) ) {
	  } else {
	  
	    simLDFSSD->SetPoint(simLDFSSD->GetN(), rMIP, num/2);
	    rStep = (1+factor)*rMIP;
	    rMIP = rStep / (1-factor);
	
	  }
	  
	  
	  //cout << dirp->d_name << ": " << num << endl;
	  //Note MIP was not calibrated to MIP in GEANT4 (hence 2)
	  //simLDFSSD->SetPoint(simLDFSSD->GetN(), rMIP, num/2);
	  //rMIP = rMIP + rStep;
	  
	} else if ( file.find("VEM") != std::string::npos ) { //VEM file

	    
	  if ( file.find("Photon") != std::string::npos) {
	    pSimLDFSD->SetPoint(pSimLDFSD->GetN(), prVEM, num);
	    rVEMStep = (1+factor)*prVEM;
	    prVEM =  rVEMStep / (1-factor);
	  } if (file.find("Muon") != std::string::npos) {
	    mSimLDFSD->SetPoint(mSimLDFSD->GetN(), mrVEM, num);
	    rVEMStep = (1+factor)*mrVEM;
	    mrVEM = rVEMStep / (1-factor);

	  } if (file.find("Electron") != std::string::npos) {
	    eSimLDFSD->SetPoint(eSimLDFSD->GetN(), erVEM, num);
	    rVEMStep = (1+factor)*erVEM;
	    erVEM = rVEMStep / (1-factor);

	  
	  
	  }

	  if ( ( file.find("Electron") !=std::string::npos ) || ( file.find("Muon") != std::string::npos) || ( file.find("Photon") != std::string::npos ) ) {
	  } else {
	    if ( ( file.find("inject") !=std::string::npos ) ) {
	      
	      simLDFSD->SetPoint(simLDFSD->GetN(), rVEM, num);
	      rVEMStep = (1+factor)*rVEM;
	      rVEM = rVEMStep / (1-factor);
	      sum[idx % nEvents] += num;
	      
	  }
	 
	    // cout << sum[idx % nEvents] << endl;
	   
	  }
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


      
  simLDFSSD->SetMarkerColor(kBlue);
  simLDFSD->SetMarkerColor(kGreen);
  eSimLDFSD->SetMarkerColor(kRed);
  eSimLDFSSD->SetMarkerColor(kRed);
  pSimLDFSD->SetMarkerColor(kYellow);
  pSimLDFSSD->SetMarkerColor(kYellow);
  mSimLDFSD->SetMarkerColor(kBlue);
  mSimLDFSSD->SetMarkerColor(kBlue);
  simLDFSDerrors->SetMarkerColor(kRed);
  TFile *Trace = new TFile("/remote/tesla/bmanning/work/Modules/ADST_Analysis/Trace.root");
  TFile *simTrace = new TFile("MuonSimTrace.root", "RECREATE");

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
  mg2->SetTitle("Laterial Distribution Function (My G4 vs OffLine)");

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
  //mg3->Add(simLDFSD);
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
  //leg3->AddEntry(simLDFSD, "My G4 - Signal (VEM PEAK)", "p");
  leg3->Draw();

  c3->SetTitle("Lateral Distribution Function (My G4 vs OffLine)");
  c3->SetLogy();
  c3->Write();
  
  TCanvas *simComponentsSD = new TCanvas("ComponentsSD", "ComponentsSD-Sim", 1920, 1000);
  TMultiGraph *SDmg = new TMultiGraph();
  simComponentsSD->cd();
  simComponentsSD->SetLogy();
  SDmg->Add(eSimLDFSD);
  SDmg->Add(pSimLDFSD);
  SDmg->Add(mSimLDFSD);
  SDmg->Draw("AP*");
  SDmg->SetMinimum(0.1);
  SDmg->SetMaximum(15000);
  SDmg->GetXaxis()->SetLimits(0,4000);
  SDmg->GetXaxis()->SetTitle("r (m)");
  SDmg->SetTitle("Laterial Distribution Function for components (SD Sim)");
  SDmg->Write();
  simComponentsSD->Write();

  TCanvas *simComponentsSSD = new TCanvas("ComponentsSSD", "ComponentsSSD-Sim", 1920, 1000);
  TMultiGraph *SSDmg = new TMultiGraph();
  simComponentsSSD->cd();
  simComponentsSSD->SetLogy();
  SSDmg->Add(eSimLDFSSD);
  SSDmg->Add(pSimLDFSSD);
  SSDmg->Add(mSimLDFSSD);
  SSDmg->Draw("AP*");
  SSDmg->SetMinimum(0.1);
  SSDmg->SetMaximum(15000);
  SSDmg->GetXaxis()->SetLimits(0,4000);
  SSDmg->GetXaxis()->SetTitle("r (m)");
  SSDmg->SetTitle("Laterial Distribution Function for components (SD Sim)");
  SSDmg->Write();
  simComponentsSSD->Write();





  
  simTrace->Write();
  return 0;
}
