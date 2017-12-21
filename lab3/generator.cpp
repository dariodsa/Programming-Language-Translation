#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string.h>
#include <string>
#include <utility>
using namespace std;
int KR_INT = 1;
int KR_CHAR = 2;
int KR_VOID = 3;
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
	void definiraj()
	{
		definicija = true;
	}
	void deklaracija()
	{
		deklaracija = true;
	}
};
struct Izraz
{
	int l-izraz;
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
vector<pair<string,int>>ulaz[5000];
vector<int>V[10000];
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
		V[ulaz[dubina-1][ulaz[dubina-1].size()-1].push_back(id);
		++id;
	}
	
	return 0;
}
