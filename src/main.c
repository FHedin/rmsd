#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "io.h"
#include "error.h"
#include "rmsd.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	int i=0,j=0;

        //  how to use this program
	if (argc != 5)
	{
		printf("Please provide 4 arguments : path to CHARMM dcd file (binary) , path to reference structure "
                       "(CHARMM .cor text file) , atoms selection for rmsd and for centering (cf. next) . \n"
                       "Note that it is possible to use 2 different selections for centering and rmsd \n\n");

                puts("For a selection by residue name (3rd column of CHARRMM .cor file): ");
                printf("%s *.dcd *.cor rmsd=resname:GLY,LEU center=resname:GLY,LEU \n\n",argv[0]);

                puts("For a selection by atom name (4th column of CHARRMM .cor file) : ");
                printf("%s *.dcd *.cor rmsd=atname:C,CA,N center=atname:C,CA,N \n\n",argv[0]);

                puts("For a selection by residue number (2nd column of CHARMM .cor file) : ");
                printf("%s *.dcd *.cor rmsd=resnum:15,19,62,101 center=resnum:15,19,63 \n\n",argv[0]);

                puts("For a selection by atom number (1st column of CHARMM .cor file) : ");
                printf("%s *.dcd *.cor rmsd=atnum:1114,1123,1169,2056,6987 center=atname:C,CA,N \n\n",argv[0]);

		return EXIT_SUCCESS;
	}

        // pointers to arguments for I/O and parsing
	char *dcdname = argv[1];
	char *corname = argv[2];

        char *cent_selec=NULL , *rmsd_selec=NULL;

        if(determine_selection_mode(&argv[3]) == center_selec)
        {
            cent_selec = argv[3];
            rmsd_selec = argv[4];
        }
        else
        {
            cent_selec = argv[4];
            rmsd_selec = argv[3];
        }

        // opening files
	FILE *dcdf=NULL;
	dcdf=fopen(dcdname,"rb");
	if (dcdf==NULL)
	{
		printf("Error while opening the file '%s' with binary read mode\n",dcdname);
		exit(DCD_OPEN_ERROR);
	}
        
        FILE *dcdnew=NULL;
        dcdnew=fopen("new.dcd","rb");
	if (dcdnew==NULL)
	{
		printf("Error while opening the file '%s' with binary read mode\n","new.dcd");
		exit(DCD_OPEN_ERROR);
	}

	FILE *corf=NULL;
	corf=fopen(corname,"rt");
	if (corf==NULL)
	{
		printf("Error while opening the file '%s' with text read mode\n",corname);
		exit(CORFILE_OPEN_ERROR);
	}

        FILE *rmsdf=NULL;
        char *out_name=(char*)malloc( (strlen(rmsd_selec)+20)*sizeof(char) );
        strcpy(out_name,"rmsd_");
        strcat(out_name,rmsd_selec);
        strcat(out_name,"_.dat");

	rmsdf=fopen(out_name,"wt");
	if (rmsdf==NULL)
	{
		printf("Error while opening the file '%s' with text write mode\n",out_name);
		exit(OUTFILE_OPEN_ERROR);
	}

        // a structure for reading header of dcd : will contain a lot of informations read from dcd
        DCD dcd;
	read_dcd_header(dcdf,&dcd);

	printf("ATOMS : %d \t FRAMES : %d \n\n",dcd.NATOM,dcd.NFILE);

        //for storing the atoms selections : one selection for centering and one for rmsd
        char **sele_cent=NULL , **sele_rmsd=NULL;
        int elem_cent=0 , elem_rmsd=0;
        PARSING_MODE pmode_cent , pmode_rmsd;

        parse_selection(&cent_selec,&elem_cent,&sele_cent,&pmode_cent);
        parse_selection(&rmsd_selec,&elem_rmsd,&sele_rmsd,&pmode_rmsd);

	//index to know if it is an "active atom" (i.e. if not it will not be used for centering or evaluating rmsd)
	int *index_cent=NULL , *index_rmsd=NULL ;
	index_cent=(int*)calloc(dcd.NATOM,sizeof(int));
        index_rmsd=(int*)calloc(dcd.NATOM,sizeof(int));
	int nsele_cent=0 , nsele_rmsd=0;

	// NATOM*3 matrix for storing coordinates
	float **tmpref=NULL;
	tmpref=(float**)malloc(dcd.NATOM * sizeof(float*));
        for(i=0;i<dcd.NATOM;i++)
		tmpref[i]=(float*)malloc(3 * sizeof(float));

        // reads coordinates and obtains to indexes for knowing which atoms are active for centering or rmsd
        read_charmm_cor(corf,&dcd,tmpref,
                        sele_cent, elem_cent, &pmode_cent, index_cent, &nsele_cent,
                        sele_rmsd, elem_rmsd, &pmode_rmsd, index_rmsd, &nsele_rmsd);

        printf("%d atoms in centering selection.\n",nsele_cent);
        if(nsele_cent==0)
        {
            puts("Error : no atom matching this selection ! Exit ...");
            exit(SELECTION_NO_ATOMS);
        }

        printf("%d atoms in rmsd selection.\n",nsele_rmsd);
        if(nsele_rmsd==0)
        {
            puts("Error : no atom matching this selection ! Exit ...");
            exit(SELECTION_NO_ATOMS);
        }

        /* note that :
         * 1) the centering selection should contain the rmsd selection .
         * 2)  nsele_cent >= nsele_rmsd
         */
        /*
        if(nsele_cent < nsele_rmsd)
        {
            puts("Error : less atoms in centering selection than in rmsd selection.")
            puts("You should always provide a bigger or equivalent selection for centering than for rmsd.");
            exit(SELECTION_NOT_MATCH);
        }

        for(i=0;i<dcd.NATOM;i++)
        {
            if (index_rmsd[i] && (!index_cent[i]))
            {
                printf("Error : atom %d present in rmsd_selection but not in centering_selection.");
                exit(SELECTION_NOT_MATCH);
            }
        }*/

        int ntot=0;
        for(i=0;i<dcd.NATOM;i++)
        {
            //printf("indexes[%d]:\t%d\t%d\n",i,index_cent[i],index_rmsd[i]);
            if(index_cent[i] || index_rmsd[i])
                ntot++;
        }

        printf("ntot : %d\n",ntot);

        float **crd=NULL , **ref=NULL;
        crd=(float**)malloc(ntot * sizeof(float*));
	ref=(float**)malloc(ntot * sizeof(float*));
	for(i=0;i<ntot;i++)
                crd[i]=(float*)malloc(3 * sizeof(float));
        for(i=0;i<ntot;i++)
		ref[i]=(float*)malloc(3 * sizeof(float));

        j=0;
        for(i=0;i<dcd.NATOM;i++)
        {
            if(index_cent[i] || index_rmsd[i])
            {
                memcpy(ref[j],tmpref[i],3*sizeof(float));
                //printf("refs[%d] (old new) :\t%f\t%f\t%f\t%f\t%f\t%f\n",i,tmpref[i][0],tmpref[i][1],tmpref[i][2],ref[j][0],ref[j][1],ref[j][2]);
                j++;
            }
        }

        for(i=0;i<dcd.NATOM;i++)
                free(tmpref[i]);
        free(tmpref);

	center_sys(dcd.NATOM,ref,ntot,index_cent,index_rmsd);

	double rms=0. ;

	for (i=0;i<dcd.NFILE;i++)
	{
                read_dcd_traj(dcdf,&dcd,crd,index_cent,index_rmsd,ntot);
                //for(j=0;j<ntot;j++)
                //    printf("dcd[%d] = \t%f\t%f\t%f\n",j,crd[j][0],crd[j][1],crd[j][2]);
		rms = rmsd(dcd.NATOM,crd,ref,ntot,index_cent,index_rmsd);
		fprintf(rmsdf,"%lf\n",rms);
	}
	printf("\n");

	for(i=0;i<ntot;i++)
	{
		free(crd[i]);
		free(ref[i]);
	}
	free(crd);
	free(ref);

        for(i=0;i<elem_cent;i++)
            free(sele_cent[i]);
        free(sele_cent);

        for(i=0;i<elem_rmsd;i++)
            free(sele_rmsd[i]);
        free(sele_rmsd);

        free(out_name);

        free(index_cent);
        free(index_rmsd);

	fclose(dcdf);
	fclose(rmsdf);
	fclose(corf);

        if((dcd.LNFREAT) != (dcd.NATOM))
        {
            free(dcd.FREEAT);
        }

	return EXIT_SUCCESS;
}
