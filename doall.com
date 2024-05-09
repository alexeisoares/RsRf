#!/usr/bin/bash

cp InputFiles/final* .
cp final.pdb start.pdb
./domap.com > domap.log
./domask.com > domask.log
./dostart.com > dostart.log
rm doRsRf.log

grep CRYST start.pdb >  head.pdb
grep SCALE start.pdb >> head.pdb

START=2
END=504
for (( i=START;i<=END;i++ )) ; do
    echo -n "* REAL SPACE R VALUE FOR AMINO ACID NUMBER $i: "
    cp head.pdb temp.pdb
    if [ $i -le 9 ]; then
        grep "ATOM.................A   $i" start.pdb >> temp.pdb
    fi
    if [ $i -le 99 -a $i -ge 10 ]; then
        grep "ATOM.................A  $i" start.pdb >> temp.pdb
    fi
    if [ $i -le 999 -a $i -ge 100 ]; then
        grep "ATOM.................A $i" start.pdb >> temp.pdb
    fi
    echo 'END' >> temp.pdb
    ./domask1.com > domask1.log
    ./doRsRf >> doRsRf.log
    cat temp.log >> doRsRf.log
    grep 'R FACTOR IS' temp.log | sed 's/RFAC...................................//g' > tmp.tmp
    tail -3 tmp.tmp | head -1 > tmp4.tmp
    tail -2 tmp.tmp | head -1 > tmp5.tmp
    tail -1 tmp.tmp > tmp6.tmp
    cat tmp4.tmp | tr -d '\n' 
    cat tmp5.tmp | tr -d '\n' 
    cat tmp6.tmp
done
