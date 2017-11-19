#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <utility>
#include <string.h>
#include <stack>
#include <map>
#include <vector>
#include <iostream>
using namespace std;
int a,b,c,d,e,f;
int n,m;
map<pair<string,int> ,int>novoStanjeTablica;
struct Akcija
{
	char action;
	int additionalNumber;
	int stanje;
	string znak;
	bool jesiLiReduciraj()
	{
		return action=='R';
	}
	bool jesiLiProdukcija()
	{
		return action=='P';
	}
	bool jesiLiPrihvati()
	{
		return action=='A';
	}
	bool jesiLiOdbaci()
	{
		return action=='F';
	}
};
struct Redukcija
{
	int stanje;
	int brojZnakova;
	string lijeviZnak;
};
vector<Akcija>akcije;
vector<string>znakovi;
vector<string>nezavrsni;
char temp[30];
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
	string pomS=";";
	vector<char>znamenke;
	while(broj)
	{
		znamenke.push_back(broj%10);
		broj/=10;
	}
	reverse(znamenke.begin(),znamenke.end());
	for(int i=0;i<znamenke.size();++i)
		pomS+=(znamenke[i]+'0');
	S.push(pomS);
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
int odrediNovoStanje(string input)
{
	int stanjeNaStogu=StogDohvatiBrojStanje();
	for(int i=0;i<akcije.size();++i)
	{
		if(akcije[i].jesiLiProdukcija() && akcije[i].stanje==stanjeNaStogu && akcije[i].znak==input)
		{
			return akcije[i].additionalNumber;
		}
	}
	return -1;
}
bool nasliNovoStanje(string input)
{
	return odrediNovoStanje(input)!=-1;
}
bool nasaoPrihvati(string input)
{
	int stanje=StogDohvatiBrojStanje();
	for(int i=0;i<akcije.size();++i)
	{
		if(akcije[i].stanje==stanje && akcije[i].znak==input && akcije[i].jesiLiPrihvati())
		{
			return true;
		}
	}
	return false;
}
bool nasaoOdbaci(string input)
{
	int stanje=StogDohvatiBrojStanje();
	for(int i=0;i<akcije.size();++i)
	{
		if(akcije[i].stanje==stanje && akcije[i].znak==input && akcije[i].jesiLiOdbaci())
		{
			return true;
		}
	}
	return false;
}
int main()
{
	FILE *pfile=fopen("tablica.txt","r");
	//Znakovi koji se mogu naci na stogu, u ulaznoj datoteci je vec ukljucen kraj stoga
	
	fscanf(pfile,"%d",&n);
	for(int i=0;i<n;++i)
	{
		fscanf(pfile,"%s",temp);
		string p=temp;
		znakovi.push_back(p);
	}
	
	fscanf(pfile,"%d",&n);
	
	for(int i=0;i<n;++i)
	{
		fscanf(pfile,"%s",temp);
		string p=temp;
		nezavrsni.push_back(p);
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
	printf("Novo stanje\n");
	//ucitavam tablicu NovoStanje
	fscanf(pfile,"%d%d",&n,&m);
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			fscanf(pfile,"%s",temp);
			if(temp[0]=='S')
			{
				fscanf(pfile,"%d",&a);
				cout<<nezavrsni[j]<<","<<i<<"-->"<<a<<endl;
				novoStanjeTablica[make_pair(nezavrsni[j],i)]=a;
			}
		}
	}
	
	fscanf(pfile,"%d",&n);
	for(int i=0;i<n;++i)
	{
		fscanf(pfile,"%s",temp);
		string lijeviZnak=temp;
		fscanf(pfile,"%d",&m);
		
		Redukcija R;
		R.lijeviZnak=temp;
		R.brojZnakova=m;
		R.stanje=i;
		cout<<"redukcija ->"<<R.lijeviZnak<<" + "<<R.brojZnakova<<endl;
		redukcije.push_back(R);
	}
	/**/
	
	string opis="";
	int linija=0;
	while(scanf("%s",temp)!=EOF)
	{
		scanf("%d",&linija);
		getline(cin,opis);
		cout<<temp<<"--> u liniji "<<linija<<". Sa opisom ->"<<opis<<endl;
		string ulaz=temp;
		ulazniZnakovi.push_back(ulaz);
	}
	ulazniZnakovi.push_back("%");
	int pos=0;
	printf("Input prosao.\n");
	S.push(znakovi[znakovi.size()-1]);
	StogDodajBrojStanje(0);
	while(pos<ulazniZnakovi.size())
	{
		if(nasaoPrihvati(ulazniZnakovi[pos]))
		{
			printf("PRIHVACEN NIZ!!!!\n");
			break;
		}
		if(nasaoOdbaci(ulazniZnakovi[pos]))
		{
			printf("%d/%d\n",pos,ulazniZnakovi.size());
			printf("ODBACEN NIZ!!!!\n");
			break;
		}
		if(nasliNovoStanje(ulazniZnakovi[pos]))
		{
			int novoStanje=odrediNovoStanje(ulazniZnakovi[pos]);
			S.push(ulazniZnakovi[pos]);
			printf("Novo stanje %d\n",novoStanje);
			StogDodajBrojStanje(novoStanje);//S.push(novoStanje)
			++pos;
		}
		else if(nasliReduciraj(ulazniZnakovi[pos])!=-1)
		{
			Redukcija R=redukcije[nasliReduciraj(ulazniZnakovi[pos])];
			int broj=R.brojZnakova;
			printf("Redukcija stanje %d\n",nasliReduciraj(ulazniZnakovi[pos]));
			StogMakniNClanaSaStoga(broj*2);
			int brojZaNovoStanje=StogDohvatiBrojStanje();
			cout<<"Broj za novo Stanje"<<brojZaNovoStanje<<endl;
			cout<<"Dodajem "<<R.lijeviZnak<<endl;
			S.push(R.lijeviZnak);//dodajLijeviZnak(R.lijeviZnak);
			//pogledaj u tablicu novoStanje
			cout<<R.lijeviZnak<<" "<<brojZaNovoStanje<<endl;
			int novoStanje=novoStanjeTablica[make_pair(R.lijeviZnak,brojZaNovoStanje)];
			StogDodajBrojStanje(novoStanje);
			cout<<"novo stanje"<<novoStanje<<endl;
		}
	}
	
	
	return 0;
}
