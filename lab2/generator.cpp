#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
using namespace std;
struct Produkcija
{
	int lijevaStrana;
	vector<int>desnaStrana;
};
const string EPSILON="$";
const string POCETNI="%";
int a,b,c,d,e,f;
int brojSvihZnakova=0;
string q;
string temp;
vector<string> nezavrsniZnakovi;
vector<string> zavrsniZnakovi;
vector<string> sinkronizacijskiZnakovi;
vector<Produkcija>produkcije;
map<string,int>skupSvihZnakova;
vector<string>popisSvihZnakova;
map<string,bool>sinkronizacijskiZnak;
int zapocinje[1500][1500];
vector<string> split(string S,char spliter)
{
	vector<string> answer;
	string pom="";
	for(int i=0,len=S.length();i<len;++i)
	{
		if(S[i]==spliter)
		{
			if(pom.length()>0)
			{
				answer.push_back(pom);
			}
			pom="";
		}
		else
		{			
			pom+=S[i];
		}
	}
	if(pom.length()>0)
	{
		answer.push_back(pom);
	}
	return answer;
}
bool ideLiTrazeniZnakUEpsilon(int lijeviZnak)
{
	for(int i=0;i<produkcije.size();++i)
	{
		if(produkcije[i].lijevaStrana==lijeviZnak &&
		   produkcije[i].desnaStrana.size()>0 &&
		   produkcije[i].desnaStrana[0]==skupSvihZnakova[EPSILON])
		{
		   return true;
		}
	}
	return false;
}
void postaviTablicuZapocinje()
{
	//kreiraj tablicu zapocinje 
	//prvo idemo sa zapocinjeIzravno
	//uvijek si sam svoj
	for(int i=0;i<brojSvihZnakova;++i)
		zapocinje[i][i]=1;
	for(int i=0;i<produkcije.size();++i)
	{
		if(produkcije[i].desnaStrana.size()==0)continue;
		int pos=0;
		while(pos<produkcije[i].desnaStrana.size())
		{
			int lijevi=produkcije[i].lijevaStrana;
			int desni=produkcije[i].desnaStrana[pos];
			
			zapocinje[lijevi][desni]=1;
			
			if(ideLiTrazeniZnakUEpsilon(desni)==false)
			{
				break;
			}
			++pos;
		}
	}
}
void postaviTablicuZapocinjeZnakom()
{
	int dodanih=1;
	while(dodanih>0)
	{
		dodanih=0;
		for(int j=0;j<brojSvihZnakova;++j)
		{
			for(int k=0;k<brojSvihZnakova;++k)
			{
				for(int l=0;l<brojSvihZnakova;++l)
				{
					if(zapocinje[j][k] && zapocinje[k][l] && !zapocinje[j][l])
					{
						zapocinje[j][l]=1;
						++dodanih;
					}
				}
			}
		}
	}
}
int main()
{
	getline(cin,temp);
	for(int i=3;i<temp.length();++i) q+=temp[i];
	nezavrsniZnakovi=split(q,' ');
	
	q="";
	getline(cin,temp);
	for(int i=3;i<temp.length();++i) q+=temp[i];
	zavrsniZnakovi=split(q,' ');
	
	q="";
	getline(cin,temp);
	for(int i=5;i<temp.length();++i) q+=temp[i];
	sinkronizacijskiZnakovi=split(q,' ');
	
	for(int i=0;i<sinkronizacijskiZnakovi.size();++i)
	{
		sinkronizacijskiZnak[sinkronizacijskiZnakovi[i]]=true;
	}
	
	brojSvihZnakova=0;
	for(int i=0;i<nezavrsniZnakovi.size();++i)
	{
		skupSvihZnakova[nezavrsniZnakovi[i]]=brojSvihZnakova++;
		
		popisSvihZnakova.push_back(nezavrsniZnakovi[i]);
	}
	for(int i=0;i<zavrsniZnakovi.size();++i)
	{
		skupSvihZnakova[zavrsniZnakovi[i]]=brojSvihZnakova++;
		
		popisSvihZnakova.push_back(zavrsniZnakovi[i]);
	}
	skupSvihZnakova[EPSILON]=brojSvihZnakova++; // dodajem dodatno epsilon znak u skup svih znakova
	popisSvihZnakova.push_back(EPSILON);
	
	//dodajem pocetni nezavrsni znak
	skupSvihZnakova[POCETNI]=brojSvihZnakova++;
	popisSvihZnakova.push_back(POCETNI);
	
	//dodaj produkciju da pocetni nezavrsni znak ide u prvi nezavrsni
	Produkcija P;
	P.lijevaStrana=skupSvihZnakova[POCETNI];
	P.desnaStrana.push_back(skupSvihZnakova[nezavrsniZnakovi[0]]);
	produkcije.push_back(P);
	
	string znak;
	while(getline(cin,temp))
	{
		if(temp[0]!=' ')
		{
			//Pocinje novi skup produkcija
			znak=temp;
		}
		else
		{
			//Nova produkcija
			Produkcija P;
			P.lijevaStrana=skupSvihZnakova[znak];
			
			vector<string>desniZnakovi=split(temp,' ');
			for(int i=0;i<desniZnakovi.size();++i)
			{
				P.desnaStrana.push_back(skupSvihZnakova[desniZnakovi[i]]);
			}
			produkcije.push_back(P);
		}
	}
	
	postaviTablicuZapocinje();
	postaviTablicuZapocinjeZnakom();
	
	for(int i=0;i<brojSvihZnakova;++i)
	{
		for(int j=0;j<brojSvihZnakova;++j)
		{
			if(zapocinje[i][j])
			{
				cout<<popisSvihZnakova[i]<<" --> "<<popisSvihZnakova[j]<<endl;
			}
		}
	}
	return 0;
}
