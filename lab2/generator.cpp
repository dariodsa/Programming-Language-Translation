#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
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
		printf("mirkec");
		cout<<" "<<popisSvihZnakova[lijevaStrana]<<" --> ";
		for(int j=0;j<desnaStrana.size();++j)
		{
			if(tocka==j)cout<<"*";
			cout<<popisSvihZnakova[desnaStrana[j]];
		}
		cout<<" {";
		for(int j=0;j<skup.size();++j)
		{
			cout<<popisSvihZnakova[skup[j]]<<",";
		}
		cout<<"}"<<endl;
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
struct Produkcija
{
	int lijevaStrana;
	vector<int>desnaStrana;
};
const string EPSILON="$";
const string POCETNI="<%>";
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
map<string,int>skupSvihZnakova;
map<string,bool>sinkronizacijskiZnak;
int zapocinje[1500][1500];
vector<int>zapocinjeZnakom[5000];
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
void dodajStanjaKojaSlijedeIz(Stanje S,int pos)
{
	if(S.tocka!=0)return;
	//moras dodati i prijelaze
	for(int i=0;i<S.desnaStrana.size();++i)
	{
		Stanje S2;
		S2.lijevaStrana=S.lijevaStrana;
		S2.tocka=i+1;
		S2.podesiSkup(S.skup);
		
		for(int j=0;j<S.desnaStrana.size();++j)
			S2.desnaStrana.push_back(S.desnaStrana[j]);
		
		stanja.push_back(S2);
		
		Prijelaz P=Prijelaz(pos,stanja.size()-1,S.desnaStrana[i]);
		pos=stanja.size()-1;
		prijelazi.push_back(P);
	}
	return;
}
void bfs(int pos,bool* bio)
{
	vector<Stanje>istaStanja;
	queue<int>Q;
	Q.push(pos);
	bio[pos]=true;
	while(!Q.empty())
	{
		int pos=Q.front();
		Q.pop();
		istaStanja.push_back(stanja[pos]);
		for(int i=0;i<prijelazi.size();++i)
		{
			if(prijelazi[i].izStanja==pos && prijelazi[i].epsilon && bio[prijelazi[i].uStanje]==false)
			{
				bio[prijelazi[i].uStanje]=true;
				Q.push(prijelazi[i].uStanje);
			}
		}
	}
	cout<<"Ista stanja"<<endl;
	for(int i=0;i<istaStanja.size();++i)
	{
		istaStanja[i].ispis();
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
	brojZaEpsilon=brojSvihZnakova;
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
	
	postaviIhUVektorZapocinjeZnakom();
	
	Prijelaz pocetni=Prijelaz(0,1,true);
	prijelazi.push_back(pocetni);
	
	//kreiraj Stanja sa tockama
	for(int i=1;i<produkcije.size();++i)
	{
		if(produkcije[i].desnaStrana.size()==1 
		  && produkcije[i].desnaStrana[0]==brojZaEpsilon)
		{
			  	Stanje S;
				S.tocka=1;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				stanja.push_back(S);
		}
		else  
			for(int j=0;j<produkcije[i].desnaStrana.size()+1;++j)
			{
				Stanje S;
				S.tocka=j;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				stanja.push_back(S);
			}
	}	
	//kreni u dodavanje skupova u stanja i kreiranje mogucih prijelaza
	
	for(int i=0;i<stanja.size();++i)
	{
		//if(i==10)break;
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
					
					prijelazi.push_back(P);
				}
				else
				{
					if(stanja[j].imamoLiIstiSkup(stanja[i].skup))
					{
						Prijelaz P=Prijelaz(i,j,stanja[i].znakIzaTocke(),false);
						
						prijelazi.push_back(P);
					}
					//kreiraj novo stanje koje ce se razlikovati po skupu, a po ostalome ce biti jednako
					else
					{
						Stanje S;
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						S.podesiSkup(stanja[i].skup);
						
						Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),stanja[i].znakIzaTocke(),false);
					
						
						prijelazi.push_back(P);
						
						tempStanja.push_back(S);
					}
				}
				
			}
			else if(stanja[i].znakIzaTocke() == stanja[j].lijevaStrana &&
			        stanja[j].tocka==0
			        )
			{
				printf("USAO %d %d\n",i,j);
				int znak=stanja[i].znakIza2Tocke();
				if(znak<0)
				{
					if(stanja[j].skup.size()==0)
					{
						stanja[j].podesiSkup(stanja[i].skup);
						Prijelaz P=Prijelaz(i,j,true);
						
						
						prijelazi.push_back(P);	
					}
					else
					{
						Stanje S;
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						S.podesiSkup(stanja[i].skup);
						
						Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),true);
						
						prijelazi.push_back(P);
						
						tempStanja.push_back(S);
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
						
						prijelazi.push_back(P);
					}
					else
					{
						Stanje S;
						S.lijevaStrana=stanja[j].lijevaStrana;
						S.dodajDesnuStranu(stanja[j].desnaStrana);
						S.tocka=stanja[j].tocka;
						
						S.podesiSkup(zapocinjeZnakom[znak]);
						if(ideLiTrazeniZnakUEpsilon(znak))
							S.dodajSkup(stanja[i].skup);
						
						if(stanja[j].imamoLiIstiSkup(S.skup))
						{
							Prijelaz P=Prijelaz(i,j,true);
						
							prijelazi.push_back(P);
						}
						else
						{
							Prijelaz P=Prijelaz(i,stanja.size()+tempStanja.size(),true);
						
							prijelazi.push_back(P);
							tempStanja.push_back(S);
						}
					}
				}
			}
		}
		for(int k=0;k<tempStanja.size();++k)stanja.push_back(tempStanja[k]);
	}
	for(int i=0;i<stanja.size();++i)
	{
		stanja[i].ispis();
	}
	
	for(int i=0;i<prijelazi.size();++i)
	{
		string znak=prijelazi[i].epsilon==false?popisSvihZnakova[prijelazi[i].znakZaPrijelaz]:"oko";
		cout<<"Znak za prijelaz "<<znak<<endl;
		stanja[prijelazi[i].izStanja].ispis();
		stanja[prijelazi[i].uStanje].ispis();
	}
	
	/*Stanje S;
	S.lijevaStrana=-1;
	S.skup.push_back(skupSvihZnakova[POCETNI]);
	stanja.push_back(S);
	
	for(int i=0;i<nezavrsniZnakovi.size();++i)
	{
		for(int j=0;j<produkcije.size();++j)
		{
			if(produkcije[j].lijevaStrana==i)
			{
				Stanje Q;
				Q.lijevaStrana=i;
				Q.tocka=0;
				
				for(int k=0;k<produkcije[j].desnaStrana.size();++k)
					Q.desnaStrana.push_back(produkcije[j].desnaStrana[k]);
				
				stanja.push_back(Q);
				dodajStanjaKojaSlijedeIz(Q,stanja.size()-1);
			}
		}
	}
	for(int i=1,len=stanja.size();i<len;++i)
	{
		//if(stanja[i].lijevaStrana==-1)continue;
		if(stanja[i].tocka==stanja[i].desnaStrana.size())
		{
			//poptune stavke
		}
		else
		{
			int Znak=stanja[i].desnaStrana[stanja[i].tocka];//znak iza tocke
			for(int j=0,len=stanja.size();j<len;++j)
			{
				if(stanja[j].lijevaStrana==Znak)
				{
					Prijelaz P=Prijelaz(i,j,true); //epsilon prijelaz izmedu stavki(stanja)
					prijelazi.push_back(P);
					//podesni skup u stanju j ( cilju)
					//ovdje ide pametni kod
					//provjera nalazi li se iza tocke i iza naseg znaka jos jedan znak za operaciju zapocinje
					
					if(stanja[i].desnaStrana.size()>(stanja[i].tocka+1) && stanja[j].tocka==0)
					{
						int znakIzaIzaTocke = stanja[i].desnaStrana[stanja[i].tocka+1];
						
						cout<<"Zapocinje od "<<popisSvihZnakova[znakIzaIzaTocke]<<" "<<popisSvihZnakova[stanja[i].lijevaStrana]<< " " << i <<","<<j<<endl;
						
						for(int k=0;k<zapocinjeZnakom[znakIzaIzaTocke].size();++k)
						{
							cout<<popisSvihZnakova[zapocinjeZnakom[znakIzaIzaTocke][k]]<<endl;
						}
						
						//stanja[j].podesiSkup(zapocinjeZnakom[znakIzaIzaTocke]);
						
						Stanje novoStanje;
						novoStanje.lijevaStrana=stanja[j].lijevaStrana;
						novoStanje.podesiSkup(zapocinjeZnakom[znakIzaIzaTocke]);
						novoStanje.tocka=0;
						novoStanje.dodajDesnuStranu(stanja[j].desnaStrana);
						stanja.push_back(novoStanje);
						
						dodajStanjaKojaSlijedeIz(novoStanje,stanja.size()-1);
					}
				}	
			}	
		}
	}
	//stanja[0].ispis();
	/*bool* bio =(bool*)malloc(stanja.size());
	memset(bio,false,sizeof bio);
	for(int i=1;i<stanja.size();++i)
	{
		if(!bio[i])
			bfs(i,bio);
	}
	*/
	printf("E-NKA stanja %d\n",stanja.size());
	return 0;
}
