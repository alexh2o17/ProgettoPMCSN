#include "basic.h"


int main()
{
	double ThroughputMedio;
	double sommaThroughputMedio;
	double dato;
	FILE *fp;
	int n = 0;

	fp = openFile("Throughput.txt","r");

	while(!feof(fp))
	{
		fscanf(fp, "%lf", &dato);
		if(n == 0)
		{
			ThroughputMedio = dato;
			n++;
		}
		else
		{
			n++;
			sommaThroughputMedio = sommaThroughputMedio + (dato - ThroughputMedio)*(dato - ThroughputMedio)*(n-1.0)/n;
			ThroughputMedio = ThroughputMedio + (dato - ThroughputMedio)/n;

		}
	}
	fclose(fp);
	printf("         **THROUGHPUT MEDIO DEL SISTEMA**         \n");
	CalcolaConfidenza(ThroughputMedio, sqrt(sommaThroughputMedio/n), n, ALPHA);
	printf("ThroughputMedio........................................=%lf\n", ThroughputMedio);
	printf("Deiviazione standard...................................=%lf\n", sqrt(sommaThroughputMedio/n) );
	printf("\n");

}
