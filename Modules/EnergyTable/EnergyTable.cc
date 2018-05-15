// Bradley Manning 10/5/2018

// Includes

// C++

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

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

using namespace std;
using namespace boost;
using namespace boost::property_tree;

// double integral(double(*f)(double x), double a, double b, int n)
// {
//   double step = (b-a) / n ; //width of each rectangle
//   double area = 0.0;
//   for (int i = 0; i < n; i++ )
//     {
//       area += f(a + (i + 0.5) * step) * step; // sum up each small rectangle
//     }
//   return area;
// }

// double f(double theta)
// {
//   return pow(10,(parameter1*pow((1/cos(theta)-1),2)+parameter0))*sin(theta)*cos(theta);
// }

struct parameters
{
  std::string directory;
  std::string tankPosition;
  std::string detector;
  std::vector<std::string> compositions;
  std::vector<std::string> primary;
  std::vector<double> energies;
  std::vector<double> zenith;
};

struct signal
{
  double error;
  double VEM;
};

const ptree& empty_ptree() {
  static ptree t;
  return t;
}

parameters read( std::istream & is)
{
  using boost::property_tree::ptree;
  ptree pt;
  read_xml(is,pt);
  parameters p;
  BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("EnergyTable"))
    {
      if( v.first == "EnergyTableParameters" ) {
	p.directory = v.second.get<std::string>("directory");
	p.tankPosition = v.second.get<std::string>("tankPosition");
	p.detector = v.second.get<std::string>("detector");
      }
      
    }

  BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("EnergyTable.EnergyTableParameters"))
  {
    if ( v.first == "energies" )
      {
	ptree subtree = v.second;
	BOOST_FOREACH(const ptree::value_type & f, subtree.equal_range("energy"))
	  {
	    std::string a = f.second.data();
	    p.energies.push_back(std::stod(a));
	  }
      }
    if ( v.first == "zenith" )
      {
	ptree subtree = v.second;
	BOOST_FOREACH(const ptree::value_type & f, subtree.equal_range("angle"))
	  {
	    std::string a = f.second.data();
	    p.zenith.push_back(std::stod(a));
	  }
      }
    if ( v.first == "primary" )
      {
	ptree subtree = v.second;
	BOOST_FOREACH(const ptree::value_type & f, subtree.equal_range("particle"))
	  {
	    std::string a = f.second.data();
	    p.primary.push_back(a);
	  }
      }
    if ( v.first == "compositions" )
      {
	ptree subtree = v.second;
	BOOST_FOREACH(const ptree::value_type & f, subtree.equal_range("model"))
	  {
	    std::string a = f.second.data();
	    p.compositions.push_back(a);
	  }
      }
  }
  return p;
}

signal getDataFromDirectory(string directory, parameters parameter)
{
  ifstream fin;
  string dir, filepath, file;
  double num; DIR *dp;
  
  struct dirent *dirp;
  struct stat filestat;
  vector<double> VEM;
  double nFiles = 0;
  double sumVEM = 0;
  double finalVEM;
  
  //cout << "dir to get files of: " << flush;
  dir = directory;
  dp = opendir( dir.c_str() );
  if (dp == NULL)
    {
      cout << "Error(" << errno << ") opening " << dir << endl;
    }
  while ((dirp = readdir( dp )))
    {
      filepath = dir + "/" + dirp->d_name;
      file = dirp->d_name;
      // If the file is a directory (or is in some way invalid) we'll skip it
      if (stat( filepath.c_str(), &filestat )) continue;
      if (S_ISDIR( filestat.st_mode )) continue;

      // Endeavor to read a single number from the file and display it
      fin.open( filepath.c_str() );

      //File is opened

      while ( fin >> num ) 
	{
	  if ( file.find(parameter.tankPosition) != std::string::npos) {
	    VEM.push_back(num);
	    sumVEM = sumVEM + num;
	    nFiles++;
	  }
	}
    }
  finalVEM = sumVEM / nFiles;
  cout << "VEM  = " << finalVEM << endl;
  

  //Calculate Variance
  double variance = 0;;
  for (int i = 0; i<=VEM.size(); i++) {
    VEM[i] = pow((VEM[i] - finalVEM),2);
    variance = variance + VEM[i];
  }
  variance = variance / VEM.size();
  cout << "Variance = " << variance << endl;
  double error = sqrt(variance) / sqrt(VEM.size());

  signal signal;
  signal.VEM = finalVEM;
  signal.error = error;
  return signal;				
}

int main()
{

  std::ifstream input("EnergyTable.xml");
  parameters parameter = read( input );
   
  //PLOTS ---------------------------------------------------------------
  TApplication theApp("app",0,0);
  TCanvas plots;

  const int nAngles = parameter.zenith.size();
  const int nEnergies = parameter.energies.size();
  const int nModels = parameter.compositions.size();
  const int nPrimaries = parameter.primary.size();
  Double_t ex [nAngles];
  Double_t ey [nAngles];
  Double_t graph_angle [nAngles];
  Double_t graph_VEM [nAngles];
    
  TGraphErrors* grqgsII4p[nEnergies];
  TGraphErrors* grqgsII3p[nEnergies];
  TGraphErrors* grqgsII4i[nEnergies];
  TGraphErrors* grqgsII3i[nEnergies];

  string detector = parameter.detector;

  for (int k = 0; k < nModels; k++)
    {
      string model = parameter.compositions[k];
      for (int l = 0; l < nPrimaries; l++ )
	{
	  string primary = parameter.primary[l];
	  for (int i = 0; i < nEnergies; i++ )
	    {
	      double energy = parameter.energies[i];
	      for (int j = 0; j < nAngles; j++)
		{
		  ostringstream directory;
		  double angle = parameter.zenith[j];
		  cout << angle << " " << energy << " " << primary << " " << model << endl;
		  directory << "/remote/tesla/bmanning/data/EnergyTable/" << detector << "/" << model << "/"
			    << primary << "/" << energy << "/" << angle << "deg";
		  signal signals = getDataFromDirectory(directory.str(),parameter);
		  graph_VEM[j] = signals.VEM;
		  ex[j] = 0;
		  ey[j] = sqrt(signals.error); //Signal error is the variance
		  graph_angle[j] = 1/(cos(angle*(M_PI/180.))); // RADIANS
		} // angle
	      if (model == "qgsII4" && primary == "proton" )
		{
		  grqgsII4p[i] = new TGraphErrors(nAngles, graph_angle, graph_VEM, ex, ey);
		} else if (model == "qgsII4" && primary == "iron" )
		{
		  grqgsII4i[i] = new TGraphErrors(nAngles, graph_angle, graph_VEM, ex, ey);
		} else if (model == "qgsII3" && primary == "proton")
		{
		  grqgsII3p[i] = new TGraphErrors(nAngles, graph_angle, graph_VEM, ex, ey);
		} else if (model == "qgsII3" && primary == "iron")
		{
		  grqgsII3i[i] = new TGraphErrors(nAngles, graph_angle, graph_VEM, ex, ey);
		}
	      
	    } // energy
	  
	} // primary
    
    } // model


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



  return 1;  
}
  
