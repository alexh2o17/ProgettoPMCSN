#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>

#include "rngs.h"
#include "rvms.h"
#include "function.h"
#include "lista.h"

#define START 			0.0					/* tempo inizio simulazione */
#define STOP   			20000.0				/* tempo stop simulazione*/
#define INFINITO 		10000000.0
#define S 				20
#define N				20
#define SETUP 			0.8
#define ALPHA           0.05


