// Example code to read in an ADST and make some basic plots
// Tom Harrison, thomas.harrison@adelaide.edu.au 
// Modified by Bradley Manning, bradley.manning@adelaide.edu.au, for use of SSDs.
// Usage:
// ./ReadADST $filename
// where $filename is the ADST file(s) you want to analyse.
// For example, to run over the data from 2011:
// ./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011*
// Or just for the month of May, 2011:
// ./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011_05_generated_2012-10-28.root

// ----------------------------------------------------------------------
// Header files to include

// C++
#include <iostream> // required for cout etc
#include <fstream>  // for reading in from / writing to an external file
#include <stdio.h>
#include <vector>
// Auger
#include "RecEvent.h"
#include "RecEventFile.h"
#include "DetectorGeometry.h"
#include "FileInfo.h"
#include "Detector.h"
#include "SdBadStation.h"
#include "EyeGeometry.h"

// ROOT
#include "TH1F.h"   // Histogram
#include "TMath.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLegend.h"
#include "TGraph.h"
#include "DetectorGeometry.h"
#include "TMultiGraph.h"

//class DetectorGeometry;

void usage();

//----------------------------------------------------------------------
using namespace std; // So the compiler interprets cout as std::cout

// ----------------------------------------------------------------------
// Globals

const double kPi = TMath::Pi();
TH1F* trace_hist[30];
TLegend* legend[30];
float peak, charge, calibratedSignal, peak1, peak2, peak3, charge1, charge2, charge3, calibratedSignal1, calibratedSignal2, calibratedSignal3;
float ePeak1, ePeak2, ePeak3, pPeak1, pPeak2, pPeak3, ePeak, pPeak, mPeak, mPeak1, mPeak2, mPeak3, eCalibratedSignal1, eCalibratedSignal2, eCalibratedSignal3, eCalibratedSignal, pCalibratedSignal, pCalibratedSignal1, pCalibratedSignal2, pCalibratedSignal3, mCalibratedSignal, mCalibratedSignal1, mCalibratedSignal2, mCalibratedSignal3, eCharge, eCharge1, eCharge2, eCharge3, pCharge, pCharge1, pCharge2, pCharge3, mCharge, mCharge1, mCharge2, mCharge3;
char hist_name[50], MIPCharge[25], MIPPeak[25], S[25];
int num_hists;
TCanvas* c1[3];
std::vector < double > tank_distance;
std::vector < double > tank_signal;
std::vector < double > sdTank_signal;
std::vector < double > eTank_signal;
std::vector < double > pTank_signal;
std::vector < double > mTank_signal;
std::vector < double > eSdTank_signal;
std::vector < double > pSdTank_signal;
std::vector < double > mSdTank_signal;
TGraph* LDF = new TGraph(tank_signal.size());
TGraph* LDFSD = new TGraph(tank_signal.size());
TGraph* eLDF = new TGraph();
TGraph* eLDFSD = new TGraph();
TGraph* pLDFSD = new TGraph();
TGraph* pLDF = new TGraph();
TGraph* mLDF = new TGraph();
TGraph* mLDFSD = new TGraph();
TMultiGraph *mg = new TMultiGraph();
TMultiGraph *SDmg = new TMultiGraph();
TMultiGraph *SSDmg = new TMultiGraph();
// ----------------------------------------------------------------------

