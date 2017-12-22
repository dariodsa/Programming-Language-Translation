#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <utility>
#include <fstream>
using namespace std;
int KR_INT = 1;
int KR_CHAR = 2;
int KR_VOID = 3;
string izrazi[] = {"<primarni_izraz>","<postfiks_izraz>","<lista_argumenata>","<unarni_izraz>","<unarni_operator>","<cast_izraz>","<ime_tipa>","<specifikator_tipa>","<multiplikativni_izraz>","<aditivni_izraz>","<odnosni_izraz>","<jednakosni_izraz>","<bin_i_izraz>","<bin_xili_izraz>","<bin_ili_izraz>","<log_i_izraz>","<log_ili_izraz>","<izraz_pridruzivanja>","<izraz>"};
string strukture[] = {"<slozena_naredba>","<lista_naredbi>","<naredba>","<izraz_naredba>","<naredba_grananja>","<naredba_petlje>","<naredba_skoka>","<prijevodna_jedinica>","<vanjska_deklaracija>"};
string deklaracije[] = {"<definicija_funkcije>","<lista_parametara>","<deklaracija_parametara>","<lista_deklaracija>","<deklaracija>","<lista_init_deklaratora>","<init_deklarator>","<izravni_deklarator>","<inicijalizator>","<lista_izraza_pridruzivanja>"};
vector<string>funkcijeIzrazi;
vector<string>funkcijeDeklaracije;
vector<string>funkcijeNaredbe;
struct Tip
{
	int tip;
	bool polje;
	bool konstanta;
	Tip(int _tip,bool _polje, bool _konstanta)
	{
		tip = _tip;
		polje = _polje;
		konstanta = _konstanta;
	}
};
bool relacijaImplicitna(Tip tip1,Tip tip2)
{
	// U ~ V
	//ako se U moze prebaciti u V
	if(tip1.konstanta && !tip2.konstanta && tip1.tip==tip2.tip && tip1.polje==false && tip2.polje==false) return true;
	if(!tip1.konstanta && !tip2.konstanta && tip1.tip==KR_INT && tip2.tip==KR_CHAR && tip1.polje==false && tip2.polje==false) return true;
	if(!tip1.konstanta && !tip2.konstanta && tip2.tip==KR_INT && tip1.tip==KR_CHAR && tip1.polje==false && tip2.polje==false) return true;
	if(!tip1.konstanta && tip2.konstanta && tip1.tip==tip2.tip && tip1.polje && tip2.polje) return true;
	return false;
}
struct Varijabla
{
	string ime;
	int tip;
	bool konstanta;
	
	Varijabla(string _ime, int _tip, bool _konstanta)
	{
		ime = _ime;
		tip = _tip;
		konstanta = _konstanta;
	}
};
struct Funkcija
{
	string naziv;
	int tip;
	bool deklaracija;
	bool definicija;
	Funkcija(string _naziv, int _tip)
	{
		naziv = _naziv;
		tip = _tip;
		definicija = false;
		deklaracija = false;
	}
	void PostaviDefiniciju()
	{
		definicija = true;
	}
	void PostaviDeklaraciju()
	{
		deklaracija = true;
	}
};

