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
