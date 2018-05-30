#!/usr/bin/env bash


startingPosition=$(sed -n 's/.*<startingPosition>\(.*\)<\/startingPosition>/\1/p' CORSIKAParameters.xml)

for e in {0..20..1}
do
    let step=1
    #for i in {366..4000..${step}}
    for (( i=startingPosition; i<=4000; i=i+step ))
    do


	#Dynamic Bin Size - Implemented 12 April 2018.
	#Starting Bin is chosen such that a tank is always placed at 1000m.
	factor=$(bc <<< 1.1 )
	rs21=$(bc <<< "scale=2; $factor*$i")
	rs20=$(bc <<< "scale=2; $i*0.9")
	rs2=$(bc <<< "scale=2; $rs21/0.9")
	step=$(bc -l <<< "scale=2; $rs2-$i")
	step=$(printf "%.0f" $step)
#	echo ${rs2}
#	echo ${rs20}
#	echo ${rs21}

	/remote/tesla/bmanning/work/Modules/UniversalCORSIKAExtraction/./UniParticleReader <<EOF #/remote/tesla/bmanning/data/qgsII3/proton/10E19/0deg/DAT200006.part 
 ${i}
EOF

	wait

	mv *weight.txt weightAll.txt

	
	/remote/tesla/bmanning/work/Geant4Simulations/WCD-build-inject/./Cherenkov qgs*


	rm -f qgsII*
	
	/remote/tesla/bmanning/work/Modules/Merge/./merge

    done

saveLocation=$(sed -n 's/.*<saveLocation>\(.*\)<\/saveLocation>/\1/p' CORSIKAParameters.xml)

mv VEM_r_bins.txt ${saveLocation}${e}VEM-inject.txt
    
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


