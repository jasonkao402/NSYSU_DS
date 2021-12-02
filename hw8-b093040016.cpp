/*
Author¡G °ª¸t³Ç(Jason) B093040016
Date¡G2020/12/17
Purpose¡G hw8 huffman encode and decode 
*/
#include<bits/stdc++.h>
#define EPS 1e-7
#define pi acos(-1.0)
#define endl '\n'
#define LL long long
#define ULL unsigned long long
#define UCHAR unsigned char
#define _ps system("pause")
#define IOS ios::sync_with_stdio(0),cin.tie(0);
#define MAX_SIZE 10000005

using namespace std;

struct hufnode {
	hufnode *lc, *rc;
    int freq;
    UCHAR c;
    hufnode(){}
    hufnode(UCHAR _c, int _f)
	{
    	c = _c, freq = _f;
    	lc = rc = NULL;
	}
	hufnode(UCHAR _c, int _f, hufnode *_lc, hufnode *_rc)
	{
    	c = _c, freq = _f;
    	lc = _lc, rc = _rc;
	}
};

struct cmp {
    bool operator() (hufnode *a, hufnode *b)
	{
    	if(a->freq != b->freq)
        	return a->freq > b->freq;
        return a->c > b->c;
    }
};

int i, asc[256], fsiz, bsiz, rtn;
UCHAR buf[MAX_SIZE], nowch;
char orgfname[99], cmpfname[99];
string tmp, binstr, rst;
FILE *fin, *fout;

priority_queue <hufnode*, vector<hufnode*>, cmp> PQ;
map <UCHAR, string> mp;
hufnode huf[512], *root;

void DFS(hufnode *now, string bin){
	if(!now->lc && !now->rc)
	{
		mp[now->c] = bin;
		return;
	}
	DFS(now->lc, bin+"0");
	DFS(now->rc, bin+"1");
}

hufnode *buildhuf(){
	hufnode *mina, *minb;
	while(PQ.size()!=1)
	{
		//take top 2 and merge
		mina = PQ.top();
		PQ.pop();
		minb = PQ.top();
		PQ.pop();
		if(mina->c < minb->c)
			PQ.emplace(new hufnode(mina->c, mina->freq + minb->freq, mina, minb));
		else
			PQ.emplace(new hufnode(minb->c, mina->freq + minb->freq, minb, mina));
	}
	return PQ.top();
}

void str2bin(UCHAR* c, int n, int b)
{
	binstr.clear();
	int i, j;
	char lstbyte[9];
	for(i = 0; i < n; i++)
    {
	    for (j = 7; j >= 0; j--)
	        binstr+=( (c[i] & (1 << j)) ? '1' : '0');
	}
	i = binstr.size(), j = strlen(itoa(c[n], lstbyte, 2));
	printf("%d/%d bits\n", i, b);
    binstr.append(b-(i+j), '0');
	binstr += lstbyte;
}

void compress(char ifname[], char ofname[])
{
	//24 = approximately hint size
	int headerSiz = 24;
	double cRate;
	fin = fopen(ifname, "rb");
	fout = fopen(ofname, "wb");
	if(fin == NULL || fout == NULL)
	{
		printf("Read / write error!\n");
		return;		
	}
	//get file byte size
    fseek(fin, 0, SEEK_END);
    fsiz = ftell(fin);
    rewind(fin);
    if(fsiz > MAX_SIZE)
	{
		printf("File size limit!\n");
		return;
	}	
    fread(buf, 1, fsiz, fin);
    for(i = 0; i < fsiz; i++)
    	asc[buf[i]]++;
	for(i = 0; i < 256; i++)
	{
		if(asc[i])
			PQ.emplace(new hufnode((UCHAR)i, asc[i]));
	}
	//huffman encode
	root = buildhuf();
	DFS(root, "");
	printf("Compressing...\n");
	for(auto it = mp.begin(); it != mp.end(); it++)
	{
		printf("%c = %s\n", it->first, it->second.c_str());
		headerSiz += (it->second.size()+2);
	}
	for(i = 0; i < fsiz; i++)
    	tmp+=mp[buf[i]];
    //print info
	bsiz = tmp.size(), cRate = 100.0 * (headerSiz + bsiz/8 + (bsiz%8 > 0))/fsiz;
	printf("\nFinished.\nRate : %0.2lf%%\nOriginal bytes : %d\n", cRate, fsiz);
	printf("Header bytes : %d\nCompressed bytes : %d\n", headerSiz, bsiz/8 + (bsiz%8 > 0));
	//save hint about original file
	fprintf(fout, "%4d %6d %4d %6.2lf\n", mp.size(), bsiz, fsiz, cRate);
	//print encode table
	for(auto it = mp.begin(); it != mp.end(); it++)
	{
		fprintf(fout, "%c%s\n", it->first, it->second.c_str());
	}
	//turn 8 bits into 1 unsigned char
	for(i = 0; i < bsiz; i += 8)
	{
		nowch = stoul(tmp.substr(i, 8), nullptr, 2);
		fprintf(fout, "%c", nowch);
		//printf("[%s] => '%c'(%d)\n", tmp.substr(i, 8).c_str(), nowch, nowch);
	}
	printf("\nFinished.\n");
    fclose(fin);
    fclose(fout);
}

void decompress(char ifname[], char ofname[])
{
	int mpsiz, btlen, chlen, unlen, i;
	char hkey, hcode[32];
	double tmp;
	string mpkey;
	map <string, char> revmp;
	rst.clear();
	fin = fopen(ifname, "rb");
	fout = fopen(ofname, "wb");
	//get hint from header
	fscanf(fin, "%d %d %d %lf", &mpsiz, &btlen, &unlen, &tmp);
	fgetc(fin);
	chlen = btlen/8 + (btlen%8 > 0);
	//build header given map
	printf("Decompressing...\nSize : %d bytes\n", chlen);
	while(mpsiz--)
    {
    	hkey = fgetc(fin);
    	//printf("%c(%d)\n", hkey, hkey);
		fscanf(fin, "%s", &hcode);
		fgetc(fin);
    	revmp.insert(make_pair(hcode, hkey));
	}
	for(auto it = revmp.begin(); it != revmp.end(); it++)
	{
		printf("%c = %s\n", it->second, it->first.c_str());
	}
	//header finished, read zip data
	fread(buf, 1, chlen, fin);
	//convert to binary
    str2bin(buf, chlen-1, btlen);
    //binary to mapped char
    for(i = 0; i < btlen; i++)
    {
    	mpkey += binstr[i];
    	if(revmp.count(mpkey))
    	{
    		//printf("[%s] => '%c'\n", mpkey.c_str(), revmp.at(mpkey));
    		rst += revmp.at(mpkey);
    		mpkey.clear();
		}
	}
	//output file
	fwrite(rst.data(), 1, unlen, fout);
	printf("\nFinished.\n");
	fclose(fin);
	fclose(fout);
}

int main(int argc, char* argv[]) {
	if(argc != 4){
		printf("argc error.\nUsage:\n  compress : hw8-b093040016.exe -z inFile outFile\ndecompress : hw8-b093040016.exe -u inFile outFile\n");
        return 0;
  	}
  	else{
  		if(argv[1][1] == 'z')
		{
			fin = fopen(argv[2], "rb");
			fout = fopen(argv[3], "rb");
			compress(argv[2], argv[3]);
		}
		else if(argv[1][1] == 'u')
		{
			fin = fopen(argv[2], "rb");
			fout = fopen(argv[3], "rb");
			decompress(argv[2], argv[3]);
		}
		else
        	printf("No such function.\nPlease use -z or -u.\n");
	}
    return 0;
}
