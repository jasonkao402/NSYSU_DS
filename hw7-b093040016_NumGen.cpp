/*
Author¡G °ª¸t³Ç(Jason) B093040016
Date¡G 2020/12/2
Purpose¡G hw7 sorting, gen numbers
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

const int VALMAX = 32767;
int n, tmp;
random_device RD;
mt19937_64 mt(RD() + time(NULL));
uniform_int_distribution<int> mtrand(0, VALMAX);
FILE *genfile;

int main() {
	genfile = fopen("input.txt", "w+");
	scanf("%d", &n);
	printf("generating input.txt with %d values...\n", n);
	fprintf(genfile, "%d\n", n);
	while(n--)
	{
		tmp = mtrand(mt);
		fprintf(genfile, "%d\n", tmp);
	}
	printf("\nfinished.\n");
	fclose(genfile);
    return 0;
}

