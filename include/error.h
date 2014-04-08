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
#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#ifdef	__cplusplus
extern "C"
{
#endif

#define DCD_OPEN_ERROR 			100
#define DCD_READ_BYTESNUM_ERROR         101
#define DCD_EOF				102

#define OUTFILE_OPEN_ERROR 		110
#define	CORFILE_OPEN_ERROR		111
#define CORFILE_NATOM_ERROR		112

#define LAPACK_SGESVD_ERROR             120

#define SELECTION_UNKNOWM_KEYWORD       130
#define SELECTION_NO_ATOMS              131
#define SELECTION_NOT_MATCH             132


#ifdef	__cplusplus
}
#endif

#endif // ERROR_H_INCLUDED
