###make_TChain_Local.sh###

First, use make_TChain_Local.sh to run over the directories you want to analyse.  Do this by altering lines 1 and 3 and then source make_TChain_Local.sh.
Line 1 is the directory containing the .root files you want to validate, and line 3 is the name of a new function you want to generate and use in histcompseparate.C.  It will add a new function and test function to Chaining.C and Chaining.h, in addition to any preexisiting functions.
The test function can be used to quickly check any modifications made to histcompseparate.C, and is simply $funcname'Test'().

###histcompseparate.C###

Code for creating a series of valitation plots, comparing data from r17 and r19, and now modified to just r19.  Chaining.h is a header file used to create chains of r19 (xAOD) and r17(NTUP) data.  To run in test mode, use $funcname'Test()' instead of $funcname() in histcompseparate.C; this will create a much smaller chain with just one .root file so that code can be run and tested quickly.

The code is designed to be run in root using CINT; i.e. load root and then type .x histcompsepatate.C.  .eps, .png and .C plots will, by defult, be put into the directory $HOME/www/public/r19Validation/**jet type**.  The jet types used are b,c and light, so these directories need to be created first, or the output directory changed.

Variables to be plotted are changed using the variables parr19 and parr17, which, along with the histogram size and number of bins should be changed depending on your needs.  The branchs are assumed to all start with "jet_".  npar on line 22 must be set to be equal to the number of variables you wish to plot.

To run in r17 mode, change the parr19[] variables of H_normr17, H_1r17 to parr17, as well as jet_truthflav on line 156 to jet_trueFlav.
