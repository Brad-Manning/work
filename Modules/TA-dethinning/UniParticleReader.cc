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

//Globals
double fTank_pos;
bool fPositionDefined;
bool createdROOTPlots = false;
TFile *Hist;
TCanvas* canvas;
TH2* h2;
TH2* h2_1;
TH2* h2_2;
TH2* h1;
TH2* h1_1;
TGraph* showerDirection;
TH1* electron_LDF;
TH1* gamma_LDF;
TH1* muon_LDF;
TH1* all_LDF;
TH1* electron_LDFValid;
TH1* gamma_LDFValid;
TH1* muon_LDFValid;
TH1* all_LDFValid;

using namespace std;

//Place Functions here:
void usage();
bool IsForwardSlash(char c)
{
  switch(c)
    {
    case '/':
      return true;
    default:
      return false;
    }
}

double Linsley_rho(const double *x, const double *p) {
  double R = x[0];
  double CN = p[0];
  double alpha = p[1];
  double eta = p[2];
  double R_0 = p[3];
  double rho = 1.0;
  R /= R_0;
  rho *= TMath::Power(R, -alpha) * TMath::Power(1.0 + R, -(eta - alpha));
  rho *= CN / R_0 / R_0;
  return rho;
}


//Parameter structure values from XML
struct parameters
{
  std::string file;
  std::string particle;
  double phi;
  double phiAngle;
  double delta;
  double startingPosition;
  bool weightedSimulation;
  int threshold;
  int detector;
  bool makeSurfacePlots;
  bool azimuthTest;
};

parameters read( std::istream & is)
{
  // reading from XML using boost property tree
  using boost::property_tree::ptree;
  ptree pt;
  read_xml(is, pt);
  parameters p;
  BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("CORSIKAParameters") ) {
    if( v.first == "CORSIKA" ) {
      p.file = v.second.get<std::string>("file");
      p.particle = v.second.get<std::string>("particle");
      p.phi = v.second.get<double>("phi");
      p.delta = v.second.get<double>("delta");
      p.startingPosition = v.second.get<double>("startingPosition");
      p.weightedSimulation = v.second.get<bool>("weightedSimulation");
      p.threshold = v.second.get<int>("threshold");
      p.detector = v.second.get<int>("detector");
      p.phiAngle = v.second.get<double>("phiAngle");
      p.makeSurfacePlots = v.second.get<bool>("makeSurfacePlots");
      p.azimuthTest = v.second.get<bool>("azimuthTest");
    }

  }
  return p;
}

std::vector<int> findTypes(std::string particle)
{
  std::vector<int> ApplicableParticles;
  
  if ( particle == "muon" )
    {
      //Corresponding type values from CORSIKA corresponding to muon
      ApplicableParticles.push_back(5);
      ApplicableParticles.push_back(6);
    } else if ( particle == "gamma" )
    {
      ApplicableParticles.push_back(1);
      ApplicableParticles.push_back(1);
    } else if ( particle == "electron")
    {
      ApplicableParticles.push_back(2);
      ApplicableParticles.push_back(3);
    } else if (particle == "all")
    {
      ApplicableParticles.push_back(0);
      ApplicableParticles.push_back(15);
    }
  return ApplicableParticles;
}

void weightedSimulation(int nParticles, bool weighted, int threshold)
{
  if (weighted)
    {
      cout << "Number of incident particles = "<< nParticles << ", Since this is  > " << threshold << ", performing weighted simulation" << endl;;
    } else { return; }
}



