Data analysis scripts for the UA9 experimental at the SPS H8 extraction beamline.

PART I: THEORY

[1] W. Scandale et al. Probability of inelastic nuclear interactions of high-energy protons in a bent crystal.
Nuclear Instruments and Methods in Physics Research B, 268:2655 – 2659, 2010.

[2] W. Scandale et al. Study of inelastic nuclear interactions of 400 GeV/c protons inbent silicon crystals for beam steering purposes.
The European Physical Journal C, 78:505, 2018.

PART II: DATA ANALYSIS

Use a bash script, which will do all the work concerning the data analysis. The software has 4 (four) main input parameters:

--> parameter 1: the name of the input file. The structure of the file is described in src/includes.hh

--> parameter 2: run mode. Depends on what do you want to do it can be as follows:

    0 - to plot histograms of the input beam distributions and to define the geometrical cuts;
    1 - to calculate the torsion parameters, to determine an optimal orientation of the crystal with respect to the incident beam, and to find a channeling efficiency within a certain incoming angle;
    2 - to compute the inelastic nuclear interaction (INI) rate in terms of angular cuts and number of INI coincidents;
    3 - to compute the inelastic nuclear interaction (INI) rate during an angular scan of the crystal, in terms of a fixed angular cut and number of INI coincidences for different goniometer orientations;

--> parameter 3: year of the data taking. Due to a small change in a data structure (especially for INI variables), there are two possibilities to receive signals from the INI plastic scintillators.

--> parameter 4: a working regime of the crystal. It can be 0 - channeling orientation, and 1 - amorphous or background cases.

A file parameters.dat contains all necessary geometrical cuts and parameters for the analysis.
Each subroutine connected to the parameter 2 has some specific ways to be tuned. Please, take a look data_ana class to understand their structure.
Фppropriate comments are written. To change the angular range for scans or cutting, please, refer to data_ana.hh.

For more information contact:

Mr. Andrii Natochii:    andrii.natochii@cern.ch & andrii.natochii@gmail.com

Mr. Marco Garattini:    marco.garattini@cern.ch
