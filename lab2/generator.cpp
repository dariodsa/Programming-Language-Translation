
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <set>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;
vector<string>popisSvihZnakova;
struct Prijelaz
{
	bool epsilon;
	int znakZaPrijelaz;
	int izStanja;
	int uStanje;
	Prijelaz(int _izStanja,int _uStanje,bool _epsilon=false)
	{
		izStanja=_izStanja;
		uStanje=_uStanje;
		epsilon=_epsilon;
	}
	Prijelaz(int _izStanja,int _uStanje,int _znakZaPrijelaz,bool _epsilon=false)
	{
		izStanja=_izStanja;
		uStanje=_uStanje;
		znakZaPrijelaz=_znakZaPrijelaz;
		epsilon=false;
	}
	
};
struct Stanje
{
	int lijevaStrana;
	vector<int>desnaStrana;
	int tocka;
	vector<int>skup;	
	int id;
	int poredakProdukcije;
	Stanje(int _id)
	{
		id=_id;
	}
	bool operator==(Stanje A)
	{
		return id==A.id;
	}
	bool operator<(Stanje A)
	{
		return id<A.id;
	}
	bool operator>(Stanje A)
	{
		return id>A.id;
	}
	bool potpunaStavka()
	{
		return tocka==desnaStrana.size();
	}
	bool istiKao(Stanje S)
	{
		if(lijevaStrana!=S.lijevaStrana)return false;
		if(tocka!=S.tocka)return false;
		if(desnaStrana.size()!=S.desnaStrana.size())return false;
		if(skup.size()!=S.skup.size())return false;
		
		vector<int>desnaStrana1;
		desnaStrana1.insert(desnaStrana1.end(), desnaStrana.begin(), desnaStrana.end());
		vector<int>desnaStrana2;
		desnaStrana2.insert(desnaStrana2.end(), S.desnaStrana.begin(), S.desnaStrana.end());
		
		vector<int>skup1=skup;
		skup1.insert(skup1.end(), skup.begin(), skup.end());
		vector<int>skup2=S.skup;
		skup2.insert(skup2.end(), S.skup.begin(), S.skup.end());
		
		sort(desnaStrana1.begin(),desnaStrana1.end());
		sort(desnaStrana2.begin(),desnaStrana2.end());
		sort(skup1.begin(),skup1.end());
		sort(skup2.begin(),skup2.end());
		for(int i=0;i<desnaStrana1.size();++i)
			if(desnaStrana1[i]!=desnaStrana2[i])return false;
		for(int i=0;i<skup2.size();++i)
			if(skup1[i]!=skup2[i])return false;
		return true;
	}
	void podesiSkup(vector<int> niz)
	{
		skup.clear();
		for(int i=0;i<niz.size();++i)
		{
			bool imaLiGa=false;
			for(int j=0;j<skup.size();++j)
			{
				if(skup[j]==niz[i])
					imaLiGa=true;
			}
			if(!imaLiGa)
				skup.push_back(niz[i]);
		}
	}
	void dodajSkup(vector<int> niz)
	{
		for(int i=0;i<niz.size();++i)
		{
			bool imaLiGa=false;
			for(int j=0;j<skup.size();++j)
			{
				if(skup[j]==niz[i])
					imaLiGa=true;
			}
			if(!imaLiGa)
				skup.push_back(niz[i]);
		}
	}
	int znakIzaTocke()
	{
		if(tocka+1>desnaStrana.size())
		   return -567; //ne postoji takav znak
		else
		{
			return desnaStrana[tocka];
		}
	}
	int znakIza2Tocke()
	{
		if(tocka+2>desnaStrana.size())
		   return -577; //ne postoji takav znak
		else
		{
			return desnaStrana[tocka+1];
		}
	}
	int znakIspredTocke()
	{
		if(tocka==0)
		 return -698; //ne postoji takav znak
		else
		 return desnaStrana[tocka-1];
	}
	void dodajDesnuStranu(vector<int> niz)
	{
		desnaStrana.clear();
		for(int i=0;i<niz.size();++i)
		{
			desnaStrana.push_back(niz[i]);
		}
	}
	void ispis()
	{
		//printf("mirkec");
		cout<<" "<<popisSvihZnakova[lijevaStrana]<<" --> ";
		for(int j=0;j<desnaStrana.size();++j)
		{
			if(tocka==j)cout<<"*";
			cout<<popisSvihZnakova[desnaStrana[j]];
		}
		if(tocka==desnaStrana.size())cout<<"*";
		cout<<" {";
		for(int j=0;j<skup.size();++j)
		{
			cout<<popisSvihZnakova[skup[j]]<<",";
		}
		cout<<"}"<<"  "<<id<<endl;
	}
	bool imamoLiIstiSkup(vector<int>drugiSkup)
	{
		if(skup.size()!=drugiSkup.size())return false;
		
		sort(drugiSkup.begin(),drugiSkup.end());
		sort(skup.begin(),skup.end());
		
		for(int i=0;i<skup.size();++i)
			if(drugiSkup[i]!=skup[i])
				return false;
		return true;
	}
};
bool cmp(Stanje A,Stanje B)
{
	return A.id<B.id;
}
struct Produkcija
{
	int lijevaStrana;
	vector<int>desnaStrana;
};

