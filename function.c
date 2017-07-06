#include "basic.h"



 double Exponential(double m)
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0
 * ---------------------------------------------------
 */
{
  return (-m * log(1.0 - Random()));
}


  double GetArrival1()			/* generate the next arrival time, with rate 1/2*/
{
  static double arrival = START;

  SelectStream(0);
  arrival += Exponential(0.30);
  return (arrival);
}

  double GetArrival2()			/* generate the next arrival time, with rate 1/2*/
{
  static double arrival = START;

  SelectStream(1);
  arrival += Exponential(0.16);
  return (arrival);
}

   double GetService1_cloud()			/* generate the next service time with rate 2/3*/
{
  SelectStream(2);
  double service = Exponential(4.0);
  return service;
}

   double GetService2_cloud()			/* generate the next service time with rate 2/3*/
{
  SelectStream(3);
  double service = Exponential(4.54);
  return service;
}

   double GetService1_cloudlet()			/* generate the next service time with rate 2/3*/
{
  SelectStream(4);
  double service = Exponential(2.2222);
  return service;
}

   double GetService2_cloudlet()      /* generate the next service time with rate 2/3*/
{
  SelectStream(5);
  double service = Exponential(3.33333);
  return service;
}

double  Min( double a, double b, double c, double d, double e, double f)
{
	double min = a;
	if(b<min) min=b;
	if(c<min) min=c;
	if(d<min) min=d;
    if(e<min) min=e;
    if(f<min) min=f;
	return min;
}

FILE *openFile(char *name, char *mode){
    FILE *f;
    f = fopen(name, mode);
    if(f == NULL) {
        perror("Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }
    return f;
}

void CalcolaConfidenza(double media, double devStand, int n, int alpha, char* file_name)
{
  double tstar = idfStudent(n-1, 1-(alpha/2));
  double inf = media - ((tstar*devStand)/sqrt(n-1));/*calcolo estremo inferiore dell'intervallo*/
  double sup = media + ((tstar*devStand)/sqrt(n-1));/*calcolo estremo superiore dell'intervallo*/
  double ampiezzaInt = (sup - inf);
  printf("....................Intervallo di Confidenza....................\n");
  printf("(%f,%f) centrato in %f e di ampiezza %f\n", inf, sup, media, ampiezzaInt);

  FILE *fp_index = NULL;

  if(strcmp(file_name,"Throughput") == 0) {
      fp_index = openFile("Confidenzathroughput.txt", "a+");
  }
  else if(strcmp(file_name,"T_Risposta") == 0) {
      fp_index = openFile("Confidenza_t_sistema", "a+");
  }
  else if(strcmp(file_name,"Utilizzazione") ==0)
    fp_index = openFile("Confidenza_utilizzazione", "a+");

    fprintf(fp_index,"%f %f\n", inf, sup);
    fclose(fp_index);



}

void LetturaConfidenza(char* file_name)
{
  double dato;
  double index_medio = 0;
  double somma_index_medio = 0;
  int n = 0;
  FILE *fp = openFile(file_name,"r");

  while(!feof(fp))
  {
    fscanf(fp, "%lf", &dato);
    if(n == 0)
    {
      index_medio = dato;
      n++;
    }
    else
    {
      n++;
      somma_index_medio = somma_index_medio + (dato - index_medio)*(dato - index_medio) * (n - 1.0)/n;
      index_medio = index_medio + (dato - index_medio)/n;

    }
  }
  fclose(fp);
//  printf("         **%s medio del sistema**         \n", file_name);
  CalcolaConfidenza(index_medio, sqrt(somma_index_medio/n), n, ALPHA, file_name);
//  printf("%s medio........................................= %lf\n", file_name, index_medio);
//  printf("Deviazione standard...................................= %lf\n", sqrt(somma_index_medio/n));
//  printf("\n");




    
}