int createGEANT4Files(int argc, char **argv, parameters parameter, bool useWeights, double nParticles)
{

  //Initialise ROOT histograms ---------------------------------------------------------------------------
  if (parameter.makeSurfacePlots)
    {
      if(!createdROOTPlots) {
	Hist = new TFile("2Dhist.root", "RECREATE");
	canvas = new TCanvas("canvas", "CanvasBrowser", 1000,1000);
	h2 = new TH2F("h2", "particles at ground", 2000, -5000, 5000, 2000, -5000,5000);
	h2_1 = new TH2F("h2_1", "particles at ground", 2000, -5000, 5000, 2000, -5000,5000);
	h2_2 = new TH2F("h2_2", "particles at ground", 2000, -5000, 5000, 2000, -5000, 5000);
	h1 = new TH2F("h1", "particles in shower plane", 1000, -5000, 5000, 1000, -5000, 5000);
	h1_1 = new TH2F("h1_1", "particles in shower plane", 1000, -5000, 5000, 1000, -5000, 5000);
	showerDirection = new TGraph();
	createdROOTPlots = true;
      }
    }
  //------------------------------------------------------------------------------------------------------

  //Mersenne Twister 1997 Algorithm ----------------------------------------------------------------------
  //General C++ random number generators have a biased for Poisson Distributions with a mean << 1
  mt19937 gen;
  gen.seed(time(NULL));
  //------------------------------------------------------------------------------------------------------

  //Find which particle types are applicable (depending on input from XML)--------------------------------
  std::vector<int> validParticles = findTypes(parameter.particle);
  //------------------------------------------------------------------------------------------------------

  //Testing Variables required for Azimuth Distribution of particles--------------------------------------
  //I could/shoudl make this a flag in the XML
  bool fSetAzimuth = false;
  int howmanyPhi =0;
  ofstream testAzimuth;
  ofstream primtest;
  double howManyParticle = 0;
  primtest.open("primtest.txt");
  testAzimuth.open("test.txt", std::ios_base::app);
  //------------------------------------------------------------------------------------------------------

  //Necessary multiplier that allows weighted simulations, for small distances from shower cores ---------
  double weightMultiplier;
  if (useWeights)
    {
      weightMultiplier = parameter.threshold / nParticles;
    }
  //------------------------------------------------------------------------------------------------------
  
  //Initialisation of variables used in particle reading, also character array containing particle names
  //in GEANT4 syntax--------------------------------------------------------------------------------------
  double count, r,phi,px, py, pz, type, weight, zenith, area_tank;
  char* strName[] = {"gamma", "e+", "e-", "?" , "mu+" , "mu-" , "pi0" , "pi+" , "pi-" , "kaon0" , "kaon+" , "kaon-" , "neutron" , "proton" , "anti_proton"};
  vector<string> vecName(strName, strName+15);
  count = 0;
  //------------------------------------------------------------------------------------------------------
  
  ofstream G4file, RunInfo, weightAll;

  //Loop over files, and read the files
  int start = (argc < 2) ? 0 : 1;
  for (int iFile = start; iFile <= argc - 1; iFile++) {
 
    corsikafile file(argv[iFile]);
    //Ensures we do not repeat output if we switch to weights
    if (!useWeights) cout << "Current File: " << argv[iFile] << "\n";
    string fileName = argv[iFile];

    //Fancy footwork to create a GEANT4 macro file name dependent on the energy/zenith/model of the shower
    size_t pos_start = fileName.find("DAT");
    size_t pos_end = fileName.find(".part");
    string G4name = fileName.substr(pos_start,pos_end-pos_start);
    G4name += std::to_string(iFile);
    G4name.erase(remove_if(G4name.begin(), G4name.end(), &IsForwardSlash), G4name.end());

    //runInfo file contains number of particles, tank position and whether or not simulation is weighted
    //This is passed to GEANT4, so that the simulation knows which methods to use
    string infoName = "runInfo.txt";
    
    if (useWeights)
      {
	string weightName = G4name+"weight.txt";
	const char * wei = weightName.c_str();
	weightAll.open(wei);
	//All new runs in GEANT4 must start with a dummy particle (may not be necessary anymore)
	//Previous motivation was so that GEANT4 could detect a new set/new shower of particles
	//	weightAll << 0 << "\n";
      }

    //Append G4 macro with appropriate file type
    G4name = G4name+".mac";
    const char * G4 = G4name.c_str();
    const char * runInfo = infoName.c_str();
    //Initialise dummy particle
    G4file.open (G4);
    G4file << "/run/initialize" << "\n";
    // G4file << "/gun/energy 0" << "\n";
    // G4file << "/run/beamOn 1" << "\n";
    RunInfo.open (runInfo);
  
    double min;
    double max;
    //Only ask for input of tank position once - put this input into a .sh script or type it manually
    //Currently no cout to ask for output - do not confuse halting of the program for crashes! 
    if (!fPositionDefined) { cin >> fTank_pos; 	fPositionDefined = true; }
    max = (1+parameter.delta)*fTank_pos;
    min = (1-parameter.delta)*fTank_pos;
    //max and min radius (shower plane) to read particles from. This is defined from delta, defined
    //in the XML, it is:
    /*
      Shower Plane:

                         _ _ _ _ _ _ _ _ _ _ _ _
                         \                     /  ^
			  \                   /   |
			   \        .        /    | 2*delta
			    \	    ^	    /     |
			     \_ _ _ | _ _ _/      ⌄
                             A      |      B
                                    |
                                    | 
                                    |r
                                    |
                                    |   phi is the angle that spans from A to B at the shower core  
                                    |
                                    ⌄   
                                    o  Shower core     
 
    */
   

    //Write to RunInfo
    RunInfo << fTank_pos << "\n";
    RunInfo << useWeights << "\n";
    
    //Create the spanning azimuth phi from input in XML
    double minPhi = -parameter.phi/2.;
    double maxPhi = parameter.phi/2.;
    //Tank radius given in metres
    double r_tank = 1.8;
     while (file.findnextrun())
      {
	run theRun=file.getnextrun();
	while (theRun.findnextevent())
	  {
	    event theEvent=theRun.getnextevent();
	    //Necessary to recalculate azimuth of the primary particle after changing co-ordinate systems
	    //to mathematical convention;
	    double ppx = theEvent.primpx;
	    double ppy = theEvent.primpy;
	    //cout << ppy << " " << ppx << endl;
	    // double ppz = theEvent.primpz;
	    double primphi = atan2(ppy, ppx);

	    if (createdROOTPlots) {
	    //Shower direction
	    //Choose arbitrary points to plot a line representing the shower direction
	    //First point is the core position/origin at (0,0)
	    showerDirection->SetPoint(showerDirection->GetN(),0,0);
	    //Second point is at 5000,0 (east) ( histogram limits)
	    double xpos = 5000;
	    double ypos = 0;
	    double xnew;
	    double ynew;
	    xnew = xpos*cos(primphi) - ypos*sin(primphi);
	    ynew = xpos*sin(primphi) + ypos*cos(primphi);
	    showerDirection->SetPoint(showerDirection->GetN(), xnew, ynew);
	    }


	    
	    while (theEvent.findnextparticle())
	      {
		particle Part=theEvent.getnextparticle();
		//xprime and yprime are x and y co-ordinates for the particles in the shower plane
		//rotate axis CLOCKWISE by shower azimuth (around Z)
	       	double xprime = Part.x*cos(primphi) + Part.y*sin(primphi);
		double yprime = Part.x*sin(primphi) - Part.y*cos(primphi);
		//Further rotate ANTICLOCKWISE axis by 90 deg (to make 0 azimuth 3 o'clock in shower plane)
		double temp = xprime;
		xprime = xprime*cos(M_PI/2.) - yprime*sin(M_PI/2.);
		yprime = temp*sin(M_PI/2.) + yprime*cos(M_PI/2.);
		//rotate axis by  shower zenith (around X)
		xprime = xprime;
		yprime = yprime*cos(Part.primtheta);
		//double zprime = Part.y*sin(Part.primtheta)/100.0; Unused as of 23/5/2018
		r = (sqrt( pow(yprime,2 ) + pow(xprime,2)))/100.0; //radius in shower plane
		phi=atan2( yprime , xprime ); //azimuth in shower plane - NOTE: Must use atan2 to consider all 4 quadrants
	
		//time=Part.t-(Part.zstart-theRun.OBSLEVELS[Part.obslev-1])*(1.0E+07/cos(Part.primtheta))/c-Part.x*1.0E+07*sin(Part.primtheta)*cos(Part.primphi)/c-Part.y*1.0E+07*sin(Part.primtheta)*sin(Part.primphi)/c;//time in shower front

		//Momentum of each particle - Used to define energy and direction in GEANT4
		px = Part.px;
		py = Part.py;
		pz = Part.pz;
		//Zenith is in RADIANS
		zenith = acos (pz / ( sqrt( pow(px,2) + pow(py,2) + pow(pz,2)) )) ;


		//Azimuth ranges from -PI to PI. With 0 Azimuth being EAST. (x,0)
		if (!fSetAzimuth) {
		  minPhi = (parameter.phiAngle)*(M_PI/180.);
		  minPhi = minPhi - parameter.phi/2.;
		  maxPhi = minPhi + parameter.phi;
		  primtest << Part.primtheta << " " << primphi << "\n";
		  //Series of if statements to keep min and max phi in the same notation as atan2 range from -PI to PI
		  if (minPhi > M_PI )
		    {
		      minPhi = -2*M_PI + minPhi;
		    } else if (minPhi < -M_PI)
		    {
		      minPhi = 2*M_PI + minPhi;
		    }
		  if (maxPhi > M_PI )
		    {
		      maxPhi = -2*M_PI + maxPhi;
		    } else if (maxPhi < -M_PI)
		    {
		      maxPhi = 2*M_PI + maxPhi;
		    }
		  //Output just once - the azimuth of the primary and min/max phi of particles considered in shower plane
		  cout << "Primary particle azimuth: " << primphi  << " Previous azimuth: " << Part.primphi << " MaxPhi: " << maxPhi << " MinPhi: " << minPhi << endl;
		  fSetAzimuth = true;
		}

		//Get type & weight of particle
		type = Part.type;
		weight = Part.weight;

		//Calculation of projected area of the water tank and scintillator
		//Assuming that the scintillator has a negligible height, and so the projection is negligible.
		area_tank = 3 * cos ( zenith ); //Area of Scintillator is 3 sq m;
		//		cout << area_tank << endl;
		//Area of the section of the ring that we are considering in the shower plane
		double area_section = M_PI * ( pow(fTank_pos+parameter.delta*fTank_pos,2) - pow(fTank_pos-parameter.delta*fTank_pos,2) ) * ( (parameter.phi) / ( 2*M_PI ) );

		//Creating a Poisson Distribution---------------------------------------------------------------------------
		//Using a Poisson randomisation allows us to create a random series of particles that may be incident in the
		//tank. The result from the poisson distribution is the number of particles that will be injected into the
		//GEANT4 simulation. The primary benefit of this is massive performance improvements.
		double average_n = weight * ( area_tank / area_section );
       		boost::poisson_distribution<int> distribution(average_n);
		int n = distribution(gen);
		
		//Simple boolean to determine whether or not a particle is within the area defined in the shower plane
		//testing for plots
			//Only create 2D histograms once
		if (parameter.makeSurfacePlots)
		  {
		    if (createdROOTPlots) {
		      if (r >= min && r <= max )
			{
			  h2->Fill(Part.x/100.,Part.y/100. , Part.weight);
			  if (phi >= minPhi && phi <= maxPhi)
			    {
			      h2_2->Fill(Part.x/100.,Part.y/100. , Part.weight);
			    }
			}
		      h2_1->Fill(Part.x/100.,Part.y/100.,Part.weight);
		      if (r >= min && r <= max)
			{
			  h1->Fill( xprime/100.0 , yprime/100.0 , Part.weight);
			  if (phi >= minPhi && phi <= maxPhi)
			    {
			      h1_1->Fill(xprime/100.0, -yprime/100., Part.weight);
			      //if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) )// howmanyPhi = howmanyPhi + Part.weight;
			    }
			  else if (maxPhi < minPhi)
			    {
			      if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                                
				{
				  //if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) //howmanyPhi = howmanyPhi + Part.weight;
				}
			    }
			}
		    }
		  }

		if (parameter.azimuthTest) {
		  if (r >= min && r <= max)
		    {
		      if (phi >= minPhi && phi <= maxPhi)
			{
			  if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) howmanyPhi = howmanyPhi + Part.weight;
			}
		      else if (maxPhi < minPhi)
			{
			  if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                                
			    {
			      if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) howmanyPhi = howmanyPhi + Part.weight;
			    }
			}
		    }
		}
		
		//Always initialise as false for each particle
	        bool particleApplicable = false;
		if (1==1)//!parameter.azimuthTest)
		  {
		    if (1==1) //Only continue following routines if there actually are particles that may be injected
		      {
			if  ((r >= min) && (r <= max)) //Check whether particle is within the area by radius
			  {
			    if (phi >= minPhi && phi <= maxPhi) //Then check whether the particle is within the area by angle
			      {
				howManyParticle = howManyParticle + weight;
				particleApplicable = true;
				// if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) // howmanyPhi++;
			      }
			    else if (maxPhi < minPhi) //At the +PI/-PI point there is a chance maxPhi < minPhi, the
			      //following if statement takes this into account.
			      {
				if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                    
				  {
				    particleApplicable =true;
				    // if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) //howmanyPhi++;
				  }
			      }
			   }
			if (particleApplicable) //true if particle is within area
			  {
			    if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) //Is the particle valid/one we are considering?
			      //This could be moved above.
			      //fill histogram for applicable particles
			      {
				
				//Write G4 macro file
				G4file << "/gun/momentum " << -px << " " << -py << " " << -pz << "\n";
				G4file << "/gun/particle " << vecName[type-1] << "\n";
				if (useWeights)   //is 10 suitable? Think about it after confirming that it works.
				  //You cannot fire a non-integer number of particles into GEANT4 it crashes if you do.
				  //This number > 10 is to somewhat guarantee that after applying the weight multiplier, we do not
				  //Try to run a non-integer number of particles that always rounds to zero. ie. 0.2, from n = 1
				  //and a weight multiplier of 0.2. Instead of running 5 instances of 0.2 particles, we would run 5
				  //instances of 0 particles. Critical if this 1 was a muon.
				  {
				    G4file << "/run/beamOn " << 1 << endl;//round(n*weightMultiplier) << "\n";
				    weightAll << n << "\n";// ( n / round(n*weightMultiplier) ) << "\n";
				    //Count is the number of particles GEANT4 is simulating. It is used to determine whether or not
				    //weights should be used
				    count = count + n;//round(n*weightMultiplier);
				    //cout << round(n*weightMultiplier) << " " << weightMultiplier << " " << n << endl;
				  } else {
				  G4file << "/run/beamOn " << (int) n <<"\n";
				  //line below is unnecessary unless weights are being used - 
				  weightAll << 1 << "\n";
				  count = count + n;
				}
				
				
			      }
			    // types > 16 are ions
			    if (type >= 16 && ( parameter.particle == "all" ))
			      {
				G4file << "/gun/momentum " << -px << " " << -py << " " << -pz << "\n";
				G4file << "/gun/particle ion " << "\n";
				G4file << "/gun/ion " << type << " " << type*2 << "\n";
				
				if (useWeights)
				  {
				    G4file << "/run/beamOn " << 1 << "\n"; //round(n*weightMultiplier) << "\n";
				    weightAll << n << "\n"; //( n / round(n*weightMultiplier) ) << "\n";
				    count = count + n; //round(n*weightMultiplier);
				  } else {
				  G4file << "/run/beamOn " << (int) n << "\n";
				  weightAll << 1 << "\n";
				  count = count + n;
				}
			      }
			  }
			
		      }
		  }
	      }
	  }
      }
     //Add number of particles to RunInfo and close ostreams
    RunInfo << count << "\n";
    RunInfo << howManyParticle << "\n";
    if (useWeights) RunInfo << 1/weightMultiplier << "\n";
    G4file.close();
    RunInfo.close();

    //Only write ROOT file once
    if (parameter.makeSurfacePlots)
      {
	if(createdROOTPlots) {
	  showerDirection->Write();
	  h1->Write();
	  h1_1->Write();
	  h2_1->Write();
	  h2_2->Write();
	  h2->Write();
	  Hist->Write();
	}
      }
    //testing purposes
    testAzimuth << howmanyPhi << " " << minPhi << " " << maxPhi << "\n";
  
  }
     
  return count;  
}