int razmak(string s)
{
	int cnt = 0;
	for(int i=0,len=s.length();i<len;++i)
	{
		if(s[i]!=' ')
			return cnt;
		++cnt;
	}
	return cnt;
}
string dohvatiPrviString(string s)
{
	string ans="";
	bool findFirstChar = false;
	for(int i=0,len=s.length();i<len;++i)
	{
		if(s[i]==' ' && !findFirstChar){}
		else if(s[i]!=' ' && !findFirstChar)
		{
			findFirstChar = true;
			ans += s[i];
		}
		else if(s[i]==' ' && findFirstChar)
		{
			return ans;
		}
		else
			ans += s[i];
	}
	return ans;
}
struct Input
{
	string identifikator;
	string ostalo;
	int linija;
	Input(string input)
	{
		int pos=0;
		identifikator = "";
		for(pos=0;pos<input.length() && input[pos]==' ';++pos);
		
		for(int i=pos;i<input.length();++i)
		{
			if(input[i]==' ')
			{
				break;
				pos=i;
			}
			else
				identifikator += input[i];
		}
		if(identifikator[0] == '<')
			return;
		
		for(int i=pos+1;i<input.length();++i)
		{
			if(input[i]>='0' && input[i]<='9')
			{
				linija = linija*10 + (input[i]-'0');
			}
			else
			{
				pos=i;
				break;
			}
		}
		
		for(int i=pos+1;i<input.length();++i)
		{
			ostalo += input[i];
		}	
	}
	bool jesamLiIzraz()
	{
		for(int i=0;i<19;++i)
		{
			if(identifikator.compare(izrazi[i])==0)
				return true;
		}
		return false;
	}
	bool jesamLiStrukturaPrograma()
	{
		for(int i=0;i<9;++i)
		{
			if(identifikator.compare(strukture[i])==0)
				return true;
		}
		return false;
	}
	bool jesamLiDeklaracija()
	{
		for(int i=0;i<10;++i)
		{
			if(identifikator.compare(deklaracije[i])==0)
				return true;
		}
		return false;
	}
};
struct Semantika
{
	int l_izraz;
	Tip tip = Tip(KR_INT,false,false);
	Semantika()
	{
		 
	};
};
vector<pair<string,int> >ulaz[5000];
vector<int>V[10000];
vector<Input>input;
vector<Semantika>semantika;
string getStringToCompare(int pos)
{
	string ans = input[pos].identifikator;
	for(int i=0;i<V[pos].size();++i)
	{
		ans += " " + input[V[pos][i]].identifikator;
	}
	return ans;
}
void init()
{
	ifstream inFile;
	inFile.open("pom.txt");
	string temp = "";
	while(getline(inFile,temp))
	{
		funkcijeNaredbe.push_back(temp);
		//cout<<temp<<endl;
	}
	inFile.close();
	
	inFile.open("pom2.txt");
	temp = "";
	while(getline(inFile,temp))
	{
		funkcijeDeklaracije.push_back(temp);
		//cout<<temp<<endl;
	}
	inFile.close();
	inFile.open("pom3.txt");
	temp = "";
	while(getline(inFile,temp))
	{
		funkcijeIzrazi.push_back(temp);
		//cout<<temp<<endl;
	}
	inFile.close();
}

