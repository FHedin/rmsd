#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "error.h"

void parse_selection(char **inSele, int *nelem, char ***outsele, PARSING_MODE *mode)
{
    char *junk,*tmp;
    int strl=0,st=0;
    int i=0;

    st=strlen((*inSele));
    tmp=malloc((st+1)*sizeof(char));
    memcpy(tmp,(*inSele),st*sizeof(char));
    tmp[st]='\0';

    //puts(tmp);

    junk=strtok(tmp,",:=");
    junk=strtok(NULL,",:=");
    junk=strtok(NULL,",:=");
    while(junk!=NULL)
    {
       (*nelem)++;
       st=strlen(junk);
       strl=(st>strl)?st:strl;
       junk=strtok(NULL,",:=");
    }

    (*outsele)=(char**)malloc((*nelem)*sizeof(char*));
    for(i=0;i<(*nelem);i++)
        (*outsele)[i]=(char*)malloc(strl+1*sizeof(char));

    i=0;
    junk=strtok((*inSele),",:=");
    junk=strtok(NULL,",:=");
    determine_parsing_mode(junk,mode);
    junk=strtok(NULL,",:=");
    while(junk != NULL)
    {
        sprintf((*outsele)[i],"%s",junk);
        i++;
        junk=strtok(NULL,",:=");
    }

    free(tmp);

}

int find_matching_ones(char *selections, PARSING_MODE *mode, char *resname,
                       char *atname, char *resnum, char *atnum)
{
    int match=0;

    switch(*mode)
    {
        case by_res_name:
            match = 0 || strcmp(resname,selections);
        break;

        case by_at_name:
            match = 0 || strcmp(atname,selections);
        break;

        case by_res_num:
            match = 0 || strcmp(resnum,selections);
        break;

        case by_at_num:
            match = 0 || strcmp(atnum,selections);
        break;

        case select_all:
            match = 0;
        break;

        default:
            printf("Unknown value for switch File %s Line %d\n",__FILE__,__LINE__);
            exit(SELECTION_UNKNOWM_KEYWORD);
        break;

    }

    return match;
}

void determine_parsing_mode(char *string, PARSING_MODE *mode)
{
    if (!strcmp(string,"resname"))
        *mode=by_res_name;
    else if (!strcmp(string,"atname"))
        *mode=by_at_name;
    else if (!strcmp(string,"resnum"))
        *mode=by_res_num;
    else if (!strcmp(string,"atnum"))
        *mode=by_at_num;
    else if (!strcmp(string,"all"))
        *mode=select_all;
    else
    {
        printf("Unknown atomic selection mode : %s\n",string);
        puts("allowed modes are : resname atname resnum atnum ");
        exit(SELECTION_UNKNOWM_KEYWORD);
    }
}

SELEC_MODE determine_selection_mode(char **string)
{
    int st;
    char *tmp,*junk;

    SELEC_MODE mode;

    st=strlen((*string));
    tmp=malloc((st+1)*sizeof(char));
    memcpy(tmp,(*string),st*sizeof(char));
    tmp[st]='\0';

    junk=strtok(tmp,",:=");

    if(!strcmp(junk,"center"))
        mode = center_selec;
    else if(!strcmp(junk,"rmsd"))
        mode = rmsd_selec;
    else
    {
        printf("Unknown selection mode : %s\n",junk);
        puts("allowed modes are : center rmsd ");
        exit(SELECTION_UNKNOWM_KEYWORD);
    }

    free(tmp);

    return mode;
}