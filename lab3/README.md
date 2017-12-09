# Opcenito
- testiranje 
   `LA <program.c | SA | SemantickiAnalizator`
- treba se obici generativno stablo i na standOut ispisati produkciju u kojoj je otkrivena prva semanti?ka pogreška
- ne treba se provoditi nikakav postupak oporavka, nego se prestaje sa radom 
- ako je stablo ispravno na izlaz se ne ispisuje nista
- nema vise direktorija analizator                     a
## Jezik ppjC
## Tipovi
### Brojevi tipovi
- char [0..255] 
- int  [- 2^31 .. 2^31- 1]
- oznaka u uputi je T         
a
### Const- kvalifikator
- const ( uniformni znak KR_CONST)
- `const(T)`
- ili u uputi kao `T ili const(T)`  
### Nizovi(array)
- isklju?ivo brojevni tipovi ( const ili ne)
- ne postoji višedimenizionalni niz ( osigurano u sintaksnoj analizi)
- `niz(X)`                    
### Void  
- **isklju?ivo** povratni tip funckije  
### Tip funkcija  
- odre?en tipom povratne vrijednosti i svih formalnih parametara  
- `funkcija([char,char]- >int` , `funckija([int]- >void)`  , `funkcija(void- >void)`  
- tipovi parametara su organizirani u **listu**   
- ako funkcija nema parametara to je ozna?eno sa void , **a ne sa [void]**  
### Implicitne promjene tipa  
- sve const(T) može se implicitno pretvoriti u vrijednost tipa T  
- **Važno je uo?iti da se promjene tipa odvijaju nad vrijednostima a ne na tipu varijable, koji se ne može promijeniti**  
- tako?er vrijedi obrat, tj. sve vrijednosti tipa int || char mogu ici u const  
- svaki char može ici u int, pri tome se njegova vrijednost ne mijenja  
- niz(T) gdje T nije const može i?i u niz(const(T))  
### Logi?ki podtip int- a ( Boolean)  
- nije poseban  
- (0 || 1)  
### Eksplicitne promjene tipa  
- dozvoljene samo nad vrijednostima brojevnih tipova => zadaje se sa cast operatorom  
-  samo iz int u char, ako je int u domeni chara super  
-  **ako ne ....**
## Konstante
- brojevne konstante ( uniformni znak BROJ) , tip int     
- znakovne konstante ( uniformni znak ZNAK) , tip char, ascii kod    
- za znakovne potrebna provjera ispravnosti                                         
- `â€™\tâ€™, â€™\nâ€™, â€™\0â€™, â€™\â€™â€™(jednostruki navodnik), â€™\"â€™ (dvostruki navodnik) i â€™\\â€™`    
- svi ostali sa jednostrukim navodicima -  **greska** ako im je duljina veca od 1       
- konstantni znakovni niz ( NIZ_ZNAKOVA) tipa niz(const(char))                            
- **implicitno završavaju sa '\0'**                                                         
- **pazi, fusnota 9**                                                                         
## Djelokrug deklaracija i životni vijek varijabli
- stati?ko pravilo **bez ugnijež?enih procedura**                                              
- ali podržava ugnjež?ene blokove                                                               
### Globalne varijable                                                                          
- život po?inje u trenutku definicije     
- završava krajem programa         
### Lokalne za neki blok
- to uklju?uje i parametre funkcija   
- život po?inje definicijom      
- završava krajem tog bloka        
- **FUSNOTA 10**                       
- **Primjeri 4.2 - 4.5** ++ **4.6**      
## Funkcije                                 
- mora imati funkciju int main(void) => ulazna to?ka za izvo?enje programa  
- sve funkcije koje ne primaju argumente => `f(void)` , `f()` je **neispravno** => sintaksna  
- prije korištenja mora biti deklarirana, pri cemu je njena definicija i deklaracija            
- definirana **to?no** jednom                                                                     
- deklarirana **više ili jednom** puta                                                              
- sve deklaracije moraju imati identicne povratne tipove i tipove formalnih parametara                
- ignorirati imena varijabla u deklaraciji ( gledati samo tipove)                                       
- **rekurzije**                                                                                           
### Povratni tip                                                                                           
- char ili int bez const   
- ili void                   
- pogreska je kada void funkcija nesto pokusa vratiti  
- ako funkcija char f() ili int f() nista ne vrati **to nije semanti?ka pogreska**  
- ako funkcija vraca neku vrijednost tip se mora moci implicitno pretvoriti u deklarirani povratni tip funkcije  
- char -> int, ali obrati **ne vrijedi**                                                                           
### Argumenti                                                                                                        
- vrijednost argumenata brojevnog tipa prenosi se u odgovarajuce parametre                                             
- ako je niz X a[] , onda se prenosi adresa                                                                              

```c++
void f(int x,int a[]){                                                                                                    
   x=x+1;
   a[0]=a[0]+1;
 }
 int main(void){
   int x=3;
   int a[8] = {0};
   f(x,a); // x = 3, a[0] = 1
   return 0;
 }
```           

## Operatori    
- a               
      
