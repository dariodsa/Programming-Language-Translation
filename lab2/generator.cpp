
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
	pair<int,int>id;
	int poredakProdukcije;
	Stanje(int x,int y)
	{
		id=make_pair(x,y);
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
	bool jednakiBezTocke(Stanje S)
	{
		if(S.desnaStrana.size()!=desnaStrana.size())return false;
		for(int i=0,len=desnaStrana.size();i<len;++i)
			if(desnaStrana[i]!=S.desnaStrana[i])return false;
		return true;
	}
	bool istiKao(Stanje S)
	{
		if(poredakProdukcije!=S.poredakProdukcije)return false;
		if(lijevaStrana!=S.lijevaStrana)return false;
		if(tocka!=S.tocka)return false;
		if(desnaStrana.size()!=S.desnaStrana.size())return false;
		if(skup.size()!=S.skup.size())return false;
		
		
		for(int i=0,len=desnaStrana.size();i<len;++i)
			if(desnaStrana[i]!=S.desnaStrana[i])return false;
		
		for(int i=0,len=skup.size();i<len;++i)
			if(skup[i]!=S.skup[i])return false;
		return true;
	}
	void podesiSkup(vector<int> niz)
	{
		skup.clear();
		for(int i=0,len=niz.size();i<len;++i)
		{
			bool imaLiGa=false;
			for(int j=0,len2=skup.size();j<len2;++j)
			{
				if(skup[j]==niz[i])
				{
					imaLiGa=true;
					break;
				}
			}
			if(!imaLiGa)
				skup.push_back(niz[i]);
		}
		sort(skup.begin(),skup.end());
	}
	void dodajSkup(vector<int> niz)
	{
		for(int i=0,len=niz.size();i<len;++i)
		{
			bool imaLiGa=false;
			for(int j=0,len2=skup.size();j<len2;++j)
			{
				if(skup[j]==niz[i])
				{
					imaLiGa=true;
					break;
				}
			}
			if(!imaLiGa)
				skup.push_back(niz[i]);
		}
		sort(skup.begin(),skup.end());
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
		for(int i=0,len=niz.size();i<len;++i)
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
		cout<<"}"<<"  "<<id.first<<","<<id.second<<" "<<poredakProdukcije<<endl;
	}
	bool imamoLiIstiSkup(vector<int>drugiSkup)
	{
		if(skup.size()!=drugiSkup.size())return false;
		
		//sort(drugiSkup.begin(),drugiSkup.end());
		//sort(skup.begin(),skup.end());
		
		for(int i=0;i<skup.size();++i)
			if(drugiSkup[i]!=skup[i])
				return false;
		return true;
	}
};
bool cmp(Stanje A,Stanje B)
{
	return A.id.first==B.id.first?A.id.second<B.id.second:A.id.first<B.id.first;
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
		for(int i=0,len=niz.size();i<len;++i)
		{
			stanja.push_back(niz[i]);
		}
		sort(stanja.begin(),stanja.end(),cmp);
	}
	bool istaStanja(vector<Stanje>niz)
	{
		bool isti=true;
		//if(niz.size()!=stanja.size())return false;
		//sort(niz.begin(),niz.end(),cmp);
		//sort(stanja.begin(),stanja.end(),cmp);
		for(int i=0,len=niz.size();i<len;++i)
		{
			if(niz[i].id.first!=stanja[i].id.first || niz[i].id.second!=stanja[i].id.second)
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

vector<Stanje>stanjaPolje[200];
//vector<Stanje>stanja2;
map< pair<int,int>,vector<pair<int,int> > >mapa;
map< pair<int,int>,vector<int> > mapaZnak;
//vector<Prijelaz>prijelazi;
vector<Prijelaz>dkaPrijelazi;
vector<DKAStanje>dkaStanja;

map<string,int>skupSvihZnakova;
map<string,bool>sinkronizacijskiZnak;
int zapocinje[1500][1500];
vector<int>zapocinjeZnakom[5000];
map<pair<int,int> , bool>bio;//bool bio[1500];
map<pair<int,int> , bool>bio2;//bool bio2[1500];
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
bool jedinstvenoStanje(Stanje S)
{
	for(int i=0,len=stanjaPolje[S.poredakProdukcije].size();i<len;++i)
	{
		if(S.tocka==stanjaPolje[S.poredakProdukcije][i].tocka)
		if(S.istiKao(stanjaPolje[S.poredakProdukcije][i]))
			return false;
	}
	return true;
}
int jedinstvenoDKAStanje(vector<Stanje>niz)
{
	int len2=niz.size();
	for(int i=0,len=dkaStanja.size();i<len;++i)
	{
		if(len2==dkaStanja[i].stanja.size())
			if(dkaStanja[i].istaStanja(niz))return i;
	}
	return -1;
}
void bfs(pair<int,int> pos,int parent,int znak,vector<pair<int,int> >dodaniZnakovi)
{
	queue<pair<int,int> > Qpos;
	queue<int>Qparent;
	queue<int>Qznak;
	queue<vector<pair<int,int> > >QdodaniZnakovi;
	
	Qpos.push(pos);
	Qparent.push(parent);
	Qznak.push(znak);
	QdodaniZnakovi.push(dodaniZnakovi);
	int bfsNum=0;
	while(!Qpos.empty())
	{
		bfsNum++;
		pos=Qpos.front();
		parent=Qparent.front();
		znak=Qznak.front();
		dodaniZnakovi=QdodaniZnakovi.front();
		Qpos.pop();Qparent.pop();Qznak.pop();QdodaniZnakovi.pop();
		
		
		if(dkaPrijelazi.size()%100==0)printf("BFS %d %d %d --> %d\n",dkaStanja.size(),dkaPrijelazi.size(),popisSvihZnakova.size(),bfsNum);
		//if(znak!=-1)cout<<popisSvihZnakova[znak]<<endl;
		vector<Stanje>istaStanja;
		
		
		queue<pair<int,int> >Q;
		
		map<pair<int,int> , bool>bio;//bool bio[1500];
		set<pair<int,int> >prijelaziUDrugaStanja[popisSvihZnakova.size()];
		Q.push(pos);
		for(int i=0,len=dodaniZnakovi.size();i<len;++i)
		{
			Q.push(dodaniZnakovi[i]);
			bio[dodaniZnakovi[i]]=true;
			istaStanja.push_back(stanjaPolje[dodaniZnakovi[i].first][dodaniZnakovi[i].second]);
		}
		bio[pos]=true;
		//int poz=pos;
		istaStanja.push_back(stanjaPolje[pos.first][pos.second]);
		
		
		int p=0;
		while(!Q.empty())
		{
			pair<int,int> pos=Q.front();
			Q.pop();
			for(int i=0,len=mapa[pos].size();i<len;++i)
			{
				if(mapaZnak[pos][i]==-1 && bio[mapa[pos][i]]==false)
				{
					bio[mapa[pos][i]]=true;
					
					istaStanja.push_back(stanjaPolje[mapa[pos][i].first][mapa[pos][i].second]);
					Q.push(mapa[pos][i]);
				}
				if(mapaZnak[pos][i]!=-1)
				{
					prijelaziUDrugaStanja[mapaZnak[pos][i]].insert(mapa[pos][i]);
					++p;
				}
				
			}
		}
		
		//Dodajem stanja povezana epsilon prijelazom u isto stanje u DKA automatu
		printf("%d %d\n",istaStanja.size(),p);
		DKAStanje S;
		S.idStanja=dkaStanja.size();
		//sort(istaStanja.begin(),istaStanja.end(),cmp);
		S.postaviStanja(istaStanja);
		int ponovljeno=jedinstvenoDKAStanje(S.stanja);
		if(ponovljeno==-1)
		{	
			dkaStanja.push_back(S);
			int idRoditelja=S.idStanja;
			if(parent!=-1)
			{
				bool find=false;
				//ako ne postoji dka prijelaz izmedu roditelja i djeteta - povezi ih
				/*for(int i=0;i<dkaPrijelazi.size();++i)
				{
					if(dkaPrijelazi[i].izStanja==parent && dkaPrijelazi[i].uStanje==dkaStanja.size()-1)find=true;
				}*/
				
				if(!find)
				{
					Prijelaz P=Prijelaz(parent,dkaStanja.size()-1,znak,false);
				//	printf("Dodajem prijelaz iz %d u %d sa znakom %d\n",parent,dkaStanja.size()-1,znak);
					dkaPrijelazi.push_back(P);
				}
				else if(find)
				{
					printf("NE.  Postoji prijelaz iz %d u %d sa znakom %d\n",parent,dkaStanja.size()-1,znak);
				}
			}
			
			//moguci prijelazi u nova stanja iz trenutnog dka stanja
			for(int j=0,len=popisSvihZnakova.size();j<len;++j)
			{
				if(prijelaziUDrugaStanja[j].empty())continue;
				else
				{
					vector<pair<int,int> >dodani;
					set<pair<int,int> >::iterator it;
					pair<int,int> Prvi;
					for(it=prijelaziUDrugaStanja[j].begin();it!=prijelaziUDrugaStanja[j].end();++it)//for(int k=1;k<prijelaziUDrugaStanja[j].size();++k)
					{
						if(it==prijelaziUDrugaStanja[j].begin())Prvi=*it;
						else dodani.push_back(*it);
					}
					
					//bfs(Prvi,idRoditelja,j,dodani);
					Qpos.push(Prvi);
					Qparent.push(idRoditelja);
					Qznak.push(j);
					QdodaniZnakovi.push(dodani);
				}
			}
		}
		else
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
void dodajStanjaV2()
{
	for(int i=0,len=produkcije.size();i<len;++i)
	{
		for(int j=0;j<stanjaPolje[i].size();++j)
		{
			for(int k=0;k<stanjaPolje[i].size();++k)
			{
				//1 slucaj
				if(stanjaPolje[i][j].tocka+1==stanjaPolje[i][k].tocka)
				{
					if(stanjaPolje[i][k].skup.size()==0)
					{
						stanjaPolje[i][k].podesiSkup(stanjaPolje[i][j].skup);
						mapa[make_pair(i,j)].push_back(make_pair(i,k));
						mapaZnak[make_pair(i,j)].push_back(stanjaPolje[i][j].znakIzaTocke());
					}
					else
					{
						if(stanjaPolje[i][k].imamoLiIstiSkup(stanjaPolje[i][j].skup))
						{
							mapa[make_pair(i,j)].push_back(make_pair(i,k));
							mapaZnak[make_pair(i,j)].push_back(stanjaPolje[i][j].znakIzaTocke());
						}
						else
						{
							Stanje S=Stanje(i,stanjaPolje[i].size());
							S.lijevaStrana=stanjaPolje[i][k].lijevaStrana;
							S.dodajDesnuStranu(stanjaPolje[i][k].desnaStrana);
							S.tocka=stanjaPolje[i][k].tocka;
							S.podesiSkup(stanjaPolje[i][j].skup);
							S.poredakProdukcije=stanjaPolje[i][k].poredakProdukcije;
							if(jedinstvenoStanje(S))
							{
								//dodaj prijelaz
								stanjaPolje[i].push_back(S);
								mapa[make_pair(i,j)].push_back(make_pair(i,stanjaPolje[i].size()-1));
								mapaZnak[make_pair(i,j)].push_back(stanjaPolje[i][j].znakIzaTocke());
							}
						}
					}
				}
			}
		}
		for(int j=0;j<len;++j)
		{
			//if(j==i)continue;
			for(int k=0;k<stanjaPolje[i].size();++k)
			{
				
				int znak=stanjaPolje[i][k].znakIzaTocke();
				if(stanjaPolje[j][0].lijevaStrana==znak)
				{
					//istrazi dublje ovdje je //2 slucaj
					for(int z=0;z<stanjaPolje[j].size();++z)
					{
						if(stanjaPolje[j][z].tocka==0 ||
						   (stanjaPolje[j][z].tocka==1 && stanjaPolje[j][z].desnaStrana[0]==brojZaEpsilon))
						{
							int znak=stanjaPolje[i][k].znakIza2Tocke();
							if(znak<0)
							{
								//nema bete
								if(stanjaPolje[j][z].skup.size()==0)
								{
									stanjaPolje[j][z].podesiSkup(stanjaPolje[i][k].skup);
									mapa[make_pair(i,k)].push_back(make_pair(j,z));
									mapaZnak[make_pair(i,k)].push_back(-1);
								}
								else
								{
									Stanje S=Stanje(j,stanjaPolje[j].size());
									S.lijevaStrana=stanjaPolje[j][z].lijevaStrana;
									S.dodajDesnuStranu(stanjaPolje[j][z].desnaStrana);
									S.tocka=stanjaPolje[j][z].tocka;
									S.poredakProdukcije=stanjaPolje[j][z].poredakProdukcije;
									S.podesiSkup(stanjaPolje[i][k].skup);
									if(S.istiKao(stanjaPolje[j][z]))
									{
										mapa[make_pair(i,k)].push_back(make_pair(j,z));
										mapaZnak[make_pair(i,k)].push_back(-1);
									}
									else if(jedinstvenoStanje(S))
									{
										stanjaPolje[j].push_back(S);
										
										mapa[make_pair(i,k)].push_back(make_pair(j,stanjaPolje[j].size()-1));
										mapaZnak[make_pair(i,k)].push_back(-1);
									}
								}
							}
							else
							{
								//posotoji beta
								if(stanjaPolje[j][z].skup.size()==0)
								{
									stanjaPolje[j][z].podesiSkup(zapocinjeZnakom[znak]);
									if(ideLiTrazeniZnakUEpsilon(znak))
										stanjaPolje[j][z].dodajSkup(stanjaPolje[i][k].skup);
									mapa[make_pair(i,k)].push_back(make_pair(j,z));
									mapaZnak[make_pair(i,k)].push_back(-1);

								}
								else
								{
									Stanje S=Stanje(j,stanjaPolje[j].size());
									S.lijevaStrana=stanjaPolje[j][z].lijevaStrana;
									S.dodajDesnuStranu(stanjaPolje[j][z].desnaStrana);
									S.tocka=stanjaPolje[j][z].tocka;
									S.poredakProdukcije=stanjaPolje[j][z].poredakProdukcije;
									
									S.podesiSkup(zapocinjeZnakom[znak]);
									if(ideLiTrazeniZnakUEpsilon(znak))
									{
										S.dodajSkup(stanjaPolje[i][k].skup);
									}
									if(S.istiKao(stanjaPolje[j][z]))
									{
										mapa[make_pair(i,k)].push_back(make_pair(j,z));
										mapaZnak[make_pair(i,k)].push_back(-1);
									}
									else if(jedinstvenoStanje(S))
									{
										stanjaPolje[j].push_back(S);
										
										mapa[make_pair(i,k)].push_back(make_pair(j,stanjaPolje[j].size()-1));
										mapaZnak[make_pair(i,k)].push_back(-1);
										
									}
									
								}
								
							}
						}
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
	
	/*for(int i=0;i<popisSvihZnakova.size();++i)
	{
		cout<<popisSvihZnakova[i]<<endl;
		cout<<" ";
		for(int j=0;j<zapocinjeZnakom[i].size();++j)
		{
			cout<<popisSvihZnakova[zapocinjeZnakom[i][j]]<<" ";
		}
		cout<<endl;
	}*/
	
	Stanje S=Stanje(0,0);
	S.tocka=0;
	S.lijevaStrana=brojSvihZnakova-1;
	S.desnaStrana.push_back(skupSvihZnakova[nezavrsniZnakovi[0]]);
	S.skup.push_back(skupSvihZnakova[POCETNI2]);
	S.poredakProdukcije=0;
	
	stanjaPolje[0].push_back(S);
	Stanje S1=Stanje(0,1);
	S1.tocka=1;
	S1.poredakProdukcije=0;
	S1.lijevaStrana=brojSvihZnakova-1;
	S1.skup.push_back(skupSvihZnakova[POCETNI2]);
	S1.desnaStrana.push_back(skupSvihZnakova[nezavrsniZnakovi[0]]);
	stanjaPolje[0].push_back(S1);
	
	
	//kreiraj Stanja sa tockama
	for(int i=1;i<produkcije.size();++i)
	{
		if(produkcije[i].desnaStrana.size()==1 
		  && produkcije[i].desnaStrana[0]==brojZaEpsilon)
		{
			  	Stanje S=Stanje(i,stanjaPolje[i].size());
				S.tocka=1;
				S.poredakProdukcije=i;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				S.skup.push_back(brojSvihZnakova-1);
				stanjaPolje[i].push_back(S);
				
				
		}
		else  
			for(int j=0;j<produkcije[i].desnaStrana.size()+1;++j)
			{
				Stanje S=Stanje(i,stanjaPolje[i].size());
				S.tocka=j;
				S.poredakProdukcije=i;
				S.lijevaStrana=produkcije[i].lijevaStrana;
				S.dodajDesnuStranu(produkcije[i].desnaStrana);
				stanjaPolje[i].push_back(S);
				
				
			}
	}	
	//printf("Stanja %d\n",stanja.size());
	
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
	
	for(int z=0;z<16;++z)
	{
		dodajStanjaV2();printf("%d\n",z);
		int stanja=0;
		//for(int i=0;i<prijelazi.size();++i)if(prijelazi[i].epsilon)++ep;
		//printf("Prijelazi %d -> %d\n",prijelazi.size(),ep);
		for(int i=0;i<produkcije.size();++i)
		{
			for(int j=0;j<stanjaPolje[i].size();++j)
			{	
				++stanja;
			}
			
		}	
		printf("Broj E-NKA stanja %d\n",stanja);
	}
	
	
	int ep=0;
	int stanja=0;
	//for(int i=0;i<prijelazi.size();++i)if(prijelazi[i].epsilon)++ep;
	//printf("Prijelazi %d -> %d\n",prijelazi.size(),ep);
	for(int i=0;i<produkcije.size();++i)
	{
		for(int j=0;j<stanjaPolje[i].size();++j)
		{	
			++stanja;
		}
		
	}
	printf("Broj E-NKA stanja %d\n",stanja);
	for(int i=0;i<produkcije.size();++i)
	{
		for(int j=0;j<stanjaPolje[i].size();++j)
		{	
			//stanjaPolje[i][j].ispis();
		}
	}
	
	
	
	vector<pair<int,int> >prazan;
	bfs(make_pair(0,0),-1,-1,prazan);
	//kreirajDka();
	
	//printf("E-NKA stanja %d\n",stanja.size());
	printf("DKA stanja %d\n",dkaStanja.size());
	
	/*for(int i=0;i<dkaStanja.size();++i)
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
	}*/
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
		if(produkcije[i].desnaStrana.size()==1 && produkcije[i].desnaStrana[0]==skupSvihZnakova["$"])cout<<0<<endl;
		else cout<<produkcije[i].desnaStrana.size()<<endl;
	}
	cout<<sinkronizacijskiZnakovi.size()<<endl;
	for(int i=0;i<sinkronizacijskiZnakovi.size();++i)
	{
		cout<<sinkronizacijskiZnakovi[i]<<endl;
	}
	return 0;
}
