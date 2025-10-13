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

	poly fx1, dfx1;
	double x, xr, x1, x2, er;
	fx1.n = 3;
	fx1.a = (double*)calloc(fx1.n+1, sizeof(double));
	if(fx1.a == NULL)
	{
		liberar_lt(lt_d);
		liberar_lt(lt_h);
		fclose(fp);
		free(datos[0]);
		free(datos);
		return 3;
	}
	fx1.a[3] = 1.0/3;
	fx1.a[2] = 1.0/2;
	fx1.a[1] = -2;
	fx1.a[0] = 5;
	x = 5;
	printf("f(%lf) = %lf\n", x, evaluar(fx1, x));
	dfx1 = derivar(fx1);
	imprimir_poly(fx1);
	imprimir_poly(dfx1);
	x1 = -5;
	x2 = 0;
	er = 1e-10;
	xr = raiz_biseccion(dfx1, x1, x2, er);
	printf("f(%lf) = %lf\n", x1, evaluar(dfx1, x1));
	printf("f(%lf) = %lf\n", x2, evaluar(dfx1, x2));
	printf("f(%lf) = %lf\n", xr, evaluar(dfx1, xr));


	liberar_lt(lt_d);
	liberar_lt(lt_h);
	fclose(fp);
	free(datos[0]);
	free(datos);
	return 0;
}