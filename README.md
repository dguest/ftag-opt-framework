General instructions
====================

(see below for suggested release and additional packages)

 - setup release:  see below
 - checkout additional packages: see below

new instructions after git migration (October 5th 2016)

In Release 20.7
---------------

To develop packages:

```sh
asetup 20.7.7.5,AtlasDerivation,here
git clone ssh://git@gitlab.cern.ch:7999/atlas-flavor-tagging-tools/FlavourTagPerformanceFramework.git
setupWorkArea.py
cd WorkArea/cmt
cmt bro cmt config
cmt bro cmt make
cd ../../FlavourTagPerformanceFramework/btagIBLAnalysis/share/
```

To work with the current Derivations release:

```sh
git clone ssh://git@gitlab.cern.ch:7999/atlas-flavor-tagging-tools/FlavourTagPerformanceFramework.git
cd FlavourTagPerformanceFramework
. setup-r207.sh
```

In Release 21:
--------------

```sh
git clone ssh://git@gitlab.cern.ch:7999/atlas-flavor-tagging-tools/FlavourTagPerformanceFramework.git
cd FlavourTagPerformanceFramework
. setup-r21.sh
```

### Developing ###

You can checkout the trunk of `BTagging`, `JetTagTool`, and any other packages that are required to make them run by running

```sh
source scripts/checkout-dev-r21.sh
```

If you add any dependencies to the trunk of the b-tagging packages,
_please_ add the lines to check them out to this script.

To run:
=======

```sh
cd run
athena jobOptions_Tag.py
```

(also `jobOptions_Tag_data.py`: with data settings ... a bit overkilling)

(follow the instructions inside to decide whether to tag or not, which jet collections to use, etc)

Package Layout
==============

Once you've built this package, it should contain several important
directories:

 - `run`: run your jobs from here. The job options are symlinked to
          `btagIBLAnalysis/share`.
 - `btagIBLAnalysis`: the main ntuple dumping package
 - `scripts`: scripts to build any packages you've checked out
 - `build`: build files from CMake
 - `packages`: any additional tags that are needed for development


To submit samples on the grid:
==============================

There are a few scripts to help with this in `scripts/`. In general, go to the 
`run/` directory and use

```bash
../scripts/ftag-grid-sub.sh -j <job-options> -d <dataset>
```

This should take care of naming the output file. Run the script with the `-h`
option to get more info.
Note that you'll have to **add and commit** all your changes for this script
to run. It's also preferred that you tag the version of the code you used.
The commit requirement can be overridden (with the `-f` flag), but _use this
with caution! It will make your ntuples hard to reproduce 6 months from now._

Also note that this script isn't smart enough to know which packages you've
checked out. If you want your ntuple to be reproducable, please add a script
that checks out _a specific tag_ of any added packages.

See `scripts/ftag-grid-sub-rnnip.sh` for an example that wraps this script.

With the "old" scripts
----------------------

 - edit `mc_samples.txt` with the samples you want to run
 - edit `submitJobs.py` with your grid usedName, production suffix


List of old packages/release:
=============================

### 2015-08-30:  Valerio Dao ###

```sh
asetup 20.1.6.3,AtlasDerivation,gcc48,here
pkgco.py JetInterface-00-00-43
pkgco.py JetMomentTools-00-03-20
pkgco.py PileupReweighting-00-03-06
```

### 2015-06-22:  Valerio Dao ###
```sh
asetup 20.1.5.3,AtlasProduction,gcc48,here 
pkgco.py AODFix-00-02-02
pkgco.py JetInterface-00-00-43
pkgco.py JetCalibTools-00-04-41
pkgco.py JetRec-03-00-50
pkgco.py JetMomentTools-00-03-20
pkgco.py JetSelectorTools-00-05-01
```

### 2015-06-04:  Valerio Dao ###
UPDATED INSTRUCTIONS ONLY WORKING FOR MC15 SAMPLES!!!!!
(in particular this is used to run over FULL xAOD applying AODfix needed for Jet calibration)

```sh
asetup 20.1.5.3,AtlasProduction,gcc48,here 
pkgco.py AODFix-00-02-02
pkgco.py JetCalibTools-00-04-41
pkgco.py JetMomentTools-00-03-11-03
pkgco.py JetRec-03-00-40-04
```
