#ifndef RMSD_H
#define	RMSD_H

#ifdef	__cplusplus
extern "C" {
#endif

double rmsd(int natom, float **crd, float **ref, int crd_size, int index_c[], int index_r[]);

POINT_3D getCentroid(int natom, float **crd, int index_c[], int index_r[]);
void center_sys(int natom, float **crd, int crd_size, int index_c[], int index_r[]);
void center_sys2(int natom, float **crd, int crd_size, int index_c[], int index_r[]);

void get_cov_mat(int natom, float **crd, float **ref, int index_c[], int index_r[], float covmat[9]);
double determinant_3x3(float inp[9]);
void matmul_3x3(float a[9], float b[9], float c[9]);

void svd_f(float a[9], float u[9], float vt[9]);
void rot_mat(double d, float u[9], float vt[9], float **crd, int crd_size);

// for the lapack routine sgesvd : singular values decomposition fo floats
extern void sgesvd_(char *jobu, char *jobvt, int *m, int *n, float *a, int *lda, float *s, float *u, int *ldu,
					float *vt, int *ldvt, float *work, int *lwork, int *info);
//for the blas routine dgemm : matrix multiplication for floats
extern void sgemm_(char* transA, char* transB, int *M, int *N, int * K, float *alpha, float *A, int *ldA, float *B, int *ldB, float *beta, float *C, int *ldC);

#ifdef	__cplusplus
}
#endif

#endif	/* RMSD_H */

