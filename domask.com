mapmask MAPIN model.map MSKOUT model.msk << EOF
SYMMETRY 19  
MASK -
    CUT 0.2
EOF
