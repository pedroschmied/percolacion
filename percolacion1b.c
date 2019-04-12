#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
/*
#define A 16807
#define Q 127773
#define R 836
#define M 2147483647
*/
int principal_simple (int L, float p, int seed);
int imprimir (int * red, int L);
int poblar(int * red, float p, int L, int seed);
int clasificar (int *red,int L, int *historial);
int etiqueta_falsa(int * red, int L, int *historial, int frag);
int percola(int * red, int L, int frag);
int cluster_size (int * red, int *cluster, int L, int frag);

int main(int argc, char *argv[])
{
	int L;
	sscanf(argv[1], "%d", & L);
	float p, pin, pfin, paso;	
	pin  = 0.69000;//0.470000;
	pfin = 1.000000;//0.679000;
	paso = 0.010000;//0.001000
	int totalsemillas, per, h;
	totalsemillas = 27000;
	FILE * fp;
	fp = fopen ("/home/pedro/Desktop/Universidad/Datos_percolacion/1_b/L16_3.txt", "w");
	float va;
	fprintf (fp,"L = %d\n", L);
	fprintf (fp,"p\t F(p)\n");
	for (p = pin; p < pfin; p = p + paso)
	{
		int *si;
		si = (int*) malloc(sizeof(int) *totalsemillas);
		// 0.47----100
		//   p-----?= p*100/0.47
		va = (p - pin) * 100 / (pfin - pin);
		printf("\n");
		printf ("%f", va);		
		printf ("%%");
		printf("\n");
		for (h = 1; h < (totalsemillas + 1); h++)
		{
			per = principal_simple (L, p, h);
//			printf ("%d,", h);
			if (per)
			{
				*(si + h - 1) = 1;
			}
			else
			{
				*(si + h - 1) = 0;
			}
		}
		//calculo el promedio y la desviación estándar de la # de percolaciones
		int j, z;
		float simean, stdcuadrado;
		simean = 0.00;
		stdcuadrado = 0.00;
		for (j = 1; j < (totalsemillas + 1); j++)
		{
			simean = simean +(float) *(si + j - 1);
		}
		simean = simean / totalsemillas;//promedio final de # de percolaciones

		for (z = 1; z < (totalsemillas + 1); z++)
		{
			stdcuadrado = stdcuadrado + ((float) *(si + z - 1) - simean) * ((float)*(si + z - 1) - simean); 
		}
		stdcuadrado = stdcuadrado / (totalsemillas - 1);//std final de # de percolaciones---> la fórmula es /(N - 1)
		fprintf (fp, "%f,", p);
		fprintf (fp, "%f,", simean);
		fprintf (fp, "%f\n", stdcuadrado);
		free(si);
	}
	fclose(fp);
	return 0;
}

int principal_simple (int L, float p, int seed)
{
	int *red;
	red = (int*) malloc(L*L*sizeof(int));
	poblar(red, p, L, seed);
	int *historial;
	historial = (int*) malloc(L * L*sizeof(int));
 	*historial =  0;
	*(historial + 1) = 1;
	int z;
	for (z = 2; z < L * L; z++)
	{
		*(historial + z) = z;
	}
	int frag, per;
	frag = clasificar (red, L, historial);
	etiqueta_falsa(red, L, historial, frag);

//	printf("\n");
///	imprimir(red, L);	

	free(historial);

	per = percola (red, L, frag);

	int *cluster;
	cluster = (int*) malloc((frag + 1)*sizeof(int));
	*cluster = 0;
	cluster_size (red, cluster, L, frag);

	free(red);
	free(cluster);

/*	printf("\n");
	int g;
	for (g = 0; g < (frag + 1); g++)
	{
		printf ("tamaño de cluster %d : ", g);
		printf ("%d", *(cluster + g));
		printf("\n");
	}
	printf ("frag = %d", frag);
	printf("\n");
*/
	return per;
}

int poblar(int *red, float p, int L, int seed) 
{
	int i;
	float random;
	srand (seed);//time(NULL));
	for(i = 0; i < L*L; i = i + 1)
	{
		random = (float) rand() / (float) RAND_MAX;	
		*(red + i) = 0;
		if(random < p)
		{
			*(red + i) = 1;
		}
	}
return 0;
}

int imprimir (int *red, int L)
{
	int i,j;
	for (i = 0; i < L; i = i + 1)
	{
		for (j = 0; j < L; j++)
		{
			printf ("%d\t", *(red + L * i + j));
		}
	printf ("\n");	
	}
	printf("\n");
	printf("___________");
	printf("\n");
return 0;
}

