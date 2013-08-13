#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "io.h"
#include "error.h"
#include "utils.h"

static int first=1;

static int dcdsize;
//static int currentposition;

//read dcd header and stores useful parameters in a structure
void read_dcd_header(FILE *dcdf, DCD *dcd)
{
        fseek(dcdf,0,SEEK_END);
        dcdsize=ftell(dcdf);
        rewind(dcdf);
        printf("Size of DCD is (in Bytes) : %d\n",dcdsize);

	int i=0;

        unsigned int fortcheck1,fortcheck2;

//	char HDR[5]="";
//	int  ICNTRL[20]={0};

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(dcd->HDR,sizeof(char),4,dcdf);
        fread(dcd->ICNTRL,sizeof(int),20,dcdf);
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);

        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : quantities do not match.");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

        dcd->HDR[4]='\0';
        printf("HDR : %s \n\n",dcd->HDR);
        printf("ICNTRL : \t");
        for (i=0;i<20;i++)
            printf("%d\t",dcd->ICNTRL[i]);
	puts("\n");

        dcd->NFILE = dcd->ICNTRL[0];
        dcd->NPRIV = dcd->ICNTRL[1];
        dcd->NSAVC = dcd->ICNTRL[2];
        dcd->NSTEP = dcd->ICNTRL[3];
        dcd->NDEGF = dcd->ICNTRL[7];
        dcd->FROZAT= dcd->ICNTRL[8];
        dcd->DELTA4= dcd->ICNTRL[9];
        dcd->QCRYS = dcd->ICNTRL[10];
        dcd->CHARMV= dcd->ICNTRL[19];

//	int NTITLE=0;
//	char TITLE[81]="";

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(&dcd->NTITLE,sizeof(int),1,dcdf);
        printf("NTITLE : %d \n",dcd->NTITLE);
        puts("TITLE : ");
        for (i=0;i<dcd->NTITLE;i++)
        {
                fread(dcd->TITLE,sizeof(char),80,dcdf);
                dcd->TITLE[80]='\0';
                puts(dcd->TITLE);
        }
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);
        puts("\n");

        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : not match");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

	int NATOM=0;

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(&NATOM,sizeof(int),1,dcdf);
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);

        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : not match");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

        dcd->NATOM = NATOM;
        dcd->LNFREAT = dcd->NATOM - dcd->FROZAT;

        if ((dcd->LNFREAT) != (dcd->NATOM))
        {
            dcd->FREEAT = (int*)malloc(dcd->LNFREAT*sizeof(int));

            fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
            fread(dcd->FREEAT,sizeof(int),dcd->LNFREAT,dcdf);
            fread(&fortcheck2,sizeof(unsigned int),1,dcdf);

            if( fortcheck1 != fortcheck2 )
            {
                puts("Error when reading data from dcd : not match");
                printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
                printf("File %s Line %d \n",__FILE__,__LINE__);
                exit(DCD_READ_BYTESNUM_ERROR);
            }
        }
}

//read one dcd step
void read_dcd_traj(FILE *dcdf, DCD *dcd, float **crd, int index_c[], int index_r[], int ntot)
{
	unsigned int fortcheck1=0 , fortcheck2=0;
	int i=0 , j=0 , k=0;

// 	double pbc[6];

	if (dcd->QCRYS)
	{
		fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
		fread(dcd->pbc,sizeof(double),6,dcdf);
		fread(&fortcheck2,sizeof(unsigned int),1,dcdf);

		if( fortcheck1 != fortcheck2 )
        	{
            		puts("Error when reading data from dcd : not match");
            		printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            		printf("File %s Line %d \n",__FILE__,__LINE__);
            		exit(DCD_READ_BYTESNUM_ERROR);
        	}
	}

        // it is less or equal to NATOM
        int siz = (first) ? dcd->NATOM : dcd->LNFREAT ;

	float *X=(float*)malloc(siz * sizeof *X);
	float *Y=(float*)malloc(siz * sizeof *Y);
	float *Z=(float*)malloc(siz * sizeof *Z);

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(X,sizeof(float),siz,dcdf);
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);
        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : not match");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(Y,sizeof(float),siz,dcdf);
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);
        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : not match");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

        fread(&fortcheck1,sizeof(unsigned int),1,dcdf);
        fread(Z,sizeof(float),siz,dcdf);
        fread(&fortcheck2,sizeof(unsigned int),1,dcdf);
        if( fortcheck1 != fortcheck2 )
        {
            puts("Error when reading data from dcd : not match");
            printf("fortcheck1 %d \t fortcheck2 %d \n",fortcheck1,fortcheck2);
            printf("File %s Line %d \n",__FILE__,__LINE__);
            exit(DCD_READ_BYTESNUM_ERROR);
        }

        i=0; // for loop on natom
        j=0; // for loop on lnfreat
        k=0; // for loop on crd

        while(1)
	{
            if (first) //if first frame all atoms presents in X Y Z
            {
                if(index_c[i] || index_r[i])
                {

                    crd[k][0] = X[i];
                    crd[k][1] = Y[i];
                    crd[k][2] = Z[i];
                    //printf("Filling crd[%d] : %f\t%f\t%f\t%f\t%f\t%f\n",k,crd[k][0],crd[k][1],crd[k][2],X[i],Y[i],Z[i]);
                    k++;
                }
                i++;
                if(i==dcd->NATOM || k==ntot)
                    break;
            }
            else //else only freeat are in X Y Z
            {
                i=dcd->FREEAT[j++]-1;
                if(index_c[i] || index_r[i])
                {
                    crd[k][0] = X[i];
                    crd[k][1] = Y[i];
                    crd[k][2] = Z[i];
                    //printf("Filling crd[%d] : %f\t%f\t%f\t%f\t%f\t%f\n",k,crd[k][0],crd[k][1],crd[k][2],X[i],Y[i],Z[i]);
                    k++;
                }
                if(i==dcd->NATOM || k==ntot)
                    break;
            }
	}//end of while(true)

	free(X); free(Y); free(Z);

        if ((dcd->LNFREAT) != (dcd->NATOM))
            first=0;
}

