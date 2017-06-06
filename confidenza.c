#include "basic.h"


void CalcolaConfidenza(double media, double devStand, int N, int alpha)
{
	double tstar = idfStudent(N-1, 1-(alpha/2));
	double inf = media - ((tstar*devStand)/sqrt(N-1));/*calcolo estremo inferiore dell'intervallo*/
	double sup = media + ((tstar*devStand)/sqrt(N-1));/*calcolo estremo superiore dell'intervallo*/
	double ampiezzaInt = (sup - inf);
	printf("....................Intervallo di Confidenza....................\n");
	printf("(%f,%f) centrato in %f e di ampiezza %f\n", inf, sup, media, ampiezzaInt);
}

int main(){
	double ThroughputMedio;
	double sommaThroughputMedio;
	FILE *fp;
	int n=0;

	fp = openFile("Throughput.txt","r");

	while(!feof(fp))
	{
		fscanf(fp,"%lf",&dato);
		if( n == 0)
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
