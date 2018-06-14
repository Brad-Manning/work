#!/usr/bin/env bash

HadronicModels=(qgsII4 qgsII3) #(qgsII3 qgsII4)
nH=2 #2
Primary=(proton iron) #(proton iron photon)
nP=2 #3
Energies=(20 19.5 19 18.5) #(20 19.5 19 18.5)
nE=4 #4
Angles=(0deg 26deg 38deg 49deg 60deg) #(0deg 26deg 38deg 49deg 60deg)
nA=5 #5

for (( hElectron=0; hElectron<nH; hElectron=hElectron+1 ))
do

    hadronicModels=${HadronicModels[$hElectron]}
    for (( pElectron=0; pElectron<nP; pElectron=pElectron+1 ))
    do
	primary=${Primary[$pElectron]}
	for (( eElectron=0; eElectron<nE; eElectron=eElectron+1 ))
	do
	    energy=${Energies[$eElectron]}
	    for (( aElectron=0; aElectron<nA; aElectron=aElectron+1 ))
	    do
		angle=${Angles[$aElectron]}
		for fileInc in {001..999};
		do
		    for mainfileInc in {0..4};
		    do
			newFileName=/remote/tesla/bmanning/data/${hadronicModels}/${primary}/${energy}/${angle}/DAT${mainfileInc}00${fileInc}.part

			newSaveLocation=/remote/tesla/bmanning/data/SimulationData/${hadronicModels}/${primary}/${energy}/${angle}/

			if [ -e "$newFileName" ]
			then

			    cd /remote/tesla/bmanning/work/Scripts/SSD-Inject/SSD-Inject-Electron
			    startingPosition=$(sed -n 's/.*<startingPosition>\(.*\)<\/startingPosition>/\1/p' CORSIKAParameters.xml)
			    maxPosition=$(sed -n 's/.*<maxPosition>\(.*\)<\/maxPosition>/\1/p' CORSIKAParameters.xml)

			    let step=1
			    for (( i=startingPosition; i<=maxPosition; i=i+step ))
			    do
				
				factor=$(bc <<< 1.1 )
				rs21=$(bc <<< "scale=2; $factor*$i")
				rs20=$(bc <<< "scale=2; $i*0.9")
				rs2=$(bc <<< "scale=2; $rs21/0.9")
				step=$(bc -l <<< "scale=2; $rs2-$i")
				step=$(printf "%.0f" $step)
				
				/remote/tesla/bmanning/work/Modules/Universal-test/./UniParticleReader ${newFileName} <<EOF
 ${i}
EOF

				wait

				mv *weight.txt weightAll.txt
	    
				/remote/tesla/bmanning/work/Geant4Simulations/SSD-build/./SSDSimulation *${j}.mac
		        	/remote/tesla/bmanning/work/Modules/Merge/./merge
				rm -f *.mac

			    done #position
			    pName=$(sed -n 's/.*<particle>\(.*\)<\/particle>/\1/p' CORSIKAParameters.xml)
			    mv MIP_r_bins.txt ${newSaveLocation}${pName}-${fileInc}MIP.txt

			    rm -f *.txt
			    rm -f *.root
			fi
		    done #mainfileInc
		done #fileInc
	    done #angle
	done #energy
    done #primary
done #model
