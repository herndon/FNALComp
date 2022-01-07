FNALComp
========

FNAL Programming Reconstruction Software for Large Computing Projects Course Code Repository

Prerequisites<br>
git<br>
c++11
root: https://root.cern/install/<br>
setup root using (from base root directory)<br>
. ./bin/thisroot.sh


checkout<br>
git clone https://github.com/herndon/FNALComp.git FNALComp

compiling<br>
make

running<br>
generate simulated data for muons traversing a muon detector (silicon tracker)<br>
./dataGen<br>
reconstruct data in the tracker<br>
./dataReco<br>
event display for reconstructed data<br>
./dataReco <eventNumber><br>
i.e.<br>
./dataReco 1<br>

configuration files<br>
configfile.txt<br>
configfilereco.txt

Software course using this code package<br>
https://indico.fnal.gov/event/8568/overview<br>
https://cdcvs.fnal.gov/redmine/projects/fnal-soft-school-summer-2014