int 
main(int argc, char **argv)
{

  // Check if the user specified what files to analyse
  if (argc < 2) {
    usage();
    return 1;
  }
  DetectorGeometry * theGeometry = new DetectorGeometry();
  //Declare Canvas
  TFile *Trace = new TFile("Trace.root", "RECREATE");
  if (Trace->IsOpen() ) printf("File opened successfully\n");
  c1[0] = new TCanvas("c1", "c1", 1920,1000);
  c1[1] = new TCanvas("c2", "c2", 1920,1000);
  c1[2] = new TCanvas("c3", "c3", 1920,1000);
  // Variables for declaring profile plots / histograms. This way it's clear what they represent.
  // ----------------------------------------------------------------------
  // Loop over files
  // read the file as the second to last argument
  for (int iFile = 1; iFile <= argc - 1; iFile++) {
    RecEventFile dataFile(argv[iFile]);
    RecEvent* theRecEvent = new RecEvent();
    dataFile.SetBuffers(&(theRecEvent));
    dataFile.ReadDetectorGeometry(*theGeometry);
    unsigned int ntotThisFile = dataFile.GetNEvents();
    cout << "reading file " << dataFile.GetActiveFileName() << " with " << ntotThisFile << " events." << endl;

    // ----------------------------------------------------------------------
    // loop over all the events in the file
    for (unsigned int iEvent = 0; iEvent < ntotThisFile; iEvent++) {

      //if there are no more events left in this file, move on the the next file
      if (!dataFile.ReadEvent(iEvent) == RecEventFile::eSuccess) continue;
      
      
      
      const SDEvent& sevent = theRecEvent->GetSDEvent();
      const int eventID = sevent.GetEventId();
      const std::vector<SdRecStation>& sevent2 = sevent.GetStationVector();
      num_hists = sevent2.size();
      int canvasCount=-1;
      int count = 0;

      for (int i = 0; i < sevent2.size(); i++)
	{
	
	  std::vector<Float_t> ssd = sevent2[i].GetPMTTraces(eTotalTrace,5).GetVEMComponent();
	  std::vector<Float_t> sd1 = sevent2[i].GetPMTTraces(eTotalTrace,1).GetVEMComponent();
	  std::vector<Float_t> sd2 = sevent2[i].GetPMTTraces(eTotalTrace,2).GetVEMComponent();
	  std::vector<Float_t> sd3 = sevent2[i].GetPMTTraces(eTotalTrace,3).GetVEMComponent();
	  std::vector<Float_t> eSignal1 = sevent2[i].GetPMTTraces(eElectronTrace,1).GetVEMComponent();
	  std::vector<Float_t> eSignal2 = sevent2[i].GetPMTTraces(eElectronTrace,2).GetVEMComponent();
	  std::vector<Float_t> eSignal3 = sevent2[i].GetPMTTraces(eElectronTrace,3).GetVEMComponent();
	  std::vector<Float_t> pSignal1 = sevent2[i].GetPMTTraces(ePhotonTrace,1).GetVEMComponent();
	  std::vector<Float_t> pSignal2 = sevent2[i].GetPMTTraces(ePhotonTrace,2).GetVEMComponent();
	  std::vector<Float_t> pSignal3 = sevent2[i].GetPMTTraces(ePhotonTrace,3).GetVEMComponent();
	  std::vector<Float_t> mSignal1 = sevent2[i].GetPMTTraces(eMuonTrace, 1).GetVEMComponent();
	  std::vector<Float_t> mSignal2 = sevent2[i].GetPMTTraces(eMuonTrace, 2).GetVEMComponent();
	  std::vector<Float_t> mSignal3 = sevent2[i].GetPMTTraces(eMuonTrace, 3).GetVEMComponent();

	  std::vector<Float_t> eSignal = sevent2[i].GetPMTTraces(eElectronTrace,5).GetVEMComponent();
	  std::vector<Float_t> pSignal = sevent2[i].GetPMTTraces(ePhotonTrace,5).GetVEMComponent();
	  std::vector<Float_t> mSignal = sevent2[i].GetPMTTraces(eMuonTrace,5).GetVEMComponent();
	  
	  
      
	  // UInt_t tankID = sevent2[i].GetId();
	  // sprintf(hist_name, "Tank ID = %d", tankID);
	  // cout << sd1.size() << " " << sd2.size() << " " << sd3.size() << endl;
	  //Fill and create histograms----------------------------------------------------------------
	  {
	    // trace_hist[i] = new TH1F(hist_name, hist_name, ssd.size(), 0, ssd.size());
	    // for (int j = 0; j < ssd.size(); j++)
	    //   {
	    // 	trace_hist[i]->Fill(count,ssd[j]);
	    // 	count++;
	    //   }
	    // count = 0;
	    //Variables for calculating S (calibrated signal)
	    // cout << "REEEEEEEEEEEEE" << endl;
	    const UShort_t StartSlot = sevent2[i].GetSignalStartSlot();
	    const UShort_t EndSlot = sevent2[i].GetSignalEndSlot();
	    //Setting the X range on to the signal start cuts out irrelevant data
	    // trace_hist[i]->GetXaxis()->SetRangeUser(StartSlot,EndSlot);
	    peak = sevent2[i].GetPMTTraces(eTotalTrace,5).GetPeak();
	    charge = sevent2[i].GetPMTTraces(eTotalTrace,5).GetCharge();
	    peak1 = sevent2[i].GetPMTTraces(eTotalTrace,1).GetPeak();
	    peak2 = sevent2[i].GetPMTTraces(eTotalTrace,2).GetPeak();
	    peak3 = sevent2[i].GetPMTTraces(eTotalTrace,3).GetPeak();
	    charge1 = sevent2[i].GetPMTTraces(eTotalTrace,1).GetCharge();
	    charge2 = sevent2[i].GetPMTTraces(eTotalTrace,2).GetCharge();
	    charge3 = sevent2[i].GetPMTTraces(eTotalTrace,3).GetCharge();

	    ePeak = sevent2[i].GetPMTTraces(eElectronTrace,5).GetPeak();
	    ePeak1 = sevent2[i].GetPMTTraces(eElectronTrace,1).GetPeak();
	    ePeak2 = sevent2[i].GetPMTTraces(eElectronTrace,2).GetPeak();
	    ePeak3 = sevent2[i].GetPMTTraces(eElectronTrace,3).GetPeak();
	    pPeak = sevent2[i].GetPMTTraces(ePhotonTrace,5).GetPeak();
	    pPeak1 = sevent2[i].GetPMTTraces(ePhotonTrace,1).GetPeak();
	    pPeak2 = sevent2[i].GetPMTTraces(ePhotonTrace,2).GetPeak();
	    pPeak3 = sevent2[i].GetPMTTraces(ePhotonTrace,3).GetPeak();
	    mPeak = sevent2[i].GetPMTTraces(eMuonTrace,5).GetPeak();
	    mPeak1 = sevent2[i].GetPMTTraces(eMuonTrace,1).GetPeak();
	    mPeak2 = sevent2[i].GetPMTTraces(eMuonTrace,2).GetPeak();
	    mPeak3 = sevent2[i].GetPMTTraces(eMuonTrace,3).GetPeak();

	    eCharge = sevent2[i].GetPMTTraces(eElectronTrace,5).GetCharge();
	    eCharge1 = sevent2[i].GetPMTTraces(eElectronTrace,1).GetCharge();
	    eCharge2 = sevent2[i].GetPMTTraces(eElectronTrace,2).GetCharge();
	    eCharge3 = sevent2[i].GetPMTTraces(eElectronTrace,3).GetCharge();
	    pCharge = sevent2[i].GetPMTTraces(ePhotonTrace,5).GetCharge();
	    pCharge1 = sevent2[i].GetPMTTraces(ePhotonTrace,1).GetCharge();
	    pCharge2 = sevent2[i].GetPMTTraces(ePhotonTrace,2).GetCharge();
	    pCharge3 = sevent2[i].GetPMTTraces(ePhotonTrace,3).GetCharge();
	    mCharge = sevent2[i].GetPMTTraces(eMuonTrace,5).GetCharge();
	    mCharge1 = sevent2[i].GetPMTTraces(eMuonTrace,1).GetCharge();
	    mCharge2 = sevent2[i].GetPMTTraces(eMuonTrace,2).GetCharge();
	    mCharge3 = sevent2[i].GetPMTTraces(eMuonTrace,3).GetCharge();
	    
	    float VEMsum1 = 0.;
	    float VEMsum2 = 0.;
	    float VEMsum3 = 0.;
	    float MIPsum = 0;
	    float eSum = 0;
	    float eSum1 = 0;
	    float eSum2 = 0;
	    float eSum3 = 0;
	    float pSum = 0;
	    float pSum1 = 0;
	    float pSum2 = 0;
	    float pSum3 = 0;
	    float mSum = 0;
	    float mSum1 = 0;
	    float mSum2 = 0;
	    float mSum3 = 0;
	    
	    for (int i=StartSlot; i < EndSlot; i++)

	      {
		
		if (sd1.size() != 0) VEMsum1 += sd1[i];
		if (sd2.size() != 0) VEMsum2 += sd2[i];
		if (sd3.size() != 0) VEMsum3 += sd3[i];
		if (ssd.size() != 0) MIPsum += ssd[i];
		if (eSignal.size() != 0) eSum += eSignal[i];
		if (eSignal1.size() != 0) eSum1 += eSignal1[i];
		if (eSignal2.size() != 0) eSum2 += eSignal2[i];
		if (eSignal3.size() != 0) eSum3 += eSignal3[i];
		if (pSignal.size() != 0) pSum += pSignal[i];
		if (pSignal1.size() != 0) pSum1 += pSignal1[i];
		if (pSignal2.size() != 0) pSum2 += pSignal2[i];
		if (pSignal3.size() != 0) pSum3 += pSignal3[i];
		if (mSignal.size() != 0) mSum += mSignal[i];
		if (mSignal1.size() != 0) mSum1 += mSignal1[i];
		if (mSignal2.size() != 0) mSum2 += mSignal2[i];
		if (mSignal3.size() != 0) mSum3 += mSignal3[i];
	      }
	    //Calculation for S (calibrated signal). Formula given by David Schmitt. References in
	    //https://www.sciencedirect.com/science/article/pii/S0168900206013593?via%3Dihub
	    
	    calibratedSignal = MIPsum*(peak/charge);
	    calibratedSignal1 = VEMsum1*(peak1/charge1);
	    calibratedSignal2 = VEMsum2*(peak2/charge2);
	    calibratedSignal3 = VEMsum3*(peak3/charge3);
	    eCalibratedSignal = eSum*(ePeak/eCharge);
	    eCalibratedSignal1 = eSum1*(ePeak1/eCharge1);
	    eCalibratedSignal2 = eSum2*(ePeak2/eCharge2);
	    eCalibratedSignal3 = eSum3*(ePeak3/eCharge3);
	    pCalibratedSignal = pSum*(pPeak/pCharge);
	    pCalibratedSignal1 = pSum1*(pPeak1/pCharge1);
	    pCalibratedSignal2 = pSum2*(pPeak2/pCharge2);
	    pCalibratedSignal3 = pSum3*(pPeak3/pCharge3);
	    mCalibratedSignal = mSum*(mPeak/mCharge);
	    mCalibratedSignal1 = mSum1*(mPeak1/mCharge1);
	    mCalibratedSignal2 = mSum2*(mPeak2/mCharge2);
	    mCalibratedSignal3 = mSum3*(mPeak3/mCharge3);
	    
	    // cout << calibratedSignal1 << " " << calibratedSignal2 << " " << calibratedSignal3 << endl;
	    double averageSdSignal = ( calibratedSignal1 + calibratedSignal2 + calibratedSignal3 ) / 3.;
	    double eAverageSdSignal = ( eCalibratedSignal1 + eCalibratedSignal2 + eCalibratedSignal3 ) / 3.;
	    double pAverageSdSignal = ( pCalibratedSignal1 + pCalibratedSignal2 + pCalibratedSignal3 ) / 3.;
	    double mAverageSdSignal = ( mCalibratedSignal1 + mCalibratedSignal2 + mCalibratedSignal3 ) / 3.;
	
	    //  cout << averageSdSignal << endl;
	    if (ssd.size() == 0) {
	      tank_signal.push_back(0.);
	      eTank_signal.push_back(0.);
	      pTank_signal.push_back(0.);
	      mTank_signal.push_back(0.);
	    } else {
	      tank_signal.push_back(calibratedSignal);
	      eTank_signal.push_back(eCalibratedSignal);
	      pTank_signal.push_back(pCalibratedSignal);
	      mTank_signal.push_back(mCalibratedSignal);
	    }
	    if ( (sd1.size() != 0) && (sd2.size() != 0) && (sd3.size() != 0) ) {
	    sdTank_signal.push_back(averageSdSignal);
	    eSdTank_signal.push_back(eAverageSdSignal);
	    pSdTank_signal.push_back(pAverageSdSignal);
	    mSdTank_signal.push_back(mAverageSdSignal);
	    } else {
	      sdTank_signal.push_back(0.);
	      eSdTank_signal.push_back(0.);
	      pSdTank_signal.push_back(0.);
	      mSdTank_signal.push_back(0.);
	    }
	    tank_distance.push_back(sevent2[i].GetSPDistance());
	   
	  }
	}



      

	  const GenShower& genShower = theRecEvent->GetGenShower();
	  const TVector3& showerCore = sevent.GetSdRecShower().GetCoreSiteCS();
	  const double zC = showerCore.Z();

	  const TVector3& showerCoreMC = genShower.GetCoreAtAltitudeSiteCS(zC);
	  const TVector3& axis = genShower.GetAxisSiteCS();
	  double xC = showerCoreMC.X() ;
	  double yC = showerCoreMC.Y() ;
	  // cout << xC << " " << yC << endl;


	  const Detector& det = theRecEvent->GetDetector();
	  for ( unsigned int iS=0;iS<sevent2.size();iS++){
	    const unsigned int id = sevent2[iS].GetId();
	    double X;
	    double Y;
	    double l;
	    if(sevent2[iS].IsDense()){
	      X= det.GetStationPosition(id).X();
	      Y= det.GetStationPosition(id).Y();
	      //Dense stations are placed 1000m from the core
	      l = 1000;
	  
	    }
	    else{
	      X = (theGeometry)->GetStationPosition(id).X();
	      Y = (theGeometry)->GetStationPosition(id).Y();
	      l = (theGeometry)->GetStationAxisDistance(id,axis,showerCoreMC);
	    }
	    //Calculate distance to true core
	    //double 
	    //	cout << X << " " << Y << endl;
	    //cout << l << " " << tank_signal[iS] << endl;
	    if (tank_signal[iS] >= 0 ) {

	      LDF->SetPoint(LDF->GetN(), l, tank_signal[iS]);
	      
	      eLDF->SetPoint(eLDF->GetN(), l, eTank_signal[iS]);
	   
	      pLDF->SetPoint(pLDF->GetN(), l, pTank_signal[iS]);
	      mLDF->SetPoint(mLDF->GetN(), l, mTank_signal[iS]);
	      //cout << "HELLO" << endl;
	    }
	    if (sdTank_signal[iS] >= 0 ) {
	      LDFSD->SetPoint(LDFSD->GetN(), l, sdTank_signal[iS]);
	      eLDFSD->SetPoint(eLDFSD->GetN(), l, eSdTank_signal[iS]);
	      pLDFSD->SetPoint(pLDFSD->GetN(), l, pSdTank_signal[iS]);
	      mLDFSD->SetPoint(mLDFSD->GetN(), l, mSdTank_signal[iS]);
	      //  cout << sdTank_signal[iS] << endl;
	    }
	    //   cout << tank_signal.size() << endl;
	    //  cout << sdTank_signal.size() << endl;
		      
	  }
	  tank_signal.clear();
	  sdTank_signal.clear();
	  eTank_signal.clear();
	  eSdTank_signal.clear();
	  pTank_signal.clear();
	  pSdTank_signal.clear();
	  mTank_signal.clear();
	  mSdTank_signal.clear();

      

	  //Divide canvas, note the 3rd and 4th values are margins---------------------------------
	  //Hard coded: NEEDS WORK
	  c1[0]->Divide(3,3,0.0001,0.001);
	  c1[1]->Divide(3,3,0.0001,0.001);
	  c1[2]->Divide(3,3,0.0001,0.001);
	  // Loop over tanks--------------------------------------------------------------
     


	  
	  // //Else statement for empty histograms (relevant incase SSDs did not pick up a signal WCDs did)---------
	  // else {
	  //   //hard-code empty histograms if no signal (NEEDS WORK)
	  //   trace_hist[i] = new TH1F(hist_name, hist_name, 2304,0,2304);
	          
	  // }
	  // //------------------------------------------------------------------------------------------------
	  
	  // //Customise Histograms---------------------------------------------------------------------------
	  // trace_hist[i]->SetFillColor(kRed);
	  // trace_hist[i]->SetLineColor(kRed);
	  // trace_hist[i]->GetYaxis()->SetTitle("Signal [MIP Peak] ");
	  // trace_hist[i]->GetXaxis()->SetTitle("t [25ns]");
	  // trace_hist[i]->SetStats(kFALSE);
	  // //-------------------------------------------------------------------------------------------------

	  // //Change canvas (9 graphs per canvas seems OK right now)--------------------------------------------
	  // if (i%9 == 0 ) {
	  //   canvasCount++;
	  // }
	  // c1[canvasCount]->cd((i% 9 )+1 );
	  // //----------------------------------------------------------------------------------------

	  // //Draw histogram and create legend--------------------------------------------
	  // //Note: Must draw histogram after making customisations, otherwise it will not write correctly to *.root
	  // trace_hist[i]->Draw();
	  // sprintf (MIPCharge, "MIP Charge = %.01f", charge);
	  // sprintf (MIPPeak, "MIP Peak = %.01f", peak);
	  // sprintf (S, "S = %.01f MIP", calibratedSignal);
	  // legend[i] = new TLegend(0.7,0.7,0.9,0.9);
	  // legend[i]->AddEntry((TObject*)0,S,"");
	  // legend[i]->AddEntry((TObject*)0, MIPCharge, "");
	  // legend[i]->AddEntry((TObject*)0, MIPPeak, "");
	  // legend[i]->SetBorderSize(0);
	  // legend[i]->SetFillStyle(0);
	  // legend[i]->Draw();
	  // ----------------------------------------------------------------------------------------
	  
	
      
    }
  }
    
  //Save canvases and plots to root file---------------------------------------------------------------------
  //Note: TApplication returns segmentation faults. (reproducible as of 3/18 on TESLA).
  //Temporary solution is to write to ROOT files and browse via root.
  c1[2]->SetLogy();
  c1[1]->SetLogy();
  c1[0]->SetLogy();
  

  
  LDF->GetYaxis()->SetRangeUser(0.2,1500);
  LDFSD->GetYaxis()->SetRangeUser(0.2,1500);
  LDF->GetXaxis()->SetRangeUser(0,4000);
  LDFSD->GetXaxis()->SetRangeUser(0,4000);

  LDF->SetMarkerColor(kRed);
  LDF->Draw("AP*");
  LDF->Write("LDFSSD");
  // Trace->Write();

  c1[2]->Write("SSD");
  c1[1]->cd();
  LDFSD->Draw("AP*");
  LDFSD->Write("LDFSD");
  c1[1]->Write();

  c1[0]->cd();
  mg->Add(LDF);
  mg->Add(LDFSD);
  mg->Draw("AP*");
  mg->SetMinimum(0.2);
  mg->SetMaximum(1500);
  mg->GetXaxis()->SetLimits(0,4000);
  mg->GetXaxis()->SetTitle("r (m)");
  mg->SetTitle("Lateral Distribution Function");
  TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->AddEntry(LDF, "Signal (MIP PEAK)","p");
  leg->AddEntry(LDFSD, "Signal (VEM PEAK)","p");
  leg->Draw();
  c1[0]->SetTitle("Lateral Distribution Function");
  mg->Write();
  c1[0]->Write();

  TCanvas* ComponentsSD = new TCanvas("Components", "Components", 1920, 1000);
  ComponentsSD->cd();
  ComponentsSD->SetLogy();
  eLDFSD->SetMarkerColor(kRed);
  pLDFSD->SetMarkerColor(kGreen);
  mLDFSD->SetMarkerColor(kBlue);
  eLDFSD->Write("eLDFSD");
  pLDFSD->Write("pLDFSD");
  mLDFSD->Write("mLDFSD");
  SDmg->Add(eLDFSD);
  SDmg->Add(pLDFSD);
  SDmg->Add(mLDFSD);
  SDmg->Draw("AP*");
  SDmg->SetMinimum(0.2);
  SDmg->SetMaximum(1500);
  SDmg->GetXaxis()->SetLimits(0,4000);
  SDmg->GetXaxis()->SetTitle("r (m)");
  SDmg->SetTitle("Lateral Distriubtion Function (Components)");
  SDmg->Write();
  ComponentsSD->Write();
  TCanvas* ComponentsSSD = new TCanvas("ComponentsSSD", "ComponentsSSD", 1920, 1000);
  ComponentsSSD->cd();
  ComponentsSSD->SetLogy();
  eLDF->SetMarkerColor(kRed);
  pLDF->SetMarkerColor(kGreen);
  mLDF->SetMarkerColor(kBlue);
  eLDF->Write("eLDF");
  pLDF->Write("pLDF");
  mLDF->Write("mLDF");
  SSDmg->Add(eLDF);
  SSDmg->Add(pLDF);
  SSDmg->Add(mLDF);
  SSDmg->Draw("AP*");
  SSDmg->SetMinimum(0.2);
  SSDmg->SetMaximum(1500);
  SSDmg->GetXaxis()->SetLimits(0,4000);
  SSDmg->GetXaxis()->SetTitle("r (m)");
  SSDmg->SetTitle("Lateral Distribution Function (Components)");
  SSDmg->Write();
  ComponentsSSD->Write();
  // for (int i = 0; i < num_hists; i++) {
  //   trace_hist[i]->Write("Trace");
  //   legend[i]->Write("my_leg");
  // }
  //---------------------------------------------------------------------------------------------------------
  Trace->Write();
  return 0;
  
}
void
usage()
{
  printf("Usage:\n"
	 "./ReadADST $filename(s)\n"
	 "where $filename is the ADST file(s) you want to analyse.\n"
	 "For example, to run over the data from 2011:\n"
	 "./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011*\n"
	 "Or just for the month of May, 2011:\n"
	 "./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011_05_generated_2012-10-28.root\n");
}
