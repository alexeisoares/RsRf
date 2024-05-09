sfall HKLOUT temp.mtz XYZIN temp.pdb HKLIN final.mtz << EOF
LABIN  FP=F   SIGFP=SIGF   FREE=FreeR_flag
labout -
   FC=FCalc PHIC=PHICalc
MODE SFCALC -
    XYZIN -
    HKLIN
SYMMETRY 19 
badd 0.0
vdwr 2.5
end
EOF

fft HKLIN temp.mtz MAPOUT temp.map << EOF
SYMMETRY 19  
scale F1 1.0
axis Z X Y
labin -
   F1=FCalc PHI=PHICalc
end
EOF

mapmask MAPIN temp.map MSKOUT temp.msk << EOF
SYMMETRY 19  
MASK -
    CUT 0.2
EOF
