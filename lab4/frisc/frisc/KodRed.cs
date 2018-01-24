using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace frisc
{
    /// <summary>
    /// Klasa koja nam predstavlja jedan red memorije(labela, memorija, naredba)
    /// </summary>
    class KodRed
    {
        #region Privatne varijable klase
        string label;
        List<string> naredba = new List<string>();
        string niz;
        #endregion

        /// <summary>
        /// Konstruktor za prazan objekt
        /// </summary>
        public KodRed()
        {
            label = "";
        }

        /// <summary>
        /// Konstruktor koji nam ispuni parametre objekta zeljenim vrijednostima
        /// </summary>
        /// <param name="label"></param>
        /// <param name="naredba"></param>
        public KodRed(string label, string naredba)
        {
            string[] nizStringova;
            this.label = label;
            this.naredba.Clear();

            nizStringova = naredba.Split(' ');
            this.naredba.Add(nizStringova[0]);
            naredba = naredba.Remove(0, this.naredba[0].Length + 1);
            nizStringova = naredba.Split(',');

            for (int i = 0; i < nizStringova.Length; i++)
            {
                //if (nizStringova[i] == "DW")
                //    continue;
                this.naredba.Add(nizStringova[i].Trim());
            }
        }
        /// <summary>
        /// Pretvara naredbu u string
        /// </summary>
        /// <returns></returns>
        public string NaredbaUString()
        {
            for (int j = 0; j < naredba.Count; j++)
            {
                niz += naredba[j];
                if (j != 0 && j != naredba.Count - 1)
                    niz += ",";
                niz += " ";
            }
            return niz;
        }
        /// <summary>
        /// Pretvara naredbu u string koji se sastoji od jedinica i nula koje reprezentiraju naredbu u memoriji
        /// </summary>
        /// <returns></returns>
        public string NaredbeUMemoriji() 
        {
            switch (naredba[0])
            {
                #region DW
                case "DW":
                    if (naredba[1][0] == '0')
                    {
                        string stringBit = "";
                        for (int i = 1; i < naredba[1].Length; i++)
                        {
                            switch (naredba[1][i])
                            {
                                case '0':
                                    stringBit += "0000";
                                    break;
                                case '1':
                                    stringBit += "0001";
                                    break;
                                case '2':
                                    stringBit += "0010";
                                    break;
                                case '3':
                                    stringBit += "0011";
                                    break;
                                case '4':
                                    stringBit += "0100";
                                    break;
                                case '5':
                                    stringBit += "0101";
                                    break;
                                case '6':
                                    stringBit += "0110";
                                    break;
                                case '7':
                                    stringBit += "0111";
                                    break;
                                case '8':
                                    stringBit += "1000";
                                    break;
                                case '9':
                                    stringBit += "1001";
                                    break;
                                case 'A':
                                    stringBit += "1010";
                                    break;
                                case 'B':
                                    stringBit += "1011";
                                    break;
                                case 'C':
                                    stringBit += "1100";
                                    break;
                                case 'D':
                                    stringBit += "1101";
                                    break;
                                case 'E':
                                    stringBit += "1110";
                                    break;
                                case 'F':
                                    stringBit += "1111";
                                    break;
                            }
                        }
                        for (int i = stringBit.Length ; i < 32; i++)
                        {
                            stringBit = "0" + stringBit;
                        }
                        return stringBit;
                    }
                    else
                    {
                        string stringBit = "";
                        int broj = int.Parse(naredba[1]), length;

                        while (broj >= 1)
                        {
                            if (broj % 2 == 1)
                            {
                                stringBit = "1" + stringBit;
                            }
                            else
                            {
                                stringBit = "0" + stringBit;
                            }

                            broj = broj / 2;
                        }

                        length = stringBit.Length;

                        for (int i = 0; i < 32 - length; i++)
                        {
                            stringBit = "0" + stringBit;
                        }

                        return stringBit;
                    }
                #endregion
            }
            return "00000000000000000000000000000000";
        }

        /// <summary>
        /// Labela
        /// </summary>
        public string Labela
        {
            get { return label; }
            set { label = value; }
        }
        /// <summary>
        /// Naredba
        /// </summary>
        public List<string> Naredba
        {
            get { return naredba; }
            set { naredba = value; }
        }
    }
}
