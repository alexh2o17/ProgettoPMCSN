#include "basic.h"

int main(){

	int x,i;
	int k = 1000;
	int n = 10000;
	int o[k];
	double u = 0.0; // numero random 
	double dato;
	double v = 0;
	double pw,v1,v2;

	FILE *fp;
	fp = openFile("TestUniformity","a+");

	for(x = 0; x < k; x++){
		o[x] = 0;
		//printf("%lf ", o[x]);
	}
		
	printf("\n");

	for (i = 0; i < n; i++){

		u = Random();
		fprintf(fp, "%f\n", u*k);
		x = (int) (u * k);
		o[x]++;
	}
	for(x = 0; x < k; x++){
		pw = pow(o[x]-(n/k), 2);
		pw = pw/(n/k);
		v+=pw;
	}
    
    v1= idfChisquare(k-1,ALPHA/2);
    v2= idfChisquare(k-1,1-(ALPHA/2));

    printf("v1: %f\n", v1);
    printf("v2: %f\n", v2);
    printf("v: %f\n", v);


	if (v < v1 || v > v2)
	{
		printf("generatore di merda");
	}


	fclose(fp);
	/*double max = 0.0;
	double min = 1.0;
	//legge il file e calcola il massimo
	fp = openFile("TestUniformity","r");

	while(!feof(fp)){
		fscanf(fp, "%lf", &dato);
		if(dato > max)
			max = dato;
	} 
	printf("max = %lf\n", max);
	fclose(fp);

	//legge il file e calcola il minimo
	fp = openFile("TestUniformity","r");	
	while(!feof(fp)){
		fscanf(fp, "%lf", &dato);
		if(dato < min)
			min = dato;
	} 
	printf("min = %lf\n", min);
	fclose(fp);
	*/

	
	
	return 0;
}