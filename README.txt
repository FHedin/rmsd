rmsd
====

C program for calculating rmsd from a charmm DCD file.


Dependencies
============

Lapack and Blas are required for compiling this software. 

How to run
==========

Please provide 4 arguments to the program:
* path to CHARMM dcd file (binary)
* path to reference structure (CHARMM .cor text file)
* atoms selection for rmsd (command line)
* atoms selection for centering (command line)

Note that it is not necessary to use 2 identical selections for centering and rmsd 

For a selection by residue name (3rd column of CHARRMM .cor file): 
./rmsd *.dcd *.cor rmsd=resname:GLY,LEU center=resname:GLY,LEU 

For a selection by atom name (4th column of CHARRMM .cor file) : 
./rmsd *.dcd *.cor rmsd=atname:C,CA,N center=atname:C,CA,N 

For a selection by residue number (2nd column of CHARMM .cor file) : 
./rmsd *.dcd *.cor rmsd=resnum:15,19,62,101 center=resnum:15,19,63 

For a selection by atom number (1st column of CHARMM .cor file) : 
./rmsd *.dcd *.cor rmsd=atnum:1114,1123,1169,2056,6987 center=atname:C,CA,N 


