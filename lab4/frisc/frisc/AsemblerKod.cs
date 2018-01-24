using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace frisc
{
    /// <summary>
    /// Klasa koja nam sluzi za pretvaranje naredbi iz tekstualne datoteke u listu objekata
    /// </summary>
    class AsemblerKod
    {
        #region Privatne varijable klase
        string datoteka, niz;
        string[] nizStringova;
        List<KodRed> lista = new List<KodRed>();
        KodRed kr;
        List<string> naredbe = new List<string>();
        #endregion

        /// <summary>
        /// Datoteka u kojoj se nalazi kod
        /// </summary>
        /// <param name="datoteka">url datoteke</param>
        public void source (string datoteka)
        {
            this.datoteka = datoteka; 
        }
        /// <summary>
        /// Cita redove iz zadane datoteke i stavlja ih u listu naredbi
        /// </summary>
        /// <returns></returns>
        public List<KodRed> podatci()
        {
            if (datoteka == null)
                return null;

            TextReader tr = new StreamReader(datoteka);

            #region naredbe
            
            naredbe.Add("DW");

            naredbe.Add("ADD");
            naredbe.Add("SUB");
            naredbe.Add("MUL");
            naredbe.Add("DIV");
            naredbe.Add("CMP");
            naredbe.Add("ADDF");
            naredbe.Add("SUBF");
            naredbe.Add("MULF");
            naredbe.Add("DIVF");
            naredbe.Add("CMPF");
            naredbe.Add("AND");
            naredbe.Add("OR");
            naredbe.Add("XOR");
            naredbe.Add("SHR");
            naredbe.Add("SHL");

            naredbe.Add("MOVE");
            naredbe.Add("ITF");
            naredbe.Add("FTI");

            naredbe.Add("LOAD");
            naredbe.Add("LOADB");
            naredbe.Add("LOADH");
            naredbe.Add("STORE");
            naredbe.Add("STOREB");
            naredbe.Add("STOREH");
            naredbe.Add("PUSH");
            naredbe.Add("POP");

            naredbe.Add("JP");
            naredbe.Add("JP_C");
            naredbe.Add("JP_NC");
            naredbe.Add("JP_V");
            naredbe.Add("JP_NV");
            naredbe.Add("JP_N");
            naredbe.Add("JP_NN");
            naredbe.Add("JP_M");
            naredbe.Add("JP_P");
            naredbe.Add("JP_Z");
            naredbe.Add("JP_NZ");
            naredbe.Add("JP_EQ");
            naredbe.Add("JP_NE");
            naredbe.Add("JP_ULE");
            naredbe.Add("JP_UGT");
            naredbe.Add("JP_ULT");
            naredbe.Add("JP_UGE");
            naredbe.Add("JP_SLE");
            naredbe.Add("JP_SGT");
            naredbe.Add("JP_SLT");
            naredbe.Add("JP_SGE");
            naredbe.Add("JR");
            naredbe.Add("JR_C");
            naredbe.Add("JR_NC");
            naredbe.Add("JR_V");
            naredbe.Add("JR_NV");
            naredbe.Add("JR_N");
            naredbe.Add("JR_NN");
            naredbe.Add("JR_M");
            naredbe.Add("JR_P");
            naredbe.Add("JR_Z");
            naredbe.Add("JR_NZ");
            naredbe.Add("JR_EQ");
            naredbe.Add("JR_NE");
            naredbe.Add("JR_ULE");
            naredbe.Add("JR_UGT");
            naredbe.Add("JR_ULT");
            naredbe.Add("JR_UGE");
            naredbe.Add("JR_SLE");
            naredbe.Add("JR_SGT");
            naredbe.Add("JR_SLT");
            naredbe.Add("JR_SGE");
            naredbe.Add("CALL");
            naredbe.Add("CALL_C");
            naredbe.Add("CALL_NC");
            naredbe.Add("CALL_V");
            naredbe.Add("CALL_NV");
            naredbe.Add("CALL_N");
            naredbe.Add("CALL_NN");
            naredbe.Add("CALL_M");
            naredbe.Add("CALL_P");
            naredbe.Add("CALL_Z");
            naredbe.Add("CALL_NZ");
            naredbe.Add("CALL_EQ");
            naredbe.Add("CALL_NE");
            naredbe.Add("CALL_ULE");
            naredbe.Add("CALL_UGT");
            naredbe.Add("CALL_ULT");
            naredbe.Add("CALL_UGE");
            naredbe.Add("CALL_SLE");
            naredbe.Add("CALL_SGT");
            naredbe.Add("CALL_SLT");
            naredbe.Add("CALL_SGE");
            naredbe.Add("RET");
            naredbe.Add("RET_C");
            naredbe.Add("RET_NC");
            naredbe.Add("RET_V");
            naredbe.Add("RET_NV");
            naredbe.Add("RET_N");
            naredbe.Add("RET_NN");
            naredbe.Add("RET_M");
            naredbe.Add("RET_P");
            naredbe.Add("RET_Z");
            naredbe.Add("RET_NZ");
            naredbe.Add("RET_EQ");
            naredbe.Add("RET_NE");
            naredbe.Add("RET_ULE");
            naredbe.Add("RET_UGT");
            naredbe.Add("RET_ULT");
            naredbe.Add("RET_UGE");
            naredbe.Add("RET_SLE");
            naredbe.Add("RET_SGT");
            naredbe.Add("RET_SLT");
            naredbe.Add("RET_SGE");
            naredbe.Add("HALT");
            naredbe.Add("HALT_C");
            naredbe.Add("HALT_NC");
            naredbe.Add("HALT_V");
            naredbe.Add("HALT_NV");
            naredbe.Add("HALT_N");
            naredbe.Add("HALT_NN");
            naredbe.Add("HALT_M");
            naredbe.Add("HALT_P");
            naredbe.Add("HALT_Z");
            naredbe.Add("HALT_NZ");
            naredbe.Add("HALT_EQ");
            naredbe.Add("HALT_NE");
            naredbe.Add("HALT_ULE");
            naredbe.Add("HALT_UGT");
            naredbe.Add("HALT_ULT");
            naredbe.Add("HALT_UGE");
            naredbe.Add("HALT_SLE");
            naredbe.Add("HALT_SGT");
            naredbe.Add("HALT_SLT");
            naredbe.Add("HALT_SGE");
            #endregion

            while (true) 
            {
                niz = tr.ReadLine();
                if (niz == null)
                    break;
                else if (niz == "")
                    continue;

                niz = niz.Replace('\t', ' ');
                niz = niz.Trim();

                kr = new KodRed();
                nizStringova = niz.Split(' ');
                
                kr.Labela = nizStringova[0];

                if (naredbe.Contains(kr.Labela))
                {    
                    kr.Labela = "";
                }
                else
                {
                    niz = niz.Remove(0, kr.Labela.Length + 1);
                }

                nizStringova = niz.Split(' ');
                kr.Naredba.Add(nizStringova[0]);

                if (kr.Naredba[0] != "HALT")
                {
                    //niz = niz.Remove(0, kr.Naredba[0].Length + 1);
                    niz = niz.Remove(0, kr.Naredba[0].Length);

                    nizStringova = niz.Split(',');

                    for (int i = 0; i < nizStringova.Length; i++)
                    {
                        kr.Naredba.Add(nizStringova[i].Trim());
                    }
                }

                lista.Add(kr);
            }

            tr.Close();

            return lista;
        }
    }
}
