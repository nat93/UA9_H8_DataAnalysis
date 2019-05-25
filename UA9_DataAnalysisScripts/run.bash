#!/bin/bash

make clean; make -j2;

#[1]: intput file name
par1=/media/andrii/F492773C92770302/recoDataSimple_2708_zcorrected.root;

#[2]: run mode:         0 - histograms; 1 - torsion/efficiency/optimization; 2 - ini fixed; 3 - ini angular scan
par2=0;

#[3]: year:             >= 2015
par3=2015;

#[4]: runID:            0 - CH (with angular cut); 1 - AM/BKG (without angular cut)
par4=0;

./run_ana $par1 $par2 $par3 $par4
