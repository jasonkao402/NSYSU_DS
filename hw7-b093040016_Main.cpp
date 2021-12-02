/*
Author¡G °ª¸t³Ç(Jason) B093040016
Date¡G 2020/12/2
Purpose¡G hw7 sorting, main program
*/
#include<bits/stdc++.h>
#define EPS 1e-7
#define pi acos(-1.0)
#define endl '\n'
#define LL long long
#define ULL unsigned long long
#define _ps system("pause")
#define IOS ios::sync_with_stdio(0),cin.tie(0);

using namespace std;

const int M = 5000000;
const int VALMAX = 32767;
int initarr[M], tgtarr[M];
int L[M], R[M];
int bu[8][M], co[8];
int SIZ;
double tstart, tstop;
FILE *infile, *outfile;

//qsort compare function
int qcmp ( const void *a , const void *b )
{
	return *(int *)a - *(int *)b;
}
//base 8 version of radix sort
void rdxSort(int arr[], int N)
{
	int	dig = 1, cur, i, j, k;
	memset(co, 0, sizeof(co));
	while(dig <= VALMAX)
	{
		for(i = 0; i < N; i++)
		{
			//get LSD, and place it in correct bucket, also update count
			cur = (arr[i] / dig) & 7;
			bu[cur][co[cur]++] = arr[i];
		}
		dig <<= 3;
		for(i = 0, cur = 0; i < 8; i++)
		{
			if(co[i])
			{
				for(j = 0, k = co[i]; j < k; j++)
					arr[cur++] = bu[i][j];
			}
			//reset count
			co[i] = 0;
		}
	}
}

void selSort(int arr[], int N)
{
	int i, j, mini;
	for(i = 0; i < N-1; i++)
	{
		mini = i;
		for(j = i+1; j < N; j++)
		{
			if(arr[j] < arr[mini])
			{
				mini = j;
			}
		}
    	swap(arr[i], arr[mini]);
	}
}

void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1, n2 = r - m;
	
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];
	
	i = 0, j = 0, k = l;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k++] = L[i++];
		}
		else
		{
			arr[k++] = R[j++];
		}
	}
	//take remaining values
	while (i < n1) {
		arr[k++] = L[i++];
	}
	while (j < n2) {
		arr[k++] = R[j++];
	}
}
//iterative merge sort
void mgeSort(int arr[], int N)
{
	int chunk = 1, lft, mid, rit;
	for (chunk = 1; chunk < N; chunk <<= 1)
	{
		for (lft = 0; lft < N - 1; lft += chunk << 1) 
		{
			mid = min(lft + chunk, N) - 1;
			rit = min(lft + (chunk << 1), N) - 1;
			merge(arr, lft, mid, rit);
		}
	}
}
//just for practice, so i did quicksort as well
void qkSort(int arr[], int lft, int rit)
{
	if (lft >= rit)
	{
		return;
	}
	int i, pidx = lft;
	int pval = arr[rit];
	for (i = lft; i < rit; i++)
	{
		if (arr[i] < pval)
		{
			swap(arr[i], arr[pidx]);
			pidx += 1;
		}
	}
	swap(arr[rit], arr[pidx]);
	qkSort(arr, lft, pidx - 1);
	qkSort(arr, pidx + 1, rit);
}
//reset array and start timer
void init(int N)
{
	memcpy(tgtarr, initarr, sizeof(int)*N);
	tstart = clock();
}
//stop timer
double result()
{
	tstop = clock();
	return (tstop - tstart) / CLOCKS_PER_SEC;
}

void fileOut(int N, const char fname[])
{
	outfile = fopen(fname, "w");
	if(!outfile)
	{
		printf("Error when writing %s!\n", fname);
		return;
	}
	for(int i = 0; i < N; i++)
	{
		fprintf(outfile, "%d\n", tgtarr[i]);
	}
	fclose(outfile);
}
int main() {
	infile = fopen("input.txt", "r+");
	if(!infile)
	{
		printf("Error when opening input.txt!\n");
		return 0;
	}
	fscanf(infile, "%d", &SIZ);
	for(int i = 0; i < SIZ; i++)
	{
		fscanf(infile, "%d", &initarr[i]);
	}
	printf("Size : %d\n", SIZ);
	//I left it for hours, will be TLE for sure.
	//big O = O^2, takes too long to finish, so I skipped to save time.
	if(SIZ <= 100000)
	{
		init(SIZ);
		selSort(tgtarr, SIZ);
		printf("Sele. sort : %.3lf s\n", result());
		fileOut(SIZ, "outputA.txt");
	}
	else  printf("Sele. sort : TLE\n");
	
	init(SIZ);
	mgeSort(tgtarr, SIZ);
	printf("Merge sort : %.3lf s\n", result());
	fileOut(SIZ, "outputB.txt");
	
	init(SIZ);
	rdxSort(tgtarr, SIZ);
	printf("Radix sort : %.3lf s\n", result());
	fileOut(SIZ, "outputC.txt");
	
	init(SIZ);
	qkSort(tgtarr, 0, SIZ - 1);
	printf("Quick sort : %.3lf s\n", result());
	
	init(SIZ);
	qsort(tgtarr, SIZ, sizeof(int), qcmp);
	printf("C qsort()  : %.3lf s\n", result());
	fileOut(SIZ, "outputD.txt");
	
	init(SIZ);
	sort(tgtarr, tgtarr + SIZ);
	printf("C++ sort() : %.3lf s\n", result());
	fileOut(SIZ, "outputE.txt");
    return 0;
}

