#!/usr/bin/env bash
startingPosition=$(sed -n 's/.*<startingPosition>\(.*\)<\/startingPosition>/\1/p' CORSIKAParameters.xml)
maxPosition=$(sed -n 's/.*<maxPosition>\(.*\)<\/maxPosition>/\1/p' CORSIKAParameters.xml)
iterations=$(sed -n 's/.*<iterations>\(.*\)<\/iterations>/\1/p' CORSIKAParameters.xml)
fileName=$(sed -n 's/.*<file>\(.*\)<\/file>/\1/p' CORSIKAParameters.xml)
nUniqueShowers=$(sed -n 's/.*<nUniqueShowers>\(.*\)<\/nUniqueShowers>/\1/p' CORSIKAParameters.xml)
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

	/remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/./usereadpart ${fileName} <<EOF #/remote/tesla/bmanning/data/qgsII3/proton/10E19/0deg/DAT200006.part 
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

mv VEM_r_bins.txt ${saveLocation}${startingPosition}-${e}.txt
    
rm -f qgsII*
rm -f rValues.txt
rm -f finalVEM.txt
rm -f weightAll.txt
rm -f SSD.root
rm -f VEM.txt
rm -f eValues.txt
rm -f VEM_r_bins.txt
rm -f Cherenkov.root
rm -f LDF.root
rm -f MIP_r_bins.txt
done
#/remote/tesla/bmanning/work/Modules/plot/./plot 
