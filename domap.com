fft HKLIN final.mtz MAPOUT final.map << EOF
SYMMETRY 19  
scale F1 1.0
axis Z X Y
labin -
   F1=F     PHI=PHIC    
end
EOF

fft HKLIN final.mtz MAPOUT model.map << EOF
SYMMETRY 19
scale F1 1.0
axis Z X Y
labin -
   F1=FC    PHI=PHIC
end
EOF
