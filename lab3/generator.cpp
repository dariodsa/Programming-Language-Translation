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
	bool jesamFunkcija;
	
	string ime;
	vector<Tip>tipovi;
	bool deklaracija;
	bool definicija;
	Tip(){}
	Tip(int _tip,bool _polje, bool _konstanta)
	{
		tip = _tip;
		polje = _polje;
		konstanta = _konstanta;
		jesamFunkcija = false;
	}
	bool operator==(Tip A)
	{
		return tip==A.tip && polje==A.polje && konstanta==A.konstanta;
	}
	Tip(string _ime, vector<Tip>_tipovi)
	{
		jesamFunkcija = true;
		ime = _ime;
		tipovi.clear();
		for(int i=0;i<_tipovi.size();++i)
			tipovi.push_back(_tipovi[i]);
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
	Tip tip;
	Varijabla(string _ime,Tip _tip)
	{
		ime = _ime;
		tip = _tip;
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
	Tip tip;
	Tip ntip;
	string ime;
	int br_elem;
	vector<string>imena;
	vector<Tip>tipovi;
	Semantika()
	{
		 
	};
};
struct Djelokrug
{
	int parent;
	vector<Varijabla>varijable;
	Djelokrug(int _parent)
	{
		parent = _parent;
	}
};
int AKTIVNI_DJELOKRUG=0;
int AKTIVNE_PETLJE = 0;
vector<Djelokrug>djelokrugovi;
vector<pair<string,int> >ulaz[5000];
vector<int>V[10000];
vector<Input>input;
vector<Semantika>semantika;
bool usporedbaParametara(vector<Tip> tipoviFunkcije, vector<Tip>tipoviParam)
{
	for(int i=0;i<tipoviParam.size();++i)
	{
		if(!(tipoviParam[i] == tipoviFunkcije[i]))
		{
			return false;
		}
	}
	return true;
}
bool findTheVariable(Varijabla varijabla,int poc)
{
	for(int i=0;i<djelokrugovi[poc].varijable.size();++i)
	{
		if(djelokrugovi[poc].varijable[i].ime.compare(varijabla.ime)==0)
			return true;
	}
	if(djelokrugovi[poc].parent==-1)return false;
	return findTheVariable(varijabla, djelokrugovi[poc].parent);
}
string getStringToCompare(int pos)
{
	string ans = input[pos].identifikator;
	for(int i=0;i<V[pos].size();++i)
	{
		ans += " " + input[V[pos][i]].identifikator;
	}
	return ans;
}
void ispis(int pos)
{
	string temp = getStringToCompare(pos);
	cout<<"Pogreska : "<<endl;
	cout<<pos<<". => "<<temp<<endl;
	
	exit(0);
}
bool castPromjena(Tip A,Tip B)
{
	//4.3.1
	if(A == Tip(KR_INT, false, false) && A == B)return true;
	if(A == Tip(KR_CHAR, false, false) && A == B)return true;
	if(A == Tip(KR_INT, false, false) && B == Tip(KR_CHAR, false, false))
		return true;
	
	return false;
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
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeIzrazi.size();++i)
	{
		if(S1.compare(funkcijeIzrazi[i])==0)
		{
			return i;
		}
	}
	return -1;
}
int izvrsiStrukturu(int pos)
{
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeNaredbe.size();++i)
	{
		if(S1.compare(funkcijeNaredbe[i])==0)
		{
			return i;
		}
	}
	return -1;
}
int izvrsiDeklaraciju(int pos)
{
	string S1 = getStringToCompare(pos);
	for(int i=0;i<funkcijeDeklaracije.size();++i)
	{
		if(S1.compare(funkcijeDeklaracije[i])==0)
		{
			return i;
		}
	}
	return -1;
}
void start(int pos);
void popisDeklaracija(int pos,int broj)
{
	if(broj==0) // <definicija_funkcije> ::= <ime_tipa> IDN L_ZAGRADA KR_VOID D_ZAGRADA <slozena_naredba>
	{
		start(V[pos][0]);
		start(V[pos][1]);
		if(semantika[V[pos][0]].tip.konstanta == true)
		{
			ispis(pos);
			exit(0);
		}
		//provjera definiranih ime funkcije
		/*bool found = false;
		for(int i=0;i<djelokrugovi[AKTIVNI_DJELOKRUG].varijable.size();++i)
		{
			if(djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].definicija==false && 
			   djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].ime.compare(semantika[V[pos][1]].ime)!=0)
			   {}
			else 
			{
				found = true;
				break;
			}
		}
		if(found)
		{
			printf("ERROR\n");exit(0);
		}
		//provjera deklariranih
		found = false;
		for(int i=0;i<djelokrugovi[AKTIVNI_DJELOKRUG].varijable.size();++i)
		{
			if(djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].deklaracija && 
			   djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].ime.compare(semantika[V[pos][1]].ime)==0)
			{
				if(djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].tipovi[0]==Tip(KR_VOID, false, false) && 
				   djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].tipovi[1]==semantika[V[pos][0]].tip &&
				   djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i].tipovi.size()==2)
				   {}
				else found = true;
			}
		}
		if(found)
		{
			printf("ERROR\n");exit(0);
		}
		//zabiljezi deklaraciju i definiciju
		vector<Tip>_tipovi;
		_tipovi.push_back(Tip(KR_VOID, false, false));
		_tipovi.push_back(semantika[V[pos][0]].tip);
		Funkcija F = Funkcija(semantika[V[pos][1]].ime,_tipovi);
		F.definicija = true;
		F.deklaracija = true;
		funkcije.push_back(F);
		
		start(V[pos][5]);*/
	}
	else if(broj==1) // <definicija_funkcije> ::= <ime_tipa> IDN L_ZAGRADA <lista_parametara> D_ZAGRADA <slozena_naredba>
	{
		start(V[pos][0]);
		if(semantika[V[pos][0]].tip.konstanta == true)
		{
			printf("ERROR\n");
			exit(0);
		}
		bool found = false;
		/*for(int i=0;i<funkcije.size();++i)
		{
			if(funkcije[i].ime.compare(semantika[V[pos][1]].ime)==0 && funkcije[i].definicija==true)
			{
				found = true;
			}
		}*/
		if(found)
		{
			printf("ERROR\n");
			exit(0);
		}
		start(V[pos][3]);
		start(V[pos][5]);
		
	}
	else if(broj==2) // <lista_parametara> ::= <deklaracija_parametra>
	{
		start(V[pos][0]);
		semantika[pos].tipovi.push_back(semantika[V[pos][0]].tip);
		semantika[pos].imena.push_back(semantika[V[pos][0]].ime);
	}
	else if(broj==3) // <lista_parametara> ::= <lista_parametara> ZAREZ <deklaracija_parametra>
	{
		start(V[pos][0]);
		start(V[pos][2]);
		bool found = false;
		for(int i=0;i<semantika[V[pos][0]].imena.size();++i)
		{
			if(semantika[V[pos][2]].ime.compare(semantika[V[pos][0]].imena[i])==0)
				found = true;
		}
		if(found)
		{
			printf("ERROR\n");exit(0);
		}
		semantika[pos].tipovi.clear();
		semantika[pos].imena.clear();
		for(int i=0;i<semantika[V[pos][0]].imena.size();++i)
		{
			semantika[pos].imena.push_back(semantika[V[pos][0]].imena[i]);
			semantika[pos].tipovi.push_back(semantika[V[pos][0]].tipovi[i]);
		}
		semantika[pos].imena.push_back(semantika[V[pos][2]].ime);
		semantika[pos].tipovi.push_back(semantika[V[pos][2]].tip);
	}
	else if(broj==4) // <deklaracija_parametra> ::= <ime_tipa> IDN
	{
		start(V[pos][0]);
		start(V[pos][1]);
		if(semantika[V[pos][0]].tip.tip==KR_VOID)
		{
			printf("ERROR\n");exit(0);
		}
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].ime = semantika[V[pos][1]].ime;
		//semantika[pos].ime = 
	}
	else if(broj==5) // <deklaracija_parametra> ::= <ime_tipa> IDN L_UGL_ZAGRADA D_UGL_ZAGRADA
	{
		start(V[pos][0]);
		start(V[pos][1]);
		if(semantika[V[pos][0]].tip.tip==KR_VOID)
		{
			printf("ERROR\n");exit(0);
		}
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].ime = semantika[V[pos][1]].ime;
		
	}
	else if(broj==6) // <lista_deklaracija> ::= <deklaracija>
	{
		start(V[pos][0]);
	}
	else if(broj==7) // <lista_deklaracija> ::= <lista_deklaracija> <deklaracija>
	{
		start(V[pos][0]);
		start(V[pos][1]);
	}
	else if(broj==8) // <deklaracija> ::= <ime_tipa> <lista_init_deklaratora> TOCKAZAREZ
	{
		start(V[pos][0]);
		semantika[V[pos][1]].tip = semantika[V[pos][0]].tip;
		start(V[pos][1]);
	}
	else if(broj==9) // <lista_init_deklaratora> ::= <init_deklarator>
	{
		semantika[V[pos][0]].tip = semantika[pos].tip;
		start(V[pos][0]);
	}
	else if(broj==10) // <lista_init_deklaratora>1 ::= <lista_init_deklaratora>2 ZAREZ <init_deklarator>
	{
		semantika[V[pos][0]].tip = semantika[pos].tip;
		start(V[pos][0]);
		semantika[V[pos][2]].tip = semantika[pos].tip;
		start(V[pos][2]);
	}
	else if(broj==11) // <init_deklarator> ::= <izravni_deklarator>
	{
		semantika[V[pos][0]].tip = semantika[pos].tip;
		start(V[pos][0]);
		if(semantika[V[pos][0]].tip.konstanta || 
		  (semantika[V[pos][0]].tip.konstanta && semantika[V[pos][0]].tip.polje))
		{
			ispis(pos);
		}
	}	
	else if(broj==12) // <init_deklarator> ::= <izravni_deklarator> OP_PRIDRUZI <inicijalizator>
	{
		start(V[pos][0]);
		start(V[pos][2]);
	}
	else if(broj==13) // <izravni_deklarator> ::= IDN
	{
		start(V[pos][0]);
		if(semantika[pos].tip.tip==KR_VOID)
			ispis(pos);
		bool find = false;
		for(int i=0;i<djelokrugovi[AKTIVNI_DJELOKRUG].varijable.size();++i)
		{
			Varijabla temp = djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i];
			if(temp.ime.compare(semantika[V[pos][0]].ime)==0)
				find = true;
		}
		if(find) ispis(pos);
		Varijabla _varijabla = Varijabla(semantika[V[pos][0]].ime,semantika[pos].tip);
		djelokrugovi[AKTIVNI_DJELOKRUG].varijable.push_back(_varijabla);
	}
	else if(broj==14) // <izravni_deklarator> ::= IDN L_UGL_ZAGRADA BROJ D_UGL_ZAGRADA
	{
		if(semantika[pos].tip.tip==KR_VOID)ispis(pos);
		start(V[pos][0]);
		bool find = false;
		for(int i=0;i<djelokrugovi[AKTIVNI_DJELOKRUG].varijable.size();++i)
		{
			Varijabla temp = djelokrugovi[AKTIVNI_DJELOKRUG].varijable[i];
			if(temp.ime.compare(semantika[V[pos][0]].ime)==0)
				find = true;
		}
		if(find) ispis(pos);
		if(!rasponBroja(input[V[pos][2]].ostalo,"1","1024"))ispis(pos);
		
		Tip _tip;
		_tip.tip =  semantika[pos].tip.tip;
		_tip.polje = true;
		_tip.konstanta = semantika[pos].tip.konstanta;
		Varijabla _varijabla = Varijabla(semantika[V[pos][0]].ime,_tip);
		djelokrugovi[AKTIVNI_DJELOKRUG].varijable.push_back(_varijabla);
		
	}
	else if(broj==15) //  <izravni_deklarator> ::= IDN L_ZAGRADA KR_VOID D_ZAGRADA
	{
	}
	else if(broj==16) // <izravni_deklarator> ::= IDN L_ZAGRADA <lista_parametara> D_ZAGRADA
	{
		start(V[pos][2]);
	}
	else if(broj==17) //  <inicijalizator> ::= <izraz_pridruzivanja>
	{
		start(V[pos][0]);
	}
	else if(broj==18) // <inicijalizator> ::= L_VIT_ZAGRADA <lista_izraza_pridruzivanja> D_VIT_ZAGRADA
	{
		start(V[pos][1]);
	}
	else if(broj==19) // <lista_izraza_pridruzivanja> ::= <izraz_pridruzivanja>
	{
		start(V[pos][0]);
	}
	else if(broj==20) // <lista_izraza_pridruzivanja> ::= <lista_izraza_pridruzivanja> ZAREZ <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		start(V[pos][2]);
	}
	else
	{
		cout<<getStringToCompare(pos)<<endl;
		fprintf(stderr,"ERROR: Nisam nasao dobru deklaraciju!\n");
		exit(0);
	}
	
}
void popisNaredba(int pos,int broj)
{
	printf("%d %d\n",pos,broj);
	if(broj==0) // <slozena_naredba> ::= L_VIT_ZAGRADA <lista_naredbi> D_VIT_ZAGRADA
	{
		Djelokrug D = Djelokrug(AKTIVNI_DJELOKRUG);
		djelokrugovi.push_back(D);
		AKTIVNI_DJELOKRUG = djelokrugovi.size() - 1;
		start(V[pos][1]);
		AKTIVNI_DJELOKRUG = djelokrugovi[AKTIVNI_DJELOKRUG].parent;
	}
	else if(broj==1) // <slozena_naredba> ::= L_VIT_ZAGRADA <lista_deklaracija> <lista_naredbi> D_VIT_ZAGRADA
	{
		Djelokrug D = Djelokrug(AKTIVNI_DJELOKRUG);
		djelokrugovi.push_back(D);
		AKTIVNI_DJELOKRUG = djelokrugovi.size() - 1;
		start(V[pos][1]);
		start(V[pos][2]);
		AKTIVNI_DJELOKRUG = djelokrugovi[AKTIVNI_DJELOKRUG].parent;
	}
	else if(broj==2) // <lista_naredbi> ::= <naredba>
	{
		start(V[pos][0]);
	}
	else if(broj==3) // <lista_naredbi> ::= <lista_naredbi> <naredba>
	{
		start(V[pos][0]);
		start(V[pos][1]);
	}
	else if(broj==4) // <naredba> <slozena_naredba>
	{
		start(V[pos][0]);
	}
	else if(broj==5) // <naredba> <izraz_naredba>
	{
		start(V[pos][0]);
	}
	else if(broj==6) // <naredba> <naredba_grananja>
	{
		start(V[pos][0]);
	}
	else if(broj==7) // <naredba> <naredba_petlje>
	{
		start(V[pos][0]);
	}
	else if(broj==8) // <naredba> <naredba_skoka>
	{
		start(V[pos][0]);
	}
	else if(broj==9) // <izraz_naredba> ::= TOCKAZAREZ
	{
		semantika[pos].tip = Tip(KR_INT, false, false);
	}
	else if(broj==10) // <izraz_naredba> ::= <izraz> TOCKAZAREZ
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
	}
	else if(broj==11) // KR_IF L_ZAGRADA <izraz> D_ZAGRADA <naredba>
	{
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip, Tip(KR_INT, false, false)))
		{
			printf("ERROR\n");exit(0);
		}
		start(V[pos][4]);
	}
	else if(broj==12) //<naredba_grananja> ::= KR_IF L_ZAGRADA <izraz> D_ZAGRADA <naredba>1 KR_ELSE <naredba>2
	{
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip, Tip(KR_INT, false, false)))
		{
			printf("ERROR\n");exit(0);
		}
		start(V[pos][4]);
		start(V[pos][6]);
	}
	else if(broj==13) // <naredba_petlje> ::= KR_WHILE L_ZAGRADA <izraz> D_ZAGRADA <naredba>
	{
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip, Tip(KR_INT,false,false)))
		{
			printf("ERROR\n");exit(0);
		}
		AKTIVNE_PETLJE++;
		start(V[pos][4]);
		AKTIVNE_PETLJE--;
	}
	else if(broj==14) // <naredba_petlje> ::= KR_FOR L_ZAGRADA <izraz_naredba>1 <izraz_naredba>2 D_ZAGRADA <naredba>
	{
		start(V[pos][2]);
		start(V[pos][3]);
		if(!relacijaImplicitna(semantika[V[pos][3]].tip, Tip(KR_INT, false, false)))
		{
			printf("ERROR\n");exit(0);
		}
		AKTIVNE_PETLJE++;
		start(V[pos][5]);
		AKTIVNE_PETLJE--;
	}
	else if(broj==15)  //KR_FOR L_ZAGRADA <izraz_naredba>1 <izraz_naredba>2 <izraz> D_ZAGRADA <naredba>
	{
		start(V[pos][2]);
		start(V[pos][3]);
		if(!relacijaImplicitna(semantika[V[pos][3]].tip, Tip(KR_INT, false, false)))
		{
			printf("ERROR\n");exit(0);
		}
		start(V[pos][4]);
		AKTIVNE_PETLJE++;
		start(V[pos][6]);
		AKTIVNE_PETLJE--;
	}
	else if(broj==16) // <naredba_skoka> ::= (KR_CONTINUE  TOCKAZAREZ
	{
		if(AKTIVNE_PETLJE == 0)
		{
			printf("ERROR\n");exit(0);
		}
		// naredba se nalazi unutar petlje ili bloka koji je u petlji 
	}
	else if(broj==23) // <naredba_skoka> ::= (KR_BREAK  TOCKAZAREZ
	{
		if(AKTIVNE_PETLJE == 0)
		{
			printf("ERROR\n");exit(0);
		}
	}
	else if(broj==17) // <naredba_skoka> ::= KR_RETURN TOCKAZAREZ
	{
		//nalazi se unutar funkcije params -> void
	}
	else if(broj==18) // <naredba_skoka> ::= KR_RETURN <izraz> TOCKAZAREZ
	{
		start(V[pos][1]);
		//naredba se nalazi unutar funkcije tipa params -> pov i vrijedi izraz.tip ~~ pov
	}
	else if(broj==19) // <prijevodna_jedinica> ::= <vanjska_deklaracija>
	{
		start(V[pos][0]);
	}
	else if(broj==20) // <prijevodna_jedinica> ::= <prijevodna_jedinica> <vanjska_deklaracija>
	{
		start(V[pos][0]);
		start(V[pos][1]);
	}
	else if(broj==21) // <vanjska deklaracija> <definicija_funkcije>
	{
		start(V[pos][0]);
	}
	else if(broj==22) // <vanjska deklaracija> <deklaracija>
	{
		start(V[pos][0]);
	}
	else 
	{
		fprintf(stderr, "Nisam nasao produkciju u naredbenoj strukuturi\n");
		exit(0);
	}
}
void popisIzraza(int pos,int broj)
{
	if(broj==0) // <primarni_izraz> = IDN
	{
		start(V[pos][0]);
		Varijabla Va = Varijabla(semantika[V[pos][0]].ime,Tip());
		if(!findTheVariable(Va,AKTIVNI_DJELOKRUG))
		{
			printf("ERROR NISAM NASAO VARIJABLU\n");exit(0);
		}
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
		
		fprintf(stderr,"GRESKA, NE ZNAM STO DA RADIM??\n");
		//Provjera deklaracija
		//exit(0);
	}
	else if(broj==1) // <primarni_izraz> = BROJ
	{
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_INT,false,false);
		if(!rasponBroja("-2147483648","2147483647"))
			ispis(pos);
	}
	else if(broj==2) // <primarni_izraz> o= ZNAK
	{
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_CHAR,false,false);
		if(input[pos].ostalo.length()>2)ispis(pos);
		if(input[pos].ostalo.length()==2 && 
		  (input[pos].ostalo=="\t" || input[pos].ostalo=="\n" || 
		   input[pos].ostalo=="\0" || input[pos].ostalo=="\\" ||
		   (input[pos].ostalo[0]==92 && input[pos].ostalo[1]==39)
		    || (input[pos].ostalo[0]==92 && input[pos].ostalo[2]==34))){}
		else
			ispis(pos);
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
		if(semantika[V[pos][0]].tip.polje==false)
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false))==false)
		{
			ispis(pos);
		}
		semantika[pos].tip = Tip(semantika[V[pos][0]].tip.tip, false, false);
		semantika[pos].l_izraz=semantika[V[pos][0]].tip.konstanta?0:1;
		//nije dovrseno
	}
	else if(broj==7) // <postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA D_ZAGRADA
	{
		start(V[pos][0]);
		
		if(!(semantika[V[pos][0]].tip.jesamLiFunkcija && 
		   semantika[V[pos][0]].tip.tipovi[0].tip==KR_VOID && 
		   semantika[V[pos][0]].tip.tipovi.size()==2))
		   {
		   	ispis(pos);
		   }
		semantika[pos].tip     = semantika[V[pos][0]].tip.tipovi[semantika[V[pos][0]].tip.tipovi.size()-1];
		semantika[pos].l_izraz = 0;
		//semantika[pos].tip = ;
	}
	else if(broj==8) // <postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA <lista_argumenata> D_ZAGRADA
	{
		start(V[pos][0]);
		start(V[pos][2]);
		
		if(!(semantika[V[pos][0]].tip.jesamLiFunkcija && 
		     semantika[V[pos][0]].tip.tipovi.size() == semantika[V[pos][2]].tipovi.size()
		    )
	    {
	    	ispis(pos);
	    }
		
		if(!usporedbaParametar(semantika[V[pos][0]].tip.tipovi,
		                       semantika[V[pos][2]].tipovi))
        {
	    	ispis(pos);
	    }
		
		
		semantika[pos].tip = semantika[V[pos][0]].tip.tipovi[semantika[V[pos][0]].tip.tipovi.size()-1];
		semantika[pos].l_izraz = 0;
	}
	else if(broj==9) // <postfiks_izraz> ::= <postfiks_izraz> (OP_INC
	{
		start(V[pos][0]);
		if(!(relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)) && 
		   semantika[V[pos][0]].l_izraz == 1))
		{
			ispis(pos);
		}
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==10) // <postfiks_izraz> ::= <postfiks_izraz> (OP_DEC
	{
		start(V[pos][0]);
		if(!(relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)) && 
		   semantika[V[pos][0]].l_izraz == 1))
		{
			ispis(pos);
		}
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==11) // <lista_argumenata> ::= <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		semantika[pos].tipovi.clear();
		semantika[pos].tipovi.push_back(semantika[V[pos][0]].tip);
	}
	else if(broj==12) // <lista_argumenata> ::= <lista_argumenata> ZAREZ <izraz_pridruzivanja>
	{
		start(V[pos][0]);
		start(V[pos][2]);
		semantika[pos].tipovi.clear();
		for(int k=0;k<semantika[V[pos][0]].tipovi.size();++k)
		{
			semantika[pos].tipovi.push_back(semantika[V[pos][0]].tipovi[k]);
		}
		semantika[pos].tipovi.push_back(semantika[V[pos][2]].tip);
	}
	else if(broj==13) // <unarni_izraz> ::= <postfiks_izraz>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==14) //  <unarni_izraz> ::= (OP_INC  <unarni_izraz>
	{
		start(V[pos][1]);
		if(!(semantika[V[pos][1]].l_izraz==1 && relacijaImplicitna(semantika[V[pos][1]].tip,Tip(KR_INT, false, false))))
		{
			ispis(pos);
		}
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_INT, false, false);
	}
	else if(broj==15) //  <unarni_izraz> ::= (OP_DEC  <unarni_izraz>
	{
		start(V[pos][1]);
		if(!(semantika[V[pos][1]].l_izraz==1 && relacijaImplicitna(semantika[V[pos][1]].tip,Tip(KR_INT, false, false))))
		{
			ispis(pos);
		}
		semantika[pos].l_izraz = 0;
		semantika[pos].tip = Tip(KR_INT, false, false);
	}
	else if(broj==16) // <unarni_izraz> ::= <unarni_operator> <cast_izraz>
	{
		start(V[pos][1]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
			exit(0);
		}
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
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
		semantika[pos].tip =semantika[V[pos][0]].tip;
		semantika[pos].l_izraz = semantika[V[pos][0]].l_izraz;
	}
	else if(broj==22) // <cast_izraz> ::= L_ZAGRADA <ime_tipa> D_ZAGRADA <cast_izraz>
	{
		start(V[pos][1]);
		start(V[pos][3]);
		if(!castPromjena(semantika[V[pos][3]].tip,semantika[V[pos][1]].tip))
		{
			ispis(pos);
			exit(0);
		}
		semantika[pos].tip = semantika[V[pos][1]].tip;
		semantika[pos].l_izraz = 0;
	}
	else if(broj==23) // <ime_tipa> ::= <specifikator_tipa>
	{
		start(V[pos][0]);
		semantika[pos].tip = semantika[V[pos][0]].tip;
	}
	else if(broj==24) // <ime_tipa> ::= KR_CONST <specifikator_tipa>
	{
		start(V[pos][1]);
		if(semantika[V[pos][1]].tip.tip==KR_VOID)
		{
			ispis(pos);
		}
		Tip A = Tip(2,false,false);
		semantika[pos].tip = Tip(semantika[V[pos][0]].tip.tip,semantika[V[pos][0]].tip.polje,true);
		
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);	
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		semantika[pos].l_izraz=0;
		semantika[pos].tip = Tip (KR_INT,false,false);
	}
	else if(broj==30) // <multiplikativni_izraz> ::= <multiplikativni_izraz> (OP_DIJELI ) <cast_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);	
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		semantika[pos].l_izraz=0;
		semantika[pos].tip = Tip (KR_INT,false,false);
	}
	else if(broj==31) // <multiplikativni_izraz> ::= <multiplikativni_izraz> (OP_MOD ) <cast_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);	
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		semantika[pos].tip = Tip (KR_INT,false,false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==34) // <aditivni_izraz> ::= <aditivni_izraz> (PLUS) <multiplikativni_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==37) // <odnosni_izraz> ::= <odnosni_izraz> OP_GT  <aditivni_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==38) // <odnosni_izraz> ::= <odnosni_izraz> OP_LTE  <aditivni_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
		semantika[pos].tip = Tip(KR_INT,false,false);	
		semantika[pos].l_izraz = 0;
	}
	else if(broj==39) // <odnosni_izraz> ::= <odnosni_izraz> OP_GTE  <aditivni_izraz>
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
	}
	else if(broj==42) // <jednakosni_izraz> ::= <jednakosni_izraz> OP_NEQ
	{
		start(V[pos][0]);
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
		semantika[pos].tip = Tip(KR_INT, false, false);
		semantika[pos].l_izraz = 0;
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT,false,false)))
		{
			ispis(pos);
		}
		
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
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
		if(!relacijaImplicitna(semantika[V[pos][0]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip,Tip(KR_INT, false, false)))
		{
			ispis(pos);
		}
		
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
		if(semantika[V[pos][0]].l_izraz != 1) 
		{
			ispis(pos);
		}
		start(V[pos][2]);
		if(!relacijaImplicitna(semantika[V[pos][2]].tip, semantika[V[pos][0]].tip))
		{
			ispis(pos);
		}
		
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
	else
	{
		fprintf(stderr, "ERROR:  izraz se ne nalazi u izrazima.\n");
		exit(0);
	}
}
void start(int pos)
{
	if(pos>=input.size())return;
	cout<<getStringToCompare(pos)<<endl;
	//cout<<input[pos].identifikator<<endl;
	printf("Pos %d\n",pos);
	for(int i=0;i<V[pos].size();++i)
	{
		printf("djeca %d\n",V[pos][i]);
	}
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
		if(input[pos].identifikator.compare("IDN")==0)
		{
			semantika[pos].ime=input[pos].ostalo;
			for(int k=0;k<djelokrugovi[AKTIVNI_DJELOKRUG].varijable.size();++k)
			{
				if(djelokrugovi[AKTIVNI_DJELOKRUG].varijable[k].ime.compare(semantika[pos].ime)==0)
				{
					if((djelokrugovi[AKTIVNI_DJELOKRUG].varijable[k].tip.tip==KR_INT ||
					    djelokrugovi[AKTIVNI_DJELOKRUG].varijable[k].tip.tip==KR_CHAR) && 
						djelokrugovi[AKTIVNI_DJELOKRUG].varijable[k].tip.konstanta==false)
						{
							semantika[pos].l_izraz = 1;
							break;
						}
					else semantika[pos].l_izraz = 0;
				}
			}
		}
		else
			fprintf(stderr,"Greska: ovo se ne bi trebalo dogoditi.\n");
	}
	else
	{
		fprintf(stderr,"Greska: nas izraz se ne nalazi u polju.\n");
	}
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
	Djelokrug D = Djelokrug(-1);
	djelokrugovi.push_back(D);
	AKTIVNI_DJELOKRUG = 0;
	start(0);
	return 0;
}
