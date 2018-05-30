#!/usr/bin/env bash

Compositions=(qgsII4 qgsII3) #(qgsII3 qgsII4)
nC=2 #2
Particles=(proton iron) # (proton iron)
nP=2 #2
Energies=(20 19.5 19 18.5)         #  (20 19.5 19 18.5)
nE=4 #4
Angles=(0deg 26deg 38deg 49deg 60deg)   #(0deg 26deg 38deg 49deg 60deg)
nA=5  #5

for (( c=0; c<nC; c=c+1 ))
do
    composition=${Compositions[$c]}
    for (( p=0; p<nP; p=p+1 ))
    do
	particle=${Particles[$p]}
	for (( en=0; en<nE; en=en+1 ))
	do
	    energy=${Energies[$en]}
	    for (( a=0; a<nA; a=a+1 ))
	    do
		angle=${Angles[a]}
	        newFileName=/remote/tesla/bmanning/data/${composition}/${particle}/${energy}/${angle}/*
		#	newSaveLocation=/remote/tesla/bmanning/data/EnergyTable/Auger/${composition}/${particle}/${energy}/${angle}/
		newSaveLocation=/remote/tesla/bmanning/data/ShowerComponents/qgsII4/proton/19/60deg/
		
sed -i "
    /<CORSIKA>/,/<\/CORSIKA>/ s@<file>.*</file>@<file>$newFileName</file>@;
    /<CORSIKA>/,/<\/CORSIKA>/ s@<saveLocation>.*</saveLocation>@<saveLocation>$newSaveLocation</saveLocation>@
" "CORSIKAParameters.xml"

startingPosition=$(sed -n 's/.*<startingPosition>\(.*\)<\/startingPosition>/\1/p' CORSIKAParameters.xml)
maxPosition=$(sed -n 's/.*<maxPosition>\(.*\)<\/maxPosition>/\1/p' CORSIKAParameters.xml)
iterations=$(sed -n 's/.*<iterations>\(.*\)<\/iterations>/\1/p' CORSIKAParameters.xml)
fileName=$(sed -n 's/.*<file>\(.*\)<\/file>/\1/p' CORSIKAParameters.xml)
nUniqueShowers=$(sed -n 's/.*<nUniqueShowers>\(.*\)<\/nUniqueShowers>/\1/p' CORSIKAParameters.xml)
#particle2=$(sed -n 's/.*<particle>\(.*\)<\/particle>/\1/p' CORSIKAParameters.xml)

for (( e=1; e<=iterations; e=e+1 ))
do
    let step=1
    #for i in {366..4000..${step}}
    for (( i=startingPosition; i<=maxPosition; i=i+step ))
    do


	#Dynamic Bin Size - Implemented 12 April 2018.
	factor=$(bc <<< 1.1 )
	rs21=$(bc <<< "scale=2; $factor*$i")
	rs20=$(bc <<< "scale=2; $i*0.9")
	rs2=$(bc <<< "scale=2; $rs21/0.9")
	step=$(bc -l <<< "scale=2; $rs2-$i")
	step=$(printf "%.0f" $step)
#	echo ${rs2}
#	echo ${rs20}
#	echo ${rs21}

	/remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/./UniParticleReader ${fileName} <<EOF #/remote/tesla/bmanning/data/qgsII3/proton/10E19/0deg/DAT200006.part 
 ${i}
EOF

	wait

	mv *weight.txt weightAll.txt

	for (( j=1; j<=nUniqueShowers; j=j+1 ))
	do	
	    /remote/tesla/bmanning/work/Geant4Simulations/WCD-build-inject/./Cherenkov *${j}.mac
	  
	    /remote/tesla/bmanning/work/Modules/Merge/./merge
	done
	rm -f qgsII*
    done

saveLocation=$(sed -n 's/.*<saveLocation>\(.*\)<\/saveLocation>/\1/p' CORSIKAParameters.xml)

echo ${particle}
echo ${e}
echo ${startingPosition}
echo ${saveLocation}${startingPosition}${particle}-${e}.txt

mv VEM_r_bins.txt ${saveLocation}${startingPosition}-${e}.txt
    
rm -f qgsII*
rm -f rValues.txt
rm -f finalVEM.txt
rm -f weightAll.txt
rm -f SSD.root
rm -f VEM.txt
rm -f eValues.txt
#rm -f VEM_r_bins.txt
rm -f Cherenkov.root
rm -f LDF.root
rm -f MIP_r_bins.txt
done
	    done
	done
    done
done    
#/remote/tesla/bmanning/work/Modules/plot/./plot 
