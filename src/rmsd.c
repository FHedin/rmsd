#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "io.h"
#include "error.h"
#include "rmsd.h"

//computes rmsd between 2 structures

double rmsd(int natom, float **crd, float **ref, int crd_size, int index_c[], int index_r[])
{
    int i = 0, j = 0;

    double rmsd = 0.0;

    //covariance matrix 3x3
    float covmat[3 * 3] = { 0.f };
    //matrices 3x3 for SVD
    float u[3 * 3] = { 0.f }, vt[3 * 3] = { 0.f };

    center_sys(natom, crd, crd_size, index_c, index_r);

    get_cov_mat(natom, crd, ref, index_c, index_r, covmat);

    double d = determinant_3x3(covmat);
    d = SIGN(d);

    svd_f(covmat, u, vt);

    rot_mat(d, u, vt, crd, crd_size);

    i = 0;
    j = 0;
    for ( i = 0; i < natom; i++ )
    {
        if ( index_r[i] )
        {
            rmsd += X2(crd[j][0] - ref[j][0]) + X2(crd[j][1] - ref[j][1]) + X2(crd[j][2] - ref[j][2]);
            j++;
        }
    }

    rmsd /= ( double ) j;
    rmsd = sqrt(rmsd);

    return rmsd;
}

//get the centroid of a system

POINT_3D getCentroid(int natom, float **cd, int index_c[], int index_r[])
{
    int i = 0, j = 0, k = 0;
    POINT_3D c = { 0., 0., 0. };

    for ( i = 0; i < natom; i++ )
    {
        if ( (!index_c[i]) && (!index_r[i]) )
            continue;
        else if ( (!index_c[i]) && index_r[i] )
        {
            j++;
        }
        else if ( index_c[i] )
        {
            c.X += cd[j][0];
            c.Y += cd[j][1];
            c.Z += cd[j][2];
            j++;
            k++;
        }
    }

    c.X /= ( double ) k;
    c.Y /= ( double ) k;
    c.Z /= ( double ) k;

    return c;
}

//center the system so that the centroid is at the origin
void center_sys(int natom, float **crd, int crd_size, int index_c[], int index_r[])
{
    int i = 0;
    POINT_3D c = { 0., 0., 0. };

    c = getCentroid(natom, crd, index_c, index_r);

    for ( i = 0; i < crd_size; i++ )
    {
        crd[i][0] -= c.X;
        crd[i][1] -= c.Y;
        crd[i][2] -= c.Z;
    }
}

void center_sys2(int natom, float **crd, int crd_size, int index_c[], int index_r[])
{
    int i = 0, j = 0, k = 0;
    POINT_3D c = { 0., 0., 0. };

    //c=getCentroid(natom,crd,index_c,index_r);

    for ( i = 0; i < natom; i++ )
    {
        if ( index_r[i] )
        {
            c.X += crd[j][0];
            c.Y += crd[j][1];
            c.Z += crd[j][2];
            j++;
            k++;
        }
        else if ( index_c[i] )
            j++;
    }

    c.X /= ( double ) k;
    c.Y /= ( double ) k;
    c.Z /= ( double ) k;

    for ( i = 0; i < crd_size; i++ )
    {
        crd[i][0] -= c.X;
        crd[i][1] -= c.Y;
        crd[i][2] -= c.Z;
    }
}

//get the covariance matrix
void get_cov_mat(int natom, float **crd, float **ref, int index_c[], int index_r[], float covmat[9])
{
    int i = 0, j = 0, k = 0, l = 0;

    for ( i = 0; i < 3; i++ )
    {
        for ( j = 0; j < 3; j++ )
        {
            covmat[i + 3 * j] = 0.f;
            l = 0;
            for ( k = 0; k < natom; k++ )
            {
                if ( (!index_c[k]) && (!index_r[k]) )
                    continue;
                else if ( (!index_c[k]) && index_r[k] )
                {
                    //                                puts("hello from get_cov_mat");
                    l++;
                }
                else if ( index_c[k] )
                {
                    covmat[i + 3 * j] += crd[l][i] * ref[l][j];
                    l++;
                }
            }
        }
    }
}

