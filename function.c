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


