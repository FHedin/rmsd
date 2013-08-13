#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifdef	__cplusplus
extern "C" {
#endif

#define X2(a)		(a)*(a)
#define SIGN(a)		(a==0.0)?0.0:(a/fabs(a))

typedef struct{
    char HDR[5];
    int  ICNTRL[20];
    int NTITLE;
    char TITLE[81];
    
    double pbc[6];

    /*content of ICNTRL : non detailed ones are 0 */

    int NFILE;  //ICNTRL(1)  number of frames in this dcd
    int NPRIV;  //ICNTRL(2)  if restart, total number of frames before first print
    int NSAVC;  //ICNTRL(3)  frequency of writting dcd
    int NSTEP;  //ICNTRL(4)  number of steps ; note that NSTEP/NSAVC = NFILE
    int NDEGF;  //ICNTRL(8)  number of degrees of freedom
    int FROZAT; //ICNTRL(9) is NATOM - NumberOfFreeAtoms : it is the number of frozen (i.e. not moving atoms)
    int DELTA4; //ICNTRL(10) timestep in AKMA units but stored as a 32 bits integer !!!
    int QCRYS;  //ICNTRL(11) is 1 if CRYSTAL used.
    int CHARMV; //ICNTRL(20) is charmm version

    int NATOM; // Number of atoms */

    int LNFREAT; // Number of free (moving) atoms.
    int *FREEAT; // Array storing indexes of moving atoms.
}DCD;

typedef struct{
  double X;
  double Y;
  double Z;
}POINT_3D;

#ifdef	__cplusplus
}
#endif

#endif // GLOBAL_H_INCLUDED