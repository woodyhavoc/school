#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <mpi.h>
#include <prand.h>

double runRandExp();
double estPi();

int id;
int nproc;
long long n;
long long count;
long long share;
long long seed;
int interval = 10;
double pi;

const int PI_EST = 1;
const int PROGRESS =2;

int main(int argc, char **argv)
{
    int source;
	int i;
	double* piEst;
	MPI_Status status;
	double startTime, totalTime;

  	if (argc != 2)
	{
		fprintf(stderr,"Usage: %s  <number of elements>\n",argv[0]); 
		exit(1);
	} 
	
	n = atoll(argv[1]);
	
	seed = atoll(argv[2]);
	
	if (argc == 4)
	{
		interval = atoi(argv[3]);
	}

	

	srandom(seed);

  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	share = n/nproc;
	
	if (id == nproc-1)
	{
		share += n%nproc;
	}

	startTime = MPI_Wtime();
	
	signal(SIGALRM, print_state);
	alarm(interval);
  	
	pi = estPi();

	if (id == 0)
	{
  	    piEst = malloc(sizeof(double) * nproc);
		
	    piEst[0] = pi;
	} 
	else
	{
  		double sendPi = pi; 	
		MPI_Send(&sendPi, 1, MPI_DOUBLE, 0, PI_EST, MPI_COMM_WORLD);
	}

	if (id == 0)
	{
		double recvPi;
  		for (i=0; i<nproc-1; i++)
		{
			MPI_Recv(&recvPi, 1, MPI_DOUBLE, MPI_ANY_SOURCE, PI_EST, MPI_COMM_WORLD, &status);
            		source = status.MPI_SOURCE;
            		piEst[source] = recvPi;
    		}
		
  		for (i=1; i<nproc; i++)
		{
			pi += piEst[i];
		}
		
		pi /= nproc;
    		
		totalTime = MPI_Wtime() - startTime;
		printf("The estimated value of pi is %f. Time taken = %lf seconds\n", pi, totalTime);
		free(piEst);
	}

  	MPI_Finalize();
  	exit(0);
}


double runRandExp(int id, long long share)
{
	unrankRand(id * share);
	long long  m;
	double x, y;

	m = 0;
	for (count=0; count<n; count++) {
		x = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		y = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		/* check if (x,y) is within the unit circle */
		if ((x*x + y*y) < 1)  {
			m++;
		}
	}
	return m;


}

double estPi(int id, long long share)
{
	long long m;

	m = runRandExp(id, share);

	return (4 * (double) m / share);
}

void print_state(int signo)
{
        if(id == 0)
	{
		long long tempCount;
		long long total = 0;
		int i;
		for(i = 0; i < nproc-1; i++)
		{
			MPI_Recv(&tempCount, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, PROGRESS, MPI_COMM_WORLD, &status);
			total += tempCount;
		}

		printf("\rSimulation progress: %04.1lf%% done\n", ((double)total/n)*100);
		fflush(NULL);
	}
	else
	{
		long long sendCount = count;
		MPI_Send(&sendCount, 1, MPI_LONG_LONG, 0, PROGRESS, MPI_COMM_WORLD);
	}	
    alarm(interval);
}

