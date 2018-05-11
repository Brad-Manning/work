// Bradley Manning 10/5/2018

// Includes

// C++

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>


using namespace std;


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
	
	p.composition.push_back
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
      // If th fileis a directory (or is in some way invalid) we'll skip it
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
  vector<signal> signal;
  signal.push_back(getDataFromDirectory("/remote/tesla/bmanning/data/EnergyTable/Auger/qgsII3/proton/10E19/0deg/", parameter));
  return 1;  
}
  
