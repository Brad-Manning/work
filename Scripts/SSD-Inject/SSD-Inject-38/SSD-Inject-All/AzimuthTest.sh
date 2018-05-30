#!/usr/bin/env bash

for (( fileInc=118; fileInc<=998; fileInc=fileInc+20))
do
    newFileName=/remote/tesla/bmanning/data/qgsII3/proton/19/38deg/DAT200${fileInc}.part
    echo $newFileName
    sed -i "
    	/<CORSIKA>/,/<\/CORSIKA>/ s@<file>.*</file>@<file>$newFileName</file>@
	" "CORSIKAParameters.xml"

    
    for (( angle=0; angle<=0; angle=angle+18 ))
    do
	newAngle=$angle
	sed -i "
    	    /<CORSIKA>/,/<\/CORSIKA>/ s@<phiAngle>.*</phiAngle>@<phiAngle>$newAngle</phiAngle>@
	    " "CORSIKAParameters.xml"

	startingPosition=$(sed -n 's/.*<startingPosition>\(.*\)<\/startingPosition>/\1/p' CORSIKAParameters.xml)
	fileName=$(sed -n 's/.*<file>\(.*\)<\/file>/\1/p' CORSIKAParameters.xml)
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
	    
	    /remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/./UniParticleReader ${fileName} <<EOF #/remote/tesla/bmanning/data/qgsII3/proton/10E19/0deg/DAT200006.part 
 ${i}
EOF

	    wait

	    mv *weight.txt weightAll.txt
	    
	    /remote/tesla/bmanning/work/Geant4Simulations/WCD-build-inject/./Cherenkov *${j}.mac
	    #/remote/tesla/bmanning/work/Geant4Simulations/SSD-build/./SSDSimulation *${j}.mac
	  
	    /remote/tesla/bmanning/work/Modules/Merge/./merge
	    rm -f qgsII*
	done
	
	saveLocation=$(sed -n 's/.*<saveLocation>\(.*\)<\/saveLocation>/\1/p' CORSIKAParameters.xml)
	pName=$(sed -n 's/.*<particle>\(.*\)<\/particle>/\1/p' CORSIKAParameters.xml)
#echo  ${saveLocation}${pName}-${fileInc}VEM-inject.txt
	mv VEM_r_bins.txt ${saveLocation}${pName}-${fileInc}VEM.txt
       
	#mv MIP_r_bins.txt ${saveLocation}${pName}-${fileInc}MIP.txt

	rm -f qgsII*
	rm -f rValues.txt
	rm -f finalVEM.txt
	rm -f weightAll.txt
	rm -f SSD.root
	rm -f VEM.txt
	rm -f eValues.txt
	#	rm -f VEM_r_bins.txt
#	echo $angle >> VEM_r_bins.txt
	rm -f Cherenkov.root
	rm -f LDF.root
	#rm -f MIP_r_bins.txt
#	echo $angle >> MIP_r_bins.txt
    done
    
    echo "-1 -1 -1" >> test.txt
   
done
