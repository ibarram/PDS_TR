#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "libarram.h"

int nu_registros(FILE *fp)
{
	char c;
	int nr;
	rewind(fp);
	nr = 0;
	do{
		c = fgetc(fp);
		if(c==10)
			nr++;
	}while(c!=EOF);
	return nr;
}

int **crear_mat(int nr, int nc)
{
	int *pd, **datos, i;
	pd = (int*)malloc(nr*nc*sizeof(int));
	if(pd==NULL)
		return NULL;
	datos = (int**)malloc(nr*sizeof(int*));
	if(datos==NULL)
	{
		free(pd);
		return NULL;
	}
	for(i=0; i<nr; i++)
		datos[i] = pd+i*nc;
	return datos;
}

int lectura_datos(FILE *fp, int **datos, int nr, int nc)
{
	int i, j;
	rewind(fp);
	for(i=0; i<nr; i++)
		for(j=0; j<nc; j++)
			fscanf(fp, "%d", datos[i]+j);
	return 0;
}

int imprimir_datos(int **datos, int nr, int nc)
{
	int i, j;
	for(i=0; i<nr; i++)
	{
		for(j=0; j<nc; j++)
			printf("%d\t", datos[i][j]);
		printf("\n");
	}
	return 0;
}

lt *unicos(int **datos, int nr, int nc, int ind)
{
	int i;
	lt *lt_c, *lt_n, *lt_t;
	lt_c = NULL;
	for(i=0; i<nr; i++)
	{
		if(lt_c==NULL)
		{
			lt_n = (lt*)malloc(sizeof(lt));
			lt_n->x = datos[i][ind];
			lt_n->s=NULL;
			lt_c = lt_n;
		}
		else
		{
			lt_t = lt_c;
			while(lt_t!=NULL)
			{
				if(lt_t->x==datos[i][ind])
					break;
				lt_t = lt_t->s;
			}
			if(lt_t==NULL)
			{
				lt_n = (lt*)malloc(sizeof(lt));
				lt_n->x = datos[i][ind];
				lt_n->s=lt_c;
				lt_c = lt_n;
			}
		}
	}
	return lt_c;
}

double val_inicial(lt *lt_h)
{
	return (double)(lt_h->x);
}

double val_final(lt *lt_h)
{
	if(lt_h->s==NULL)
		return (double)(lt_h->x);
	else
		return val_final(lt_h->s);
}

int imprimir_lt(lt *lt_c)
{
	while(lt_c!=NULL)
	{
		printf("%d\n", lt_c->x);
		lt_c = lt_c->s;
	}
	return 0;
}

int contar_lt(lt *lt_d)
{
	return lt_d==NULL?0:1+contar_lt(lt_d->s);
}

int liberar_lt(lt *lt_c)
{
	lt *lt_t;
	while(lt_c!=NULL)
	{
		lt_t=lt_c;
		lt_c = lt_c->s;
		free(lt_t);
	}
	return 0;
}

double evaluar(poly fx, double x)
{
	int i;
	double r;
	for(i=1, r=fx.a[fx.n]; i<=fx.n; i++)
		r = r*x+fx.a[fx.n-i];
	return r;
}

poly derivar(poly fx)
{
	poly dfx;
	int i;
	dfx.n = fx.n-1;
	dfx.a = (double*)calloc(fx.n, sizeof(double));
	if(dfx.a == NULL)
	{
		dfx.n = 0;
		return dfx;
	}
	for(i=0; i<fx.n; i++)
		dfx.a[i] = (i+1)*fx.a[i+1];
	return dfx;
}

double raiz_biseccion(poly fx, double x1, double x2, double ea)
{
	double fx1, fx2, xr, fxr, ea_a;
	fx1 = evaluar(fx, x1);
	fx2 = evaluar(fx, x2);
	if(fx1*fx2>0)
		return 0;
	ea_a = ea+1;
	while(ea_a>ea)
	{
		xr = (x1+x2)/2;
		fxr = evaluar(fx, xr);
		if(fxr*fx2<0)
		{
			ea_a = fabs((xr-x1)/xr);
			x1 = xr;
			fx1 = fxr;
		}
		else if(fx1*fxr<0)
		{
			ea_a = fabs((xr-x2)/xr);
			x2 = xr;
			fx2 = fxr;
		}
		else
			return xr;
	}
	return xr;
}

double raiz_secante(poly fx, double x1, double ea)
{
	double x2;
	double fx1, dfx1, ea_a=100;
	int i;
	poly dfx = derivar(fx);
	i=0;
	while(ea_a>ea)
	{
		fx1 = evaluar(fx, x1);
		dfx1 = evaluar(dfx, x1);
		if(dfx1)
			x2 = x1-fx1/dfx1;
		ea_a = fabs((x2-x1)/x2);
		x1 = x2;
		if(i<10)
			printf("%lf (%lf, %lf, %lf)\n", x1, ea_a, fx1, dfx1);
		i++;
	}
	return x2;
}

int imprimir_poly(poly fx)
{
	int i;
	printf("f(x) = %lfx^%d", fx.a[fx.n], fx.n);
	for(i=fx.n-1; i>0; i--)
		printf("%+lfx^%d", fx.a[i], i);
	printf("%+lf\n", fx.a[0]);
	return 0;
}






