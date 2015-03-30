--- histcompseparate.C ---

Code for creating a series of valitation plots from two data sets.  The code is designed to be run in root using runAll.C; i.e. root -l runAll.C.  Here, the input files are selected, can either be a directory or individual files.  .eps, .png and .C plots will, by defult, be put into the output directory specified in runAll.C  The jet types used are b,c and light, so these directories need to be created first, or the output directory changed.

Variables to be plotted are changed using the variables parr19 and parr17, which, along with the histogram size and number of bins should be changed depending on your needs.  The branchs are assumed to all start with "jet_".  npar on line 22 must be set to be equal to the number of variables you wish to plot.

To run in r17 mode, change the parr19[] variables of H_normr17, H_1r17 to parr17, as well as jet_truthflav on line 156 to jet_trueFlav.
