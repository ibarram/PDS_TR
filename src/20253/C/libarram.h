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

int nu_registros(FILE *fp);
int **crear_mat(int nr, int nc);
int lectura_datos(FILE *fp, int **datos, int nr, int nc);
int imprimir_datos(int **datos, int nr, int nc);
lt *unicos(int **datos, int nr, int nc, int ind);
int imprimir_lt(lt *lt_c);
int liberar_lt(lt *lt_c);
int contar_lt(lt *lt_d);

#endif // LIBARRAM_H