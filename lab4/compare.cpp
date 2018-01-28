#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int a, b;
	FILE *pFile1 = fopen(argv[1],"r");
	FILE *pFile2 = fopen(argv[2],"r");
	FILE *pFile3 = fopen(argv[3],"w");
	fscanf(pFile1,"%d",&a);
	fscanf(pFile2,"%d",&b);
	printf("%d %d\n",a,b);
	if(a!=b)
 		fprintf(pFile3,"1");
 	else
 	    fprintf(pFile3,"");
	return 0;
}