int createLDF(int argc, char **argv, parameters parameter)
{
  electron_LDF = new TH1F("electron LDF histogram", "LDF - Electrons", 100, 0, 4000);
  gamma_LDF = new TH1F("gamma LDF histogram", "LDF - Gamma", 100, 0, 4000);
  muon_LDF = new TH1F("muon LDF histogram", "LDF - Muon", 100, 0, 4000);
  all_LDF = new TH1F("all LDF histogram", "LDF - All", 100, 0, 4000);
  electron_LDFValid = new TH1F("electron vLDF histogram", "LDF - Electrons (Incident on 1 Detector)", 100, 0, 4000);
  gamma_LDFValid = new TH1F("gamma vLDF histogram", "LDF - Gamma (Incident on 1 Detector)", 100, 0, 4000);
  muon_LDFValid = new TH1F("muon vLDF histogram", "LDF - Muon (Incident on 1 Detector)", 100, 0, 4000);
  all_LDFValid = new TH1F("all vLDF histogram", "LDF - All (Incident on 1 Detector)", 100, 0, 4000);  

  TFile* componentLDFs = new TFile("componentLDFs.root", "RECREATE");
  TCanvas* LogScale = new TCanvas("LogScale Canvas", "LogScale Canvas", 800,600);
  double minXmax = 10000;
  double maxXmax = 0;
  TCanvas* multipleLDFs_g = new TCanvas("MultipleLDFs_g", "MultipleLDFs_g", 800, 600);
  TCanvas* multipleLDFs_m = new TCanvas("MultipleLDFs_m", "MultipleLDFs_m", 800, 600);
  TCanvas* multipleLDFs_e = new TCanvas("MultipleLDFs_e", "MultipleLDFs_e", 800, 600);
  TCanvas* multipleLDFs_a = new TCanvas("MultipleLDFs_a", "MultipleLDFs_a", 800, 600);
  bool firstDraw = true;
  bool firstSave = false;
  bool isRangeDrawn[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	
  TH1F* min_LDF_g;// = new TH1F("min LDF", "LDF", 100, 0, 4000);
  TH1F* max_LDF_g;// = new TH1F("max LDF", "LDF", 100, 0, 4000);
  TH1F* min_LDF_e;// = new TH1F("min LDF", "LDF", 100, 0, 4000);
  TH1F* max_LDF_e;// = new TH1F("max LDF", "LDF", 100, 0, 4000);
  TH1F* min_LDF_m;// = new TH1F("min LDF", "LDF", 100, 0, 4000);
  TH1F* max_LDF_m;// = new TH1F("max LDF", "LDF", 100, 0, 4000);


  //Jose TEST files---------------------------------------------------------------------------------------
  ofstream JoseTestFile_e;
  JoseTestFile_e.open("JoseTestFile_e.txt");
  ofstream JoseTestFile_g;
  JoseTestFile_g.open("JoseTestFile_g.txt");
  ofstream JoseTestFile_m;
  JoseTestFile_m.open("JoseTestFile_m.txt");
  ofstream JoseTestFile_all;
  JoseTestFile_all.open("JoseTestFile_all.txt");
  int event_num = 0;
  //------------------------------------------------------------------------------------------------------
  
  //Mersenne Twister 1997 Algorithm ----------------------------------------------------------------------
  //General C++ random number generators have a biased for Poisson Distributions with a mean << 1
  mt19937 gen;
  gen.seed(time(NULL));
  //------------------------------------------------------------------------------------------------------

  //Find which particle types are applicable (depending on input from XML)--------------------------------
  std::vector<int> validParticles = findTypes(parameter.particle);
  //------------------------------------------------------------------------------------------------------

  //Testing Variables required for Azimuth Distribution of particles--------------------------------------
  //I could/shoudl make this a flag in the XML
  bool fSetAzimuth = false;
  int howmanyPhi =0;
  ofstream testAzimuth;
  ofstream primtest;
  double howManyParticle = 0;
  primtest.open("primtest.txt");
  testAzimuth.open("test.txt", std::ios_base::app);
  //------------------------------------------------------------------------------------------------------


  //Initialisation of variables used in particle reading, also character array containing particle names
  //in GEANT4 syntax--------------------------------------------------------------------------------------
  double count, r,phi,px, py, pz, type, weight, zenith, area_tank;
  char* strName[] = {"gamma", "e+", "e-", "?" , "mu+" , "mu-" , "pi0" , "pi+" , "pi-" , "kaon0" , "kaon+" , "kaon-" , "neutron" , "proton" , "anti_proton"};
  vector<string> vecName(strName, strName+15);
  count = 0;
  //------------------------------------------------------------------------------------------------------
  
  ofstream G4file, RunInfo, weightAll;

  //Loop over files, and read the files
  int start = (argc < 2) ? 0 : 1;
  for (int iFile = start; iFile <= argc - 1; iFile++) {
 
    corsikafile file(argv[iFile]);
    //Ensures we do not repeat output if we switch to weights
    cout << "Current File: " << argv[iFile] << "\n";
    string fileName = argv[iFile];
    event_num++;
    //Fancy footwork to create a GEANT4 macro file name dependent on the energy/zenith/model of the shower
    //runInfo file contains number of particles, tank position and whether or not simulation is weighted
    //This is passed to GEANT4, so that the simulation knows which methods to use
    
    double min;
    double max;
    //Only ask for input of tank position once - put this input into a .sh script or type it manually
    //Currently no cout to ask for output - do not confuse halting of the program for crashes! 
    if (!fPositionDefined) { cin >> fTank_pos; 	fPositionDefined = true; }
    max = (1+parameter.delta)*fTank_pos;
    min = (1-parameter.delta)*fTank_pos;
    //max and min radius (shower plane) to read particles from. This is defined from delta, defined
    //in the XML, it is:
    /*
      Shower Plane:

                         _ _ _ _ _ _ _ _ _ _ _ _
                         \                     /  ^
			  \                   /   |
			   \        .        /    | 2*delta
			    \	    ^	    /     |
			     \_ _ _ | _ _ _/      ⌄
                             A      |      B
                                    |
                                    | 
                                    |r
                                    |
                                    |   phi is the angle that spans from A to B at the shower core  
                                    |
                                    ⌄   
                                    o  Shower core     
 
    */
   
    //Create the spanning azimuth phi from input in XML
    double minPhi = -parameter.phi/2.;
    double maxPhi = parameter.phi/2.;
    //Tank radius given in metres
    double r_tank = 1.8;
     while (file.findnextrun())
      {
	run theRun=file.getnextrun();
	while (theRun.findnextevent())
	  {
	    event theEvent=theRun.getnextevent();
	    //Necessary to recalculate azimuth of the primary particle after changing co-ordinate systems
	    //to mathematical convention;
	    double ppx = theEvent.primpx;
	    double ppy = theEvent.primpy;
	    //cout << ppy << " " << ppx << endl;
	    // double ppz = theEvent.primpz;
	    double primphi = atan2(ppy, ppx);

	    if (createdROOTPlots) {
	    //Shower direction
	    //Choose arbitrary points to plot a line representing the shower direction
	    //First point is the core position/origin at (0,0)
	    showerDirection->SetPoint(showerDirection->GetN(),0,0);
	    //Second point is at 5000,0 (east) ( histogram limits)
	    double xpos = 5000;
	    double ypos = 0;
	    double xnew;
	    double ynew;
	    xnew = xpos*cos(primphi) - ypos*sin(primphi);
	    ynew = xpos*sin(primphi) + ypos*cos(primphi);
	    showerDirection->SetPoint(showerDirection->GetN(), xnew, ynew);
	    }


	    
	    while (theEvent.findnextparticle())
	      {
		particle Part=theEvent.getnextparticle();
		//xprime and yprime are x and y co-ordinates for the particles in the shower plane
		//rotate axis CLOCKWISE by shower azimuth (around Z)
	       	double xprime = Part.x*cos(primphi) + Part.y*sin(primphi);
		double yprime = Part.x*sin(primphi) - Part.y*cos(primphi);
		//Further rotate ANTICLOCKWISE axis by 90 deg (to make 0 azimuth 3 o'clock in shower plane)
		double temp = xprime;
		xprime = xprime*cos(M_PI/2.) - yprime*sin(M_PI/2.);
		yprime = temp*sin(M_PI/2.) + yprime*cos(M_PI/2.);
		//rotate axis by  shower zenith (around X)
		xprime = xprime;
		yprime = yprime*cos(Part.primtheta);
		//double zprime = Part.y*sin(Part.primtheta)/100.0; Unused as of 23/5/2018
		r = (sqrt( pow(yprime,2 ) + pow(xprime,2)))/100.0; //radius in shower plane
		phi=atan2( yprime , xprime ); //azimuth in shower plane - NOTE: Must use atan2 to consider all 4 quadrants
	
		//time=Part.t-(Part.zstart-theRun.OBSLEVELS[Part.obslev-1])*(1.0E+07/cos(Part.primtheta))/c-Part.x*1.0E+07*sin(Part.primtheta)*cos(Part.primphi)/c-Part.y*1.0E+07*sin(Part.primtheta)*sin(Part.primphi)/c;//time in shower front

		//Momentum of each particle - Used to define energy and direction in GEANT4
		px = Part.px;
		py = Part.py;
		pz = Part.pz;
		//Zenith is in RADIANS
		zenith = acos (pz / ( sqrt( pow(px,2) + pow(py,2) + pow(pz,2)) )) ;


		//Azimuth ranges from -PI to PI. With 0 Azimuth being EAST. (x,0)
		if (!fSetAzimuth) {
		  minPhi = (parameter.phiAngle)*(M_PI/180.);
		  minPhi = minPhi - parameter.phi/2.;
		  maxPhi = minPhi + parameter.phi;
		  primtest << Part.primtheta << " " << primphi << "\n";
		  //Series of if statements to keep min and max phi in the same notation as atan2 range from -PI to PI
		  if (minPhi > M_PI )
		    {
		      minPhi = -2*M_PI + minPhi;
		    } else if (minPhi < -M_PI)
		    {
		      minPhi = 2*M_PI + minPhi;
		    }
		  if (maxPhi > M_PI )
		    {
		      maxPhi = -2*M_PI + maxPhi;
		    } else if (maxPhi < -M_PI)
		    {
		      maxPhi = 2*M_PI + maxPhi;
		    }
		  //Output just once - the azimuth of the primary and min/max phi of particles considered in shower plane
		  cout << "Primary particle azimuth: " << primphi  << " Previous azimuth: " << Part.primphi << " MaxPhi: " << maxPhi << " MinPhi: " << minPhi << endl;
		  fSetAzimuth = true;
		}

		//Get type & weight of particle
		type = Part.type;
		weight = Part.weight;

		//Calculation of projected area of the water tank and scintillator
		//Assuming that the scintillator has a negligible height, and so the projection is negligible.
		area_tank = 3 * cos ( zenith ); //Area of Scintillator is 3 sq m;
		//		cout << area_tank << endl;
		//Area of the section of the ring that we are considering in the shower plane
		double area_section = M_PI * ( pow(fTank_pos+parameter.delta*fTank_pos,2) - pow(fTank_pos-parameter.delta*fTank_pos,2) ) * ( (parameter.phi) / ( 2*M_PI ) );

		//Creating a Poisson Distribution---------------------------------------------------------------------------
		//Using a Poisson randomisation allows us to create a random series of particles that may be incident in the
		//tank. The result from the poisson distribution is the number of particles that will be injected into the
		//GEANT4 simulation. The primary benefit of this is massive performance improvements.
		double average_n = weight * ( area_tank / area_section );
       		boost::poisson_distribution<int> distribution(average_n);
		int n = distribution(gen);
		
		if ( type == 1 ) {
		  gamma_LDF->Fill(r, weight);
		} else if ( type == 2 || type == 3 )  {
		  electron_LDF->Fill(r, weight);
		} else if ( type == 5 || 6 ) {
		  muon_LDF->Fill(r, weight);
		}
		all_LDF->Fill(r, weight);
		
      
		//Simple boolean to determine whether or not a particle is within the area defined in the shower plane
		//testing for plots
			//Only create 2D histograms once
		if (parameter.makeSurfacePlots)
		  {
		    if (createdROOTPlots) {
		      if (r >= min && r <= max )
			{
			  h2->Fill(Part.x/100.,Part.y/100. , Part.weight);
			  if (phi >= minPhi && phi <= maxPhi)
			    {
			      h2_2->Fill(Part.x/100.,Part.y/100. , Part.weight);
			    }
			}
		      h2_1->Fill(Part.x/100.,Part.y/100.,Part.weight);
		      if (r >= min && r <= max)
			{
			  h1->Fill( xprime/100.0 , yprime/100.0 , Part.weight);
			  if (phi >= minPhi && phi <= maxPhi)
			    {
			      h1_1->Fill(xprime/100.0, -yprime/100., Part.weight);
			      //if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) )// howmanyPhi = howmanyPhi + Part.weight;
			    }
			  else if (maxPhi < minPhi)
			    {
			      if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                                
				{
				  //if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) //howmanyPhi = howmanyPhi + Part.weight;
				}
			    }
			}
		    }
		  }

		if (parameter.azimuthTest) {
		  if (r >= min && r <= max)
		    {
		      if (phi >= minPhi && phi <= maxPhi)
			{
			  if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) howmanyPhi = howmanyPhi + Part.weight;
			}
		      else if (maxPhi < minPhi)
			{
			  if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                                
			    {
			      if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) howmanyPhi = howmanyPhi + Part.weight;
			    }
			}
		    }
		}
		
		//Always initialise as false for each particle
	        bool particleApplicable = false;
		if (1==1)//!parameter.azimuthTest)
		  {
		    if (1==1) //Only continue following routines if there actually are particles that may be injected
		      {
			if  ((r >= min) && (r <= max)) //Check whether particle is within the area by radius
			  {
			    if (phi >= minPhi && phi <= maxPhi) //Then check whether the particle is within the area by angle
			      {
				howManyParticle = howManyParticle + weight;
				particleApplicable = true;
				// if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) // howmanyPhi++;
			      }
			    else if (maxPhi < minPhi) //At the +PI/-PI point there is a chance maxPhi < minPhi, the
			      //following if statement takes this into account.
			      {
				if ((phi >= -M_PI && phi <= maxPhi) || (phi <= M_PI && phi >= minPhi)  )                    
				  {
				    particleApplicable =true;
				    // if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) ) //howmanyPhi++;
				  }
			      }
			   }
			if (particleApplicable) //true if particle is within area
			  {
			    if ( type == 1 ) {
			      gamma_LDFValid->Fill(r, n);
			    } else if ( type == 2 || type == 3 )  {
			      electron_LDFValid->Fill(r, n);
			    } else if ( type == 5 || 6 ) {
			      muon_LDFValid->Fill(r, n);
			    }
			    all_LDFValid->Fill(r, n);
			  }
		      }
		  }
	      }
	  }
      }
    //Only write ROOT file once

     //---------------------Get XMAX of shower-------------------------------------
     //get .long file
     double xmax;
     string partname = string(argv[iFile]);
     size_t lastindex = partname.find_last_of(".");
     string rawname = partname.substr(0,lastindex);
     rawname.append(".long");
     ifstream fin;
     string nameOfLine;
     fin.open (rawname);
      while (getline(fin, nameOfLine)) 
	    {
	      string word;
	      int found2 = nameOfLine.find("PARAMETERS");
	      if  ( found2!=std::string::npos)
		{
		  istringstream iss(nameOfLine, istringstream::in);
		  int fcount = 0;
		  while( iss >> word )
		    {
		      fcount++;
		      if (fcount == 5)
			{
			  cout << word << endl;
			  xmax = stod(word);
			}
		    }
		}
	    }
      //Fill JoseTestFiles
      for (int j = 0; j < 100; j++)
	{
	  JoseTestFile_g << event_num << " " << xmax << " " << (j)*40 << " " << gamma_LDF->GetBinContent(j) << "\n";
	  JoseTestFile_m << event_num << " " << xmax << " " << (j)*40 << " " << muon_LDF->GetBinContent(j) << "\n";
	  JoseTestFile_e << event_num << " " << xmax << " " << (j)*40 << " " <<  electron_LDF->GetBinContent(j) << "\n";
	  JoseTestFile_all << event_num << " " << xmax << " " << (j)*40 << " " << all_LDF->GetBinContent(j) << "\n";
	}





      //-----------------------------------------------------------------------
      //Fit NKG to histograms?? --

      Double_t R_0 = 100.;
      Double_t alpha = 0.5;
      Double_t eta = 3.5;
      TF1 *fitdistrlat = new TF1("fitdistrlat", Linsley_rho, 100, 4e3, 4);
      fitdistrlat->SetParNames("C(#alpha,#eta) N", "#alpha", "#eta", "R_{0}^{}");
      fitdistrlat->SetParameters(1., alpha, eta, R_0);
      fitdistrlat->FixParameter(3, R_0); // fix "R_0"
      const char *fit_options = "R";
      
      //----------------------------------------------------------------------
      //Create necessary canvas for each LDF type
      //Write them to a directory that depends on XMAX. 
      //.root file is called componentLDFs, and has a hierarchy organised by xmax
      TDirectory *xmaxDirectory = componentLDFs->mkdir(to_string(xmax).c_str());
      stringstream comparisonName, canvasName, canvasName1, canvasName2;
 
      xmaxDirectory->cd();
      stringstream gammaCanvasName, electronCanvasName, muonCanvasName, gammaCanvasName2, electronCanvasName2, muonCanvasName2, allCanvasName, allCanvasName2;
      gammaCanvasName << "Gamma LDF " << xmax; electronCanvasName << "Electron LDF " << xmax; muonCanvasName << "Muon LDF " << xmax;
      gammaCanvasName2 << "Gamma LDFValid " << xmax; electronCanvasName2 << "Electron LDFValid " << xmax; muonCanvasName2 << "Muon LDFValid " << xmax;
      allCanvasName << "All LDF " << xmax; allCanvasName2 << "All LDFValid " << xmax;
      TCanvas *gamma_LDF_canvas = new TCanvas(gammaCanvasName.str().c_str(), "Gamma LDF", 800, 600);
      TCanvas *electron_LDF_canvas = new TCanvas(electronCanvasName.str().c_str(), "Electron LDF", 800, 600);
      TCanvas *muon_LDF_canvas = new TCanvas(muonCanvasName.str().c_str(), "Muon LDF", 800, 600);
      TCanvas *gamma_LDFValid_canvas = new TCanvas(gammaCanvasName2.str().c_str(), "Gamma LDF (Injected Particles on 1 Detector)", 800, 600);
      TCanvas *electron_LDFValid_canvas = new TCanvas(electronCanvasName2.str().c_str(), "Electron LDF (Injected Particles on 1 Detector)", 800, 600);
      TCanvas *muon_LDFValid_canvas = new TCanvas(muonCanvasName2.str().c_str(), "Muon LDF (Injected Particles on 1 Detector)", 800, 600);
      TCanvas *all_LDF_canvas = new TCanvas(allCanvasName.str().c_str(), "All LDF", 800, 600);
      TCanvas *all_LDFValid_canvas = new TCanvas(allCanvasName2.str().c_str(), "All LDF (Injected Particles on 1 Detector)", 800, 600);
      TPaveText *xmaxOnGraph = new TPaveText(0.8,1,0.8,1);
      stringstream xmaxTitle_legend;
      xmaxTitle_legend << "xmax: " << xmax;
      xmaxOnGraph->AddText(xmaxTitle_legend.str().c_str());
      bool plot = false;
      if (xmax < minXmax) {
	  minXmax = xmax;
	  min_LDF_g = (TH1F*)gamma_LDF->Clone("gamma min LDF");
	  min_LDF_e = (TH1F*)electron_LDF->Clone("electron min LDF");
	  min_LDF_m = (TH1F*)muon_LDF->Clone("muon min LDF");
	  plot = true;
	}
      if (xmax > maxXmax) {
	maxXmax = xmax;
	max_LDF_g = (TH1F*)gamma_LDF->Clone("gamma max LDF");
	max_LDF_e = (TH1F*)electron_LDF->Clone("electron max LDF");
	max_LDF_m = (TH1F*)muon_LDF->Clone("muon max LDF");

	//	max_LDF_g->SetLineColor(kRed);
	//	max_LDF_e->SetLineColor(kRed);
	//max_LDF_m->SetLineColor(kRed);
	plot = true;
      }
      //May just try plotting a LDF for every 20 XMAX
      double range[] = {660., 680., 700., 720., 740., 760., 780., 800., 820., 840., 860., 880., 900., 920., 940., 960., 980., 1000.};
     

      // for (int i = 0; i < 18-1; i++) {
	// if (xmax >= range[i] && xmax <= range[i+1])
	//   {
	//if (isRangeDrawn[i] == 0) {
      int i = 1;
	      if (firstSave)
		{
		  multipleLDFs_g = (TCanvas*)componentLDFs->Get("MultipleLDFs_g");
		}
	      cout << " ?? " << endl;
	      isRangeDrawn[i] = true;
	      multipleLDFs_g->cd();
	      multipleLDFs_g->SetLogy();
	      gamma_LDF->SetLineColor(i);
	      electron_LDF->SetLineColor(i);
	      muon_LDF->SetLineColor(i);
	      all_LDF->SetLineColor(kMagenta);
	      if (firstDraw){ gamma_LDF->Draw(""); cout << "first draw" << endl; }
	      else {gamma_LDF->Draw("SAME"); cout << "drawn same" << endl;}

	        if (firstSave)
		{
		  multipleLDFs_e = (TCanvas*)componentLDFs->Get("MultipleLDFs_e");
		}
	      multipleLDFs_e->cd();
	      multipleLDFs_e->SetLogy();
	      if (firstDraw){ electron_LDF->Draw("SAME");}
	      else {electron_LDF->Draw("SAME");}
	      
	      if (firstSave)
		{
		  multipleLDFs_m = (TCanvas*)componentLDFs->Get("MultipleLDFs_m");
		}
	      multipleLDFs_m->cd();
	      multipleLDFs_m->SetLogy();
	      if (firstDraw) {muon_LDF->Draw("SAME");}
	      else {muon_LDF->Draw("SAME");}

	      if (firstSave)
		{
		  multipleLDFs_a = (TCanvas*)componentLDFs->Get("MultipleLDFs_a");
		}
	      multipleLDFs_a->cd();
	      multipleLDFs_a->SetLogy();
	      if (firstDraw) {all_LDF->Draw("SAME");}
	      else {all_LDF->Draw("SAME");}
	      componentLDFs->cd();
	      firstDraw = false;
	      // multipleLDFs_g->Write();
	      //multipleLDFs_e->Write();
	      //multipleLDFs_m->Write();
	      //multipleLDFs_a->Write();
	      firstSave = true;
	      
	      //  }
	    // }
      
	      // }
      cout << " <?? " << endl;
      
      comparisonName << minXmax << "_" << maxXmax;
      TDirectory *comparisonMinMax = componentLDFs->mkdir(comparisonName.str().c_str());
      canvasName << comparisonName.str() << " gamma";
      canvasName1 << comparisonName.str() << " electron";
      canvasName2 << comparisonName.str() << " muon";
      
      //min and max xmax
      TCanvas *minmax_LDF_canvas_g = new TCanvas(canvasName.str().c_str(),comparisonName.str().c_str(),800,600);
      TCanvas *minmax_LDF_canvas_e = new TCanvas(canvasName1.str().c_str(),comparisonName.str().c_str(),800,600);
      TCanvas *minmax_LDF_canvas_m = new TCanvas(canvasName2.str().c_str(),comparisonName.str().c_str(),800,600);
      //Draw comparison histograms
      if (plot)
	{
	  comparisonMinMax->cd();
	  minmax_LDF_canvas_g->cd();
	  minmax_LDF_canvas_g->SetLogy();
	  min_LDF_g->Draw();
	  max_LDF_g->Draw("SAME");
	  minmax_LDF_canvas_g->Write();
	  
	  minmax_LDF_canvas_e->cd();
	  minmax_LDF_canvas_e->SetLogy();
	  min_LDF_e->Draw();
	  max_LDF_e->Draw("SAME");
	  minmax_LDF_canvas_e->Write();
	  
	  minmax_LDF_canvas_m->cd();
	  minmax_LDF_canvas_m->SetLogy();
	  min_LDF_m->Draw();
	  max_LDF_m->Draw("SAME");
	  minmax_LDF_canvas_m->Write();

	} 
      xmaxDirectory->cd();
      
      gamma_LDF_canvas->cd();
      gamma_LDF->Draw();
      gamma_LDF->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, gamma_LDF->GetBinContent(1)/fitdistrlat->Eval(gamma_LDF->GetBinCenter(1)));
      // gamma_LDF->Fit(fitdistrlat, fit_options);
      gamma_LDF_canvas->SetLogy();
      gamma_LDF_canvas->Write();
      
      gamma_LDFValid_canvas->cd();
      gamma_LDFValid->Draw();
      gamma_LDFValid->SetStats(false); 
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, gamma_LDFValid->GetBinContent(1)/fitdistrlat->Eval(gamma_LDFValid->GetBinCenter(1)));
      // gamma_LDFValid->Fit(fitdistrlat, fit_options);
      gamma_LDFValid_canvas->SetLogy();
      gamma_LDFValid_canvas->Write();

      electron_LDF_canvas->cd();
      electron_LDF->Draw();
      electron_LDF->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, electron_LDF->GetBinContent(1)/fitdistrlat->Eval(electron_LDF->GetBinCenter(1)));
      // electron_LDF->Fit(fitdistrlat, fit_options);
      electron_LDF_canvas->SetLogy();
      electron_LDF_canvas->Write();
      
      electron_LDFValid_canvas->cd();
      electron_LDFValid->Draw();
      electron_LDFValid->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, electron_LDF->GetBinContent(1)/fitdistrlat->Eval(electron_LDF->GetBinCenter(1)));
      // electron_LDF->Fit(fitdistrlat, fit_options);
      electron_LDFValid_canvas->SetLogy();
      electron_LDFValid_canvas->Write();
      
      muon_LDF_canvas->cd();
      muon_LDF->Draw();
      muon_LDF->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, muon_LDF->GetBinContent(1)/fitdistrlat->Eval(muon_LDF->GetBinCenter(1)));
      // muon_LDF->Fit(fitdistrlat, fit_options);
      muon_LDF_canvas->SetLogy();
      muon_LDF_canvas->Write();
      
      muon_LDFValid_canvas->cd();
      muon_LDFValid->Draw();
      muon_LDFValid->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, muon_LDF->GetBinContent(1)/fitdistrlat->Eval(muon_LDF->GetBinCenter(1)));
      // muon_LDF->Fit(fitdistrlat, fit_options);
      muon_LDFValid_canvas->SetLogy();
      muon_LDFValid_canvas->Write();

      all_LDF_canvas->cd();
      all_LDF->Draw();
      all_LDF->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, all_LDF->GetBinContent(1)/fitdistrlat->Eval(all_LDF->GetBinCenter(1)));
      // all_LDF->Fit(fitdistrlat, fit_options);
      all_LDF_canvas->SetLogy();
      all_LDF_canvas->Write();
      
      
      all_LDFValid_canvas->cd();
      all_LDFValid->Draw();
      all_LDFValid->SetStats(false);
      xmaxOnGraph->Draw();
      // fitdistrlat->SetParameters(1., alpha, eta, R_0);
      // fitdistrlat->SetParameter(0, all_LDF->GetBinContent(1)/fitdistrlat->Eval(all_LDF->GetBinCenter(1)));
      // all_LDF->Fit(fitdistrlat, fit_options);
      all_LDFValid_canvas->SetLogy();
      all_LDFValid_canvas->Write();

      componentLDFs->cd();
      multipleLDFs_g->Write();
      multipleLDFs_e->Write();
      multipleLDFs_m->Write();
      multipleLDFs_a->Write();
      componentLDFs->Write();
      gamma_LDF->Reset();
      muon_LDF->Reset();
      electron_LDF->Reset();
      all_LDF->Reset();
      //------------------------------------------------------------------------------
  }

  componentLDFs->Write();
  
  return count;  
}



