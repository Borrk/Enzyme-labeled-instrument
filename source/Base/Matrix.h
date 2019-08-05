
/////////////////////////////////
//
// æÿ’Û¿‡ Matrix.h
//
/////////////////////////////////
//Begin of file Matrix.h

#ifndef __ENM_MATRIX_H__ 
#define __ENM_MATRIX_H__
/* Matrix operation functions and typedefs */
/* Written by Nigel Salt */


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int rows;
  int cols;
  double *block;
} matrix,*matrixptr;

/* Function prototypes */
void smmult(matrixptr,double);
int madd(matrixptr m1,matrixptr m2,matrixptr dm);
int mmult(matrixptr m1,matrixptr m2,matrixptr dm);
int mcopy(matrixptr sm,matrixptr dm);
int mtrans(matrixptr sm,matrixptr dm);
double det(matrixptr m);
int minv(matrixptr sm,matrixptr dm);
int nsolve(int rows,double *data);
int mid(matrixptr);
void mzero(matrixptr);

#ifdef __cplusplus
 }
#endif


#endif 