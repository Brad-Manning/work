#!/usr/bin/env bash

rm -f qgsII*
rm -f rValues.txt
rm -f finalVEM.txt
rm -f weightAll.txt
rm -f SSD.root
rm -f VEM.txt
rm -f eValues.txt
rm -f VEM_r_bins.txt
for e in {0..20..1}
do
    let step=1
    #for i in {366..4000..${step}}
    for (( i=1000; i<=4000; i=i+step ))
    do

	#Dynamic Bin Size - Implemented 12 April 2018.
	#Starting Bin is chosen such that a tank is always placed at 1000m.
	factor=$(bc <<< 1.1 )
	rs21=$(bc <<< "scale=2; $factor*$i")
	rs20=$(bc <<< "scale=2; $i*0.9")
	rs2=$(bc <<< "scale=2; $rs21/0.9")
	step=$(bc -l <<< "scale=2; $rs2-$i")
	step=$(printf "%.0f" $step)
	echo ${rs2}

	/remote/tesla/bmanning/work/Modules/CORSIKA_thin/CORSIKA_thin_v2.0/./usereadpart /remote/tesla/bmanning/data/qgsII3/proton/10E19/49deg/DAT200014 <<EOF
   ${rs20}
   ${rs21}
EOF

	wait

	mv *weight.txt weightAll.txt

	
	/remote/tesla/bmanning/work/Geant4Simulations/WCD-build/./Cherenkov qgs* <<EOF
${i}
${i}
EOF

	rm -f qgsII*
	
	/remote/tesla/bmanning/work/Modules/Merge/./merge

    done
mv VEM_r_bins.txt /remote/tesla/bmanning/data/SimulationData/proton/10E19/0deg/${e}VEM.txt
    
    
done
#/remote/tesla/bmanning/work/Modules/plot/./plot 


