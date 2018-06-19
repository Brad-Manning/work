#!/usr/bin/env bash

HadronicModels=(qgsII4) #(qgsII3 qgsII4)
nH=1 #2
Primary=(proton) #(proton iron photon)
nP=1 #3
Energies=(19) #(20 19.5 19 18.5)
nE=1 #4
Angles=(38deg) #(0deg 26deg 38deg 49deg 60deg)
nA=1 #5

for (( hAll=0; hAll<nH; hAll=hAll+1 ))
do

    hadronicModels=${HadronicModels[$hAll]}
    for (( pAll=0; pAll<nP; pAll=pAll+1 ))
    do
	primary=${Primary[$pAll]}
	for (( eAll=0; eAll<nE; eAll=eAll+1 ))
	do
	    energy=${Energies[$eAll]}
	    for (( aAll=0; aAll<nA; aAll=aAll+1 ))
	    do
		angle=${Angles[$aAll]}
		for fileInc in {001..999};
		do
		    for mainfileInc in {0..4};
		    do
			newFileName=/remote/tesla/bmanning/data/${hadronicModels}/${primary}/${energy}/${angle}/DAT${mainfileInc}00${fileInc}.part

			newSaveLocation=/remote/tesla/bmanning/data/SimulationData/${hadronicModels}/${primary}/${energy}/${angle}/

			if [ -e "$newFileName" ]
			then

			    cd /remote/tesla/bmanning/work/Scripts/Cherenkov-Inject/WCD-Inject-All
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
	    
				/remote/tesla/bmanning/work/Geant4Simulations/WCD-build-inject/./Cherenkov *${j}.mac
		        	/remote/tesla/bmanning/work/Modules/Merge/./merge
				rm -f *.mac

			    done #position
			    pName=$(sed -n 's/.*<particle>\(.*\)<\/particle>/\1/p' CORSIKAParameters.xml)
			    mv VEM_r_bins.txt ${newSaveLocation}${pName}-${fileInc}VEM.txt

			    rm -f *.txt
			    rm -f *.root
			fi
		    done #mainfileInc
		done #fileInc
	    done #angle
	done #energy
    done #primary
done #model
