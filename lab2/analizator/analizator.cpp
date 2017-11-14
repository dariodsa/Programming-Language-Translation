#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <string.h>
#include <stack>
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
	bool jesiLiReduciraj()
	{
		return action=='R';
	}
};
struct Redukcija
{
	int brojZnakova;
	string lijeviZnak;
};
vector<Akcija>akcije;
vector<char>znakovi;
char temp[20];
vector<string>ulazniZnakovi;
vector<Redukcija>redukcije;
stack<string>S;
void StogMakniNClanaSaStoga(int broj)
{
	while(broj--)
		S.pop();
	return;	
}
void StogDodajBrojStanje(int broj)
{
	string S=";";
	vector<char>znamenke;
	while(broj)
	{
		znamenke.push_back(broj%10);
		broj/=10;
	}
	reverse(znamenke.begin(),znamenke.end());
	for(int i=0;i<znamenke.size();++i)
		S+=(znamenke[i]+'0');
	S.insert(S);
}
int StogDohvatiBrojStanje()
{
	string vrh=S.top();
	if(vrh[0]!=';'){printf("NEEEEEEEEEEEEEEEEEEEEE!!!!!!!!!!!!!!!!!\n");exit(404);}
	int broj=0;
	for(int i=1;i<vrh.length();++i)
		broj=broj*10+vrh[i]-'0';
	return broj;
}
int nasliReduciraj(string ulaz)
{
	int idRedukcije=-1;
	/*int idZnak=0;
	for(int i=0;i<znakovi.size();++i)
	{
		if(znakovi[i]==ulaz)
			idZnak=i;
	}*/
	int broj=StogDohvatiBrojStanje();
	
	/*
	nadi u tablici akcije
	*/
	
	for(int i=0;i<akcije.size();++i)
	{
		if(akcije[i].stanje==broj && akcije[i].znak==ulaz && akcije[i].jesiLiReduciraj())
			idRedukcije=akcije[i].additionalNumber;
	}
	
	return idRedukcije;
}
int main()
{
	FILE *pfile=fopen("tablica.txt","r");
	//Znakovi koji se mogu naci na stogu, u ulaznoj datoteci je vec ukljucen kraj stoga
	fscanf(pfile,"%d",&n);
	for(int i=0;i<n;++i)
	{
		fscanf(pfile,"%s",temp);
		znakovi.push_back(temp[0]);
	}
	//Ucitavam tablicu Akcija
	fscanf(pfile,"%d%d",&n,&m);
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			Akcija A;
			fscanf(pfile,"%s",temp);
			if(temp[0]=='F' || temp[0]=='A')
			{
				//failed or accepted
				A.action=temp[0];
			}
			else
			{
				fscanf(pfile,"%d",&a);
				A.action=temp[0];
				A.additionalNumber=a;
			}
			A.stanje=i;
			A.znak=znakovi[j];
			akcije.push_back(A);
		}
	}
	//ucitavam tablicu NovoStanje
	fscanf(pfile,"%d%d",&n,&m);
	
	
	fscanf(pfile,"%d",&n);
	for(int i=0;i<n;++i)
	{
		scanf("%s",temp);
		string lijeviZnak=temp;
		scanf("%d",&m);
		
		Redukcija R;
		R.lijeviZnak=temp;
		R.brojZnakova=m;
		
		redukcije.push_back(R);
	}
	/**/
	
	string ulaz="";
	while(getline(cin,ulaz))
	{
		ulazniZnakovi.push_back(ulaz);
	}
	int pos=0;
	while(pos<ulazniZnakovi.size())
	{
		if(nasliNovoStanje(ulazniZnakovi[pos]))
		{
			int novoStanje=novoStanje(ulazniZnakovi[pos]);
			S.push(ulazniZnakovi[pos]);
			StogDodajBrojStanje(novoStanje);//S.push(novoStanje)
			++pos;
		}
		else if(nasliReduciraj(ulazniZnakovi[pos])!=-1)
		{
			Redukcija R=redukcije[nasliReduciraj(ulazniZnakovi[pos])];
			int broj=R.brojZnakova;
			StogMakniNClanaSaStoga(broj*2);
			dodajLijeviZnak(R.lijeviZnak);
		}
	}
	
	
	return 0;
}
