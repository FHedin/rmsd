/**
Copyright (c) 2014, Florent Hedin
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of the University of Basel nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifdef	__cplusplus
extern "C"
{
#endif

#define X2(a)		(a)*(a)
#define SIGN(a)		(a==0.0)?0.0:(a/fabs(a))

    typedef struct
    {
        char HDR[5];
        int ICNTRL[20];
        int NTITLE;
        char TITLE[81];

        double pbc[6];

        /*content of ICNTRL : non detailed ones are 0 */

        int NFILE; //ICNTRL(1)  number of frames in this dcd
        int NPRIV; //ICNTRL(2)  if restart, total number of frames before first print
        int NSAVC; //ICNTRL(3)  frequency of writting dcd
        int NSTEP; //ICNTRL(4)  number of steps ; note that NSTEP/NSAVC = NFILE
        int NDEGF; //ICNTRL(8)  number of degrees of freedom
        int FROZAT; //ICNTRL(9) is NATOM - NumberOfFreeAtoms : it is the number of frozen (i.e. not moving atoms)
        int DELTA4; //ICNTRL(10) timestep in AKMA units but stored as a 32 bits integer !!!
        int QCRYS; //ICNTRL(11) is 1 if CRYSTAL used.
        int CHARMV; //ICNTRL(20) is charmm version

        int NATOM; // Number of atoms */

        int LNFREAT; // Number of free (moving) atoms.
        int *FREEAT; // Array storing indexes of moving atoms.
    } DCD;

    typedef struct
    {
        double X;
        double Y;
        double Z;
    } POINT_3D;

#ifdef	__cplusplus
}
#endif

#endif // GLOBAL_H_INCLUDED