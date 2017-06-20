#include "stdlib.h"
#include "stdio.h"
#include <math.h>

int Euclide(int a, int b) 
{
    int r = a % b;
	while (r > 0)
	{
		a = b;
		b = r;
		r = a % b;
	};
	return b;
}

int IsPrime(int number)
{
    int i;
    for (i=2; i<number; i++)
    {
        if (number % i == 0 && i != number) 
        	return 0;
    }
    return 1;
}

int main()
{
	int m = 2147483647;
	int a = 7;
	int i = 1;
	int x = a;
	int t = 0;
	int fatt1 = 0;
	int fatt1_1 = 0;
	int fatt1_2 = 0;
	int fatt2 = 0;
	while(x != 1)
	{
		if(Euclide(i, m - 1) == 1 && ((m % x) < (m/x)))
			printf("Moltiplicatore a pieno periodo e modulo combatibile: %d\n", x);
		i++;
		if((m % x) < (m/x) && IsPrime(m) == 1)
		{
			fatt1_2 = (m/x);
			fatt1_1 = (x % fatt1_2);
			fatt1 = a * fatt1_1;
			fatt2 = (m % x) * (x/(m/x));
			t = fatt1 - fatt2;
			if(t > 0)
				x = t;
			else
				x = t + m;
		}
		else
			x = (a * x) % m;
	}
	printf("Ho finito\n");
	return EXIT_SUCCESS;
}