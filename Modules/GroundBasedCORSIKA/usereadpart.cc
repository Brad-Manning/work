#include "readPart_1_7b.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
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
#include <algorithm>
//#include <random>
#include <boost/random/poisson_distribution.hpp>
using boost::poisson_distribution;
#include <boost/random/variate_generator.hpp>
using boost::variate_generator;
#include <boost/random/mersenne_twister.hpp>
using boost::mt19937;

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

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
};

double fTank_pos;
bool fPositionDefined;

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
  mt19937 gen;
  gen.seed(time(NULL));
  std::vector<int> validParticles = findTypes(parameter.particle);
  double weightMultiplier;
  bool fSetAzimuth = false;
  if (useWeights)
    {
      weightMultiplier = parameter.threshold / nParticles;
    }
  

  
  double count,r,phi,px, py, pz, type, weight, zenith, area_tank, arear;
  char* strName[] = {"gamma", "e+", "e-", "?" , "mu+" , "mu-" , "pi0" , "pi+" , "pi-" , "kaon0" , "kaon+" , "kaon-" , "neutron" , "proton" , "anti_proton"};
  vector<string> vecName(strName, strName+15);
  count = 0;
  
  ofstream G4file;
  ofstream RunInfo;
  ofstream weightAll;
  ofstream zenithAngles;
  zenithAngles.open ("ZenithAngles.txt");
  //Loop over files, and read the files
  int start = (argc < 2) ? 0 : 1;
  for (int iFile = start; iFile <= argc - 1; iFile++) {
    corsikafile file(argv[iFile]);
    if (!useWeights) cout << "Current File: " << argv[iFile] << "\n";
    string fileName = argv[iFile];
    size_t pos_start = fileName.find("qgs");
    size_t pos_end = fileName.find("/DAT");
    string G4name = fileName.substr(pos_start,pos_end-pos_start);

    G4name.erase(remove_if(G4name.begin(), G4name.end(), &IsForwardSlash), G4name.end());
   
    string infoName = "runInfo.txt";
    if (useWeights)
      {
	string weightName = G4name+"weight.txt";
	const char * wei = weightName.c_str();
	weightAll.open(wei);
	weightAll << 0 << "\n";
      }
    
    G4name = G4name+".mac";
    const char * G4 = G4name.c_str();
    const char * runInfo = infoName.c_str();
 
    G4file.open (G4);
    G4file << "/run/initialize" << "\n";
    G4file << "/gun/energy 0" << "\n";
    G4file << "/run/beamOn 1" << "\n";
    RunInfo.open (runInfo);
    
    int min;
    double max;
    if (!fPositionDefined) { cin >> fTank_pos; }

    
    double delta = parameter.delta;
    double minPhi = -parameter.phi/2.;
    double maxPhi = parameter.phi/2.;

    max = (1+delta)*fTank_pos;
    min = (1-delta)*fTank_pos;
    RunInfo << fTank_pos << "\n";
    RunInfo << useWeights << "\n";
    //Tank radius given in metres
    double r_tank = 1.8;
     while (file.findnextrun())
      {
        
	run theRun=file.getnextrun();

	while (theRun.findnextevent())
	  {
	    event theEvent=theRun.getnextevent();
	
	    while (theEvent.findnextparticle())
	      {
		particle Part=theEvent.getnextparticle();

		//corer=(sqrt(pow((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)),2)+pow(((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)),2)))/100.0;//core dist in shower plane (m)

		r = ( sqrt( pow( Part.x, 2 ) + pow( Part.y, 2) ) )/100.0 ;
	
		//	phi=atan2((Part.x*sin(Part.primphi)-Part.y*cos(Part.primphi)) ,((Part.x*cos(Part.primphi)+Part.y*sin(Part.primphi))*cos(Part.primtheta)));//azimuth in shower plane
		//time=Part.t-(Part.zstart-theRun.OBSLEVELS[Part.obslev-1])*(1.0E+07/cos(Part.primtheta))/c-Part.x*1.0E+07*sin(Part.primtheta)*cos(Part.primphi)/c-Part.y*1.0E+07*sin(Part.primtheta)*sin(Part.primphi)/c;//time in shower front
		px = Part.px;
		py = Part.py;
		pz = Part.pz;

		//Creating object at ground ---------
		if (Part.primtheta != 0) { arear = fTank_pos / sin(Part.primtheta) ; }
		else { arear = fTank_pos; } 
		phi = atan2(Part.x, Part.y); 
		max = (1+delta)*arear;
		min = (1-delta)*arear;
	
		if (!fSetAzimuth) {
		  minPhi = minPhi + Part.primphi + (parameter.phiAngle*(M_PI/180.));
		  maxPhi = maxPhi + Part.primphi + (parameter.phiAngle*(M_PI/180.));
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
		  
		  fSetAzimuth = true;
		}
	
		//Zenith is in RADIANS
		//zenith =  (atan2 ( px , pz  ) );

		zenith = acos (pz / ( sqrt( pow(px,2) + pow(py,2) + pow(pz,2)) ) ) ;
		//cout << zenith*180/M_PI << endl;
		pz = -pz;
		type = Part.type;
		weight = Part.weight;
	
		//1.2 = height of tank (m)
		if (!(parameter.detector)) { area_tank = M_PI * pow(r_tank,2) * cos( zenith ) + 2*r_tank*1.2*sin( zenith );}
		else { area_tank = 4 * cos ( zenith ); } //Area of Scintillator is 4 sq m;
		
		double area_section = M_PI * ( pow(arear+delta*arear,2) - pow(arear-delta*arear,2) ) * ( (maxPhi-minPhi) / ( 2*M_PI ) )* cos(zenith);
		//	cout << "Before: " << area_section/(cos(zenith)) << " After : " << area_section << " zenith : " << zenith << endl;
		double average_n = weight * ( area_tank / area_section );
		//cout << average_n << " " << area_tank << " " << area_section << " " << zenith <<  endl;
		//if ( (phi >= minPhi && phi <= maxPhi)  && (r >= min) && (r <= max)) {
		//	zenithAngles << type << " " << zenith*(180. /M_PI) <<  " " << weight << "\n";
		//	}
       		boost::poisson_distribution<int> distribution(average_n);
		int n = distribution(gen);
		// cout << n << endl;
		if (n != 0)
		  {
		    if ( (phi >= minPhi && phi <= maxPhi)  && (r >= min) && (r <= max)) 
		      {
	
			if ( type > (validParticles[0]-1) && type < (validParticles[1]+1) )
			  {
			    G4file << "/gun/momentum " << px << " " << py << " " << pz << "\n";
			    G4file << "/gun/particle " << vecName[type-1] << "\n";
			    if (useWeights && n >= 10) //is 10 suitable? Think about it after confirming that it works
			      {
				G4file << "/run/beamOn " << round(n*weightMultiplier) << "\n";
				weightAll << ( n / round(n*weightMultiplier) ) << "\n";
				//	cout << n << " " << round(n*weightMultiplier) << " " << n / round(n*weightMultiplier) << endl; 
				count = count + round(n*weightMultiplier);
			      } else {
			      G4file << "/run/beamOn " << (int) n <<"\n";
			      weightAll << 1 << "\n";
			      count = count + n;
			    }
		
			    
			  }
			if (type >= 16 && ( parameter.particle == "all" ))
			  {
			    G4file << "/gun/momentum " << pz << " " << py << " " << pz << "\n";
			    G4file << "/gun/particle ion " << "\n";
			    G4file << "/gun/ion " << type << " " << type*2 << "\n";

			    if (useWeights && n >= 10)
			      {
				G4file << "/run/beamOn " << round(n*weightMultiplier) << "\n";
				weightAll << ( n / round(n*weightMultiplier) ) << "\n";
				count = count + round(n*weightMultiplier);
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
    RunInfo << count << "\n";						     
    G4file.close();
    RunInfo.close();
  }
    
  return count;  
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
    std::strcpy(argv[0],args.c_str());
    // cout << argv[0] << endl;
    
  }

  nParticles = createGEANT4Files(argc, argv, parameter, useWeights, nParticles);
  if ( nParticles > parameter.threshold ) {
    weightedSimulation(nParticles, parameter.weightedSimulation,parameter.threshold);
    useWeights = 1;
    fPositionDefined = true;
    //cout << fTank_pos << endl;
    nParticles = createGEANT4Files(argc, argv, parameter, useWeights, nParticles);
   
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

