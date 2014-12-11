Code for creating a series of valitation plots, comparing data from r17 and r19 NTuples.  Chaining.h is a header file used to create chains of r19 (xAOD) and r17(NTUP) data.  To run in test mode, use xAODTest() and NTUPTest() instead of xAOD() and NTUP() in histcompseparate.C; this will create a much smaller chain with just on NTuple so that code can be run and tested quickly.

The code is designed to be run in root using CINT; i.e. load root and then type .x histcompsepatate.C.  .eps, .png and .C plots will, by defult, be put into the directory $HOME/www/public/r19Validation/**jet type**.  The jet types used are b,c and light, so these directories need to be created first, or the output directory changed.

Variables to be plotted are changed using the variables parr19 and parr17, which, along with the histogram size and number of bins should be changed depending on your needs.  The branchs are assumed to all start with "jet_".
