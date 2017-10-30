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
int a,b,c,d,e,f;
int brojac=0;
string q;
string temp;
vector<string> nezavrsniZnakovi;
vector<string> zavrsniZnakovi;
vector<string> sinkronizacijskiZnakovi;
vector<Produkcija>produkcije;
map<string,int>skupSvihZnakova;
map<string,bool>sinkronizacijskiZnak;
vector<string> split(string S,char spliter)
{
	vector<string> answer;
	string pom="";
	for(int i=0,len=S.length();i<len;++i)
	{
		if(S[i]==spliter)
		{
			if(pom.length()>0)
				answer.push_back(pom);
			pom="";
		}
		else
			pom+=S[i];
	}
	if(pom.length()>0)
		answer.push_back(pom);
	return answer;
}
int main()
{
	getline(cin,temp);
	for(int i=3;i<temp.length();++i) q+=temp[i];
	nezavrsniZnakovi=split(q,' ');
	
	getline(cin,temp);
	for(int i=3;i<temp.length();++i) q+=temp[i];
	zavrsniZnakovi=split(q,' ');
	
	getline(cin,temp);
	for(int i=5;i<temp.length();++i) q+=temp[i];
	sinkronizacijskiZnakovi=split(q,' ');
	
	for(int i=0;i<sinkronizacijskiZnakovi.size();++i)
	{
		sinkronizacijskiZnak[sinkronizacijskiZnakovi[i]]=true;
	}
	
	brojac=0;
	for(int i=0;i<nezavrsniZnakovi.size();++i)
	{
		skupSvihZnakova[nezavrsniZnakovi[i]]=brojac++;
	}
	for(int i=0;i<zavrsniZnakovi.size();++i)
	{
		skupSvihZnakova[zavrsniZnakovi[i]]=brojac++;
	}
	skupSvihZnakova["$"]=brojac++; // dodajem dodatno epsilon znak u skup svih znakova
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
	return 0;
}
