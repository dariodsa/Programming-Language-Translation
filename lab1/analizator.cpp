#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;
char q[1000];
int br_automata;
struct Automat
{
	string ime;
	int id;
	vector<int>V[1500];
	vector<char>Va[1500];
	int pocetno;
	int prihvatljivo;
	vector<string>naredbe;
	set<int>S;
	void pocetak()
	{
		S.clear();
		S.insert(pocetno);
	}
	bool dobar()
	{
		set<int>::iterator it;
		for(it=S.begin();it!=S.end();++it)
		{
			if(*it==prihvatljivo)return true;
		}
		return false;
	}
	bool gotov()
	{
		return S.size()==0;
	}
	void prijelaz(char w)
	{
		set<int>::iterator it;
		vector<int>temp;
		
		while(true)
		{
			int size=S.size();
			set<int>::iterator it;
			set<int>S1;
			S1.clear();
			bool add=false;
			for(it=S.begin();it!=S.end();++it)
			{
				int br=*it;
				for(int i=0;i<V[br].size();++i)
				{
					if(Va[br][i]=='¸')
					{
						S1.insert(V[br][i]);
					
					}
				}
			}
			for(it=S1.begin();it!=S1.end();++it)S.insert(*it);
			if(S.size()==size)break;
		}
	
		for(it=S.begin();it!=S.end();++it)
		{
			int i=*it;
			bool dobar=false;
			for(int j=0;j<V[i].size();++j)
			{
				if(Va[i][j]==w)dobar=true;
			}
			if(!dobar)temp.push_back(*it);
		}
		for(int i=0;i<temp.size();++i)S.erase(temp[i]);
		temp.clear();
		int kol=0;
		int size=S.size();
		
		set<int>S1;
		bool add=false;
		set<int>brisi;
		S1.clear();
		for(it=S.begin();it!=S.end();++it)
		{
			int br=*it;
			for(int i=0;i<V[br].size();++i)
			{
				if(Va[br][i]==w)
				{
			
					S1.insert(V[br][i]);
					brisi.insert(br);
					
				}
			}
		}
		
		for(it=brisi.begin();it!=brisi.end();++it)S.erase(*it);
		brisi.clear();
		for(it=S1.begin();it!=S1.end();++it)S.insert(*it);
		S1.clear();
		while(true)
		{
			int size=S.size();
			set<int>::iterator it;
			set<int>S1;
			bool add=false;
			for(it=S.begin();it!=S.end();++it)
			{
				int br=*it;
				for(int i=0;i<V[br].size();++i)
				{
					if(Va[br][i]=='¸')
						S1.insert(V[br][i]);
				}
			}
			for(it=S1.begin();it!=S1.end();++it)S.insert(*it);
			if(S.size()==size)break;
		}
		
	}
	
};
int BR_REDAKA=1;
string STANJE="S_pocetno";
int BR=-1;
string izraz="";
char t[14400];
string kod="";
void obrada(int poc,string naredba,string query);
void naredbeAll(int poc,vector<string>naredbe,string query)
{
	string po="";
	for(int i=0;i<naredbe.size();++i)
	{
		if(naredbe[i][0]=='V' && naredbe[i][1]=='R')
		{
			int broj=0;
			for(int k=strlen("VRATI_SE ");k<naredbe[i].length();++k)broj=(broj*10)+(naredbe[i][k]-'0');
			for(int k=poc;k<poc+broj;++k)po+=kod[k];
			query=po;
		}
		
	}
	for(int i=0;i<naredbe.size();++i)obrada(poc,naredbe[i],query);
}
void obrada(int poc, string naredba,string query)
{
	if(naredba=="-")
	{
		return;
	}
	if(naredba=="NOVI_REDAK")
	{
		BR_REDAKA++;
		return;
	}
	int found1=naredba.find("UDJI_U_STANJE ");
	int found2=naredba.find("VRATI_SE ");
	if(found1!=string::npos)
	{
		string stanje="";
		for(int i=strlen("UDJI_U_STANJE ");i<naredba.length();++i)stanje+=naredba[i];
		
		STANJE=stanje;
		return;
	}
	if(found2!=string::npos)
	{
		int broj=0;
		
		for(int i=strlen("VRATI_SE ");i<naredba.length();++i)broj=(broj*10)+(naredba[i]-'0');
		BR=broj;
		return;
	}
	else cout<<naredba<<" "<<BR_REDAKA<<" "<<query<<endl;	
	
}
vector<Automat>automati;
int a,b,c,d;
int main()
{
	FILE *pfile;
	pfile=fopen("tablica.txt","r");
	fscanf(pfile,"%d",&br_automata);
	for(int z=0;z<br_automata;++z)
	{
		Automat A=Automat();
		fscanf(pfile,"%s",q);
		string ime=q;
		fscanf(pfile,"%d",&a);
		
		//prijelazi
		for(int i=0;i<a;++i)
		{
			fscanf(pfile,"%d%d%d",&b,&c,&d);
			
			A.V[b].push_back(c);
			A.Va[b].push_back(d);
		}
		
		fscanf(pfile,"%d%d",&b,&c);
		//pocetno stanje // prihvatljivo
		fscanf(pfile,"%d\n",&d);
		for(int i=0;i<d;++i)
		{
			
			fgets(q,sizeof q, pfile);
			string w="";
			for(int j=0;j<strlen(q);++j)if(q[j]!='\n')w+=q[j];
			
			//dodaj u naredbe
			A.naredbe.push_back(w);
		}
		A.ime=ime;
		A.id=z;
		A.pocetno=b;
		A.prihvatljivo=c;
		A.pocetak();
		automati.push_back(A);
	}
	fscanf(pfile,"%s",q);
	STANJE=q;
	
	string k;
	while(getline(cin,k))
	{
		kod+=k+"\n";
	}
	int id2=4545646;
	int id;
	int kol=0;
	int lastOne=0;
	string query="";
	int i=0;
	
	for(i=0;i<kod.length();++i)
	{
		string query="";
		int last_Automat=-1;
		int last_id=-1;
		for(int j=i;j<kod.length();++j)
		{
			for(int k=0;k<automati.size();++k)
			{
				if(automati[k].ime==STANJE && !automati[k].gotov())
				{
					automati[k].prijelaz(kod[j]);
					if(automati[k].dobar() && last_id!=j)last_id=j,last_Automat=k;
				}
			}
		}
		if(last_id==-1)
		{
			//pogreska
			for(int j=0;j<automati.size();++j)automati[j].pocetak();
			continue;
		}
		else
		{
			string ki="";
			for(int k=i;k<=last_id;++k)ki+=kod[k];
			naredbeAll(i,automati[last_Automat].naredbe,ki);
			if(BR!=-1){i=i+BR-1;BR=-1;}
			else i=last_id;
			for(int j=0;j<automati.size();++j)automati[j].pocetak();
		}
	}
	
	return 0;
}
