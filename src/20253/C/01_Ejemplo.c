#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libarram.h"

#define N 	50
#define NC 	3
#define NG  4

int main(int argc, char *argv[])
{
	char filename[N];
	int nr, **datos, i, j, k, l, nud, nuh;
	int *x_test, *y_test, *x_training, *y_training, cmp1;
	double *sx, *sy, **A, **A2, *b, fct;
	double prm[8];
	FILE *fp;
	poly fx1, dfx1, dfx2;
	double x, xr1, xr2, xr3, x1, x2, er = 0, MSE;
	double fxr1, fxr2, fxr3, xth, ds2, ps2, dm, xth1, xth2, rz_fct, vN;
	lt *lt_d, *lt_h, *lt_s;
	if(argc<2)
	{
		printf("Ingrese el nombre del archivo: ");
		scanf("%s", filename);
	}
	else
		strcpy(filename, argv[1]);
	printf("%s\n", filename);
	fp = fopen(filename, "rt");
	if(fp==NULL)
		return 1;
	nr = nu_registros(fp);
	printf("NR = %d\n", nr);
	datos = crear_mat(nr, NC);
	if(datos==NULL)
	{
		fclose(fp);
		return 2;
	}
	lectura_datos(fp, datos, nr, NC);
//	imprimir_datos(datos, nr, NC);
	lt_d = unicos(datos, nr, NC, 0);
	lt_h = unicos(datos, nr, NC, 1);
	x1 = val_final(lt_h);
	nud = contar_lt(lt_d);
	nuh = contar_lt(lt_h);
	printf("Dias: %d\nHoras: %d\n", nud, nuh);
	x_training = (int*)malloc((nud-1)*nuh*sizeof(int));
	if(x_training==NULL)
		return 3;
	y_training = (int*)malloc((nud-1)*nuh*sizeof(int));
	if(y_training==NULL)
	{
		free(x_training);
		return 4;
	}
	x_test = (int*)malloc(nuh*sizeof(int));
	if(x_test==NULL)
	{
		free(x_training);
		free(y_training);
		return 5;
	}
	y_test = (int*)malloc(nuh*sizeof(int));
	if(y_test==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		return 6;
	}
	sx = (double*)calloc(2*NG+1,sizeof(double));
	if(sx==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		return 7;
	}
	A = (double**)malloc((NG+1)*sizeof(double*));
	if(A==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		free(sx);
		return 8;
	}
	A2 = (double**)malloc((NG+1)*sizeof(double*));
	if(A2==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		free(sx);
		free(A);
		return 9;
	}
	A2[0] = (double*)calloc((NG+1)*(NG+1),sizeof(double));
	if(A2[0]==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		free(sx);
		free(A);
		free(A2);
		return 10;
	}
	b = (double*)calloc((NG+1),sizeof(double));
	if(b==NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		free(sx);
		free(A);
		free(A2[0]);
		free(A2);
		return 11;
	}
	for(i=0; i<(NG+1); i++)
		A[i] = sx+i;
	for(i=1; i<(NG+1); i++)
		A2[i] = A2[i-1]+NG+1;
	fx1.n = NG;
	fx1.a = (double*)calloc(fx1.n+1, sizeof(double));
	if(fx1.a == NULL)
	{
		free(x_training);
		free(y_training);
		free(x_test);
		free(y_test);
		free(sx);
		free(A);
		free(A2[0]);
		free(A2);
		free(b);
		return 12;
	}
	lt_s = lt_d;
	printf("%p\t%p\n", lt_s, lt_d);
	printf("%d\n", lt_d->x);
	for(i=0; i<nud; i++)
	{
		for(j=0, k=0, l=0; j<nr; j++)
		{
			if(datos[j][0]==lt_d->x)
			{
				x_test[k] = datos[j][1];
				y_test[k++] = datos[j][2];
			}
			else
			{
				x_training[l] = datos[j][1];
				y_training[l++] = datos[j][2];
			}
		}
		for(j=0; j<nuh; j++)
			printf("%d\t%d\t%d\t%d\n", i, j, x_test[j], y_test[j]);
		sx[0] = (nud-1)*nuh;
		for(j=1; j<(2*NG+1); j++)
		{
			for(k=0; k<((nud-1)*nuh); k++)
				sx[j] += pow(x_training[k],j);
		}
		for(j=0; j<(NG+1); j++)
			for(k=0, b[j]=0; k<(nud-1)*nuh; k++)
				b[j]+=(y_training[k]*pow(x_training[k],j));
		printf("Sel: %d\n", lt_s->x);
		lt_s = lt_s->s;
		printf("Numero de prueba: %d\n", i+1);
		for(j=0; j<NG+1; j++)
			for(k=0; k<NG+1; k++)
				A2[j][k] = A[j][k];
		for(j=1; j<NG+1; j++)
			for(k=j; k<NG+1; k++)
			{
				for(l=0, fct=A2[k][j-1]/A2[j-1][j-1]; l<NG+1; l++)
					A2[k][l]-=(fct*A2[j-1][l]);
				b[k]-=(fct*b[j-1]);
			}
		for(j=NG; j>-1; j--)
		{
			for(k=NG, fct=0; k>j; k--)
				fct+=(A[j][k]*fx1.a[k]);
//					fct+=(A[j+1][k]);
			fx1.a[j]=(b[j]-fct)/A[j][j];
		}
		dfx1 = derivar(fx1);
			imprimir_poly(fx1);
			imprimir_poly(dfx1);
		xr1 = raiz_secante(dfx1, xr1, er);
		dfx2 = deflacion(dfx1, xr1);
		if(dfx2.n==0)
			return 14;
//			imprimir_poly(dfx2);
		if(pow(dfx2.a[1],2)<4*dfx2.a[0]*dfx2.a[2])
		{
			xr2 = 0;
			xr3 = 0;
			xth = xr1;
		}
		else
		{
			xr2 = (-dfx2.a[1]-sqrt(pow(dfx2.a[1],2)-4*dfx2.a[0]*dfx2.a[2]))/(2*dfx2.a[2]);
			xr3 = (-dfx2.a[1]+sqrt(pow(dfx2.a[1],2)-4*dfx2.a[0]*dfx2.a[2]))/(2*dfx2.a[2]);
			fxr1 = evaluar(dfx1, xr1);
			fxr2 = evaluar(dfx1, xr2);
			fxr3 = evaluar(dfx1, xr3);
			if(fxr1<fxr2&&fxr1<fxr3)
				xth = xr1;
			else if(fxr2<fxr1&&fxr2<fxr3)
				xth = xr2;
			else
				xth = xr3;
		}
//			printf("f(%lf) = %lf\n", x1, evaluar(dfx1, x1));
			printf("f(%lf) = %lf\n", xr1, evaluar(dfx1, xr1));
			printf("f(%lf) = %lf\n", xr2, evaluar(dfx1, xr2));
			printf("f(%lf) = %lf\n", xr3, evaluar(dfx1, xr3));
		printf("f(%lf) = %lf\n", xth, evaluar(dfx1, xth));
		// prm[0] -> max(1), prm[4] -> max(2)
		// prm[1] -> m(1), prm[5] -> m(2)
		// prm[2] -> s2(1), parm[6] -> s2(2)
		// prm[3] -> #(1), parm[7] -> #(2)
		prm[0] = -1;
		prm[1] = 0;
		prm[2] = 0;
		prm[3] = 0;
		prm[4] = -1;
		prm[5] = 0;
		prm[6] = 0;
		prm[7] = 0;
		for(j=0; j<(nud-1)*nuh; j++)
		{
			cmp1 = x_training[j]<=xth;
			if(prm[cmp1?0:4]<y_training[j])
				prm[cmp1?0:4] = y_training[j];
			prm[cmp1?1:5]+=(y_training[j]*x_training[j]);
			prm[cmp1?2:6]+=(y_training[j]*pow(x_training[j], 2));
			prm[cmp1?3:7] += y_training[j];
		}
		prm[1]/=prm[3];
		prm[5]/=prm[7];
		prm[2]/=prm[3];
		prm[6]/=prm[7];
		prm[2]-=pow(prm[1],2);
		prm[6]-=pow(prm[5],2);
		printf("1. N(%lf, %lf, %lf)\n2. N(%lf, %lf, %lf)\n", 
			prm[0], prm[1], prm[2], prm[4], prm[5], prm[6]);
		ds2 = prm[2]-prm[6];
		ps2 = prm[2]*prm[6];
		dm = prm[1]-prm[5];
		rz_fct = sqrt(ps2*(pow(dm, 2)-log(pow(prm[0]/prm[4],2*ds2))));
		xth1 = (prm[5]*prm[2]-prm[1]*prm[6]+rz_fct)/ds2;
		xth2 = (prm[5]*prm[2]-prm[1]*prm[6]-rz_fct)/ds2;
		if(xth1>x_training[0]&&xth1<x_training[nuh-1])
			xth = xth1;
		else
			xth = xth2;
		printf("Umbral: %lf\n", xth);
		for(j=0, MSE=0; j<nuh; j++)
		{
			cmp1 = x_test[j]<xth;
			vN = prm[cmp1?0:4]*exp(-pow(x_test[j]-prm[cmp1?1:5],2)/(2*prm[cmp1?2:6]));
			MSE += pow(vN-y_test[j], 2);
		}
		MSE /= nuh;
		MSE = sqrt(MSE);
		printf("MSE: %lf\n", MSE);
	}
	liberar_lt(lt_d);
	liberar_lt(lt_h);
	fclose(fp);
	free(datos[0]);
	free(datos);
	return 0;
}