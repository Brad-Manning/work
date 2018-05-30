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
  // cout << " dfdf " << endl;
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
      cout << sum << " " << sumPoint.pos << endl;
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



  ifstream VEMMuonTestFile ("/remote/tesla/bmanning/work/Scripts/AzimuthTest/VEMMuon_r_bins");
  ifstream VEMGammaTestFile("/remote/tesla/bmanning/work/Scripts/AzimuthTest/VEMGamma_r_bins");
  ifstream VEMElectronTestFile("/remote/tesla/bmanning/work/Scripts/AzimuthTest/VEMElectron_r_bins");
  ifstream MIPMuonTestFile("/remote/tesla/bmanning/work/Scripts/AzimuthTest/MIPMuon_r_bins");
  ifstream MIPGammaTestFile("/remote/tesla/bmanning/work/Scripts/AzimuthTest/MIPGamma_r_bins");
  ifstream MIPElectronTestFile("/remote/tesla/bmanning/work/Scripts/AzimuthTest/MIPElectron_r_bins");

  double signal, angle;

  vector < point > _VEMMuonPoint;
  if (VEMMuonTestFile.is_open() )
    {
      point VEMMuonPoint;
      
      while ( VEMMuonTestFile >> signal >> angle )
	{
	  VEMMuonPoint.pos = angle;
	  VEMMuonPoint.value = signal;
	  _VEMMuonPoint.push_back(VEMMuonPoint);
	  // cout << angle << " " << signal << endl;
	}
    }
  vector < point > sum_VEMMuonPoint;
  if (VEMMuonTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _VEMMuonPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _VEMMuonPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_VEMMuonPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  
  vector < point > _VEMGammaPoint;
  if (VEMGammaTestFile.is_open() )
    {
      point VEMGammaPoint;
      while ( VEMGammaTestFile >> signal >> angle )
	{
	  VEMGammaPoint.pos = angle;
	  VEMGammaPoint.value = signal;
	  _VEMGammaPoint.push_back(VEMGammaPoint);
	}
    }

  vector < point > sum_VEMGammaPoint;
  if (VEMGammaTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _VEMGammaPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _VEMGammaPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_VEMGammaPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }

  
  vector < point > _VEMElectronPoint;
  if (VEMElectronTestFile.is_open() )
    {
      point VEMElectronPoint;
      while ( VEMElectronTestFile >> signal >> angle )
	{
	  VEMElectronPoint.pos = angle;
	  VEMElectronPoint.value = signal;
	  _VEMElectronPoint.push_back(VEMElectronPoint);
	}
    }

  vector < point > sum_VEMElectronPoint;
  if (VEMElectronTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _VEMElectronPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _VEMElectronPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_VEMElectronPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  vector < point > _MIPMuonPoint;
  if (MIPMuonTestFile.is_open() )
    {
      point MIPMuonPoint;
      while ( MIPMuonTestFile >> signal >> angle )
	{
	  MIPMuonPoint.pos = angle;
	  MIPMuonPoint.value = signal;
	  _MIPMuonPoint.push_back(MIPMuonPoint);
	}
    }

  vector < point > sum_MIPMuonPoint;
  if (MIPMuonTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _MIPMuonPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _MIPMuonPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_MIPMuonPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  vector < point > _MIPGammaPoint;
  if (MIPGammaTestFile.is_open() )
    {
      point MIPGammaPoint;
      while ( MIPGammaTestFile >> signal >> angle )
	{
	  MIPGammaPoint.pos = angle;
	  MIPGammaPoint.value = signal;
	  _MIPGammaPoint.push_back(MIPGammaPoint);
	}
    }

  vector < point > sum_MIPGammaPoint;
  if (MIPGammaTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _MIPGammaPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _MIPGammaPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_MIPGammaPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  vector < point > _MIPElectronPoint;
  if (MIPElectronTestFile.is_open() )
    {
      point MIPElectronPoint;
      while ( MIPElectronTestFile >> signal >> angle )
	{
	  MIPElectronPoint.pos = angle;
	  MIPElectronPoint.value = signal;
	  _MIPElectronPoint.push_back(MIPElectronPoint);
	}
    }
  
  vector < point > sum_MIPElectronPoint;
  if (MIPElectronTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  for (int l = 0; l < nShowers; l++)
	    {
	      sum = sum + _MIPElectronPoint[k+vectorOfPoints[0].size()*l].value;
	      sumPoint.pos = _MIPElectronPoint[k].pos;
	      sumPoint.value = sum;
	    }
	  sum_MIPElectronPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }

  vector < point > sum_VEMTotalPoint;
  if (VEMMuonTestFile.is_open() && VEMGammaTestFile.is_open() && VEMElectronTestFile.is_open() )
    {
      for (int k = 0; k < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  
	  sum = sum + sum_VEMMuonPoint[k].value;
	  sum = sum + sum_VEMGammaPoint[k].value;
	  sum = sum + sum_VEMElectronPoint[k].value;
	  
	  sumPoint.pos = _VEMMuonPoint[k].pos;
	  sumPoint.value = sum;
	    
	  sum_VEMTotalPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  vector < point > sum_MIPTotalPoint;
  if (MIPMuonTestFile.is_open() && MIPGammaTestFile.is_open() && MIPElectronTestFile.is_open() )
    {
      for (int k = 0;k  < vectorOfPoints[0].size(); k++)
	{
	  sum = 0;
	  
	  sum = sum + sum_MIPMuonPoint[k].value;
	  sum = sum + sum_MIPGammaPoint[k].value;
	  sum = sum + sum_MIPElectronPoint[k].value;
	  
	  sumPoint.pos = _MIPMuonPoint[k].pos;
	  sumPoint.value = sum;
	  
	  sum_MIPTotalPoint.push_back(sumPoint);
	  // cout << sumPoint.pos << "        " << sum << endl;
	}
    }
  
  TGraph *VEMMuonTestGraph =  new TGraph(); VEMMuonTestGraph->SetLineColor(kBlue);
  TGraph *VEMGammaTestGraph = new TGraph(); VEMGammaTestGraph->SetLineColor(kGreen);
  TGraph *VEMElectronTestGraph = new TGraph(); VEMElectronTestGraph->SetLineColor(kRed);
  TGraph *MIPMuonTestGraph = new TGraph(); MIPMuonTestGraph->SetLineColor(kBlue);
  TGraph *MIPGammaTestGraph = new TGraph(); MIPGammaTestGraph->SetLineColor(kGreen);
  TGraph *MIPElectronTestGraph = new TGraph(); MIPElectronTestGraph->SetLineColor(kRed);
  TGraph *VEMTotalTestGraph = new TGraph();
  TGraph *MIPTotalTestGraph = new TGraph();
  TGraph *VEMMuonTestGraph1 =  new TGraph(); VEMMuonTestGraph1->SetLineColor(kBlue);
  TGraph *VEMGammaTestGraph1 = new TGraph(); VEMGammaTestGraph1->SetLineColor(kGreen);
  TGraph *VEMElectronTestGraph1 = new TGraph(); VEMElectronTestGraph1->SetLineColor(kRed);
  TGraph *MIPMuonTestGraph1 = new TGraph(); MIPMuonTestGraph1->SetLineColor(kBlue);
  TGraph *MIPGammaTestGraph1 = new TGraph(); MIPGammaTestGraph1->SetLineColor(kGreen);
  TGraph *MIPElectronTestGraph1 = new TGraph(); MIPElectronTestGraph1->SetLineColor(kRed);
  TGraph *VEMTotalTestGraph1 = new TGraph();
  TGraph *MIPTotalTestGraph1 = new TGraph();
  for (int k = 0; k < vectorOfPoints[0].size(); k++)
    {
      VEMMuonTestGraph->SetPoint(k,sum_VEMMuonPoint[k].pos, (sum_VEMMuonPoint[k].value/nShowers)/(sum_VEMTotalPoint[k].value/nShowers));
      VEMGammaTestGraph->SetPoint(k, sum_VEMGammaPoint[k].pos, (sum_VEMGammaPoint[k].value/nShowers)/(sum_VEMTotalPoint[k].value/nShowers));
      VEMElectronTestGraph->SetPoint(k, sum_VEMElectronPoint[k].pos, (sum_VEMElectronPoint[k].value/nShowers)/(sum_VEMTotalPoint[k].value/nShowers));
      MIPMuonTestGraph->SetPoint(k,sum_MIPMuonPoint[k].pos, (sum_MIPMuonPoint[k].value/nShowers)/(sum_MIPTotalPoint[k].value/nShowers));
      MIPGammaTestGraph->SetPoint(k, sum_MIPGammaPoint[k].pos, (sum_MIPGammaPoint[k].value/nShowers)/(sum_MIPTotalPoint[k].value/nShowers));
      MIPElectronTestGraph->SetPoint(k,sum_MIPElectronPoint[k].pos,(sum_MIPElectronPoint[k].value/nShowers)/(sum_MIPTotalPoint[k].value/nShowers));
      VEMTotalTestGraph->SetPoint(k, sum_VEMTotalPoint[k].pos, 1);// (sum_VEMTotalPoint[k].value/nShowers));
      MIPTotalTestGraph->SetPoint(k, sum_MIPTotalPoint[k].pos, 1);//(sum_MIPTotalPoint[k].value/nShowers));
      VEMMuonTestGraph1->SetPoint(k,sum_VEMMuonPoint[k].pos, (sum_VEMMuonPoint[k].value/nShowers));
      VEMGammaTestGraph1->SetPoint(k, sum_VEMGammaPoint[k].pos, (sum_VEMGammaPoint[k].value/nShowers));
      VEMElectronTestGraph1->SetPoint(k, sum_VEMElectronPoint[k].pos, (sum_VEMElectronPoint[k].value/nShowers));
      MIPMuonTestGraph1->SetPoint(k,sum_MIPMuonPoint[k].pos, (sum_MIPMuonPoint[k].value/nShowers));
      MIPGammaTestGraph1->SetPoint(k, sum_MIPGammaPoint[k].pos, (sum_MIPGammaPoint[k].value/nShowers));
      MIPElectronTestGraph1->SetPoint(k,sum_MIPElectronPoint[k].pos,(sum_MIPElectronPoint[k].value/nShowers));
      VEMTotalTestGraph1->SetPoint(k, sum_VEMTotalPoint[k].pos, (sum_VEMTotalPoint[k].value/nShowers));
      MIPTotalTestGraph1->SetPoint(k, sum_MIPTotalPoint[k].pos, (sum_MIPTotalPoint[k].value/nShowers));
    }
  TCanvas * ComponentTestCanvas = new TCanvas("Component Testing", "Component Testing", 1900, 1000);
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(VEMMuonTestGraph);
  mg->Add(VEMGammaTestGraph);
  mg->Add(VEMElectronTestGraph);
  mg->Add(VEMTotalTestGraph);
  mg->Draw("AL");
  mg->GetYaxis()->SetTitle("Proportion of Total signal");
  mg->GetXaxis()->SetTitle("azimuth in shower plane (deg)");
  ComponentTestCanvas->Write();

  TCanvas * ComponentTestCanvasMIP = new TCanvas("Component Testing MIP", "Component Testing MIP", 1900, 1000);
  TMultiGraph *mg2 = new TMultiGraph();
  mg2->Add(MIPMuonTestGraph);
  mg2->Add(MIPGammaTestGraph);
  mg2->Add(MIPElectronTestGraph);
  mg2->Add(MIPTotalTestGraph);
  mg2->Draw("AL");
  mg2->GetYaxis()->SetTitle("Proportion of total signal");
  mg2->GetXaxis()->SetTitle("azimuth in shower plane (deg)");
  ComponentTestCanvasMIP->Write();


  TCanvas * ComponentSignalTestCanvas = new TCanvas("Comonent Signal Testing", "Component Signal Testing", 1900,1000);
  TMultiGraph *mg3 = new TMultiGraph();
  mg3->Add(VEMMuonTestGraph1);
  mg3->Add(VEMGammaTestGraph1);
  mg3->Add(VEMElectronTestGraph1);
  mg3->Add(VEMTotalTestGraph1);
  mg3->Draw("AL");
  mg3->GetYaxis()->SetTitle("Signal per detector (1000m) (VEM)");
  mg3->GetXaxis()->SetTitle("azimuth in shower plane (deg)");
  ComponentSignalTestCanvas->Write();

  TCanvas * ComponentSignalTestCanvasMIP = new TCanvas("Comonent Signal Testing MIP", "Component Signal Testing", 1900,1000);
  TMultiGraph *mg4 = new TMultiGraph();
  mg4->Add(MIPMuonTestGraph1);
  mg4->Add(MIPGammaTestGraph1);
  mg4->Add(MIPElectronTestGraph1);
  mg4->Add(MIPTotalTestGraph1);
  mg4->Draw("AL");
  mg4->GetXaxis()->SetTitle("azimuth in shower plane (deg)");
  mg4->GetYaxis()->SetTitle("Signal per detector (1000m) (MIP)");
  ComponentSignalTestCanvasMIP->Write();

	  
  TCanvas * AzimuthTestCanvas = new TCanvas( "AzimuthTest", "AzimuthTest", 1920, 1000 );
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

