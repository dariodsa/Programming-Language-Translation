#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>
using namespace std;
int a,b,c,d,e,f;
int n,m;
struct Akcija
{
	char action;
	int additionalNumber;
	int stanje;
	char znak;
	void run()
	{
		if(action=='A')
		{
			//prihvati
			
		}
		if(action=='F')
		{
			//odbaci
			
		}
		if(action=='P')
		{
			//pomakni
			
		}
		if(action=='R')
		{
			//reduciraj
			
		}
		
	}
};
vector<Akcija>akcije;
vector<char>znakovi;
char temp[20];

int main()
{
	//Znakovi koji se mogu naci na stogu, u ulaznoj datoteci je vec ukljucen kraj stoga
	scanf("%d",&n);
	for(int i=0;i<n;++i)
	{
		scanf("%s",temp);
		znakovi.push_back(temp[0]);
	}
	//Ucitavam tablicu Akcija
	scanf("%d%d",&n,&m);
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			Akcija A;
			scanf("%s",temp);
			if(temp[0]=='F' || temp[0]=='A')
			{
				//failed or accepted
				A.action=temp[0];
			}
			else
			{
				scanf("%d",&a);
				A.action=temp[0];
				A.additionalNumber=a;
			}
			A.stanje=i;
			A.znak=znakovi[j];
			akcije.push_back(A);
		}
	}
	//ucitavam tablicu NovoStanje
	scanf("%d%d",&n,&m);
	return 0;
}