int clasificar (int *red,int L, int *historial)
{
	int i,j;
	int frag;
	frag = 2;
	int s1, s2;

//primer fila
	if (*(red + 0))
	{
		*(red + 0) = frag;
		frag++;
	}
	for (j = 1; j< L; j++)
	{
		if (*(red + j ))
		{
			if (*(red + j - 1) == 0)
			{		
				*(red + j) = frag++;
			}
			else
			{
				*(red + j) = *(red + j - 1);
			}	
		}
	}
//primer columna
	for (i = 1; i< L; i++)
	{
		if (*(red + L*i ))
		{
			if (*(red + L*(i - 1)) == 0)
			{		
				*(red + L*i) = frag++;
			}
			else
			{
				*(red + L*i) = *(red + L*(i - 1));
			}
		}
	}

// Interior

	for (i = 1; i < L; i++)
	{
		for (j = 1; j < L; j++)
		{
			int minimo, maximo;
	//defino los vecinos: s1 y s2 que son el de arriba e izq. respectivamente
			s1 = *(red + L * (i - 1) + j);
			s2 = *(red + L * i + j - 1);
			if (*(red + L * i + j) )
			{
//vecinos vacíos
				if ((s1 + s2) == 0)
				{
					frag++;
					*(red + L * i + j) = frag;

				}
//1 vecino vacío y 1 lleno
				if ((s1 + s2) > 0 && (s1 * s2) == 0)
				{
					if (s2>0)
					{
						*(red + L * i + j) = s2;

					}
					if (s1>0)
					{
						*(red + L * i + j) = s1;

					}
				}
//vecinos llenos
				if ((s1 * s2) > 0 )
				{
					while(*(historial + s1) < 0)
					{
						s1 = - (*(historial + s1));
					}
					while(*(historial + s2) < 0)
					{
						s2 = - (*(historial + s2));
					}
					
					if (s1 < s2) 
					{
						minimo = s1;
						maximo = s2;
					}

					else
					{
						minimo = s2;
						maximo = s1;
					}							
					*(red + L * i + j) = minimo;
					*(historial + maximo) = - minimo;
					*(historial + minimo) = minimo; 

				}
			}
		}			
	}
	return frag;
}

int etiqueta_falsa(int * red, int L, int *historial, int frag)
{
	int s;
	int i,j;
// Imprimo el historial
/*	printf("Historial");
	printf("\n");
	printf("___________");		
	printf("\n");
	int z;
	for (z = 0; z< (frag+1); z++)
	{
		printf("%d\t",*(historial + z));
	}
	printf("\n");
	printf("___________");
	printf("\n");
*/

	for (s = 2; s < L*L; s++)
	{	
		while (*(historial + s) < 0)
		{
			*(historial + s) = *(historial - *(historial + s));
		}
	}
/*	printf("Historial ya arreglado");
	printf("\n");
	printf("___________");		
	printf("\n");
	for (s = 0; s < (frag+1); s++)
	{
		printf("%d\t", *(historial+s));
	}
	printf("\n");
	printf("___________");
	printf("\n");
*/	for (i = 0; i < L; i++)
	{
		for (j = 0; j < L; j++)
		{
			if ( *(historial + *(red + L * i + j) ) == *(red + L * i + j) )
			{
				*(red + L * i + j) = *(red + L * i + j);
			}
			else
			{
				*(red + L * i + j) = *( historial + *(red + L * i + j) );
			}
		}
	}

	return 0;
}

int percola(int * red, int L, int frag)
{
	int j, z;
	int fila1 [frag + 1];
	int filaL [frag + 1];
	int columna1 [frag + 1];
	int columnaL [frag + 1];
	for (z = 0; z <frag + 1; z++)
	{
		fila1[z] = 0;
		filaL[z] = 0;
		columna1[z] = 0;
		columnaL[z] = 0;
	}	

	for (j = 0; j< L; j++)
	{
		if ( *(red + j) > 0)
		{
			fila1 [*(red + j)] = 1;
		}

		if ( *(red + L * (L - 1) + j) > 0)
		{
			filaL [*(red + L * (L - 1) + j)] = 1;
		}

		if ( *(red + L * j + 0) > 0)
		{
			columna1 [*(red + L * j + 0)] = 1;
		}

		if ( *(red + L * j + L - 1) > 0)
		{
			columnaL [*(red + L * j + L - 1)] = 1;
		}
	}
	int x;
	x = 0;
	for (z = 0; z < frag + 1; z++)
	{
		if ((fila1[z] * filaL[z]) == 1)
		{
/*			printf("\n");
			printf("___");
			printf("Hay percolación (vertical) en cluster n° ");
			printf(" %d", z);
*/			x = 1;
		}
	
		if ( (columna1[z] * columnaL[z]) == 1)
		{
/*			printf("\n");
			printf("___");
			printf("Hay percolación (horizontal) en cluster n° ");
			printf(" %d", z);
			printf("\n");
*/			x = 1;
		}
	}
	return x;
}

int cluster_size (int * red, int * cluster,int L, int frag)
{
	int i, j;
	for (i = 0; i < L; i++)
	{
		for (j = 0; j < L; j++)
		{
			*( cluster + *(red + i * L +  j) ) = *( cluster + *(red + i * L +  j) ) + 1;
		}
	}
	return 0;
}
