
#include "readPart_1_7b.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
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
#include <stdio.h>
#include <math.h>
using namespace std;
#define PI 3.14159265
double parameter1;
double parameter0;
double integral(double(*f)(double x), double a, double b, int n)
{
  double step = (b-a) / n ; //width of each rectangle
  double area = 0.0;
  for (int i = 0; i < n; i++ )
    {
      area += f(a + (i + 0.5) * step) * step; // sum up each small rectangle
    }
  return area;
}

double f(double theta)
{
  return pow(10,(parameter1*pow((1/cos(theta)-1),2)+parameter0))*sin(theta)*cos(theta);
}



int main ()

{
  double var;
  double temp = 0;
  int count = 0;
  //The number of showers used per angle
  int nShowers = 19;
  int nEnergies = 4;
  int nAngles = 5;
  ifstream qgsII4proton;
  ifstream qgsII3proton;
  ifstream qgsII4iron;
  ifstream qgsII3iron;
  
  vector < double > energyqgsII4p;
  vector < double > angleqgsII4p;
  vector < double > VEMqgsII4p;
  vector < double > energyqgsII3p;
  vector < double > angleqgsII3p;
  vector < double > VEMqgsII3p;
  vector < double > energyqgsII4i;
  vector < double > angleqgsII4i;
  vector < double > VEMqgsII4i;
  vector < double > energyqgsII3i;
  vector < double > angleqgsII3i;
  vector < double > VEMqgsII3i;
  
  qgsII4proton.open ("TAprotonqgsII4");
  qgsII3proton.open ("TAprotonqgsII3");
  qgsII4iron.open ("TAironqgsII4");
  qgsII3iron.open ("TAironqgsII3");
  
  if (qgsII4proton.is_open())
    {
      while ( qgsII4proton >> var )
	{
	  count++;
	  if ( count %21 == 1 )
	    {
	      
		  energyqgsII4p.push_back(var);
		  // cout << var << endl;
	        
	    }
	  else if ( count % 21 == 2 )
	    {
	      
		  angleqgsII4p.push_back(var);
		  temp = 0;
		  //  cout << var << endl;
		  
	    }
	  else
	    {
	      VEMqgsII4p.push_back((var-temp)/4.822);
	      
	   
	      temp = var;
	      // cout << var << endl;
	    
	    }
		
	}
    }
  count = 0;
  //--------------------------------------
  if (qgsII3proton.is_open())
    {
      while ( qgsII3proton >> var )
	{
	  count++;
	  if ( count %21 == 1 )
	    {
	      
		  energyqgsII3p.push_back(var);
		 
	        
	    }
	  else if ( count % 21 == 2 )
	    {
	      
		  angleqgsII3p.push_back(var);
		  temp = 0;
	        
	    }
	  else
	    {
	      VEMqgsII3p.push_back((var-temp)/4.822);
	      
	      
	      temp = var;
	      
	    }
		
	}
    }
  count = 0;
  //--------------------------------------------
  if (qgsII4iron.is_open())
    {
      while ( qgsII4iron >> var )
	{
	  count++;
	  if ( count % 19 == 1 )
	    {
	      
		  energyqgsII4i.push_back(var);
		 
	        
	    }
	  else if ( count % 19 == 2 )
	    {
	      
		  angleqgsII4i.push_back(var);
		  temp = 0;
	        
	    }
	  else
	    {
	      VEMqgsII4i.push_back((var-temp)/4.822);
	      
	      
	      temp = var;
	      //  cout << var << endl;
	      
	    }
		
	}
    }
  count = 0;
  //----------------------------------------

  if (qgsII3iron.is_open())
    {
      while ( qgsII3iron >> var )
	{
	  count++;
	  if ( count % 21 == 1 )
	    {
	      
		  energyqgsII3i.push_back(var);
		 
	        
	    }
	  else if ( count % 21 == 2 )
	    {
	      
		  angleqgsII3i.push_back(var);
		  temp = 0;
	        
	    }
	  else
	    {
	      VEMqgsII3i.push_back((var-temp)/4.822);
	     
	      
	      temp = var;

	     
	      
	    }
		
	}
    }


//ROOT PLOTS
  double sum;
  double sum2;
  vector < double > meanqgsII4p;
  vector < double > stdevqgsII4p;
  vector < double > meanqgsII3p;
  vector < double > stdevqgsII3p;
  vector < double > meanqgsII4i;
  vector < double > stdevqgsII4i;
  vector < double > meanqgsII3i;
  vector < double > stdevqgsII3i;
  vector < double > meanqgsII4p_log;
  vector < double > stdev_norm;
  vector < double > meanqgsII3p_log;
  vector < double > meanqgsII4i_log;
  vector < double > meanqgsII3i_log;
  double value;
  double value2;
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2= 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  sum += VEMqgsII4p[i+nShowers*j];
	  sum2 += log10 ( VEMqgsII4p[i+nShowers*j]*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10))) );
	  		   
	}
      meanqgsII4p.push_back(sum/nShowers);
      meanqgsII4p_log.push_back(sum2/nShowers);
    }
  
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  value = log10 ( VEMqgsII4p[i+nShowers*j]*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10))) ) - meanqgsII4p_log[j];
	  sum += pow(value,2);
	  value2 = ( VEMqgsII4p[i+nShowers*j] - meanqgsII4p[j] )*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10)));
	  sum2 += pow(value2,2);
	}
      stdevqgsII4p.push_back(sqrt(sum/nShowers));
      stdev_norm.push_back(sqrt(sum2/nShowers));
    }
  //------------------------------------------------------------------------------------------------
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  sum += VEMqgsII3p[i+nShowers*j];
	  sum2 += log10 ( VEMqgsII3p[i+nShowers*j]*(PI*25*(cos(angleqgsII3p[j]*PI/180)/(2*PI*800*10))) );
	}
      meanqgsII3p.push_back(sum/nShowers);
      meanqgsII3p_log.push_back(sum2/nShowers);

    }
  
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  value = log10 ( VEMqgsII3p[i+nShowers*j]*(PI*25*(cos(angleqgsII3p[j]*PI/180)/(2*PI*800*10))) ) - meanqgsII3p_log[j];
	  sum += pow(value,2);
	  value2 = ( VEMqgsII3p[i+nShowers*j] - meanqgsII3p[j] )*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10)));
	  sum2 += pow(value2,2);
	}
      stdevqgsII3p.push_back(sqrt(sum/nShowers));
      stdev_norm.push_back(sqrt(sum2/nShowers));
    }
  //------------------------------------------------------------------------------------------------
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  sum += VEMqgsII3i[i+nShowers*j];
	  sum2 += log10 ( VEMqgsII3i[i+nShowers*j]*(PI*25*(cos(angleqgsII3i[j]*PI/180)/(2*PI*800*10))) );
	}
      meanqgsII3i.push_back(sum/nShowers);
      meanqgsII3i_log.push_back(sum2/nShowers);
      //  cout << sum2/nShowers << endl;
    }
  //  double value;
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowers; i++ )
	{
	  value = log10 ( VEMqgsII3i[i+nShowers*j]*(PI*25*(cos(angleqgsII3i[j]*PI/180)/(2*PI*800*10))) ) - meanqgsII3i_log[j];
	  sum += pow(value,2);
	  value2 = ( VEMqgsII3i[i+nShowers*j] - meanqgsII3i[j] )*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10)));
	  sum2 += pow(value2,2);

	}
      stdevqgsII3i.push_back(sqrt(sum/nShowers));
      stdev_norm.push_back(sqrt(sum2/nShowers));
    }

  //------------------------------------------------------------------------------------------------
  int nShowersi = 17;
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowersi; i++ )
	{
	  sum += VEMqgsII4i[i+nShowersi*j];
	  sum2 += log10 ( VEMqgsII4i[i+nShowersi*j]*(PI*25*(cos(angleqgsII4i[j]*PI/180)/(2*PI*800*10))) );
	 
	}
      meanqgsII4i.push_back(sum/nShowersi);
      meanqgsII4i_log.push_back(sum2/nShowers);

    }
  
  for (int j = 0; j < nAngles*nEnergies; j++)
    {
      sum = 0;
      sum2 = 0;
      for (int i = 0; i < nShowersi; i++ )
	{
	  value = log10 ( VEMqgsII4i[i+nShowersi*j]*(PI*25*(cos(angleqgsII4i[j]*PI/180)/(2*PI*800*10))) ) - meanqgsII4i_log[j];
	  sum += pow(value,2);
	  value2 = ( VEMqgsII4i[i+nShowersi*j] - meanqgsII4i[j] )*(PI*25*(cos(angleqgsII4p[j]*PI/180)/(2*PI*800*10)));
	  sum2 += pow(value2,2);


	}
      stdevqgsII4i.push_back(sqrt(sum/nShowersi));
      stdev_norm.push_back(sqrt(sum2/nShowers));
    }

  //---------------------------------------------
  for (int i = 0; i < nAngles*nEnergies; i++)
    {
      //Convert to sec(theta)
      angleqgsII4p[i] = 1/cos(angleqgsII4p[i]* PI / 180.0);
    }
   
  //---------------------------------------------
  for (int i = 0; i < nAngles*nEnergies; i++)
    {
      //Convert to sec(theta)
      angleqgsII3p[i] = 1/cos(angleqgsII3p[i]* PI / 180.0);
    }
 //---------------------------------------------
  for (int i = 0; i < nAngles*nEnergies; i++)
    {
      //Convert to sec(theta)
      angleqgsII3i[i] = 1/cos(angleqgsII3i[i]* PI / 180.0);
    }
 //---------------------------------------------
  for (int i = 0; i < nAngles*nEnergies; i++)
    {
      //Convert to sec(theta)
      angleqgsII4i[i] = 1/cos(angleqgsII4i[i]* PI / 180.0);
    }

  //-------------------------------------------


  TApplication theApp("app",0,0);
  TCanvas plots;
  // plots.SetLogy();

  
  Double_t ex [nAngles];
  Double_t eyqgsII4p [nAngles];
  Double_t graph_angleqgsII4p [nAngles];
  Double_t graph_VEMqgsII4p [nAngles];

  Double_t eyqgsII3p [nAngles];
  Double_t graph_angleqgsII3p [nAngles];
  Double_t graph_VEMqgsII3p [nAngles];


  Double_t eyqgsII4i [nAngles];
  Double_t graph_angleqgsII4i [nAngles];
  Double_t graph_VEMqgsII4i [nAngles];

  Double_t eyqgsII3i [nAngles];
  Double_t graph_angleqgsII3i [nAngles];
  Double_t graph_VEMqgsII3i [nAngles];

  //--------------------------------------------


  TGraphErrors* grqgsII4p[nEnergies];
  TGraphErrors* grqgsII3p[nEnergies];
  TGraphErrors* grqgsII4i[nEnergies];
  TGraphErrors* grqgsII3i[nEnergies];


 for (int j = 0; j < nEnergies; j++)
    {
      for (int i = 0; i < nAngles; i++)
	{
	  //Erros disabled for now
	  eyqgsII4p[i] =( stdevqgsII4p[i+j*nAngles] / (sqrt(nShowers)));//*(PI*100/(graph_angleqgsII4p[i]*(2*PI*800*10)));
	  graph_angleqgsII4p[i] = angleqgsII4p[i+j*nAngles];
	  graph_VEMqgsII4p[i] = log10 (meanqgsII4p[i+j*nAngles]*(PI*100/((graph_angleqgsII4p[i]*2*PI*800*10))));
	  ex[i] = 0;
	 
	}
      grqgsII4p[j] = new TGraphErrors(nAngles, graph_angleqgsII4p, graph_VEMqgsII4p,ex,eyqgsII4p);
      
    }
 //----------------------------------------------------------------------------
 for (int j = 0; j < nEnergies; j++)
    {
      for (int i = 0; i < nAngles; i++)
	{
	  //Erros disabled for now
	  eyqgsII3p[i] =(stdevqgsII3p[i+j*nAngles] / (sqrt(nShowers)));//*(PI*100/(graph_angleqgsII3p[i]*(2*PI*800*10)));
	  graph_angleqgsII3p[i] = angleqgsII3p[i+j*nAngles];
	  graph_VEMqgsII3p[i] = log10 (meanqgsII3p[i+j*nAngles]*(PI*100/((graph_angleqgsII3p[i]*2*PI*800*10))));
	  ex[i] = 0;
	 
	}
      grqgsII3p[j] = new TGraphErrors(nAngles, graph_angleqgsII3p, graph_VEMqgsII3p,ex,eyqgsII3p);
      
    }
 //----------------------------------------------------------------------------
 for (int j = 0; j < nEnergies; j++)
    {
      for (int i = 0; i < nAngles; i++)
	{
	  //Erros disabled for now
	  eyqgsII4i[i] =( stdevqgsII4p[i+j*nAngles] / (sqrt(nShowersi)));//*(PI*100/(graph_angleqgsII4i[i]*(2*PI*800*10)));
	  graph_angleqgsII4i[i] = angleqgsII4i[i+j*nAngles];
	  graph_VEMqgsII4i[i] = log10 (meanqgsII4i[i+j*nAngles]*(PI*100/((graph_angleqgsII4i[i]*2*PI*800*10))));
	  ex[i] = 0;

	 
	}
      grqgsII4i[j] = new TGraphErrors(nAngles, graph_angleqgsII4i, graph_VEMqgsII4i,ex,eyqgsII4i);
      
    }
 //----------------------------------------------------------------------------
 for (int j = 0; j < nEnergies; j++)
    {
      for (int i = 0; i < nAngles; i++)
	{
	  //Erros disabled for now
	 
	  graph_angleqgsII3i[i] = angleqgsII3i[i+j*nAngles];
	  graph_VEMqgsII3i[i] = log10 (meanqgsII3i[i+j*nAngles]*(PI*100/((graph_angleqgsII3i[i]*2*PI*800*10))));
	  eyqgsII3i[i] =   ( stdevqgsII3i[i+j*nAngles] / (sqrt(nShowers)));//*(PI*100/(graph_angleqgsII3i[i]*(2*PI*800*10)));
	  ex[i] = 0;
	  // cout << stdevqgsII3i[i+j*nShowers] << endl;
	 
	}
      grqgsII3i[j] = new TGraphErrors(nAngles, graph_angleqgsII3i, graph_VEMqgsII3i,ex,eyqgsII3i);
      
    }
 //----------------------------------------------------------------------------

 //number of points
 const Int_t n = nAngles*nEnergies;
 TF1 *myFit = new TF1("myFit","[1]*(x-1)^2+[0]");
   
   grqgsII4p[0]->SetName("grqgsII4p");
   grqgsII4p[1]->SetName("grqgsII4p1");
   grqgsII4p[2]->SetName("grqgsII4p2");
   grqgsII4p[3]->SetName("grqgsII4p3");
   grqgsII4p[3]->Draw("AP");
   grqgsII4p[1]->Draw("P");
   grqgsII4p[2]->Draw("P");
   grqgsII4p[0]->Draw("P");
   grqgsII4p[3]->GetYaxis()->SetTitle("log_{10} [S800 / (VEM m^{-2}) ]");
   grqgsII4p[3]->GetXaxis()->SetTitle("sec (#theta)");
   grqgsII4p[3]->GetYaxis()->SetRangeUser(0.,3.5);
   grqgsII4p[3]->GetXaxis()->SetRangeUser(0,2);
   grqgsII4p[0]->SetLineColor(1);
   grqgsII4p[1]->SetLineColor(1);
   grqgsII4p[2]->SetLineColor(1);
   grqgsII4p[3]->SetLineColor(1);
   grqgsII4p[0]->SetMarkerStyle(kMultiply);
   grqgsII4p[1]->SetMarkerStyle(kFullDotLarge);
   grqgsII4p[2]->SetMarkerStyle(kStar);
   grqgsII4p[3]->SetMarkerStyle(kCircle);
   grqgsII4p[0]->SetMarkerColor(1);
   grqgsII4p[1]->SetMarkerColor(1);
   grqgsII4p[2]->SetMarkerColor(1);
   grqgsII4p[3]->SetMarkerColor(1);
   grqgsII4p[0]->Fit("myFit");
   grqgsII4p[1]->Fit("myFit");
   grqgsII4p[2]->Fit("myFit");
   grqgsII4p[3]->Fit("myFit");
   grqgsII4p[0]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4p[1]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4p[2]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4p[3]->GetFunction("myFit")->SetLineColor(1);
   double qgsII4p0_p0 = grqgsII4p[0]->GetFunction("myFit")->GetParameter(0);
   double qgsII4p0_p1 = grqgsII4p[0]->GetFunction("myFit")->GetParameter(1);
   double qgsII4p1_p0 = grqgsII4p[1]->GetFunction("myFit")->GetParameter(0);
   double qgsII4p1_p1 = grqgsII4p[1]->GetFunction("myFit")->GetParameter(1);
    double qgsII4p2_p0 = grqgsII4p[2]->GetFunction("myFit")->GetParameter(0);
   double qgsII4p2_p1 = grqgsII4p[2]->GetFunction("myFit")->GetParameter(1);
    double qgsII4p3_p0 = grqgsII4p[3]->GetFunction("myFit")->GetParameter(0);
   double qgsII4p3_p1 = grqgsII4p[3]->GetFunction("myFit")->GetParameter(1);
   // double qgsII4p3_p2 = grqgsII4p[3]->GetFunction("myFit")->GetParameter(2);
   
   
   //--------------------------------------------------
   grqgsII3p[0]->SetName("grqgsII3p");
   grqgsII3p[3]->Draw("P");
   grqgsII3p[1]->Draw("P");
   grqgsII3p[2]->Draw("P");
   grqgsII3p[0]->Draw("P");
   grqgsII3p[0]->SetLineColor(2);
   grqgsII3p[1]->SetLineColor(2);
   grqgsII3p[2]->SetLineColor(2);
   grqgsII3p[3]->SetLineColor(2);
   grqgsII3p[0]->SetMarkerStyle(kMultiply);
   grqgsII3p[1]->SetMarkerStyle(kFullDotLarge);
   grqgsII3p[2]->SetMarkerStyle(kStar);
   grqgsII3p[3]->SetMarkerStyle(kCircle);
   grqgsII3p[0]->SetMarkerColor(2);
   grqgsII3p[1]->SetMarkerColor(2);
   grqgsII3p[2]->SetMarkerColor(2);
   grqgsII3p[3]->SetMarkerColor(2);
   grqgsII3p[0]->Fit("myFit");
   grqgsII3p[1]->Fit("myFit");
   grqgsII3p[2]->Fit("myFit");
   grqgsII3p[3]->Fit("myFit");
   double qgsII3p0_p0 = grqgsII3p[0]->GetFunction("myFit")->GetParameter(0);
   double qgsII3p0_p1 = grqgsII3p[0]->GetFunction("myFit")->GetParameter(1);
   double qgsII3p1_p0 = grqgsII3p[1]->GetFunction("myFit")->GetParameter(0);
   double qgsII3p1_p1 = grqgsII3p[1]->GetFunction("myFit")->GetParameter(1);
   double qgsII3p2_p0 = grqgsII3p[2]->GetFunction("myFit")->GetParameter(0);
   double qgsII3p2_p1 = grqgsII3p[2]->GetFunction("myFit")->GetParameter(1);
   double qgsII3p3_p0 = grqgsII3p[3]->GetFunction("myFit")->GetParameter(0);
   double qgsII3p3_p1 = grqgsII3p[3]->GetFunction("myFit")->GetParameter(1);
   
   //--------------------------------------------------
   grqgsII4i[0]->SetName("grqgsII4i");
   grqgsII4i[3]->Draw("P");
   grqgsII4i[1]->Draw("P");
   grqgsII4i[2]->Draw("P");
   grqgsII4i[0]->Draw("P");
   grqgsII4i[0]->SetLineColor(1);
   grqgsII4i[1]->SetLineColor(1);
   grqgsII4i[2]->SetLineColor(1);
   grqgsII4i[3]->SetLineColor(1);
   grqgsII4i[0]->SetLineStyle(2);
   grqgsII4i[1]->SetLineStyle(2);
   grqgsII4i[2]->SetLineStyle(2);
   grqgsII4i[3]->SetLineStyle(2);
   grqgsII4i[0]->SetLineWidth(2);
   grqgsII4i[1]->SetLineWidth(2);
   grqgsII4i[2]->SetLineWidth(2);
   grqgsII4i[3]->SetLineWidth(2);
   grqgsII4i[0]->SetMarkerStyle(kMultiply);
   grqgsII4i[1]->SetMarkerStyle(kFullDotLarge);
   grqgsII4i[2]->SetMarkerStyle(kStar);
   grqgsII4i[3]->SetMarkerStyle(kCircle);
   grqgsII4i[0]->SetMarkerColor(1);
   grqgsII4i[1]->SetMarkerColor(1);
   grqgsII4i[2]->SetMarkerColor(1);
   grqgsII4i[3]->SetMarkerColor(1);
   grqgsII4i[0]->Fit("myFit");
   grqgsII4i[1]->Fit("myFit");
   grqgsII4i[2]->Fit("myFit");
   grqgsII4i[3]->Fit("myFit");
   grqgsII4i[0]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4i[1]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4i[2]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4i[3]->GetFunction("myFit")->SetLineColor(1);
   grqgsII4i[0]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII4i[1]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII4i[2]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII4i[3]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII4i[0]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII4i[1]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII4i[2]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII4i[3]->GetFunction("myFit")->SetLineWidth(2);
   double qgsII4i0_p0 = grqgsII4i[0]->GetFunction("myFit")->GetParameter(0);
   double qgsII4i0_p1 = grqgsII4i[0]->GetFunction("myFit")->GetParameter(1);
   double qgsII4i1_p0 = grqgsII4i[1]->GetFunction("myFit")->GetParameter(0);
   double qgsII4i1_p1 = grqgsII4i[1]->GetFunction("myFit")->GetParameter(1);
   double qgsII4i2_p0 = grqgsII4i[2]->GetFunction("myFit")->GetParameter(0);
   double qgsII4i2_p1 = grqgsII4i[2]->GetFunction("myFit")->GetParameter(1);
   double qgsII4i3_p0 = grqgsII4i[3]->GetFunction("myFit")->GetParameter(0);
   double qgsII4i3_p1 = grqgsII4i[3]->GetFunction("myFit")->GetParameter(1);
   
   //--------------------------------------------------
   grqgsII3i[0]->SetName("grqgsII3i");
   grqgsII3i[0]->SetLineStyle(2);
   grqgsII3i[1]->SetLineStyle(2);
   grqgsII3i[2]->SetLineStyle(2);
   grqgsII3i[3]->SetLineStyle(2);
   grqgsII3i[3]->Draw("P");
   grqgsII3i[1]->Draw("P");
   grqgsII3i[2]->Draw("P");
   grqgsII3i[0]->Draw("P");
   grqgsII3i[0]->SetLineColor(2);
   grqgsII3i[1]->SetLineColor(2);
   grqgsII3i[2]->SetLineColor(2);
   grqgsII3i[3]->SetLineColor(2);
   grqgsII3i[0]->SetLineWidth(2);
   grqgsII3i[1]->SetLineWidth(2);
   grqgsII3i[2]->SetLineWidth(2);
   grqgsII3i[3]->SetLineWidth(2);
   grqgsII3i[0]->SetMarkerStyle(kMultiply);
   grqgsII3i[1]->SetMarkerStyle(kFullDotLarge);
   grqgsII3i[2]->SetMarkerStyle(kStar);
   grqgsII3i[3]->SetMarkerStyle(kCircle);
   grqgsII3i[0]->SetMarkerColor(2);
   grqgsII3i[1]->SetMarkerColor(2);
   grqgsII3i[2]->SetMarkerColor(2);
   grqgsII3i[3]->SetMarkerColor(2);
   grqgsII3i[0]->Fit("myFit");
   grqgsII3i[1]->Fit("myFit");
   grqgsII3i[2]->Fit("myFit");
   grqgsII3i[3]->Fit("myFit");
   grqgsII3i[0]->GetFunction("myFit")->SetLineColor(2);
   grqgsII3i[1]->GetFunction("myFit")->SetLineColor(2);
   grqgsII3i[2]->GetFunction("myFit")->SetLineColor(2);
   grqgsII3i[3]->GetFunction("myFit")->SetLineColor(2);
   grqgsII3i[0]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII3i[1]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII3i[2]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII3i[3]->GetFunction("myFit")->SetLineWidth(2);
   grqgsII3i[0]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII3i[1]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII3i[2]->GetFunction("myFit")->SetLineStyle(2);
   grqgsII3i[3]->GetFunction("myFit")->SetLineStyle(2);
   double qgsII3i0_p0 = grqgsII3i[0]->GetFunction("myFit")->GetParameter(0);
   double qgsII3i0_p1 = grqgsII3i[0]->GetFunction("myFit")->GetParameter(1);
   double qgsII3i1_p0 = grqgsII3i[1]->GetFunction("myFit")->GetParameter(0);
   double qgsII3i1_p1 = grqgsII3i[1]->GetFunction("myFit")->GetParameter(1);
   double qgsII3i2_p0 = grqgsII3i[2]->GetFunction("myFit")->GetParameter(0);
   double qgsII3i2_p1 = grqgsII3i[2]->GetFunction("myFit")->GetParameter(1);
   double qgsII3i3_p0 = grqgsII3i[3]->GetFunction("myFit")->GetParameter(0);
   double qgsII3i3_p1 = grqgsII3i[3]->GetFunction("myFit")->GetParameter(1);

   TLegend* legend;
   legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetNColumns(2);
   legend->SetHeader("Energies-Composition");
   legend->AddEntry("grqgsII4p","QGSJETII-04 proton","l");
   legend->AddEntry("grqgsII4p","10E18.5","p");
 
   legend->AddEntry("grqgsII4i","QGSJETII-04 iron", "l");
   legend->AddEntry("grqgsII4p2","10E19.5","p");
   legend->AddEntry("grqgsII3p", "QGSJETII-03 proton","l");
   legend->AddEntry("grqgsII4p1","10E19","p");
   legend->AddEntry("grqgsII3i","QGSJETII-03 iron", "l");
   legend->AddEntry("grqgsII4p3","10E20","p");
   
   legend->Draw();

   double TAmax = 45;
   double intZenDist = 0.25;
   // Calculate Integrals for weighted mean
   parameter1 = qgsII4p0_p1;
   parameter0 = qgsII4p0_p0;
   double qgsII4p0_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII4p1_p1;
   parameter0 = qgsII4p1_p0;
   double qgsII4p1_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII4p2_p1;
   parameter0 = qgsII4p2_p0;
   double qgsII4p2_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII4p3_p1;
   parameter0 = qgsII4p3_p0;
   double qgsII4p3_int = integral(f,0,45*PI/180,100)/intZenDist;
   // cout << qgsII4p3_int << endl;
   parameter1 = qgsII3p0_p1;
   parameter0 = qgsII3p0_p0;
   double qgsII3p0_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII3p1_p1;
   parameter0 = qgsII3p1_p0;
   double qgsII3p1_int = integral(f,0,45*PI/180,100)/intZenDist;   
   parameter1 = qgsII3p2_p1;
   parameter0 = qgsII3p2_p0;
   double qgsII3p2_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII3p3_p1;
   parameter0 = qgsII3p3_p0;
   double qgsII3p3_int = integral(f,0,45*PI/180,100)/intZenDist;

   parameter1 = qgsII4i0_p1;
   parameter0 = qgsII4i0_p0;
   double qgsII4i0_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII4i1_p1;
   parameter0 = qgsII4i1_p0;
   double qgsII4i1_int = integral(f,0,45*PI/180,100)/intZenDist;   
   parameter1 = qgsII4i2_p1;
   parameter0 = qgsII4i2_p0;
   double qgsII4i2_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII4i3_p1;
   parameter0 = qgsII4i3_p0;
   double qgsII4i3_int = integral(f,0,45*PI/180,100)/intZenDist;

   parameter1 = qgsII3i0_p1;
   parameter0 = qgsII3i0_p0;
   double qgsII3i0_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII3i1_p1;
   parameter0 = qgsII3i1_p0;
   double qgsII3i1_int = integral(f,0,45*PI/180,100)/intZenDist;   
   parameter1 = qgsII3i2_p1;
   parameter0 = qgsII3i2_p0;
   double qgsII3i2_int = integral(f,0,45*PI/180,100)/intZenDist;
   parameter1 = qgsII3i3_p1;
   parameter0 = qgsII3i3_p0;
   double qgsII3i3_int = integral(f,0,45*PI/180,100)/intZenDist;

   
   Double_t S800_ref[nEnergies*4];
   Double_t lgE[nEnergies*4];
   Double_t X[nEnergies*4];

   S800_ref[0] = qgsII4p0_int/qgsII3p0_int;
   S800_ref[1] = qgsII4p1_int/qgsII3p1_int;
   S800_ref[2] = qgsII4p2_int/qgsII3p2_int;
   S800_ref[3] = qgsII4p3_int/qgsII3p3_int;
   // cout << qgsII4p0_int;
   S800_ref[4] = qgsII3p0_int/qgsII3p0_int;
   S800_ref[5] = qgsII3p1_int/qgsII3p1_int;
   S800_ref[6] = qgsII3p2_int/qgsII3p2_int;
   S800_ref[7] = qgsII3p3_int/qgsII3p3_int;
   
   S800_ref[8] = qgsII4i0_int/qgsII3p0_int;
   S800_ref[9] = qgsII4i1_int/qgsII3p1_int;
   S800_ref[10] = qgsII4i2_int/qgsII3p2_int;
   S800_ref[11] = qgsII4i3_int/qgsII3p3_int;
   
   S800_ref[12] = qgsII3i0_int/qgsII3p0_int;
   S800_ref[13] = qgsII3i1_int/qgsII3p1_int;
   S800_ref[14] = qgsII3i2_int/qgsII3p2_int;
   S800_ref[15] = qgsII3i3_int/qgsII3p3_int;
   
   X[0] = qgsII4p0_int;
   X[1] = qgsII4p1_int;
   X[2] = qgsII4p2_int;
   X[3] = qgsII4p3_int;
   X[4] = qgsII3p0_int;
   X[5] = qgsII3p1_int;
   X[6] = qgsII3p2_int;
   X[7] = qgsII3p3_int;
   X[8] = qgsII4i0_int;
   X[9] = qgsII4i1_int;
   X[10] = qgsII4i2_int;
   X[11] = qgsII4i3_int;
   X[12] = qgsII3i0_int;
   X[13] = qgsII3i1_int;
   X[14] = qgsII3i2_int;
   X[15] = qgsII3i3_int;

   TCanvas plots2;
   Double_t energy[nEnergies];
   Double_t Y[nEnergies];
   Double_t EY[nEnergies];
   Double_t EYStdAv[nEnergies*4];
   for (int j = 0; j < 4; j++)
     {
       
       for (int k = 0; k < nEnergies; k++)
	 {
	   sum = 0;
	   for (int i = 0; i < nAngles; i++)
	     {
	       sum += stdev_norm[i+k*nAngles+j*nEnergies*nAngles];
	       //   cout << stdev_norm[i+k*nAngles+j*nEnergies*nAngles] << endl;;
	     }
	   EYStdAv[k+j*4] = sum/nAngles;
	   //   cout << sum/nAngles << endl;
	 }
     }
   //cout << "blah" << endl;
   //   for (int i = 0; i < 80; i++)
   //   {
       //  cout << stdev_norm[i] << endl;;
   //    }
   count = 0;
   TGraphErrors* graph_normalised[4];
   for (int j = 0; j < 4; j++)
     {
       count++;
       for (int i = 0; i < nEnergies; i++)
	 {
	   Y[i] = S800_ref[i+j*nEnergies];
	   if ( i % 4 == 0 )
	     energy[i] = 18.5;
	   else if ( i % 4 == 1)
	     energy[i] = 19;
	   else if ( i % 4 == 2)
	     energy[i] = 19.5;
	   else
	     energy[i] = 20;
	   // if ( count == 0 )
	   //   {
	   EY[i] = ( sqrt ( pow (EYStdAv[i+j*nEnergies] / X[i+4],2) + pow ( ( EYStdAv[i+4] * X[i+j*nEnergies] ) / ( pow ( X[i+4],2)),2)) ) / sqrt(4);
	       //   }
	       	   cout << sqrt ( pow (EYStdAv[i+j*nEnergies] / X[i+4],2) + pow ( ( EYStdAv[i+4] * X[i+j*nEnergies] ) / ( pow ( X[i+4],2)),2)) << endl;
		     cout << X[i+j*nEnergies] << " " << X[i+4] << " " << EYStdAv[i+j*nEnergies] << " " << EYStdAv[i+4] << endl;
	 }
       
       graph_normalised[j] = new TGraphErrors(nEnergies,energy,Y,0,EY);
     }   
   graph_normalised[0]->SetName("0");
   graph_normalised[1]->SetName("1");
   graph_normalised[2]->SetName("2");
   graph_normalised[3]->SetName("3");
   graph_normalised[3]->Draw("ALP");
   graph_normalised[1]->Draw("PL");
   graph_normalised[2]->Draw("PL");
   graph_normalised[0]->Draw("PL");
   graph_normalised[0]->SetLineColor(1);
   graph_normalised[1]->SetLineColor(2);
   graph_normalised[2]->SetLineColor(1);
   graph_normalised[3]->SetLineColor(2);
   graph_normalised[2]->SetLineStyle(2);
   graph_normalised[3]->SetLineStyle(2);
   graph_normalised[2]->SetLineWidth(2);
   graph_normalised[3]->SetLineWidth(2);
   graph_normalised[3]->SetTitle("");
   graph_normalised[3]->GetXaxis()->SetTitle("log_{10}(E)");
   graph_normalised[3]->GetYaxis()->SetTitle("#frac{<S800>}{<S800>_{p-QGSJETII-03}}");
   TLegend* legend2;
   legend2 = new TLegend(0.1,0.7,0.48,0.9);
   legend2->SetNColumns(1);
   legend2->SetHeader("Composition");
   legend2->AddEntry("0","QGSJETII-04 proton","l");
   legend2->AddEntry("2","QGSJETII-04 iron", "l");
   legend2->AddEntry("1", "QGSJETII-03 proton","l");
   legend2->AddEntry("3","QGSJETII-03 iron", "l");
   legend2->Draw();
   

   // TA graph - Original Graph
   int nPoints = 20;
   Double_t TA_lgE[] = {18.25, 18.35,18.45,18.55,18.65,18.75,18.85,18.95,19.05,19.15,19.25,19.35,19.45,19.55,19.65,19.75,19.85,19.95,20.05,20.15,20.25};
   Double_t TA_E3J[] = {2.022e+24,1.849e+24,1.701e+24,1.610e+24,1.539e+24,1.493e+24,1.632e+24,1.777e+24,1.967e+24,2.056e+24,2.346e+24, 2.380e+24, 2.376e+24, 2.668e+24, 2.364e+24,3.245e+24,2.726e+24,1.028e+24,2.283e+24,4.397e+23,1.385e+24};


   TCanvas TA2;
   TA2.SetLogy();
   TGraph* TA;
   TA = new TGraph(nPoints,TA_lgE,TA_E3J);
   TA->Draw("AP");
   TA->SetMarkerStyle(kFullDotLarge);

   //Need to modify TA graph by factors seen on my graph


   double slope;
   Double_t* II4protonY;
   II4protonY = graph_normalised[0]->GetY();
   Double_t* II3protonY;
   II3protonY = graph_normalised[1]->GetY();
   Double_t* II4ironY;
   II4ironY = graph_normalised[2]->GetY();
   Double_t* II3ironY;
   II3ironY = graph_normalised[3]->GetY();
   
 



   theApp.Run();
}
