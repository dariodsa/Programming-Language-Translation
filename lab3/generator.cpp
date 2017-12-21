#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <utility>
using namespace std;
int KR_INT = 1;
int KR_CHAR = 2;
int KR_VOID = 3;
string izrazi[] = {"<primarni_izraz>","<postfiks_izraz>","<lista_argumenata>","<unarni_izraz>","<unarni_operator>","<cast_izraz>","<ime_tipa>","<specifikator_tipa>","<multiplikativni_izraz>","<aditivni_izraz>","<odnosni_izraz>","<jednakosni_izraz>","<bin_i_izraz>","<bin_xili_izraz>","<bin_ili_izraz>","<log_i_izraz>","<log_ili_izraz>","<izraz_pridruzivanja>","<izraz>"};
string strukture[] = {"<slozena_naredba>","<lista_naredbi>","<naredba>","<izraz_naredba>","<naredba_grananja>","<naredba_petlje>","<naredba_skoka>","<prijevodna_jedinica>","<vanjska_deklaracija>"};
string deklaracije[] = {"<definicija_funkcije>","<lista_parametara>","<deklaracija_parametara>","<lista_deklaracija>","<deklaracija>","<lista_init_deklaratora>","<init_deklarator>","<izravni_deklarator>","<inicijalizator>","<lista_izraza_pridruzivanja>"};
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
struct Izraz
{
	int l_izraz;
	Tip tip;
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
	string indentifikator;
	string ostalo;
	int linija;
	Input(string input)
	{
		int pos=0;
		for(pos=0;pos<input.length() && input[pos]!=' ';++pos);
		
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
		
		for(int i=pos+1;i<linija.length();++i)
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
		
		for(int i=pos+1;i<linija.length();++i)
		{
			ostalo += input[i];
		}	
	}
	bool jesamLiIzraz()
	{
		for(int i=0;i<izrazi.length();++i)
		{
			if(compare(indentifikator,izrazi[i])==0)
				return true;
		}
		return false;
	}
	bool jesamLiStrukturaPrograma()
	{
		for(int i=0;i<strukture.length();++i)
		{
			if(compare(indentifikator,strukture[i])==0)
				return true;
		}
		return false;
	}
	bool jesamLiDeklaracija()
	{
		for(int i=0;i<deklaracije.length();++i)
		{
			if(compare(indentifikator,deklaracije[i])==0)
				return true;
		}
		return false;
	}
};
vector<pair<string,int> >ulaz[5000];
vector<int>V[10000];
vector<Input>input;
void funkcijeStrukture(int pos)
{
	for(int i=0;i<
}
void start(int pos)
{
	if(input[pos].jesamLiIzraz())
	{
		
	}
	else if(input[pos].jesamLiStrukturaPrograma())
	{
		funkcijeStrukture(pos);
	}
	else if(input[pos].jesamLiDeklaracija())
	{
		
	}
	else if(input[pos].identifikator[0]!='<')
	{
		
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
		if(id==0)continue;
		//spoji gornjeg sa nize dubine sa trenutnim
		V[ulaz[dubina-1][ulaz[dubina-1].size()-1].second].push_back(id);
		input.push_back(Input(temp));
		++id;
	}
	start(0);
	return 0;
}