//Simple function to read and receive files from a directory - Allows this program to use directories and input
vector<string> getFilesFromDirectory(string directory)
{
  ifstream fin;
  string dir, filepath, file;
  DIR *dp;
  
  struct dirent *dirp;
  struct stat filestat;
  vector<string> fileNames;
  
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

      cout << file << endl;
      fileNames.push_back(file);
    }
  return fileNames;
}



int main (int argc, char **argv){

  //See if use specified what files to analyse

  //Read Values from XML
  std::ifstream input("CORSIKAParameters.xml");
  parameters parameter = read( input );
  bool useWeights = 0;
  int nParticles = 0;
  if (argc < 2) {
    //usage();
    std::string args = parameter.file;
   
    if ( parameter.file.find(".part") != std::string::npos) {
      std::strcpy(argv[0],args.c_str());
    } else if ( parameter.file.find(".long") !=std::string::npos) {
      cout << "Provide a .part CORSIKA file, (.long does not contain particle information)" << endl;
    }    
  }

  //  nParticles = createGEANT4Files(argc, argv, parameter, useWeights, nParticles);
  // if ( nParticles > parameter.threshold ) {
  //   weightedSimulation(nParticles, parameter.weightedSimulation,parameter.threshold);
  //   useWeights = 1;
  //   fPositionDefined = true;
  //   //cout << fTank_pos << endl;
  //   nParticles = createGEANT4Files(argc, argv, parameter, useWeights, nParticles);
  // }
  bool produceLDF = true;
  if (produceLDF)
    {
      createLDF(argc, argv, parameter);
    }
}


void usage()
{
  printf("OPTIONAL Usage:\n"
	 "./usereadpart $filename(s)\n"
	 "where $filename is the CORSIKA file(s) you want to analyse.\n"
	 "For example, to run over all data at 10e20 eV:\n"
	 "./usereadpart /remote/tesla/bmanning/data/qgsII4/proton/10E20/*\n"
	 );
}

