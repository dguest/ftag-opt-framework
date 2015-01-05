Simple macros to produce some plots:

-----  PrintROC.py  ----------
- producing the efficiency VS rejection curve for many taggers
  * "python   PrintROC.py  help " for usage

-----  CompareROC.py  ----------
- allows to compare rejection curves from 2 sources file
  * "python   CompareROC.py  help " for usage

-----  Plotter_pt2.C  ---------
- allows you to print either variables (CURRENTLY BROKEN) or efficiency for fixed tagger WP
- it works with as steered from runAll.C: 
  * "root -l runAll.C"

-----  Plotter_pt3.C  ---------
- allows you to print the light-jet efficiency for fixed b-jets efficiency
- it works with as steered from runAll.C: 
  * "root -l runAll_special.C"

-----  CompareEff.py  ----------
- allows to compare performance of different taggers in the same file
  * "python   CompareEff.py  help " for usage
  * WARNING: some input commands are ineffective!!!!

-----  QuickPlotting.py  ----------
- allows to compare performance tagger between different files
  * "python   QuickPlotting.py  help " for usage


