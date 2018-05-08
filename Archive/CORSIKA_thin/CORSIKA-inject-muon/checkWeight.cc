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

int main ()

{
int nParticles1 = 0;
int nParticles2 = 0;
double nPartWeight1 = 0;
double nPartWeight2 = 0;

ifstream weight1;
ifstream weight2;
double w1;
double w2;

weight1.open ("qgsII4proton10E200degweight.txt");
weight2.open ("qgsII4proton10E190degweight.txt");

while ( weight1 >> w1 )
  {
nParticles1 += 1;
nPartWeight1 += w1;
}

while ( weight2 >> w2 )
  {
nParticles2 += 1;
nPartWeight2 += w2;
}
cout << "The number of particles for 10E20 is " << nParticles1 << " with weight : " << nPartWeight1 << endl;
cout << "The number of particles for 10E19 is " << nParticles2 << " with weight : " << nPartWeight2 << endl; 


}
