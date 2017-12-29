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

#include <iomanip>
#include <locale>
#include <sstream>
#include <string> 

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
char temp[300];
vector<string>ulazniZnakovi;
vector<Redukcija>redukcije;
stack<string>S;
vector<int>V[1500];
string naziviCvorova[1500];
string input[1500];
vector<string>sinkronizacijskiZnakovi;
void ispisStabla(int pos,int razmak)
{
	for(int i=0;i<razmak;++i)cout<<" ";
	cout<<naziviCvorova[pos]<<endl;
	if(V[pos].size()==0 && naziviCvorova[pos][0]=='<')
	{
		for(int i=0;i<razmak+1;++i)cout<<" "; 
		cout<<"$"<<endl;	
	}
	for(int i=V[pos].size()-1;i>=0;--i)
	{
		ispisStabla(V[pos][i],razmak+1);
	}
	return;
}
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
		if(akcije[i].stanje==stanjeNaStogu && akcije[i].znak==input)
		{
			//cout<<"Akcija "<<akcije[i].action<<endl;
			if(akcije[i].jesiLiProdukcija())
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
	//printf("Novo stanje\n");
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
				//cout<<nezavrsni[j]<<","<<i<<"-->"<<a<<endl;
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
		//cout<<"redukcija ->"<<R.lijeviZnak<<" + "<<R.brojZnakova<<endl;
		redukcije.push_back(R);
	}
	fscanf(pfile,"%d",&n);
	for(int i=0;i<n;++i)
	{
		fscanf(pfile,"%s",temp);
		string s=temp;
		sinkronizacijskiZnakovi.push_back(s);
	}
	/**/
	
	string opis="";
	int linija=0;
	int br=0;
	while(scanf("%s",temp)!=EOF)
	{
		scanf("%d",&linija);
		getline(cin,opis);
		
		//cout<<temp<<"--> u liniji "<<linija<<". Sa opisom ->"<<opis<<endl;
		string ulaz=temp;
		string linijaBr = static_cast<ostringstream*>( &(ostringstream() << linija) )->str();
		
		
		input[br++]=ulaz+" "+linijaBr+""+opis;
		ulazniZnakovi.push_back(ulaz);
	}
	
	ulazniZnakovi.push_back("%");
	int pos=0;
	
	S.push(znakovi[znakovi.size()-1]);
	StogDodajBrojStanje(0);
	stack<int>lokalniStog;
	int brojCvorova=0;
	while(pos<ulazniZnakovi.size())
	{
		if(nasaoPrihvati(ulazniZnakovi[pos]))
		{
			//printf("PRIHVACEN NIZ!!!! %d/%d\n",pos,ulazniZnakovi.size());
			break;
		}
		if(nasaoOdbaci(ulazniZnakovi[pos]))
		{
			/*printf("%d/%d\n",pos,ulazniZnakovi.size());
			cout<<"Znak "<<ulazniZnakovi[pos]<<endl;*/
			/*printf("ODBACEN NIZ!!!!\n");
			printf("Oporavak\n");*/
			//++pos;
			
			for(int i=pos;i<ulazniZnakovi.size();++i)
			{
				bool sinkro=false;
				//printf("%d %d\n",i,pos);
				for(int j=0;j<sinkronizacijskiZnakovi.size();++j)
				{
					if(sinkronizacijskiZnakovi[j]==ulazniZnakovi[i])
					{
						pos=i;
						sinkro=true;
						break;
					}
				}
				if(sinkro)break;
			}
			
			//printf("Nasli sinkro\n");
			//odbacuje sa stoga stanja ( i cvorove i ostale podatke)
			while(nasaoOdbaci(ulazniZnakovi[pos]))
			{
				S.pop();
				//printf("Izbacen cvor\n");
				cout<<naziviCvorova[lokalniStog.top()]<<endl;
				lokalniStog.pop();
				
				while(S.top()[0]!=';')
				{
				  S.pop();
				  
				}
				//break;
			}
			//++pos;
			
			//dokle god ne nade akcija[s,sinkro znak]
					
			//printf("Pos %d\n",pos);	
		}
		if(nasliNovoStanje(ulazniZnakovi[pos]))
		{
			naziviCvorova[brojCvorova]=input[pos];
			lokalniStog.push(brojCvorova++);
			//S.push(";")
			
			int novoStanje=odrediNovoStanje(ulazniZnakovi[pos]);
			S.push(ulazniZnakovi[pos]);
			//printf("Novo stanje %d\n",novoStanje);
			StogDodajBrojStanje(novoStanje);//S.push(novoStanje)
			++pos;
		}
		else if(nasliReduciraj(ulazniZnakovi[pos])!=-1)
		{
			Redukcija R=redukcije[nasliReduciraj(ulazniZnakovi[pos])];
			int broj=R.brojZnakova;
			//printf("Redukcija stanje %d\n",nasliReduciraj(ulazniZnakovi[pos]));
			StogMakniNClanaSaStoga(broj*2);
			int brojZaNovoStanje=StogDohvatiBrojStanje();
			//cout<<"Broj za novo Stanje"<<brojZaNovoStanje<<endl;
			
			//cout<<"Dodajem "<<R.lijeviZnak<<endl;
			naziviCvorova[brojCvorova++]=R.lijeviZnak;
			for(int i=0;i<broj;++i)
			{
				int br=lokalniStog.top();
				V[brojCvorova-1].push_back(br);
				//cout<<"Cvor "<<naziviCvorova[br]<<endl;
				lokalniStog.pop();
			}
			
			lokalniStog.push(brojCvorova-1);
			S.push(R.lijeviZnak);//dodajLijeviZnak(R.lijeviZnak);
			//pogledaj u tablicu novoStanje
			//cout<<R.lijeviZnak<<" "<<brojZaNovoStanje<<endl;
			int novoStanje=novoStanjeTablica[make_pair(R.lijeviZnak,brojZaNovoStanje)];
			StogDodajBrojStanje(novoStanje);
			//cout<<"novo stanje"<<novoStanje<<endl;
		}
	}
	ispisStabla(brojCvorova-1,0);
	
	return 0;
}
