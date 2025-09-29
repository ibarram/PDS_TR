#include <stdio.h>
#include <string.h>
#include "libarram.h"

#define N 	50
#define NC 	3

typedef struct lt{
	int x;
	struct lt *s;
}lt;

int main(int argc, char *argv[])
{
	char filename[N];
	char c;
	int nr, **datos, *pd, i, j, k;
	FILE *fp;
	lt *lt_c, *lt_n, *lt_t;
	lt_c = NULL;
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
	nr = 0;
	do{
		c = fgetc(fp);
		if(c==10)
			nr++;
	}while(c!=EOF);
	printf("NR = %d\n", nr);
	pd = (int*)malloc(nr*NC*sizeof(int));
	if(pd==NULL)
	{
		fclose(fp);
		return 2;
	}
	datos = (int**)malloc(nr*sizeof(int*));
	if(datos==NULL)
	{
		free(pd);
		fclose(fp);
		return 3;
	}
	for(i=0; i<nr; i++)
		datos[i] = pd+i*NC;
	rewind(fp);
	for(i=0; i<nr; i++)
	{
		for(j=0; j<NC; j++)
		{
			fscanf(fp, "%d", datos[i]+j);
			printf("%d\t", datos[i][j]);
		}
		if(lt_c==NULL)
		{
			lt_n = (lt*)malloc(sizeof(lt));
			lt_n->x = datos[i][0];
			lt_n->s=NULL;
			lt_c = lt_n;
		}
		else
		{
			lt_t = lt_c;
			while(lt_t!=NULL)
			{
				if(lt_t->x==datos[i][0])
					break;
				lt_t = lt_t->s;
			}
			if(lt_t==NULL)
			{
				lt_n = (lt*)malloc(sizeof(lt));
				lt_n->x = datos[i][0];
				lt_n->s=lt_c;
				lt_c = lt_n;
			}
		}
		printf("\n");
	}
	lt_t = lt_c;
	while(lt_t!=NULL)
	{
		printf("%d\n", lt_t->x);
		lt_t = lt_t->s;
	}
	fclose(fp);
	free(pd);
	free(datos);
	return 0;
}