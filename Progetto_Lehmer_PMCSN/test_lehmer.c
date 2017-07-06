#include "basic.h"

int main()
{
    int j;
    int k = 1000;
	int n = 10000;
	int x,i;
	int o[k];
	double u = 0.0; 
	double dato;
	FILE *fp = openFile("TestUniformity.txt","a+");
	PlantSeeds(123456789);
    for(j = 0; j < 256; j++)
    {
	    SelectStream(j);
		double v = 0;
		double pw,v1,v2;
		for(x = 0; x < k; x++)
		{
			o[x] = 0;
		}
			
		printf("\n");

		for (i = 0; i < n; i++)
		{

			u = Random();
			x = (int) (u * k);
			o[x]++;
		}
		for(x = 0; x < k; x++){
			pw = pow(o[x]-(n/k), 2);
			pw = pw/(n/k);
			v+=pw;
		}
	    
	    v1 = idfChisquare(k-1,ALPHA/2);
	    v2 = idfChisquare(k-1,1-(ALPHA/2));

	    printf("v1: %f\n", v1);
	    printf("v2: %f\n", v2);
	    printf("v: %f\n", v);
	    fprintf(fp, "%f\n", v);
	     
		if (v < v1 || v > v2)
		{
			printf("Il generatore è cattivo\n");
		}
    }
	fclose(fp);
	return 0;
}