struct DKAStanje
{
	int idStanja;
	vector<Stanje>stanja;
	void postaviStanja(vector<Stanje> niz)
	{
		for(int i=0;i<niz.size();++i)
		{
			stanja.push_back(niz[i]);
		}
	}
	bool istaStanja(vector<Stanje>niz)
	{
		bool isti=true;
		sort(niz.begin(),niz.end(),cmp);
		sort(stanja.begin(),stanja.end(),cmp);
		for(int i=0;i<niz.size();++i)
		{
			if(niz[i].id!=stanja[i].id)
				return false;
		}
		return true;
	}
};


const string EPSILON="$";
const string POCETNI="<%>";
const string POCETNI2="%";
int brojZaEpsilon=0;
int a,b,c,d,e,f;
int brojSvihZnakova=0;
string q;
string temp;
vector<string> nezavrsniZnakovi;
vector<string> zavrsniZnakovi;
vector<string> sinkronizacijskiZnakovi;
vector<Produkcija>produkcije;
vector<Stanje>stanja;
vector<Stanje>stanja2;
vector<Prijelaz>prijelazi;
vector<Prijelaz>dkaPrijelazi;
vector<DKAStanje>dkaStanja;

map<string,int>skupSvihZnakova;
map<string,bool>sinkronizacijskiZnak;
int zapocinje[1500][1500];
vector<int>zapocinjeZnakom[5000];
bool bio[1500];
bool bio2[1500];
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
void postaviIhUVektorZapocinjeZnakom()
{
	for(int i=0;i<brojSvihZnakova;++i)
	{
		for(int j=0;j<brojSvihZnakova;++j)
		{
			if(zapocinje[i][j])
			{
				if(popisSvihZnakova[j][0]!='<')
				{
					//radi se o zavrsnom znaku
					zapocinjeZnakom[i].push_back(j);
				}
			}
		}
	}
}
bool jedinstvenoStanje(Stanje S,vector<Stanje>temp)
{
	for(int i=0;i<temp.size();++i)
	{
		if(S.istiKao(temp[i]))
			return false;
	}
	for(int i=0;i<stanja.size();++i)
	{
		if(S.istiKao(stanja[i]))
			return false;
	}
	return true;
}
int jedinstvenoDKAStanje(vector<Stanje>niz)
{
	for(int i=0;i<dkaStanja.size();++i)
	{
		if(dkaStanja[i].istaStanja(niz))return i;
	}
	return -1;
}
void bfs(int pos,int parent,int znak,vector<int>dodaniZnakovi)
{
	vector<Stanje>istaStanja;
	vector<Stanje>mogucaNovaStanja;
	vector<int>prijelazZnak;
	
	queue<int>Q;
	memset(bio,false, sizeof bio);
	memset(bio2,false, sizeof bio2);
	Q.push(pos);
	for(int i=0;i<dodaniZnakovi.size();++i)
	{
		Q.push(dodaniZnakovi[i]);
		bio[dodaniZnakovi[i]]=true;
		istaStanja.push_back(stanja[dodaniZnakovi[i]]);
	}
	bio[pos]=true;
	int poz=pos;
	istaStanja.push_back(stanja[pos]);
	//if(poz==0) {cout<<"bfs "<<endl;stanja[poz].ispis();}
	while(!Q.empty())
	{
		int pos=Q.front();
		Q.pop();
		for(int i=0;i<prijelazi.size();++i)
		{
			if(prijelazi[i].izStanja!=pos)continue;
			if(prijelazi[i].epsilon && bio[prijelazi[i].uStanje]==false)
			{
				//cout<<"povezan sa "<<endl;
				stanja[prijelazi[i].uStanje].ispis();
				bio[prijelazi[i].uStanje]=true;
				
				istaStanja.push_back(stanja[prijelazi[i].uStanje]);
				Q.push(prijelazi[i].uStanje);
			}
			else if(prijelazi[i].epsilon==false && bio[prijelazi[i].uStanje]==false)
			{
				mogucaNovaStanja.push_back(stanja[prijelazi[i].uStanje]);
				prijelazZnak.push_back(prijelazi[i].znakZaPrijelaz);
			}
			if(prijelazi[i].epsilon==false && bio[prijelazi[i].uStanje]==true)
			{
				mogucaNovaStanja.push_back(stanja[prijelazi[i].uStanje]);
				prijelazZnak.push_back(prijelazi[i].znakZaPrijelaz);
				cout<<"Mirko jede jaja."<<endl;
			}
		}
	}
	/*
	 * Dodajem stanja povezana epsilon prijelazom u isto stanje u DKA automatu
	*/
	DKAStanje S;
	S.idStanja=dkaStanja.size();
	S.postaviStanja(istaStanja);
	if(jedinstvenoDKAStanje(istaStanja)==-1)
	{	
		//printf("Broj istih stanja %d\n",istaStanja.size());
		dkaStanja.push_back(S);
		if(parent!=-1)
		{
			bool find=false;
			for(int i=0;i<dkaPrijelazi.size();++i)
			{
				if(dkaPrijelazi[i].izStanja==parent && dkaPrijelazi[i].uStanje==dkaStanja.size()-1)find=true;
			}
			
			if(!find)
			{
				Prijelaz P=Prijelaz(parent,dkaStanja.size()-1,znak,false);
				//cout<<parent<<">-<"<<dkaStanja.size()-1<<" "<< popisSvihZnakova[znak]<<endl;
				dkaPrijelazi.push_back(P);
			}
		}
		int parentId=dkaStanja.size()-1;
		
		for(int j=0;j<mogucaNovaStanja.size();++j)
		{
			vector<int>dodani;
			for(int k=0;k<mogucaNovaStanja.size();++k)
			{
				if(k==j)continue;
				if(prijelazZnak[j]==prijelazZnak[k])
				{
					dodani.push_back(mogucaNovaStanja[k].id);
				}
			}
			bfs(mogucaNovaStanja[j].id,parentId,prijelazZnak[j],dodani);
		}
	}
	else
	{
		int ponovljeno=jedinstvenoDKAStanje(istaStanja);
		bool find=false;
		for(int i=0;i<dkaPrijelazi.size();++i)
		{
			if(dkaPrijelazi[i].izStanja==parent && dkaPrijelazi[i].uStanje==ponovljeno)find=true;
		}
		if(!find)
		{
			Prijelaz P=Prijelaz(parent,ponovljeno,znak,false);
			
			dkaPrijelazi.push_back(P);
		}
	}
	return;
}
bool isOver(int stanje,string znak)
{
	for(int i=0;i<dkaStanja[stanje].stanja.size();++i)
	{
		if(dkaStanja[stanje].stanja[i].potpunaStavka() && dkaStanja[stanje].stanja[i].lijevaStrana==skupSvihZnakova[nezavrsniZnakovi[nezavrsniZnakovi.size()-1]])
		{
			for(int j=0;j<dkaStanja[stanje].stanja[i].skup.size();++j)
			{
				if(dkaStanja[stanje].stanja[i].skup[j]==skupSvihZnakova[znak])
					return true;
			}
		}
	}
	return false;
}
int probajSePomaknuti(int stanje,string znak)
{
	for(int j=0;j<dkaPrijelazi.size();++j)
	{
				if(dkaPrijelazi[j].izStanja==stanje && dkaPrijelazi[j].znakZaPrijelaz==skupSvihZnakova[znak])
				return dkaPrijelazi[j].uStanje;
	}
	return -1;
}
int probajSeReducirati(int stanje,string znak)
{
	DKAStanje D;
	for(int i=0;i<dkaStanja[stanje].stanja.size();++i)
	{
		if(dkaStanja[stanje].stanja[i].potpunaStavka())
		{
			
			for(int j=0;j<dkaStanja[stanje].stanja[i].skup.size();++j)
			{
				if(dkaStanja[stanje].stanja[i].skup[j]==skupSvihZnakova[znak])
					return dkaStanja[stanje].stanja[i].poredakProdukcije;
			}
		}
	}
	return -1;
}
void tablicaNovoStanje(int stanje,string input)
{
	for(int i=0;i<dkaPrijelazi.size();++i)
	{
		if(dkaPrijelazi[i].izStanja==stanje && dkaPrijelazi[i].znakZaPrijelaz==skupSvihZnakova[input])
		{
			cout<<"S "<<dkaPrijelazi[i].uStanje<<" ";
			return;
		}
		
	}
	cout<<"- ";
}
void kreirajDka()
{
	DKAStanje D;
	D.idStanja=0;
	vector<Stanje>istaStanja;
	istaStanja.push_back(stanja[0]);
	set<Stanje>Sa;
	/*while
	
	for(int i=0;i<dkaStanja.size();++i)
	{
		
	}*/
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
	brojZaEpsilon=brojSvihZnakova;
	skupSvihZnakova[EPSILON]=brojSvihZnakova++; // dodajem dodatno epsilon znak u skup svih znakova
	popisSvihZnakova.push_back(EPSILON);
	
	//znak za kraj niza
	skupSvihZnakova[POCETNI2]=brojSvihZnakova++;
	popisSvihZnakova.push_back(POCETNI2);
	zavrsniZnakovi.push_back(POCETNI2);
	
	
	//dodajem pocetni nezavrsni znak
	skupSvihZnakova[POCETNI]=brojSvihZnakova++;
	popisSvihZnakova.push_back(POCETNI);
	
	nezavrsniZnakovi.push_back(POCETNI);
	
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
	
	postaviIhUVektorZapocinjeZnakom();
	Stanje S=Stanje(0);
	S.tocka=0;
	S.lijevaStrana=brojSvihZnakova-1;
	S.desnaStrana.push_back(skupSvihZnakova[nezavrsniZnakovi[0]]);
	S.skup.push_back(skupSvihZnakova[POCETNI2]);
	stanja.push_back(S);
	Stanje S1=Stanje(1);
	S1.tocka=1;
	S1.lijevaStrana=brojSvihZnakova-1;
	S1.skup.push_back(skupSvihZnakova[POCETNI2]);
	S1.desnaStrana.push_back(skupSvihZnakova[nezavrsniZnakovi[0]]);
	stanja.push_back(S1);
	
	//kreiraj Stanja sa tockama
	for(int i=1;i<produkcije.size();++i)
	{
		if(produkcije[i].desnaStrana.size()==1 
		  && produkcije[i].desnaStrana[0]==brojZaEpsilon)
		{
			  	Stanje S=Stanje(stanja.size());
				S.tocka=1;
				S.poredakProdukcije=i;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				S.skup.push_back(brojSvihZnakova-1);
				stanja.push_back(S);
				
		}
		else  
			for(int j=0;j<produkcije[i].desnaStrana.size()+1;++j)
			{
				Stanje S=Stanje(stanja.size());
				S.tocka=j;
				S.poredakProdukcije=i;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				stanja.push_back(S);
				
			}
	}	
	printf("Stanja %d\n",stanja.size());
	/*for(int i=0;i<stanja.size();++i)
	{
		if(stanja[i].lijevaStrana==skupSvihZnakova[nezavrsniZnakovi[0]] && stanja[i].tocka==0)
		{
			Prijelaz pocetni=Prijelaz(0,i,true);
			prijelazi.push_back(pocetni);
		}
		if(stanja[i].lijevaStrana==skupSvihZnakova[nezavrsniZnakovi[0]])
		{
			stanja[i].skup.push_back(brojSvihZnakova-1);
		}
	}*/
	
	
	//kreni u dodavanje skupova u stanja i kreiranje mogucih prijelaza
	
	for(int i=0;i<stanja.size();++i)
	{
		//if(i==29)break;
		
		vector<Stanje>tempStanja;
		for(int j=0;j<stanja.size();++j)
		{
			//imamo 2 slucaja
			//cout<<stanja[i].znakIzaTocke()<<" "<<stanja[j].lijevaStrana<<" "<<stanja[j].tocka<<endl;
			if(stanja[i].lijevaStrana == stanja[j].lijevaStrana &&
			   stanja[i].znakIzaTocke() == stanja[j].znakIspredTocke())
			{
				if(stanja[j].skup.size()==0)
				{
				 	stanja[j].podesiSkup(stanja[i].skup);
				 	Prijelaz P=Prijelaz(i,j,stanja[i].znakIzaTocke(),false);
					stanja[i].ispis();
					stanja[j].ispis();
					cout<<"----->  "<<popisSvihZnakova[stanja[i].znakIzaTocke()]<<endl;
					prijelazi.push_back(P);
				}
				else
				{
					if(stanja[j].imamoLiIstiSkup(stanja[i].skup))
					{
						Prijelaz P=Prijelaz(i,j,stanja[i].znakIzaTocke(),false);
						stanja[i].ispis();
						stanja[j].ispis();
						cout<<"----->  "<<popisSvihZnakova[stanja[i].znakIzaTocke()]<<endl;
						prijelazi.push_back(P);
					}
					//kreiraj novo stanje koje ce se razlikovati po skupu, a po ostalome ce biti jednako
					else
					{
						Stanje S=Stanje(-1);
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						S.podesiSkup(stanja[i].skup);
						S.poredakProdukcije=stanja[j].poredakProdukcije;
						if(jedinstvenoStanje(S,tempStanja))
						{
							Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),stanja[i].znakIzaTocke(),false);
							prijelazi.push_back(P);
							tempStanja.push_back(S);
						}
						else if(stanja[j].istiKao(S))
						{
							Prijelaz P=Prijelaz(i,j,stanja[i].znakIzaTocke(),false);
							stanja[i].ispis();
							stanja[j].ispis();
							cout<<"----->  "<<popisSvihZnakova[stanja[i].znakIzaTocke()]<<endl;
							prijelazi.push_back(P);	
						}
					}
				}
				
			}
			else if((stanja[i].znakIzaTocke() == stanja[j].lijevaStrana &&
			        stanja[j].tocka==0) ||
			        (stanja[i].znakIzaTocke() == stanja[j].lijevaStrana &&
			        stanja[j].tocka==1 && stanja[j].desnaStrana[0]==brojZaEpsilon)
			        )
			{
				//printf("USAO %d %d\n",i,j);
					
				int znak=stanja[i].znakIza2Tocke();
				/*stanja[i].ispis();
				stanja[j].ispis();
				cout<<stanja[j].skup.size()<<"+++++++"<<endl;*/
				if(znak<0)
				{
					
					
					if(stanja[j].skup.size()==0)
					{
						stanja[j].podesiSkup(stanja[i].skup);
						Prijelaz P=Prijelaz(i,j,true);
						
						stanja[i].ispis();
						stanja[j].ispis();
						cout<<"------"<<endl;
						prijelazi.push_back(P);	
					}
					else
					{
						Stanje S=Stanje(-1);
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						S.poredakProdukcije=stanja[j].poredakProdukcije;
						S.podesiSkup(stanja[i].skup);
						
						
						
						if(jedinstvenoStanje(S,tempStanja))
						{
							Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),true);
							stanja[i].ispis();
							S.ispis();
							cout<<"------"<<endl;
							prijelazi.push_back(P);
							tempStanja.push_back(S);
						}
						else if(stanja[j].istiKao(S))
						{
						
							stanja[i].ispis();
							stanja[j].ispis();
							cout<<"--+++++--"<<endl;
							Prijelaz P=Prijelaz(i,j,true);
							prijelazi.push_back(P);	
						}
					}
				}
				else
				{
					//postoji znak beta
					
					if(stanja[j].skup.size()==0)
					{
						stanja[j].podesiSkup(zapocinjeZnakom[znak]);
						if(ideLiTrazeniZnakUEpsilon(znak))
							stanja[j].dodajSkup(stanja[i].skup);
						Prijelaz P=Prijelaz(i,j,true);
						
						stanja[i].ispis();
						stanja[j].ispis();
						cout<<"------"<<endl;
						
						prijelazi.push_back(P);
					}
					else
					{
						Stanje S=Stanje(-1);
						S.poredakProdukcije=stanja[j].poredakProdukcije;
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						
						S.podesiSkup(zapocinjeZnakom[znak]);
						if(ideLiTrazeniZnakUEpsilon(znak))
							S.dodajSkup(stanja[i].skup);
						
						if(stanja[j].imamoLiIstiSkup(S.skup))
						{
							Prijelaz P=Prijelaz(i,j,true);
							
							stanja[i].ispis();
							stanja[j].ispis();
							cout<<"------"<<endl;
							
							prijelazi.push_back(P);
						}
						else if(jedinstvenoStanje(S,tempStanja))
						{
							Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),true);
							
							stanja[i].ispis();
							S.ispis();
							cout<<"------"<<endl;
							
							prijelazi.push_back(P);
							tempStanja.push_back(S);
						}
					}
				}
			}
			
		}
		for(int k=0;k<tempStanja.size();++k)
		{
			tempStanja[k].id=stanja.size();
			stanja.push_back(tempStanja[k]);
		}
	}
	int ep=0;
	for(int i=0;i<prijelazi.size();++i)if(prijelazi[i].epsilon)++ep;
	printf("Prijelazi %d -> %d\n",prijelazi.size(),ep);
	for(int i=0;i<stanja.size();++i)
	{
		stanja[i].ispis();
	}
	
	for(int i=0;i<prijelazi.size();++i)
	{
		/*string znak=prijelazi[i].epsilon==false?popisSvihZnakova[prijelazi[i].znakZaPrijelaz]:"oko";
		cout<<"Znak za prijelaz "<<znak<<endl;
		stanja[prijelazi[i].izStanja].ispis();
		stanja[prijelazi[i].uStanje].ispis();*/
	}
	
	vector<int>prazan;
	bfs(0,-1,-1,prazan);
	//kreirajDka();
	
	printf("E-NKA stanja %d\n",stanja.size());
	printf("DKA stanja %d\n",dkaStanja.size());
	
	for(int i=0;i<dkaStanja.size();++i)
	{
		printf("%d. %d\n",i,dkaStanja[i].stanja.size());
		for(int j=0;j<dkaStanja[i].stanja.size();++j)
		{
			cout<<" ";dkaStanja[i].stanja[j].ispis();
		}
	}
	for(int i=0;i<dkaPrijelazi.size();++i)
	{
		cout<<dkaPrijelazi[i].izStanja<<" , "<<dkaPrijelazi[i].uStanje<<" --> "<<popisSvihZnakova[dkaPrijelazi[i].znakZaPrijelaz]<<endl;
	}
	freopen("analizator/tablica.txt","w",stdout);
	
	cout<<zavrsniZnakovi.size()<<endl;
	for(int i=0;i<zavrsniZnakovi.size();++i)
	{
		cout<<zavrsniZnakovi[i]<<" ";
	}
	cout<<endl;
	
	cout<<nezavrsniZnakovi.size()<<endl;
	for(int i=0;i<nezavrsniZnakovi.size();++i)
	{
		cout<<nezavrsniZnakovi[i]<<" ";
	}
	cout<<endl;
	cout<<dkaStanja.size()<<" "<<zavrsniZnakovi.size()<<endl;
	//tablica akcija
	
	for(int i=0;i<dkaStanja.size();++i)
	{
		//F A P1 R1
		for(int j=0;j<zavrsniZnakovi.size();++j)
		{
			if(isOver(i,zavrsniZnakovi[j]))
			{
				cout<<"A ";
				continue;
			}
			int st=probajSePomaknuti(i,zavrsniZnakovi[j]);
			if(st!=-1)
			{
				cout<<"P "<<st<<" ";
				continue;
			}
			st=probajSeReducirati(i,zavrsniZnakovi[j]);
			if(st!=-1)
			{
				cout<<"R "<<st<<" ";
				continue;
			}
			cout<<"F ";
			
		}
		cout<<endl;
		
	}
	//novo stanje
	cout<<dkaStanja.size()<<" "<<nezavrsniZnakovi.size()<<endl;
	for(int i=0;i<dkaStanja.size();++i)
	{
		for(int j=0;j<nezavrsniZnakovi.size();++j)
		{
			tablicaNovoStanje(i,nezavrsniZnakovi[j]);
		}
		cout<<endl;
	}
	
	cout<<produkcije.size()<<endl;
	for(int i=0;i<produkcije.size();++i)
	{
		cout<<popisSvihZnakova[produkcije[i].lijevaStrana]<<" ";
		cout<<produkcije[i].desnaStrana.size()<<endl;
	}
	
	return 0;
}