//get determinant of a 3x3 matrix stored in col order in an array 1d
double determinant_3x3(float inp[9])
{
    int i, j;
    double det = 0.0;
    double m[3][3] = {
        {0.0 }
    };

    for ( i = 0; i < 3; i++ )
        for ( j = 0; j < 3; j++ )
            m[i][j] = inp[i + 3 * j];

    det = m[0][0]*(m[1][1] * m[2][2] - m[1][2] * m[2][1]);
    det += -m[0][1]*(m[1][0] * m[2][2] - m[1][2] * m[2][0]);
    det += m[0][2]*(m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    return det;
}

void matmul_3x3(float a[9], float b[9], float c[9])
{
    char trans_a = 'N';
    char trans_b = 'N';
    float one = 1.0;
    int dim = 3;

    sgemm_(&trans_a, &trans_b, &dim, &dim, &dim, &one, a, &dim, b, &dim, &one, c, &dim);
}

//singular value decomposition of the covariance matrix a

void svd_f(float a[9], float u[9], float vt[9])
{
    /**
        subroutine SGESVD	(	CHARACTER 	JOBU,
                                CHARACTER 	JOBVT,
                                INTEGER 	M,
                                INTEGER 	N,
                                REAL, dimension( lda, * ) 	A,
                                INTEGER 	LDA,
                                REAL, dimension( * ) 	S,
                                REAL, dimension( ldu, * ) 	U,
                                INTEGER 	LDU,
                                REAL, dimension( ldvt, * ) 	VT,
                                INTEGER 	LDVT,
                                REAL, dimension( * ) 	WORK,
                                INTEGER 	LWORK,
                                INTEGER 	INFO
        )

        extern void sgesvd_(char *jobu, char *jobvt, int *m, int *n, float *a, int *lda, float *s, float *u, int *ldu,
                            float *vt, int *ldvt, float *work, int *lwork, int *info);

     */

    char jobu = 'A', jobvt = 'A';
    int m = 3, n = 3; //dimensions of covmat a
    int lda = 3, ldu = 3, ldvt = 3; //leading dimension : here the same m for lda, etc...
    float s[3] = { 0.f };
    float wk_siz = 0.f;
    int lwork = -1;
    int info = 0;

    //first we get the optimal size of the working array in wk_siz
    sgesvd_(&jobu, &jobvt, &m, &n, a, &lda, s, u, &ldu,
            vt, &ldvt, &wk_siz, &lwork, &info);

    if ( info != 0 )
    {
        printf("LAPACK error for SGESVD : info = %d\nCheck file %s line %d\n", info, __FILE__, __LINE__);
        exit(LAPACK_SGESVD_ERROR);
    }

    lwork = ( int ) wk_siz;
    float *work = NULL;
    work = ( float* ) malloc(lwork * sizeof *work);

    //now we launch the routine ...
    sgesvd_(&jobu, &jobvt, &m, &n, a, &lda, s, u, &ldu,
            vt, &ldvt, work, &lwork, &info);

    if ( info != 0 )
    {
        printf("LAPACK error for SGESVD : info = %d\nCheck file %s line %d\n", info, __FILE__, __LINE__);
        exit(LAPACK_SGESVD_ERROR);
    }

    free(work);
}

//compute and apply the rotation Matrix

void rot_mat(double d, float u[9], float vt[9], float **crd, int crd_size)
{
    float magic_matrix[3 * 3] = {
                                 1.f, 0.f, 0.f,
                                 0.f, 1.f, 0.f,
                                 0.f, 0.f, ( float ) d
    };

    float v[3 * 3] = { 0.f };
    float ut[3 * 3] = { 0.f };

    float tmp1[3 * 3] = { 0.f };

    float rot[3 * 3] = { 0.f };

    int i, j, n;

    for ( i = 0; i < 3; i++ )
    {
        for ( j = 0; j < 3; j++ )
        {
            v[i + 3 * j] = vt[j + 3 * i];
            ut[i + 3 * j] = u[j + 3 * i];
        }
    }

    matmul_3x3(v, magic_matrix, tmp1);
    matmul_3x3(tmp1, ut, rot);

    for ( n = 0; n < crd_size; n++ )
    {
        float work[3] = { 0.f };

        for ( i = 0; i < 3; i++ )
            for ( j = 0; j < 3; j++ )
                work[i] += rot[i + 3 * j] * crd[n][j];

        for ( i = 0; i < 3; i++ )
            crd[n][i] = work[i];
    }
}


