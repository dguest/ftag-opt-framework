Simple macros to produe some plots:

-----  PrintROC.py  ----------
- producing the efficiency VS rejection curve for many taggers
- "python   PrintROC.py  help " for usage


-----  Plotter_pt2.C  ---------
- allows you to print either variables or efficiency
- this is very WORK IN PROGRESS, still need to work on:
       * steering input files from outside
       * steering which variables/efficiencies to plot from file
       * compiled version
- for the moment it works with: 
"root -l Plotter_pt2.C" (but need to change file inside

