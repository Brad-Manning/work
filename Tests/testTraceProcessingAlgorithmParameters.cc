#include "SdCalibrator.h"

#include <fwk/CentralConfig.h>
#include <fwk/RunController.h>
#include <fwk/SVNGlobalRevision.h>

#include <det/Detector.h>
#include <sdet/SDetector.h>
#include <sdet/Station.h>
#include <sdet/StationTriggerAlgorithm.h>
#include <sdet/TraceProcessingAlgorithmParameters.h>
#include <sdet/PMTConstants.h>

#include <evt/Event.h>

#include <sevt/EventTrigger.h>
#include <sevt/Header.h>
#include <sevt/SEvent.h>
#include <sevt/Station.h>
#include <sevt/Scintillator.h>
#include <sevt/PMT.h>
#include <sevt/PMTCalibData.h>
#include <sevt/PMTRecData.h>
#include <sevt/StationGPSData.h>
#include <sevt/StationRecData.h>
#include <sevt/ScintillatorRecData.h>
#include <sevt/StationTriggerData.h>
#include <sevt/StationCalibData.h>

#include <utl/TimeStamp.h>
#include <utl/TimeInterval.h>
#include <utl/Trace.h>
#include <utl/TraceAlgorithm.h>
#include <utl/Reader.h>
#include <utl/ErrorLogger.h>
#include <utl/Math.h>
#include <utl/Accumulator.h>
#include <utl/QuadraticFitter.h>
#include <utl/ExponentialFitter.h>
#include <utl/String.h>
#include <utl/ShadowPtr.h>
#include <utl/TabularStream.h>

#include <iostream>
#include <sstream>
#include <algorithm>

#define DO_PEAK_HISTOS 1

#include <config.h>

using namespace fwk;
using namespace evt;
using namespace sevt;
using namespace utl;
using namespace std;


int main(int argc, char **argv) {

  
  if (argc < 2) {
    usage();
    return 1;

  }
  DetectorGeometry * theGeometry = new DetectorGeometry();


  //Loop over files
  //Read the file as the second to last argument
  for (int iFile = 1; iFile <= argc - 1; iFile++) {
    RecEventFile dataFile(argv[iFile]);
    RecEvent* theRecEvent = new RecEvent();
    dataFile.SetBuffers(&(theRecEvent));
    dataFile.ReadDetectorGeometry(*theGeometry);
    unsigned int ntotThisFile = dataFile.GetNEvents();
    cout << "Reading file " << dataFile.GetActiveFileName() << " with " << ntotThisFile << " events." << endl;

    //Loop over the events in the file
    for (unsigned int iEvent = 0; iEvent < ntotThisFile; iEvent++) {

      if (!dataFile.ReadEvent(iEvent) == RecEventFile::eSuccess) continue;

      const SD
      
    }
  }
  
}


void usage()
{
   printf("Usage:\n"
	 "./ReadADST $filename(s)\n"
	 "where $filename is the ADST file(s) you want to analyse.\n"
	 "For example, to run over the data from 2011:\n"
	 "./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011*\n"
	 "Or just for the month of May, 2011:\n"
	 "./ReadADST /remote/kennya/auger/data/ADST/Offline_v2r7p8-Shannon/HybridRec/HybridRec_v7r6_2011_05_generated_2012-10-28.root\n");
}
