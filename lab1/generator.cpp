#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
using  namespace std;
map<string,string>mapa;
string q;
vector<string>regularne_definicije;
vector<string>stanja_analizatora;
vector<string>lek_jedinke;
vector<string>pravila;
map<int,string >ulazak_u_lek_analizatora;
map<int,vector<string> >pravila_lek_analizatora;
void obradi_reg()
{
	string ime="";
	string definicija="";
	int i;
	for(i=1;i<q.length();++i)
	{
		if(q[i]=='}')break;
		ime+=q[i];
	}
	regularne_definicije.push_back(ime);
	for(int j=i+2;j<q.length();++j)
		definicija+=q[j];
	cout<<q<<endl;
	cout<<ime<<endl;
	mapa[ime]=definicija;
	cout<<"+++"<<mapa[ime]<<endl;
}
void obrada_stanja_lek_ana()
{
	int i=3;
	string w="";
	for(i=3;i<q.length();++i)
	{
		if(q[i]==' ')
		{
			stanja_analizatora.push_back(w);
			w="";
		}
		else w+=q[i];
	}
	stanja_analizatora.push_back(w);
}
void obrada_imena_lek_jed()
{
	int i=3;
	string w="";
	for(i=3;i<q.length();++i)
	{
		if(q[i]==' ')
		{
			lek_jedinke.push_back(w);
			w="";
		}
		else w+=q[i];
	}
	lek_jedinke.push_back(w);
}
string obrada_pravila()
{
	string ime="";
	int i;
	for(i=1;i<q.length();++i)
	{
		if(q[i]=='>')break;
		ime+=q[i];
	}
	string pravilo="";
	for(int j=i+1;j<q.length();++j)
		pravilo+=q[j];
	
	ulazak_u_lek_analizatora[pravila.size()]=pravilo;
	return ime;
}
string obrada_reg_definicija(string def)
{
	bool jos=true;
	while(jos)
	{
		jos=false;
		for(int j=0;j<regularne_definicije.size();++j)
		{
			int found=def.find(regularne_definicije[j]);
			if(found!=string::npos)
			{
				//"Nasao "<<regularne_definicije[j]<< " "<<mapa[regularne_definicije[j]];
				
				string nova_def="("+mapa[regularne_definicije[j]]+")";
				def.replace(found-1,regularne_definicije[j].size()+2,nova_def);
				
				jos=true;
			}
		}
	}
	return def;
}
struct ParStanja
{
	int lijevo_stanje;
	int desno_stanje;
};
struct Automat
{
	int prihvatljivo_stanje;
	int pocetno_stanje;
	int br_stanja;
	//char prijelazi[500][500];
	Automat()
	{
	}
	vector<int>Q1;
	vector<int>Q2;
	vector<int>P;
	void brisi()
	{
		//memset(prijelazi,0,sizeof prijelazi);
		Q1.clear();
		Q2.clear();
		P.clear();
	}
	void dodaj_prijelaz(int stanjeA,int stanjeB,int znak)
	{
		Q1.push_back(stanjeA);
		Q2.push_back(stanjeB);
		P.push_back(znak);
		
	}
	void dodaj_epsilon_prijelaz(int stanjeA,int stanjeB)
	{
		dodaj_prijelaz(stanjeA,stanjeB,'¸');
	}
	int novo_stanje()
	{
		br_stanja++;
		return br_stanja-1;
	}
	
