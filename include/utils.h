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
#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C"
{
#endif

    typedef enum
    {
        by_res_name,
        by_at_name,
        by_res_num,
        by_at_num,
        select_all
    } PARSING_MODE;

    typedef enum
    {
        center_selec,
        rmsd_selec
    } SELEC_MODE;

    void parse_selection(char **inSele, int *nelem, char ***outsele, PARSING_MODE *mode);
    int find_matching_ones(char *selections, PARSING_MODE *mode, char *resname,
                           char *atname, char *resnum, char *atnum);
    void determine_parsing_mode(char *string, PARSING_MODE *mode);
    SELEC_MODE determine_selection_mode(char **string);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

