#include <stdio.h>
#include <string.h>
#include "libarram.h"

#define N 	50
#define NC 	3
#define NG  4

int main(int argc, char *argv[])
{
	char filename[N];
	int nr, **datos, i, j, k, l, nud, nuh;
	int *x_test, *y_test, *x_training, *y_training;
	double *sx, *sy, **A, **A2, *b, fct;
	FILE *fp;
	poly fx1, dfx1;
	double x, xr, x1, x2, er = 1e-10;
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
	x1 = val_inicial(lt_h);
	x2 = val_final(lt_h);
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
		sx[0] = (nud-1)*nuh;
		for(j=1; j<(2*NG+1); j++)
		{
			for(k=0; k<((nud-1)*nuh); k++)
				sx[j] += pow(x_training[k],j);
		}
		for(j=0; j<(NG+1); j++)
			for(k=0, b[j]=0; k<(nud-1)*nuh; k++)
				b[j]+=(y_training[k]*pow(x_training[k],j));
		lt_s = lt_s->s;
		if(i==0)
		{
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
					fct+=(A[j+1][k]);
				fx1.a[j]=(b[j]-fct)/A[j][j];
			}
			dfx1 = derivar(fx1);
			imprimir_poly(fx1);
			imprimir_poly(dfx1);
			xr = raiz_secante(dfx1, x1, er);
			printf("f(%lf) = %lf\n", x1, evaluar(dfx1, x1));
			printf("f(%lf) = %lf\n", x2, evaluar(dfx1, x2));
			printf("f(%lf) = %lf\n", xr, evaluar(dfx1, xr));
		}
	}
	liberar_lt(lt_d);
	liberar_lt(lt_h);
	fclose(fp);
	free(datos[0]);
	free(datos);
	return 0;
}