void write_dcd_header(FILE *dcdnew, DCD *dcd, int ntot)
{
    unsigned int size;
    
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    fwrite(dcd->HDR,sizeof(char),4,dcdnew);
    fwrite(dcd->ICNTRL,sizeof(int),20,dcdnew);
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    
    int NTITLE=3;
    char TITLE1[80]="";
    char TITLE2[80]="";
    char TITLE3[80]="";
    char tmp[4096]="";
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *user=NULL , *host=NULL , *pwd=NULL;
    user=getenv("USER");
    host=getenv("HOSTNAME");
    pwd=getenv("PWD");
    
    sprintf(tmp,"* CREATION DATE : %s",asctime(timeinfo));
    strncat(TITLE1,tmp,79);
    sprintf(tmp,"* USER : %s HOSTNAME : %s",(user!=NULL)?user:"UNKNOWN",(host!=NULL)?host:"UNKNOWN");
    strncat(TITLE2,tmp,79);
    sprintf(tmp,"* PWD : %s",(pwd!=NULL)?pwd:"UNKNOWN");
    strncat(TITLE3,tmp,79);
    
    size = sizeof(int) + NTITLE*80*sizeof(char);
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    fwrite(&NTITLE,sizeof(int),1,dcdnew);
    fwrite(TITLE1,sizeof(char),80,dcdnew);
    fwrite(TITLE2,sizeof(char),80,dcdnew);
    fwrite(TITLE3,sizeof(char),80,dcdnew);
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    
    size = sizeof(int);
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    fwrite(&dcd->NATOM,sizeof(int),1,dcdnew);
    fwrite(&size,sizeof(unsigned int),1,dcdnew);
    
    if(dcd->NATOM != dcd->LNFREAT)
    {
        size=dcd->LNFREAT*sizeof(int);
        fwrite(&size,sizeof(unsigned int),1,dcdnew);
        fwrite(dcd->FREEAT,sizeof(int),dcd->LNFREAT,dcdnew);
        fwrite(&size,sizeof(unsigned int),1,dcdnew);
    }
}

void write_dcd_traj(FILE *dcdnew, DCD *dcd, float **crd)
{
    unsigned int size;
    
    if (dcd->QCRYS)
    {
        size = 6*sizeof(double);
        fwrite(&size,sizeof(unsigned int),1,dcdnew);
        fwrite(dcd->pbc,sizeof(double),6,dcdnew);
        fwrite(&size,sizeof(unsigned int),1,dcdnew);
    }
    
    int nsiz = (first) ? dcd->NATOM : dcd->LNFREAT ;
    float *X=(float*)malloc(nsiz * sizeof *X);
    float *Y=(float*)malloc(nsiz * sizeof *Y);
    float *Z=(float*)malloc(nsiz * sizeof *Z);
    
    
    
}

//read useful parts of charmm cor file
void read_charmm_cor(FILE *corfile, DCD *dcd, float **ref,
                     char **sele_cent, int elem_cent, PARSING_MODE *pmode_cent, int index_cent[], int *nsel_cent,
                     char **sele_rmsd, int elem_rmsd, PARSING_MODE *pmode_rmsd, int index_rmsd[], int *nsel_rmsd)
{
	char buff1[1024]="", *buff2=NULL , *buff3=NULL ;
	int differentSelection=0;
	int i=0 , j=0;

	while( (i<dcd->NATOM) && (fgets(buff1,1024,corfile)!=NULL) )
	{
		buff2=strtok(buff1," \n\t");

		if (buff2!=NULL && buff2[0]=='*') continue;

		else
		{
			buff3=strtok(NULL," \n\t");
			if (buff3==NULL)
			{
				if (atoi(buff2)!=dcd->NATOM)
				{
					printf("Error : Number of atoms in DCD file and COR file are not the same !\n");
					exit(CORFILE_NATOM_ERROR);
				}
				continue;
			}
			else
			{
                                char *atnum=buff2 , *resnum=buff3;
				char *resname , *atname ;

				resname=strtok(NULL," \n\t");
				atname=strtok(NULL," \n\t");

                                // centering selection
                                differentSelection=0;
                                for (j=0;j<elem_cent;j++)
                                {
                                    differentSelection=find_matching_ones(sele_cent[j],pmode_cent,resname,atname,resnum,atnum);
                                    if(!differentSelection)
                                    {
                                        index_cent[i]=1;
                                        (*nsel_cent)++;
                                    }
                                }

                                // rmsd selection
                                differentSelection=0;
                                for (j=0;j<elem_rmsd;j++)
                                {
                                    differentSelection=find_matching_ones(sele_rmsd[j],pmode_rmsd,resname,atname,resnum,atnum);
                                    if(!differentSelection)
                                    {
                                        index_rmsd[i]=1;
                                        (*nsel_rmsd)++;
                                    }
                                }

                                // coordinates
				ref[i][0] = (float) atof(strtok(NULL," \n\t"));
				ref[i][1] = (float) atof(strtok(NULL," \n\t"));
				ref[i][2] = (float) atof(strtok(NULL," \n\t"));
				i++;

				strtok(NULL," \n\t");
				strtok(NULL," \n\t");
				strtok(NULL," \n\t");
			}
		}
		buff2=strtok(NULL," \n\t");
	}
}
