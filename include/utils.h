/*
 * File:   utils.h
 * Author: hedin
 *
 * Created on September 27, 2012, 11:38 AM
 */

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

