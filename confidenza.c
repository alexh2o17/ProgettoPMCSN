#include "basic.h"

#define ALPHA		0.05 

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

}
