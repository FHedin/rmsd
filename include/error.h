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