int izvrsiIzraz(int pos)
{
	fprintf(stderr,"Jesam izraz.\n");
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeIzrazi.size();++i)
	{
		if(S1.compare(funkcijeIzrazi[i])==0)
		{
			fprintf(stderr,"Nasao %d\n",i);
			return i;
		}
	}
}
int izvrsiStrukturu(int pos)
{
	fprintf(stderr,"Jesam struktura.\n");
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeNaredbe.size();++i)
	{
		if(S1.compare(funkcijeNaredbe[i])==0)
		{
			fprintf(stderr,"Nasao %d\n",i);
			return i;
		}
	}
}
int izvrsiDeklaraciju(int pos)
{
	fprintf(stderr,"Jesam deklaracija.\n");
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeDeklaracije.size();++i)
	{
		if(S1.compare(funkcijeDeklaracije[i])==0)
		{
			fprintf(stderr,"Nasao %d\n",i);
			return i;
		}
	}
}
void start(int pos);
void popisDeklaracija(int pos,int broj)
{}
void popisNaredba(int pos,int broj)
{
	if(broj==0) // <slozena_naredba> ::= L_VIT_ZAGRADA <lista_naredbi> D_VIT_ZAGRADA
	{
		start(V[pos][1]);
	}
	else if(broj==1) // <slozena_naredba> ::= L_VIT_ZAGRADA <lista_deklaracija> <lista_naredbi> D_VIT_ZAGRADA
	{
		start(V[pos][1]);
		start(V[pos][2]);
	}
	else if(broj==2)
	{
	}
}
void popisIzraza(int pos,int broj)
{
	if(broj==0)
	{
		fprintf(stderr,"GRESKA, NE ZNAM STO DA RADIM??\n");
		//Provjera deklaracija
		exit(404);
	}
	else if(broj==1) // <primarni_izraz> = BROJ
	{
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_INT,false,false);
	}
	else if(broj==2) // <primarni_izraz> o= ZNAK
	{
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_CHAR,false,false);
	}
	else if(broj==3) // <primarni_izraz> = NIZ_ZNAKOVA
	{
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_CHAR,true,true); // niz(const(char))
	}
	else if(broj==4) // <primarni_izraz> = L_ZAGRADA <izraz> D_ZAGRADA
	{
		start(V[pos][1]);
		semantika[pos].l_izraz = semantika[V[pos][1]].l_izraz;
		semantika[pos].tip     = semantika[V[pos][1]].tip;
	}
	else if(broj==5) // <postfiks> = <primarni izraz>
	{
		start(V[pos][0]);
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
		semantika[pos].tip     = semantika[V[pos][0]].tip;
	}
	else if(broj==6) // <postfiks_izraz> ::= <postfiks_izraz> L_UGL_ZAGRADA <izraz> D_UGL_ZAGRADA
	{
		start(V[pos][0]);
		start(V[pos][2]);
		if(relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false))==false)
		{fprintf(stderr,"ERROR ERROR ERROR\n");}
		//nije dovrseno
	}
	else if(broj==7) // <postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA D_ZAGRADA
	{
		start(V[pos][0]);
				
	}
	else if(broj==8) // <postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA <lista_argumenata> D_ZAGRADA
	{
		start(V[pos][0]);
		start(V[pos][2]);
	}
	else if(broj==9) // <postfiks_izraz> ::= <postfiks_izraz> (OP_INC
	{
		start(V[pos][0]);
		
	}
	else if(broj==10) // <postfiks_izraz> ::= <postfiks_izraz> (OP_DEC
	{
		start(V[pos][0]);
	}
	else if(broj==11) // <lista_argumenata> ::= <izraz_pridruzivanja>
	{
		start(V[pos][0]);
	}
	else if(broj==12) // <lista_argumenata> ::= <lista_argumenata> ZAREZ <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		start(V[pos][2]);
	}
	else if(broj==13) // <unarni_izraz> ::= <postfiks_izraz>
	{
		start(V[pos][0]);
	}
	else if(broj==14) //  <unarni_izraz> ::= (OP_INC  <unarni_izraz>
	{
		start(V[pos][1]);
	}
	else if(broj==15) //  <unarni_izraz> ::= (OP_DEC  <unarni_izraz>
	{
		start(V[pos][1]);
	}
	else if(broj==16) // <unarni_izraz> ::= <unarni_operator> <cast_izraz>
	{
		start(V[pos][1]);
	}
	else if(broj==17)// <unarni_operator> plus
	{}
	else if(broj==18)// unarni_operator minus
	{}
	else if(broj==19)// <unarni_operator> op_tilda
	{}
	else if(broj==20)// <unarni_operator> OP_NEG
	{}
	else if(broj==21)// <cast_izraz> ::= <unarni_izraz>
	{
		start(V[pos][0]);
	}
	else if(broj==22) // <cast_izraz> ::= L_ZAGRADA <ime_tipa> D_ZAGRADA <cast_izraz>
	{
		start(V[pos][1]);
		start(V[pos][3]);
	}
	else if(broj==23) // <ime_tipa> ::= <specifikator_tipa>
	{
		start(V[pos][0]);
	}
	else if(broj==24) // <ime_tipa> ::= KR_CONST <specifikator_tipa>
	{
		start(V[pos][1]);
	}
	else if(broj==25) // <specifikator_tipa> ::= KR_VOID
	{
		semantika[pos].tip = Tip(KR_VOID,false,false);
	}
	else if(broj==26) // <specifikator_tipa> ::= KR_CHAR
	{
		semantika[pos].tip = Tip(KR_CHAR,false,false);
	}
	else if(broj==27) // <specifikator_tipa> ::= KR_INT
	{
		semantika[pos].tip = Tip(KR_INT,false,false);
	}
	else if(broj==28) // <multiplikativni_izraz> ::= <cast_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==29) // <multiplikativni_izraz> ::= <multiplikativni_izraz> (OP_PUTA ) <cast_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna()
		start(V[pos][2]);	
		//relacijaImplicitna()
		semantika[pos].l_izraz=0;
		semantika[pos].tip = Tip (KR_INT,false,false);
	}
	else if(broj==30) // <multiplikativni_izraz> ::= <multiplikativni_izraz> (OP_DIJELI ) <cast_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna()
		start(V[pos][2]);	
		//relacijaImplicitna()
		semantika[pos].l_izraz=0;
		semantika[pos].tip = Tip (KR_INT,false,false);
	}
	else if(broj==31) // <multiplikativni_izraz> ::= <multiplikativni_izraz> (OP_MOD ) <cast_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna()
		start(V[pos][2]);	
		//relacijaImplicitna()
		semantika[pos].l_izraz=0;
		semantika[pos].tip = Tip (KR_INT,false,false);
	}
	else if(broj==32) // <aditivni_izraz> ::= <multiplikativni_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==33) // <aditivni_izraz> ::= <aditivni_izraz> (MINUS) <multiplikativni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna()
		start(V[pos][2]);
		//relacijaImplicitna()
		semantika[pos].tip = Tip (KR_INT,false,false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==34) // <aditivni_izraz> ::= <aditivni_izraz> (PLUS) <multiplikativni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna()
		start(V[pos][2]);
		//relacijaImplicitna()
		semantika[pos].tip = Tip (KR_INT,false,false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==35) //  <odnosni_izraz> ::= <aditivni_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==36) // <odnosni_izraz> ::= <odnosni_izraz> OP_LT  <aditivni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==37) // <odnosni_izraz> ::= <odnosni_izraz> OP_GT  <aditivni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==38) // <odnosni_izraz> ::= <odnosni_izraz> OP_LTE  <aditivni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==39) // <odnosni_izraz> ::= <odnosni_izraz> OP_GTE  <aditivni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(,Tip(KR_INT,false,false))
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==40) // <jednakosni_izraz> ::= <odnosni_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==41) // <jednakosni_izraz> ::= <jednakosni_izraz> OP_EQ
	{
		start(V[pos][0]);
		//relacijaImplicitna(Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(Tip(KR_INT,false,false))
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==42) // <jednakosni_izraz> ::= <jednakosni_izraz> OP_NEQ
	{
		start(V[pos][0]);
		//relacijaImplicitna(Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(Tip(KR_INT,false,false))
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==43) // <bin_i_izraz> ::= <jednakosni_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==44) // <bin_i_izraz> ::= <bin_i_izraz> OP_BIN_I <jednakosni_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false));
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false));
		
		semantika[pos].tip = Tip(KR_INT,false,false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==45) //<bin_xili_izraz> ::= <bin_i_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==46) // <bin_xili_izraz> ::= <bin_xili_izraz> OP_BIN_XILI <bin_i_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false))
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false))
		
		semantika[pos].tip = Tip ( KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==47) //  <bin_ili_izraz> ::= <bin_xili_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==48) // <bin_ili_izraz> ::= <bin_ili_izraz> OP_BIN_ILI <bin_xili_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(semantika[V[pos][0].tip,Tip(KR_INT, false, false))
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false))
		
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==49) // <log_i_izraz> ::= <bin_ili_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}	
	else if(broj==50) // <log_i_izraz> ::= <log_i_izraz> OP_I <bin_ili_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false))
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false))
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==51) // <log_ili_izraz> ::= <log_i_izraz>
	{
		start(V[pos][0]);
		
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==52) // <log_ili_izraz> ::= <log_ili_izraz> OP_ILI <log_i_izraz>
	{
		start(V[pos][0]);
		//relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false))
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false))
		
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==53) // <izraz_pridruzivanja> ::= <log_ili_izraz>
	{
		start(V[pos][0]);
		
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==54) // <izraz_pridruzivanja> ::= <postfiks_izraz> OP_PRIDRUZI <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		//if(semantika[V[pos][0]].l_izraz == 1) 
		start(V[pos][2]);
		//relacijaImplicitna(semantika[V[pos][2]].tip, semantika[V[pos][0]].tip)
		
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = 0;
	}
	else if(broj==55) // <izraz> ::= <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==56) // <izraz> ::= <izraz> ZAREZ <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		start(V[pos][2]);
		
		semantika[pos].tip = semantika[V[pos][2]].tip;
		semantika[pos].l_izraz = 0;
	}
}
void start(int pos)
{
	if(pos>=input.size())return;
	//cout<<input[pos].identifikator<<endl;
	if(input[pos].jesamLiIzraz())
	{
		int broj = izvrsiIzraz(pos);
		popisIzraza(pos,broj);
	}
	else if(input[pos].jesamLiStrukturaPrograma())
	{
		int broj = izvrsiStrukturu(pos);
		popisNaredba(pos,broj);
	}
	else if(input[pos].jesamLiDeklaracija())
	{
		int broj = izvrsiDeklaraciju(pos);
		popisDeklaracija(pos,broj);
	}
	else if(input[pos].identifikator[0]!='<')
	{
		fprintf(stderr,"Greska: ovo se ne bi trebalo dogoditi.\n");
	}
	else
	{
		fprintf(stderr,"Greska: nas izraz se ne nalazi u polju.\n");
	}
	start(pos+1);
}
int main()
{
	string temp = "";
	int id=0;
	while(getline(cin,temp))
	{
		int dubina = razmak(temp);
		ulaz[dubina].push_back(make_pair(temp, id));
		
		input.push_back(Input(temp));
		if(id==0)
			cout<<temp<<endl;
		if(id==0){++id;continue;}
		//spoji gornjeg sa nize dubine sa trenutnim
		V[ulaz[dubina-1][ulaz[dubina-1].size()-1].second].push_back(id);
		
		
		++id;
	}
	fprintf(stderr,"Size input-a : %d\n",input.size());
	for(int i=0;i<input.size();++i)
	{
		Semantika S = Semantika();
		semantika.push_back(S);
	}
	init();
	start(0);
	return 0;
}
