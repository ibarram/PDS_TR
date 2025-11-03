#ifndef LIBARRAM_H
#define LIBARRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SWAP(a, b) {\
		a^=b;\
		b^=a;\
		a^=b;\
		}

typedef struct lt{
	int x;
	struct lt *s;
}lt;

typedef struct{
	int n;
	double *a;
}poly;

int nu_registros(FILE *fp);
int **crear_mat(int nr, int nc);
int lectura_datos(FILE *fp, int **datos, int nr, int nc);
int imprimir_datos(int **datos, int nr, int nc);
lt *unicos(int **datos, int nr, int nc, int ind);
double val_inicial(lt *lt_h);
double val_final(lt *lt_h);
int imprimir_lt(lt *lt_c);
int liberar_lt(lt *lt_c);
int contar_lt(lt *lt_d);
double evaluar(poly fx, double x);
poly derivar(poly fx);
poly deflacion(poly fx, double xr);
int imprimir_poly(poly fx);
double raiz_biseccion(poly fx, double x1, double x2, double ea);
double raiz_secante(poly fx, double x1, double ea);

#endif // LIBARRAM_H