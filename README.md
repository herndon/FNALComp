FNALComp
========

FNAL Programming Reconstruction Software for Large Computing Projects Course Code Repository

Prerequisites
git
c++11
root: https://root.cern/install/
setup root using (from base root directory)
. ./bin/thisroot.sh


checkout
git clone https://github.com/herndon/FNALComp.git FNALComp

compiling<br>
make

running
generate simulated data for muons traversing a muon detector (silicon tracker)
./dataGen
reconstruct data in the tracker
./dataReco
event display for reconstructed data
./dataReco <eventNumber>
i.e.
./dataReco 1

configuration files
configfile.txt
configfilereco.txt

Software course using this code package
https://indico.fnal.gov/event/8568/overview
https://cdcvs.fnal.gov/redmine/projects/fnal-soft-school-summer-2014


