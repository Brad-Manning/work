//Bradley Manning 4/6/2018
//SNR.cc


#include <vector>
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"

#include <sstream>
#include <iostream>
using namespace std;
//First 600 bins always contain noise and no signal(?)
//Calculate the RMS of the first 600 bins

vector < double > getFirst600Bins(TH1F* traceHistogram)
{
  vector < double > bins;
  //for (Int_t i=0;i<traceHistogram.GetSize();i++) bins[i] = traceHistogram[i];
  for (Int_t i=0; i<600; i++)
    {
      bins.push_back(traceHistogram->GetBinContent(i));
      //cout << bins[i] << " " << i <<  endl;
    }
  return bins;
}

struct meanRMS
{
  double mean;
  double RMS;
};

meanRMS calculateRMS(vector < double > first600Bins )
{
  meanRMS meanrms;
  double sum = 0;
  double sum2 = 0;
  for (int i = 0; i < first600Bins.size(); i++ )
    {
      sum = sum + pow (first600Bins[i], 2 );
      sum2 = sum2 + first600Bins[i];
    }
  double mean = sum2 / first600Bins.size();
  meanrms.mean = mean;
  //cout << mean << endl;
  sum = 0;
  for (int i = 0; i < first600Bins.size(); i++ )
    {
      sum = sum + pow (first600Bins[i]-mean, 2 );
    }
  double RMS = sqrt(sum / first600Bins.size() );
  meanrms.RMS = RMS;
  //  cout << RMS << endl;
  return meanrms;
  
}

struct IsValid
{
  bool valid;
  TH1F* histogram;
};
  
IsValid getHistogram(TFile *file, string histogramName)
{
  IsValid IsHistogram;
   // histogramName << "PMT4_20_45427494";
  if (file->GetListOfKeys()->Contains(histogramName.c_str()) )
    {
      TH1F* histogram1 = (TH1F*)file->Get(histogramName.c_str());
      IsHistogram.histogram = histogram1;
      IsHistogram.valid = true;
      // cout << "Valid HistogramName" << endl;
      return IsHistogram;
    }
  else {
    //cerr << "Invalid HistogramName" << endl;
    IsHistogram.valid = false;
    return IsHistogram;
  }
}

void makeSNRplots(TH1F *histogram, string name, double sigma, double mean)
{
  int n = 4; //arbitrary atm
  double noise = sigma / sqrt(n);
  double integral = 0;
  // vector < double > SNR;
  TH1* outputHistogram = new TH1F(name.c_str(), name.c_str(), (histogram->GetSize())/n , 0, histogram->GetSize());
  
  for (Int_t i=0; i<histogram->GetSize(); i=i+n)
    {
      for (int j = 0; j < n; j++ )
	{
	  integral  = integral + (histogram->GetBinContent(i+j)-mean);
	  // cout << integral << endl;
	}
      //  SNR.push_back(integral / noise);
      outputHistogram->Fill(i+(n/2), integral / noise);
      // cout << "noise: " << noise << " integral: " << integral << endl;
      integral = 0;
    }

  //SNR for a histogram calculated now write to new TFile
  TFile *out = new TFile("output.root", "UPDATE");
  TCanvas* canvas = new TCanvas(name.c_str(), name.c_str(), 800, 600);
  TPad pad1("Original Histogram", "ADC counts", 0.01, 0.99, 0.99, 0.5);
  TPad pad2("SNR", "SNR", 0.01, 0.5, 0.99, 0);

  pad1.Draw();
  pad1.cd();
  histogram->Draw();
  histogram->SetStats(false);
  canvas->cd();
  pad2.Draw();
  pad2.cd();
  outputHistogram->Draw();
  outputHistogram->SetTitle("SNR");
  outputHistogram->SetStats(false);
  //outputHistogram->Write();
  canvas->Write();
  out->Write();
  return;
  
}


int main()
{
    
  TFile *file = new TFile("/remote/adelphi/bmanning/Downloads/SSDSD_traces.root");

  //TFile *out  = new TFile("output.root", "NEW");
  cout << "?? " << endl;
  
  stringstream histogramName;

  for (int i = 20; i <= 22; i++)
    {
  
      for (int j = 27494; j < 33917; j++)
	{
	  histogramName << "PMT4_" <<  i << "_" << 454 << j;
	  IsValid histogram1 = getHistogram(file, histogramName.str());
	  //cout << histogramName.str() << endl;
	  if (histogram1.valid == true)
	    {
	      vector < double > first600Bins = getFirst600Bins(histogram1.histogram);
	      meanRMS meanrms = calculateRMS(first600Bins);
	      makeSNRplots(histogram1.histogram, histogramName.str(), meanrms.RMS, meanrms.mean );
	      // cout << RMS << " " << histogramName.str() << endl;
	    }
	  histogramName.str( std::string() );
	  histogramName.clear();
	  // histogramNames.push_back(histogramName.str());
	}
  
    }
  for (int i = 1733; i <= 1748; i++)
    {

      for (int j = 27494; j < 33917; j++)
      {
	histogramName << "PMT4_" << i << "_" << 454 << j;
	IsValid histogram1 = getHistogram(file, histogramName.str());
	if (histogram1.valid == true)
	  {
	    vector < double > first600Bins = getFirst600Bins(histogram1.histogram);
	    meanRMS meanrms = calculateRMS(first600Bins);
	    makeSNRplots(histogram1.histogram, histogramName.str(), meanrms.RMS, meanrms.mean );
	    //cout << RMS << " " << histogramName.str() << endl;
	  }
	histogramName.str( std::string() );
	histogramName.clear();
	//	histogramNames.push_back(histogramName.str());
      }

    }
  
  //cout << histogramName.str() << endl;

  








 

  return 0;
}




//SNR.cc