	ParStanja pretvori(string izraz)
	{
		int br_zagrada=0;
		//niz izbori
		vector<string>izbori;
		int zadnji=0;
		int br_operatora_izbora=0;
		//izraz=obrada(izraz);
		for(int i=0;i<izraz.length();++i)
		{
			printf("(%d,%d)\n",i,br_zagrada);
			if(izraz[i]=='(' && je_operator(izraz,i))
				br_zagrada++;
			else if(izraz[i]==')' && je_operator(izraz,i))
				br_zagrada--;
			else if(br_zagrada==0 && izraz[i]=='|' && je_operator(izraz,i))
			{
				++br_operatora_izbora;
				printf("Operator %d %d\n",i,izraz[i]);
				//grupiraj lijevi negrupirani dio niza znakova izraz u niz izbori
				string temp="";
				for(int k=zadnji;k<i;++k)temp+=izraz[k];
				zadnji=i+1;
				izbori.push_back(temp);
			}
		}
		/*ako je pronadjen barem jedan operator izbora
			 grupiraj preostali negrupirani dio niza znakova izraz u niz izbori*/
		if(br_operatora_izbora>0)
		{
			string temp="";
			for(int k=zadnji;k<izraz.length();++k)temp+=izraz[k];
			izbori.push_back(temp);
			printf("Nasao\n");
			for(int z=0;z<izbori.size();++z)
			{
				cout<<izbori[z]<<", ";
			}
			printf("\n");
		}
		int lijevo_stanje=novo_stanje();
		int desno_stanje=novo_stanje();
		if(br_operatora_izbora>0)
		{
			for(int i=0;i<izbori.size();++i)
			{
				ParStanja privremeno=pretvori(izbori[i]);
				dodaj_epsilon_prijelaz(lijevo_stanje,privremeno.lijevo_stanje);
				dodaj_epsilon_prijelaz(privremeno.desno_stanje,desno_stanje);
			}
		}
		else
		{
			printf("Usao\n");
			bool prefiksirano=false;
			int zadnje_stanje=lijevo_stanje;
			for(int i=0;i<izraz.length();++i)
			{
				int a,b;
				if(prefiksirano)
				{
					//1
					prefiksirano=false;
					char prijelazni_znak;
					if(izraz[i]=='t')
						prijelazni_znak='\t';
					else if(izraz[i]=='n')
						prijelazni_znak='\n';
					else if(izraz[i]=='_')
					{
						prijelazni_znak=' ';
					}
					else prijelazni_znak=izraz[i];
					a=novo_stanje();
					b=novo_stanje();
					printf("Prilazni znak %c %d\n",prijelazni_znak,prijelazni_znak);
					cout<<izraz<<endl;
					dodaj_prijelaz(a,b,prijelazni_znak);
				}
				else
				{
					
					//2
					if(izraz[i]=='\\')
					{
						prefiksirano=true;
						continue;
					}
					if(izraz[i]!='(')
					{
						//2A
						a=novo_stanje();
						b=novo_stanje();
						printf("Dodan prijelaz %c\n",izraz[i]);
						cout<<izraz<<endl;
						if(izraz[i]=='¸')
							dodaj_epsilon_prijelaz(a,b);
						else 
							dodaj_prijelaz(a,b,izraz[i]);
					}
					else
					{
						//2B
						int j;//*pronadji odgovarajucu zatvorenu zagradu*
						string izraz2="";
						int kol=0;
						for(j=i+1;j<izraz.length();++j)
							if(izraz[j]=='(' && je_operator(izraz,j)){++kol;izraz2+=izraz[j];}
							else if(izraz[j]==')' && je_operator(izraz,j) && kol>0){--kol;izraz2+=izraz[j];}
							else if(izraz[j]==')' && je_operator(izraz,j) && kol==0)break;
							else izraz2+=izraz[j];
						cout<<"Novi bolji "<<izraz2<<endl;
						ParStanja privremeno=pretvori(izraz2);
						a=privremeno.lijevo_stanje;
						b=privremeno.desno_stanje;
						i=j;
					}
				}
				
				if(i+1<izraz.length() && izraz[i+1]=='*')
				{
					int x=a;
					int y=b;
					a=novo_stanje();
					b=novo_stanje();
					dodaj_epsilon_prijelaz(a,x);
					dodaj_epsilon_prijelaz(y,b);
					dodaj_epsilon_prijelaz(a,b);
					dodaj_epsilon_prijelaz(y,x);
					++i;
				}
				dodaj_epsilon_prijelaz(zadnje_stanje,a);
				zadnje_stanje=b;
			}
			dodaj_epsilon_prijelaz(zadnje_stanje,desno_stanje);
		}
		ParStanja rez;
		rez.lijevo_stanje=lijevo_stanje;
		rez.desno_stanje=desno_stanje;
		return rez;			
	}
	bool je_operator(string izraz,int i)
	{
		int br=0;
		while(i>=0 && izraz[i-1]=='\\')
		{
			++br;
			--i;
		}
		return br%2==0;
	}
};



int main()
{
	bool aktivna_naredba=false;
	string ime_pravila;
	FILE *pfile=fopen("tablica.txt","w");
	while(getline(cin,q))
	{
		if(q[0]=='{' && !aktivna_naredba)
		{
			obradi_reg();
		}
		else if(q[0]=='%' && q[1]=='X')
		{
			obrada_stanja_lek_ana();
		}
		else if(q[0]=='%' && q[1]=='L')
		{
			obrada_imena_lek_jed();
		}
		else
		{
			//pravila lek analizatora
			if(q[0]=='}')
			{
				aktivna_naredba=false;
				continue;
			}
			if(q[0]=='<')
			{
				ime_pravila=obrada_pravila();
				pravila.push_back(ime_pravila);
				aktivna_naredba=true;
			}
			else if(q[0]!='{')
			{
				pravila_lek_analizatora[pravila.size()-1].push_back(q);
			}
		}
	}
	vector<Automat>automati;
	for(int i=0;i<regularne_definicije.size();++i)
	{
		mapa[regularne_definicije[i]]=obrada_reg_definicija(mapa[regularne_definicije[i]]);
		
	}
	for(int i=0;i<pravila.size();++i)
	{
		ulazak_u_lek_analizatora[i]=obrada_reg_definicija(ulazak_u_lek_analizatora[i]);
		
		Automat A=Automat();
		A.brisi();
		A.br_stanja=0;
		cout<<"Provjera: "<<ulazak_u_lek_analizatora[i]<<endl;
		
		ParStanja rezultat=A.pretvori(ulazak_u_lek_analizatora[i]);
		A.pocetno_stanje=rezultat.lijevo_stanje;
		A.prihvatljivo_stanje=rezultat.desno_stanje;
		
		automati.push_back(A);
	}
	freopen("tablica.txt","w",stdout);
	cout<<automati.size()<<endl;
	for(int i=0;i<automati.size();++i)
	{
		cout<<pravila[i]<<endl;
		cout<<automati[i].Q1.size()<<endl;
		for(int j=0;j<automati[i].Q1.size();++j)
		{
			cout<<automati[i].Q1[j]<<" "<<automati[i].Q2[j]<<" "<<automati[i].P[j]<<endl;
		}
		cout<<automati[i].pocetno_stanje<<" "<<automati[i].prihvatljivo_stanje<<endl;
		cout<<pravila_lek_analizatora[i].size()<<endl;
		for(int j=0;j<pravila_lek_analizatora[i].size();++j)
		{
			cout<<pravila_lek_analizatora[i][j]<<endl;
		}
	}
	cout<<stanja_analizatora[0]<<endl;
	return 0;
}
