#include <stdio.h>
#include <string.h>
#include "libarram.h"

#define N 	50
#define NC 	3

int main(int argc, char *argv[])
{
	char filename[N];
	int nr, **datos, i, j, k, nud, nuh;
	FILE *fp;
	lt *lt_d, *lt_h;
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
	imprimir_datos(datos, nr, NC);
	lt_d = unicos(datos, nr, NC, 0);
	lt_h = unicos(datos, nr, NC, 1);
	nud = contar_lt(lt_d);
	nuh = contar_lt(lt_h);
	printf("Dias: %d\nHoras: %d\n", nud, nuh);

	liberar_lt(lt_d);
	liberar_lt(lt_h);
	fclose(fp);
	free(datos[0]);
	free(datos);
	return 0;
}