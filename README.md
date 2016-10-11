General instructions
====================

(see below for suggested release and additional packages)

 - setup release:  see below
 - checkout additional packages: see below

new instructions after git migration (October 5th 2016)
in release 20.7

```sh
asetup 20.7.7.5,AtlasDerivation,here
git clone ssh://git@gitlab.cern.ch:7999/jshlomi/FlavourTagPerformanceFramework.git
setupWorkArea.py
cd WorkArea/cmt
cmt bro cmt config
cmt bro cmt make
cd ../../FlavourTagPerformanceFramework/run
```

in release 21:

```sh
. setup-r21.sh
cd run
athena jobOptions_Tag.py
```

To run:
=======

```sh
cd ../../xAODAthena/run
athena jobOptions_Tag.py
```

(also `jobOptions_Tag_data.py`: with data settings ... a bit overkilling)

(follow the instructions inside to decide whether to tag or not, which jet collections to use, etc)

To submit samples on the grid:
==============================

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
