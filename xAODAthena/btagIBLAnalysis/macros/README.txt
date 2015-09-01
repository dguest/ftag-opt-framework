Simple macros to produce some plots:

-----  PrintROC.py  ----------
- producing the efficiency VS rejection curve for many taggers
  * "python   PrintROC.py  help " for usage

-----  CompareROC.py  ----------
- allows to compare rejection curves from 2 sources file
  * "python   CompareROC.py  help " for usage

-----  Plotter_pt2.C  ---------
- allows you to print either variables (CURRENTLY BROKEN) or efficiency for fixed tagger WP
- check that WP cuts are up to date, as listed on https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks.  Currently included are MV2c20, with JVT cut, as used in PUB note.  WP is fed using steering file
- it works with as steered from runAll.C: 
  * "root -l runAll.C"

-----  Plotter_pt3.C  ---------
- allows you to print the light-jet efficiency for fixed b-jets efficiency
- it works with as steered from runAll.C: 
  * "root -l runAll_special.C"

-----  Plotter_pt3_crej.C  ---------
- allows you to print the c-jet efficiency for fixed b-jets efficiency
- it works with as steered from runAll.C: 
  * "root -l runAll_special.C"

-----  CompareEff.py  ----------
- allows to compare performance of different taggers in the same file
  * "python   CompareEff.py  help " for usage
  * WARNING: some input commands are ineffective!!!!

-----  QuickPlotting.py  ----------
- allows to compare performance tagger between different files
  * "python   QuickPlotting.py  help " for usage

-----  QuickPlotting_rej.py  ----------
- allows to compare background rejection between different files
  * "python   QuickPlotting.py  help " for usage


