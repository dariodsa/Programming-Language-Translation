using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace frisc
{
    public partial class Form1 : Form
    {
        #region Privatne varijable klase
        Dictionary<string,string> labele = new Dictionary<string, string>();
        bool kraj = false;
        #endregion

        public Form1(string[] url)
        {
            InitializeComponent();

            if (url != null)
            {
                try
                {
                    if (url[0] != "")
                    {
                        Properties.Settings.Default.path = url[0];
                    }
                }
                catch
                { 
                }
            }

            inicijalizacijaMemorije();
            inicijalizacijaRegistara();

            obojajRed(stringBitToInt(hexToStringBit(textBoxPC.Text)));
        }

        /// <summary>
        /// Puni dataGridView podatcima iz tekstualne datoteke u kojoj nam je kod za FRISC
        /// </summary>
        private void inicijalizacijaMemorije()
        {
            int ukupanBrojRijeci = 2048;
            if (Properties.Settings.Default.path != null && Properties.Settings.Default.path != "")
            {
                AsemblerKod aK = new AsemblerKod();

                aK.source(Properties.Settings.Default.path);
                dataGridView1.Rows.Clear();
                labele.Clear();

                // prebacivanje podataka iz .txt datoteke
                // u dataGridView

                List<KodRed> memorija = aK.podatci();

                DataGridViewRow r;
                DataGridViewTextBoxCell c;
                for (int i = 0; i < memorija.Count; i++)
                {
                    r = new DataGridViewRow();

                    c = new DataGridViewTextBoxCell();
                    c.Value = memorija[i].NaredbeUMemoriji();
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = stringBitToStringHex(intToStringBit(i * 4));
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = memorija[i].Labela;
                    r.Cells.Add(c);

                    if (memorija[i].Labela.Length > 0)
                    {
                        labele.Add(memorija[i].Labela, stringBitToStringHex(intToStringBit(i * 4)));
                    }

                    c = new DataGridViewTextBoxCell();
                    c.Value = memorija[i].NaredbaUString();
                    r.Cells.Add(c);

                    dataGridView1.Rows.Add(r);
                }

                // dodatna memorija za stog i spremanje podataka
                for (int i = memorija.Count; i < ukupanBrojRijeci; i++)
                {
                    r = new DataGridViewRow();

                    c = new DataGridViewTextBoxCell();
                    c.Value = "00000000000000000000000000000000";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = stringBitToStringHex(intToStringBit(i * 4));
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "";
                    r.Cells.Add(c);

                    dataGridView1.Rows.Add(r);
                }
            }
            else
            {
                DataGridViewRow r;
                DataGridViewTextBoxCell c;
                //Halt red
                r = new DataGridViewRow();
                
                c = new DataGridViewTextBoxCell();
                c.Value = "00000000000000000000000000000000";
                r.Cells.Add(c);

                c = new DataGridViewTextBoxCell();
                c.Value = stringBitToStringHex(intToStringBit(0 * 4));
                r.Cells.Add(c);

                c = new DataGridViewTextBoxCell();
                c.Value = "";
                r.Cells.Add(c);

                c = new DataGridViewTextBoxCell();
                c.Value = "HALT ";
                r.Cells.Add(c);
                
                dataGridView1.Rows.Add(r);
                

                // dodatna memorija za stog i spremanje podataka
                for (int i = 1; i < ukupanBrojRijeci; i++)
                {
                    r = new DataGridViewRow();

                    c = new DataGridViewTextBoxCell();
                    c.Value = "00000000000000000000000000000000";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = stringBitToStringHex(intToStringBit(i * 4));
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "";
                    r.Cells.Add(c);

                    dataGridView1.Rows.Add(r);
                }
            }
        }
        /// <summary>
        /// Inicijalizira sve registre na vrijednost 0
        /// </summary>
        private void inicijalizacijaRegistara()
        {
            textBoxPC.Text = "00000000000000000000000000000000";
            textBoxSR.Text = "00000000000000000000000000000000";

            textBoxR1.Text = "00000000000000000000000000000000";
            textBoxR2.Text = "00000000000000000000000000000000";
            textBoxR3.Text = "00000000000000000000000000000000";
            textBoxR4.Text = "00000000000000000000000000000000";
            textBoxR5.Text = "00000000000000000000000000000000";
            textBoxR6.Text = "00000000000000000000000000000000";
            textBoxR7.Text = "00000000000000000000000000000000";
        }

        /// <summary>
        /// Oboja red u dataGridView-u
        /// </summary>
        /// <param name="brReda">Redni broj reda kojeg treba obojati</param>
        private void obojajRed(int brReda)
        {
            if (dataGridView1.FirstDisplayedScrollingRowIndex < brReda && brReda < dataGridView1.FirstDisplayedScrollingRowIndex + 12)
            {
            }
            else if (brReda > 12)
            {
                dataGridView1.FirstDisplayedScrollingRowIndex = brReda - 12;
            }
            else
            {
                dataGridView1.FirstDisplayedScrollingRowIndex = 0;
            }

            dataGridView1.Rows[brReda].Selected = true;
        }
        /// <summary>
        /// Oboja red u dataGridView-u
        /// </summary>
        /// <param name="brReda">Redni broj reda kojeg treba obojati</param>
        private void obojajRedUBijelo(int brReda)
        {
            dataGridView1.Rows[brReda].Selected = false;
        }

        /// <summary>
        /// Vraca podatke koji se nalaze na odredenoj adresi u memoriji
        /// </summary>
        /// <param name="adresa">Adresa na kojoj se nalazi podatak</param>
        /// <param name="brBajtova">Velicina podatka u bajtovima</param>
        /// <returns>Podatak na adresi</returns>
        private string loadPodatci(int adresa, int brBajtova)
        {
            string podatci;
            switch(brBajtova)
            {
                case 4:
                    switch (adresa % 4)
                    {
                        case 0:
                            return (string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value;
                        case 1:
                            podatci = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(24, 8);
                            podatci += ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 24);
                            return podatci;
                        case 2:
                            podatci = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(16, 16);
                            podatci += ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 16);
                            return podatci;
                        case 3:
                            podatci = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(8, 24);
                            podatci += ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 8);
                            return podatci;
                    }
                    break;
                case 2:
                    switch (adresa % 4)
                    {
                        case 0:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(16, 16);
                        case 1:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(8, 16);
                        case 2:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 16);
                        case 3:
                            podatci = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(24, 8);
                            podatci += ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 8);
                            return podatci;
                    }
                    break;
                case 1:
                    switch (adresa % 4)
                    {
                        case 0:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(24, 8);
                        case 1:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(16, 8);
                        case 2:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(8, 8);
                        case 3:
                            return ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 8);
                    }
                    break;
            }
            return "Greska";
        }
        /// <summary>
        /// Odredi adresu s koje treba procitati te 
        /// vrati podatke zapisane na toj adresi.
        /// </summary>
        /// <param name="kr">KodRed iz kojega odredujemo adresu s koje citamo</param>
        /// <param name="brByte">velicina podatka u bajtovima</param>
        /// <returns>Sadrzaj na zadanoj memoriji</returns>
        private string load(KodRed kr, int brBajtova)
        {
            string niz;
            int adresa;

            if (kr.Naredba[2][0] == '(')
            {
                niz = kr.Naredba[2].TrimEnd(')').TrimStart('(');
                
                if(labele.ContainsKey(niz))
                {
                    adresa = stringBitToInt(hexToStringBit(labele[niz]));
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R1")
                {
                    adresa = stringBitToInt(textBoxR1.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R2")
                {
                    adresa = stringBitToInt(textBoxR2.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R3")
                {
                    adresa = stringBitToInt(textBoxR3.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R4")
                {
                    adresa = stringBitToInt(textBoxR4.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R5")
                {
                    adresa = stringBitToInt(textBoxR5.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R6")
                {
                    adresa = stringBitToInt(textBoxR6.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz == "R7")
                {
                    adresa = stringBitToInt(textBoxR7.Text);
                    return loadPodatci(adresa, brBajtova);
                }
                else if (niz[0] == 'R')
                {
                    string[] nizDio = niz.Split() ;
                    if (nizDio[1] == "+")
                    {
                        switch (niz[1])
                        {
                            case '1':
                                adresa = (stringBitToInt(textBoxR1.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '2':
                                adresa = (stringBitToInt(textBoxR2.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '3':
                                adresa = (stringBitToInt(textBoxR3.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '4':
                                adresa = (stringBitToInt(textBoxR4.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '5':
                                adresa = (stringBitToInt(textBoxR5.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '6':
                                adresa = (stringBitToInt(textBoxR6.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '7':
                                adresa = (stringBitToInt(textBoxR7.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                        }
                    }
                    else
                    {
                        switch (niz[1])
                        {
                            case '1':
                                adresa = (stringBitToInt(textBoxR1.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '2':
                                adresa = (stringBitToInt(textBoxR2.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '3':
                                adresa = (stringBitToInt(textBoxR3.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '4':
                                adresa = (stringBitToInt(textBoxR4.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '5':
                                adresa = (stringBitToInt(textBoxR5.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '6':
                                adresa = (stringBitToInt(textBoxR6.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                            case '7':
                                adresa = (stringBitToInt(textBoxR7.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                                return loadPodatci(adresa, brBajtova);
                        }
                    }
                }
                else
                {
                    adresa = stringBitToInt(hexToStringBit(niz));
                    return loadPodatci(adresa, brBajtova);
                }
            }

            return "greska";
        }
        /// <summary>
        /// Sprema podatke na odredenu adresu u memoriji
        /// </summary>
        /// <param name="podatak">Podatak koji spremamo</param>
        /// <param name="adresa">Adresa na koju se spremaju podaci</param>
        /// <param name="brBajtova">Velicina podatka u bajtovima</param>
        private void storePodatak(string podatak, int adresa, int brBajtova)
        {
            string temp;

            provjeriMemoriju(adresa);

            switch (brBajtova)
            {
                case 4:
                    switch (adresa % 4)
                    {
                        case 0:
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = podatak;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 1:
                            temp = podatak.Substring(8, 24) + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(24,8);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            temp = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(0,24) + podatak.Substring(0, 8);
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 2:
                            temp = podatak.Substring(16, 16) + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(16, 16);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            temp = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(0, 16) + podatak.Substring(0, 16);
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 3:
                            temp = podatak.Substring(24, 8) + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(8, 24);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            temp = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(0, 8) + podatak.Substring(0, 24);
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value);
                            break;
                    }
                    break;
                case 2:
                    switch (adresa % 4)
                    {
                        case 0:
                            temp = ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 16) + podatak;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 1:
                            temp = ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 8) + podatak + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(24, 8);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 2:
                            temp =podatak + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(16, 16);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 3:
                            temp = podatak.Substring(8, 8) + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(8, 24);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            temp = ((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value).Substring(0, 24) + podatak.Substring(0, 8);
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[(adresa + 4) / 4].Cells["ColumnMemorija"].Value);
                            break;
                    }
                    break;
                case 1:
                    switch (adresa % 4)
                    {
                        case 0:
                            temp = ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 24) + podatak;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 1:
                            temp = ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 16) + podatak + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(24, 8);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 2:
                            temp = ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(0, 8) + podatak + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(16, 16);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                        case 3:
                            temp = podatak + ((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value).Substring(8, 24);
                            dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = temp;
                            dataGridView1.Rows[adresa / 4].Cells["ColumnSadrzaj"].Value = stringBitToStringHex((string)dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value);
                            break;
                    }
                    break;
            }
        }
        /// <summary>
        /// Odredi adresu na koju treba zapisati podatke
        /// iz registra te ih spremi na tu adresu
        /// </summary>
        /// <param name="registra">Sadrzaj registra kojeg treba spremiti</param>
        /// <param name="kr">KodRed iz kojega odredujemo adresu s koje citamo</param>
        /// <param name="brByte">Velicina podatka kojega citamo u bajtima</param>
        private void store(string registra, KodRed kr, int brByte)
        {
            string niz = kr.Naredba[2].TrimEnd(')').TrimStart('(');
            int adresa;

            if (labele.ContainsKey(niz))
            {
                adresa = stringBitToInt(hexToStringBit(labele[niz]));
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R1")
            {
                adresa = stringBitToInt(textBoxR1.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R2")
            {
                adresa = stringBitToInt(textBoxR2.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R3")
            {
                adresa = stringBitToInt(textBoxR3.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R4")
            {
                adresa = stringBitToInt(textBoxR4.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R5")
            {
                adresa = stringBitToInt(textBoxR5.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R6")
            {
                adresa = stringBitToInt(textBoxR6.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz == "R7")
            {
                adresa = stringBitToInt(textBoxR7.Text);
                storePodatak(registra, adresa, brByte);
            }
            else if (niz[0] == 'R')
            {
                string[] nizDio = niz.Split();
                if (nizDio[1] == "+")
                {
                    switch (niz[1])
                    {
                        case '1':
                            adresa = (stringBitToInt(textBoxR1.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '2':
                            adresa = (stringBitToInt(textBoxR2.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '3':
                            adresa = (stringBitToInt(textBoxR3.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '4':
                            adresa = (stringBitToInt(textBoxR4.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '5':
                            adresa = (stringBitToInt(textBoxR5.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '6':
                            adresa = (stringBitToInt(textBoxR6.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '7':
                            adresa = (stringBitToInt(textBoxR7.Text) + stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                    }
                }
                else
                {
                    switch (niz[1])
                    {
                        case '1':
                            adresa = (stringBitToInt(textBoxR1.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '2':
                            adresa = (stringBitToInt(textBoxR2.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '3':
                            adresa = (stringBitToInt(textBoxR3.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '4':
                            adresa = (stringBitToInt(textBoxR4.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '5':
                            adresa = (stringBitToInt(textBoxR5.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '6':
                            adresa = (stringBitToInt(textBoxR6.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                        case '7':
                            adresa = (stringBitToInt(textBoxR7.Text) - stringBitToInt(hexToStringBit(nizDio[2])));
                            storePodatak(registra, adresa, brByte);
                            break;
                    }
                }
            }
            else
            {
                adresa =  stringBitToInt(hexToStringBit(niz));
                storePodatak(registra, adresa, brByte);
            }
        
        }

        /// <summary>
        /// Zbroji dva registra te vrati rezultat
        /// Zbroji dva cijela broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg zbrajamo</param>
        /// <param name="reg2">Drugi registar kojeg zbrajamo</param>
        /// <returns>Vrati zbroj registara u obliku string bit</returns>
        private string add(string reg1, string reg2)
        {
            int br = 0;

            switch (reg1)
            {
                case "R1":
                    br += stringBitToInt(textBoxR1.Text); 
                    break;
                case "R2":
                    br += stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToInt(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br += stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br += stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToInt(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br += stringBitToInt(hexToStringBit(reg2));
                    }
                    else
                    {
                        br += int.Parse(reg2);
                    }
                    break;
            }

            return  intToStringBit(br); 
        }
        /// <summary>
        /// Pomnozi dva registra te vrati rezultat
        /// Pomnozi dva cijela broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg mnozimo</param>
        /// <param name="reg2">Drugi registar kojeg mnozimo</param>
        /// <returns>Vrati umnozak registara u obliku string bit</returns>
        private string mul(string reg1, string reg2)
        {
            int br = 1;

            switch (reg1)
            {
                case "R1":
                    br *= stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToInt(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br *= stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToInt(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br *= stringBitToInt(hexToStringBit(reg2));
                    }
                    else
                    {
                        br *= int.Parse(reg2);
                    }
                    break;
            }

            return intToStringBit(br);
        }
        /// <summary>
        /// Podijeli sadrzej dvaju registara te vrati rezultat
        /// Podijeli dva cijela broja, rezultat je cijeli broj
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg dijelimo</param>
        /// <param name="reg2">Drugi registar s kojim dijelimo</param>
        /// <returns>Vrati rezultat dijeljenja registara u obliku string bit</returns>
        private string div(string reg1, string reg2)
        {
            int br = 1;

            switch (reg1)
            {
                case "R1":
                    br *= stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToInt(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br /= stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br /= stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br /= stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br /= stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br /= stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br /= stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br /= stringBitToInt(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br /= stringBitToInt(hexToStringBit(reg2));
                    }
                    else
                    {
                        br /= int.Parse(reg2);
                    }
                    break;
            }

            return intToStringBit(br);
        }
        /// <summary>
        /// Oduzme registre reg1 i reg2 (reg1 - reg2) te vrati rezultat
        /// Oduzme dva cijela broja
        /// </summary>
        /// <param name="reg1">Prvi registar</param>
        /// <param name="reg2">Drugi registar</param>
        /// <returns>Vrati rezultat oduzimanja u obliku string bita</returns>
        private string sub(string reg1, string reg2)
        {
            int br = 0;

            switch (reg1)
            {
                case "R1":
                    br += stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br += stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToInt(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br -= stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    br -= stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    br -= stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    br -= stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    br -= stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    br -= stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    br -= stringBitToInt(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br -= stringBitToInt(hexToStringBit(reg2));
                    }
                    else
                    {
                        br -= int.Parse(reg2);
                    }
                    break;
            }

            return intToStringBit(br); 
        }
        /// <summary>
        /// Usporedi registre i postavi zastavice, moze usporediti registar i broj
        /// Usporedi dva cijela broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojega usporedjuje</param>
        /// <param name="reg2">Drugi registar kojega usporedjuje, moze biti i broj(hex ili int)</param>
        private void cmp(string reg1, string reg2)
        {
            int r1 = 0, r2 = 0;
            uint ur1, ur2;

            #region pretvaranje registara u brojeve
            switch (reg1)
            {
                case "R1":
                    r1 = stringBitToInt(textBoxR1.Text);
                    break;
                case "R2":
                    r1 = stringBitToInt(textBoxR2.Text);
                    break;
                case "R3":
                    r1 = stringBitToInt(textBoxR3.Text);
                    break;
                case "R4":
                    r1 = stringBitToInt(textBoxR4.Text);
                    break;
                case "R5":
                    r1 = stringBitToInt(textBoxR5.Text);
                    break;
                case "R6":
                    r1 = stringBitToInt(textBoxR6.Text);
                    break;
                case "R7":
                    r1 = stringBitToInt(textBoxR7.Text);
                    break;
            }

            if (reg2 == "R1")
            {
                r2 = stringBitToInt(textBoxR1.Text);
            }
            else if (reg2 == "R2")
            {
                r2 = stringBitToInt(textBoxR2.Text);
            }
            else if (reg2 == "R3")
            {
                r2 = stringBitToInt(textBoxR3.Text);
            }
            else if (reg2 == "R4")
            {
                r2 = stringBitToInt(textBoxR4.Text);
            }
            else if (reg2 == "R5")
            {
                r2 = stringBitToInt(textBoxR5.Text);
            }
            else if (reg2 == "R6")
            {
                r2 = stringBitToInt(textBoxR6.Text);
            }
            else if (reg2 == "R7")
            {
                r2 = stringBitToInt(textBoxR7.Text);
            }
            else
            {
                if (reg2[0] == '0')
                {
                    r2 = stringBitToInt(hexToStringBit(reg2));
                }
                else
                { 
                    r2 = int.Parse(reg2);
                }
            }
            #endregion

            if (r1 == r2)
            {
                setZastavicaZ(true);
            }
            else
            {
                setZastavicaZ(false);
            }

            //ur1 i ur2
            ur1 = stringBitToUInt(intToStringBit(r1));
            ur2 = stringBitToUInt(intToStringBit(r2));

            if(ur1 > ur2) 
            {
                setZastavicaC(false);
                setZastavicaZ(false);
            }
            if (ur1 < ur2)
            {
                setZastavicaC(true);
            }

            if(r1 > r2)
            {
                setZastavicaZ(false);
                if((r1 - r2) > 0)
                {
                    setZastavicaN(false);
                    setZastavicaV(false);
                }
                else
                {
                    setZastavicaN(true);
                    setZastavicaV(true);
                }
            }

            if(r1 < r2)
            {
                setZastavicaZ(false);
                if((r1 - r2) > 0)
                {
                    setZastavicaN(false);
                    setZastavicaV(true);
                }
                else
                {
                    setZastavicaN(true);
                    setZastavicaV(false);
                }
            }
        }

        /// <summary>
        /// Zbroji dva registra te vrati rezultat
        /// Zbroji dva realna broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg zbrajamo</param>
        /// <param name="reg2">Drugi registar kojeg zbrajamo</param>
        /// <returns>Vrati zbroj registara u obliku string bit</returns>
        private string addf(string reg1, string reg2)
        {
            float br = 0;

            switch (reg1)
            {
                case "R1":
                    br += stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br += stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToFloat(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br += stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br += stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToFloat(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br += stringBitToFloat(hexToStringBit(reg2));
                    }
                    else
                    {
                        br += float.Parse(reg2);
                    }
                    break;
            }

            return floatToStringBit(br);
        }
        /// <summary>
        /// Pomnozi dva registra te vrati rezultat
        /// Pomnozi dva realna broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg mnozimo</param>
        /// <param name="reg2">Drugi registar kojeg mnozimo</param>
        /// <returns>Vrati umnozak registara u obliku string bit</returns>
        private string mulf(string reg1, string reg2)
        {
            float br = 1;

            switch (reg1)
            {
                case "R1":
                    br *= stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToFloat(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br *= stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToFloat(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br *= stringBitToFloat(hexToStringBit(reg2));
                    }
                    else
                    {
                        br *= float.Parse(reg2);
                    }
                    break;
            }

            return floatToStringBit(br);
        }
        /// <summary>
        /// Podijeli sadrzej dvaju registara te vrati rezultat
        /// Podijeli dva realna broja, rezultat je realni broj
        /// </summary>
        /// <param name="reg1">Prvi registar kojeg dijelimo</param>
        /// <param name="reg2">Drugi registar s kojim dijelimo</param>
        /// <returns>Vrati rezultat dijeljenja registara u obliku string bit</returns>
        private string divf(string reg1, string reg2)
        {
            float br = 1;

            switch (reg1)
            {
                case "R1":
                    br *= stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br *= stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br *= stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br *= stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br *= stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br *= stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br *= stringBitToFloat(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br /= stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br /= stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br /= stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br /= stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br /= stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br /= stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br /= stringBitToFloat(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br /= stringBitToFloat(hexToStringBit(reg2));
                    }
                    else
                    {
                        br /= float.Parse(reg2);
                    }
                    break;
            }

            return floatToStringBit(br);
        }
        /// <summary>
        /// Oduzme registre reg1 i reg2 (reg1 - reg2) te vrati rezultat
        /// Oduzme dva realna broja
        /// </summary>
        /// <param name="reg1">Prvi registar</param>
        /// <param name="reg2">Drugi registar</param>
        /// <returns>Vrati rezultat oduzimanja u obliku string bita</returns>
        private string subf(string reg1, string reg2)
        {
            float br = 0;

            switch (reg1)
            {
                case "R1":
                    br += stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br += stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br += stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br += stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br += stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br += stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br += stringBitToFloat(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    br -= stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    br -= stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    br -= stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    br -= stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    br -= stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    br -= stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    br -= stringBitToFloat(textBoxR7.Text);
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        br -= stringBitToFloat(hexToStringBit(reg2));
                    }
                    else
                    {
                        br -= float.Parse(reg2);
                    }
                    break;
            }

            return floatToStringBit(br);
        }
        /// <summary>
        /// Usporedi registre i postavi zastavice
        /// Usporedi dva realna broja
        /// </summary>
        /// <param name="reg1">Prvi registar kojega usporedjuje</param>
        /// <param name="reg2">Drugi registar kojega usporedjuje</param>
        private void cmpf(string reg1, string reg2)
        {
            float r1 = 0, r2 = 0;

            switch (reg1)
            {
                case "R1":
                    r1 = stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    r1 = stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    r1 = stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    r1 = stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    r1 = stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    r1 = stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    r1 = stringBitToFloat(textBoxR7.Text);
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    r2 = stringBitToFloat(textBoxR1.Text);
                    break;
                case "R2":
                    r2 = stringBitToFloat(textBoxR2.Text);
                    break;
                case "R3":
                    r2 = stringBitToFloat(textBoxR3.Text);
                    break;
                case "R4":
                    r2 = stringBitToFloat(textBoxR4.Text);
                    break;
                case "R5":
                    r2 = stringBitToFloat(textBoxR5.Text);
                    break;
                case "R6":
                    r2 = stringBitToFloat(textBoxR6.Text);
                    break;
                case "R7":
                    r2 = stringBitToFloat(textBoxR7.Text);
                    break;
                default:
                    r2 = stringBitToFloat(hexToStringBit(reg2));
                    break;
            }

            if (r1 == r2)
            {
                setZastavicaZ(true);
            }
            else
            {
                setZastavicaZ(false);
            }
            
            
            //ur1 i ur2
            //ur1 = stringBitToUInt(intToStringBit(reg1));
            //ur2 = stringBitToUInt(intToStringBit(reg2));

            //if (ur1 > ur2)
            //{
            //    setZastavicaC(false);
            //    setZastavicaZ(false);
            //}
            //if (ur1 < ur2)
            //{
            //    setZastavicaC(true);
            //}

            if (r1 > r2)
            {
                setZastavicaZ(false);
                if ((r1 - r2) > 0)
                {
                    setZastavicaN(false);
                    setZastavicaV(false);
                }
                else
                {
                    setZastavicaN(true);
                    setZastavicaV(true);
                }
            }

            if (r1 < r2)
            {
                setZastavicaZ(false);
                if ((r1 - r2) > 0)
                {
                    setZastavicaN(false);
                    setZastavicaV(true);
                }
                else
                {
                    setZastavicaN(true);
                    setZastavicaV(false);
                }
            }
        }

        /// <summary>
        /// Obavi AND operaciju nad registarima i vratirezultat
        /// </summary>
        /// <param name="reg1">Prvi registar</param>
        /// <param name="reg2">Drugi registar</param>
        /// <returns>Rezultat operacije AND</returns>
        private string and(string reg1, string reg2)
        {
            switch (reg1)
            {
                case "R1":
                    reg1 = textBoxR1.Text;
                    break;
                case "R2":
                    reg1 = textBoxR2.Text;
                    break;
                case "R3":
                    reg1 = textBoxR3.Text;
                    break;
                case "R4":
                    reg1 = textBoxR4.Text;
                    break;
                case "R5":
                    reg1 = textBoxR5.Text;
                    break;
                case "R6":
                    reg1 = textBoxR6.Text;
                    break;
                case "R7":
                    reg1 = textBoxR7.Text;
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    reg2 = textBoxR1.Text;
                    break;
                case "R2":
                    reg2 = textBoxR2.Text;
                    break;
                case "R3":
                    reg2 = textBoxR3.Text;
                    break;
                case "R4":
                    reg2 = textBoxR4.Text;
                    break;
                case "R5":
                    reg2 = textBoxR5.Text;
                    break;
                case "R6":
                    reg2 = textBoxR6.Text;
                    break;
                case "R7":
                    reg2 = textBoxR7.Text;
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        reg2 = hexToStringBit(reg2);
                    }
                    else
                    {
                        reg2 = intToStringBit(int.Parse(reg2));
                    }
                    break;
            }

            string temp = "";

            for (int i = 0; i < 32; i++)
            {
                if (reg1[i] == '0' || reg2[i] == '0')
                {
                    temp += "0";
                }
                else
                {
                    temp += "1";
                }
            }

            return temp;
        }
        /// <summary>
        /// Obavi OR operaciju nad registarima i vratirezultat
        /// </summary>
        /// <param name="reg1">Prvi registar</param>
        /// <param name="reg2">Drugi registar</param>
        /// <returns>Rezultat operacije OR</returns>
        private string or(string reg1, string reg2)
        {
            switch (reg1)
            {
                case "R1":
                    reg1 = textBoxR1.Text;
                    break;
                case "R2":
                    reg1 = textBoxR2.Text;
                    break;
                case "R3":
                    reg1 = textBoxR3.Text;
                    break;
                case "R4":
                    reg1 = textBoxR4.Text;
                    break;
                case "R5":
                    reg1 = textBoxR5.Text;
                    break;
                case "R6":
                    reg1 = textBoxR6.Text;
                    break;
                case "R7":
                    reg1 = textBoxR7.Text;
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    reg2 = textBoxR1.Text;
                    break;
                case "R2":
                    reg2 = textBoxR2.Text;
                    break;
                case "R3":
                    reg2 = textBoxR3.Text;
                    break;
                case "R4":
                    reg2 = textBoxR4.Text;
                    break;
                case "R5":
                    reg2 = textBoxR5.Text;
                    break;
                case "R6":
                    reg2 = textBoxR6.Text;
                    break;
                case "R7":
                    reg2 = textBoxR7.Text;
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        reg2 = hexToStringBit(reg2);
                    }
                    else
                    {
                        reg2 = intToStringBit(int.Parse(reg2));
                    }
                    break;
            }

            string temp = "";

            for (int i = 0; i < 32; i++)
            {
                if (reg1[i] == '0' && reg2[i] == '0')
                {
                    temp += "0";
                }
                else
                {
                    temp += "1";
                }
            }

            return temp;
        }
        /// <summary>
        /// Obavi XOR operaciju nad registarima i vratirezultat
        /// </summary>
        /// <param name="reg1">Prvi registar</param>
        /// <param name="reg2">Drugi registar</param>
        /// <returns>Rezultat operacije XOR</returns>
        private string xor(string reg1, string reg2)
        {
            switch (reg1)
            {
                case "R1":
                    reg1 = textBoxR1.Text;
                    break;
                case "R2":
                    reg1 = textBoxR2.Text;
                    break;
                case "R3":
                    reg1 = textBoxR3.Text;
                    break;
                case "R4":
                    reg1 = textBoxR4.Text;
                    break;
                case "R5":
                    reg1 = textBoxR5.Text;
                    break;
                case "R6":
                    reg1 = textBoxR6.Text;
                    break;
                case "R7":
                    reg1 = textBoxR7.Text;
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    reg2 = textBoxR1.Text;
                    break;
                case "R2":
                    reg2 = textBoxR2.Text;
                    break;
                case "R3":
                    reg2 = textBoxR3.Text;
                    break;
                case "R4":
                    reg2 = textBoxR4.Text;
                    break;
                case "R5":
                    reg2 = textBoxR5.Text;
                    break;
                case "R6":
                    reg2 = textBoxR6.Text;
                    break;
                case "R7":
                    reg2 = textBoxR7.Text;
                    break;
                default:
                    if (reg2[0] == 0)
                    {
                        reg2 = hexToStringBit(reg2);
                    }
                    else
                    {
                        reg2 = intToStringBit(int.Parse(reg2));
                    }
                    break;
            }

            string temp = "";

            for (int i = 0; i < 32; i++)
            {
                if (reg1[i] == reg2[i])
                {
                    temp += "0";
                }
                else
                {
                    temp += "1";
                }
            }

            return temp;
        }
        /// <summary>
        /// Reg1 pomakne logicki u desno za reg2 bitova
        /// </summary>
        /// <param name="reg1">Registar kojeg logicki pomicemo</param>
        /// <param name="reg2">Broj bitova za koje radimo pomak</param>
        /// <returns>Rezultat operacije SHR</returns>
        private string shr(string reg1, string reg2)
        {
            switch (reg1)
            {
                case "R1":
                    reg1 = textBoxR1.Text;
                    break;
                case "R2":
                    reg1 = textBoxR2.Text;
                    break;
                case "R3":
                    reg1 = textBoxR3.Text;
                    break;
                case "R4":
                    reg1 = textBoxR4.Text;
                    break;
                case "R5":
                    reg1 = textBoxR5.Text;
                    break;
                case "R6":
                    reg1 = textBoxR6.Text;
                    break;
                case "R7":
                    reg1 = textBoxR7.Text;
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    reg2 = textBoxR1.Text;
                    break;
                case "R2":
                    reg2 = textBoxR2.Text;
                    break;
                case "R3":
                    reg2 = textBoxR3.Text;
                    break;
                case "R4":
                    reg2 = textBoxR4.Text;
                    break;
                case "R5":
                    reg2 = textBoxR5.Text;
                    break;
                case "R6":
                    reg2 = textBoxR6.Text;
                    break;
                case "R7":
                    reg2 = textBoxR7.Text;
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        reg2 = hexToStringBit(reg2);
                    }
                    else
                    {
                        reg2 = intToStringBit(int.Parse(reg2));
                    }
                    break;
            }

            int br = stringBitToInt(reg2);

            reg1 = reg1.Remove(31 - br);

            for (int i = 0; i < br; i++)
            {
                reg1 = "0" + reg1;
            }

            return reg1;
        }
        /// <summary>
        /// Reg1 pomakne logicki u lijevo za reg2 bitova
        /// </summary>
        /// <param name="reg1">Registar kojeg logicki pomicemo</param>
        /// <param name="reg2">Broj bitova za koje radimo pomak</param>
        /// <returns>Rezultat operacije SHL</returns>
        private string shl(string reg1, string reg2)
        {
            switch (reg1)
            {
                case "R1":
                    reg1 = textBoxR1.Text;
                    break;
                case "R2":
                    reg1 = textBoxR2.Text;
                    break;
                case "R3":
                    reg1 = textBoxR3.Text;
                    break;
                case "R4":
                    reg1 = textBoxR4.Text;
                    break;
                case "R5":
                    reg1 = textBoxR5.Text;
                    break;
                case "R6":
                    reg1 = textBoxR6.Text;
                    break;
                case "R7":
                    reg1 = textBoxR7.Text;
                    break;
            }

            switch (reg2)
            {
                case "R1":
                    reg2 = textBoxR1.Text;
                    break;
                case "R2":
                    reg2 = textBoxR2.Text;
                    break;
                case "R3":
                    reg2 = textBoxR3.Text;
                    break;
                case "R4":
                    reg2 = textBoxR4.Text;
                    break;
                case "R5":
                    reg2 = textBoxR5.Text;
                    break;
                case "R6":
                    reg2 = textBoxR6.Text;
                    break;
                case "R7":
                    reg2 = textBoxR7.Text;
                    break;
                default:
                    if (reg2[0] == '0')
                    {
                        reg2 = hexToStringBit(reg2);
                    }
                    else
                    {
                        reg2 = intToStringBit(int.Parse(reg2));
                    }
                    break;
            }

            int br = stringBitToInt(reg2);

            reg1 = reg1.Remove(0, br);

            for (int i = 0; i < br; i++)
            {
                reg1 = reg1 + "0";
            }

            return reg1;
        }

        /// <summary>
        /// Vrati trazene podatke
        /// </summary>
        /// <param name="podatci">Podatci koji se trebaju vratiti</param>
        /// <returns>Podatci u obliku string bita</returns>
        private string move(string podatci)
        {

            switch (podatci)
            {
                case "R1":
                    return textBoxR1.Text;
                case "R2":
                    return textBoxR2.Text;
                case "R3":
                    return textBoxR3.Text;
                case "R4":
                    return textBoxR4.Text;
                case "R5":
                    return textBoxR5.Text;
                case "R6":
                    return textBoxR6.Text;
                case "R7":
                    return textBoxR7.Text;
                case "SR":
                    return textBoxSR.Text;
            }

            if (podatci == "0")
            {
                return "00000000000000000000000000000000";
            }
            else if (podatci[0] == '0')
            {
                return pretvoriU4Bajta(hexToStringBit(podatci));
            }
            else
            {
                return pretvoriU4Bajta(intToStringBit(int.Parse(podatci)));
            }
        }
        /// <summary>
        /// Pretvori sadrzaj reg registra iz cijelog broja u realan broj
        /// </summary>
        /// <param name="reg">Registar u kojem se nalazi cijeli broj</param>
        /// <returns>Realan broj</returns>
        private string itf(string reg)
        {
            switch (reg)
            {
                case "R1":
                    return floatToStringBit(((float)stringBitToInt(textBoxR1.Text)));
                case "R2":
                    return floatToStringBit(((float)stringBitToInt(textBoxR2.Text)));
                case "R3":
                    return floatToStringBit(((float)stringBitToInt(textBoxR3.Text)));
                case "R4":
                    return floatToStringBit(((float)stringBitToInt(textBoxR4.Text)));
                case "R5":
                    return floatToStringBit(((float)stringBitToInt(textBoxR5.Text)));
                case "R6":
                    return floatToStringBit(((float)stringBitToInt(textBoxR6.Text)));
                case "R7":
                    return floatToStringBit(((float)stringBitToInt(textBoxR7.Text)));
            }
            return "";
        }
        /// <summary>
        /// Pretvori sadrzaj reg registra iz realnog broja u cijeli broj
        /// </summary>
        /// <param name="reg">Registar u kojem se nalazi realan broj</param>
        /// <returns>Cijeli broj</returns>
        private string fti(string reg)
        {
            switch (reg)
            {
                case "R1":
                    return intToStringBit(((int)stringBitToFloat(textBoxR1.Text)));
                case "R2":
                    return intToStringBit(((int)stringBitToFloat(textBoxR2.Text)));
                case "R3":
                    return intToStringBit(((int)stringBitToFloat(textBoxR3.Text)));
                case "R4":
                    return intToStringBit(((int)stringBitToFloat(textBoxR4.Text)));
                case "R5":
                    return intToStringBit(((int)stringBitToFloat(textBoxR5.Text)));
                case "R6":
                    return intToStringBit(((int)stringBitToFloat(textBoxR6.Text)));
                case "R7":
                    return intToStringBit(((int)stringBitToFloat(textBoxR7.Text)));
            }
            return "";
        }

        /// <summary>
        /// Sprema sadrzaj registra na zadanu adresu
        /// </summary>
        /// <param name="registar">Sadrzaj registra kojeg treba spremiti</param>
        /// <param name="adresa">adresa na koju treba spremiti registar</param>
        private void push(string registar, int adresa)
        {
            provjeriMemoriju(adresa);
            storePodatak(registar, adresa, 4);
            //dataGridView1.Rows[adresa / 4].Cells["ColumnMemorija"].Value = registar;
        }
        /// <summary>
        /// Dohvati sadrzaj sa stoga
        /// </summary>
        /// <returns>Sadrzaj registra</returns>
        private string pop()
        {
            return loadPodatci(stringBitToInt(textBoxR7.Text), 4);
            //return (string)dataGridView1.Rows[stringBitToInt(textBoxR7.Text) / 4].Cells["ColumnMemorija"].Value;
        }
        /// <summary>
        /// Odredi te vrati adresu na koju treba skociti
        /// </summary>
        /// <param name="adresa">String iz kojega odredjujemo adresu</param>
        /// <returns>Adresa skoka</returns>
        private string jump(string adresa)
        {
            adresa = adresa.TrimStart('(').TrimEnd(')');

            if(labele.ContainsKey(adresa))
            {
                return hexToStringBit(labele[adresa]);
            }
            else if (adresa == "R1")
            {
                return textBoxR1.Text;
            }
            else if (adresa == "R2")
            {
                return textBoxR2.Text;
            }
            else if (adresa == "R3")
            {
                return textBoxR3.Text;
            }
            else if (adresa == "R4")
            {
                return textBoxR4.Text;
            }
            else if (adresa == "R5")
            {
                return textBoxR5.Text;
            }
            else if (adresa == "R6")
            {
                return textBoxR6.Text;
            }
            else if (adresa == "R7")
            {
                return textBoxR7.Text;
            }
            else
            {
                return hexToStringBit(adresa);
            }
        }
        /// <summary>
        /// Odredi te vrati adresu na koju treba skociti
        /// </summary>
        /// <param name="adresa">String iz kojega odredjujemo adresu</param>
        /// <returns>Adresa skoka</returns>
        private string jumpr(string adresa)
        {
            adresa = adresa.TrimStart('(').TrimEnd(')');

            if (labele.ContainsKey(adresa))
            {
                return hexToStringBit(labele[adresa]);
            }
            else if (adresa == "R1")
            {
                return textBoxR1.Text;
            }
            else if (adresa == "R2")
            {
                return textBoxR2.Text;
            }
            else if (adresa == "R3")
            {
                return textBoxR3.Text;
            }
            else if (adresa == "R4")
            {
                return textBoxR4.Text;
            }
            else if (adresa == "R5")
            {
                return textBoxR5.Text;
            }
            else if (adresa == "R6")
            {
                return textBoxR6.Text;
            }
            else if (adresa == "R7")
            {
                return textBoxR7.Text;
            }
            else
            {
                int a = stringBitToInt(textBoxPC.Text) + stringBitToInt(hexToStringBit(adresa)) - 4;
                return intToStringBit(a);
            }
        }
        /// <summary>
        /// Sprema PC na stog te napravi skok na zadanu adresu 
        /// ili labelu na kojoj se nalazi potprogram
        /// </summary>
        /// <param name="adresa">Adresa podprograma</param>
        private void call(string adresa)
        {
            push(textBoxPC.Text, stringBitToInt(textBoxR7.Text));
            textBoxR7.Text = intToStringBit((stringBitToInt(textBoxR7.Text) + 4));
            textBoxPC.Text = jump(adresa);
        }
        /// <summary>
        /// Vrati se iz potprograma tako da skoci na adresu 
        /// koja je zapisana na stogu
        /// </summary>
        private void ret()
        {
            textBoxR7.Text = intToStringBit((stringBitToInt(textBoxR7.Text) - 4));
            textBoxPC.Text = pop();
        }
        /// <summary>
        /// Naredba koja neomoguci daljnje izvodjenje programa
        /// </summary>
        private void halt()
        {
            toolStripButton1.Enabled = false;
            toolStripButton2.Enabled = false;
            textBoxPC.Text = intToStringBit(stringBitToInt(textBoxPC.Text) - 4);

            MessageBox.Show("Došao do kraja !\n\n"+
                            "Konačan rezultat izvođenja, nalazi se u registru R1, ako u izvornom kodu funkcija \"main\" vraća rezultat.\n\n" +
                            "Sadržaj registra R1:\n"+
                            "  Dekadski : " + stringBitToInt(textBoxR1.Text).ToString() + "\n" +
                            "  Realno : " + stringBitToFloat(textBoxR1.Text).ToString() + "\n" +
                            "  Heksadekadski : " + stringBitToStringHex(textBoxR1.Text));

            kraj = true;
        }

        /// <summary>
        /// Zastavica C
        /// </summary>
        /// <returns>C</returns>
        private bool zastavicaC()
        { 
            if(textBoxSR.Text[30] == '1')
                return true;
            else 
                return false;
        }
        /// <summary>
        /// Zastavica V
        /// </summary>
        /// <returns>V</returns>
        private bool zastavicaV()
        {
            if(textBoxSR.Text[29] == '1')
                return true;
            else 
                return false;
        }
        /// <summary>
        /// Zastavica N
        /// </summary>
        /// <returns>N</returns>
        private bool zastavicaN()
        {
            if(textBoxSR.Text[31] == '1')
                return true;
            else 
                return false;
        }
        /// <summary>
        /// Zastavica Z
        /// </summary>
        /// <returns>Z</returns>
        private bool zastavicaZ()
        {
            if(textBoxSR.Text[28] == '1')
                return true;
            else 
                return false;
        }
        /// <summary>
        /// Postavi zastavicu C
        /// </summary>
        /// <returns>C</returns>
        private void setZastavicaC(bool zastavica)
        {
            if (zastavica)
            {
                textBoxSR.Text = textBoxSR.Text.Remove(30, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(30, "1");
            }
            else
            {
                textBoxSR.Text = textBoxSR.Text.Remove(30, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(30, "0");
            }
        }
        /// <summary>
        /// Postavi zastavica V
        /// </summary>
        /// <returns>V</returns>
        private void setZastavicaV(bool zastavica)
        {
            if (zastavica)
            {
                textBoxSR.Text = textBoxSR.Text.Remove(29, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(29, "1");
            }
            else
            {
                textBoxSR.Text = textBoxSR.Text.Remove(29, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(29, "0");
            }
        }
        /// <summary>
        /// Postavi zastavica N
        /// </summary>
        /// <returns>N</returns>
        private void setZastavicaN(bool zastavica)
        {
            if (zastavica)
            {
                textBoxSR.Text = textBoxSR.Text.Remove(31, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(31, "1");

                //if (zastavica)
                //{ }
            }
            else
            {
                textBoxSR.Text = textBoxSR.Text.Remove(31, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(31, "0");
            }
        }
        /// <summary>
        /// Postavi zastavica Z
        /// </summary>
        /// <returns>Z</returns>
        private void setZastavicaZ(bool zastavica)
        {
            if (zastavica)
            {
                textBoxSR.Text = textBoxSR.Text.Remove(28, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(28, "1");
            }
            else
            {
                textBoxSR.Text = textBoxSR.Text.Remove(28, 1);
                textBoxSR.Text = textBoxSR.Text.Insert(28, "0");
            }
        }

        /// <summary>
        /// Radi xor dvije zastavice
        /// </summary>
        /// <param name="zastavicaA"></param>
        /// <param name="zastavicaB"></param>
        /// <returns></returns>
        private bool xor(bool zastavicaA, bool zastavicaB)
        {
            if (zastavicaA != zastavicaB)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Pretvara binarni niz u heksadekadski niz
        /// </summary>
        /// <param name="stringBit">biarni niz</param>
        /// <returns>heksadekadski niz</returns>
        private string stringBitToStringHex(string stringBit)
        {
            string stringHex = "";

            while(stringBit.Length > 3)
            {
                switch (stringBit.Substring(0, 4))
                {
                    case "0000":
                        stringHex += "0";
                        break;
                    case "0001":
                        stringHex += "1";
                        break;
                    case "0010":
                        stringHex += "2";
                        break;
                    case "0011":
                        stringHex += "3";
                        break;
                    case "0100":
                        stringHex += "4";
                        break;
                    case "0101":
                        stringHex += "5";
                        break;
                    case "0110":
                        stringHex += "6";
                        break;
                    case "0111":
                        stringHex += "7";
                        break;
                    case "1000":
                        stringHex += "8";
                        break;
                    case "1001":
                        stringHex += "9";
                        break;
                    case "1010":
                        stringHex += "A";
                        break;
                    case "1011":
                        stringHex += "B";
                        break;
                    case "1100":
                        stringHex += "C";
                        break;
                    case "1101":
                        stringHex += "D";
                        break;
                    case "1110":
                        stringHex += "E";
                        break;
                    case "1111":
                        stringHex += "F";
                        break;
                }
                stringBit = stringBit.Remove(0, 4);
            }

            return stringHex;
        }
        /// <summary>
        /// Pretvara binarni niz u cijeli broj
        /// </summary>
        /// <param name="stringBit">binarni niz</param>
        /// <returns>cijeli broj</returns>
        private int stringBitToInt(string stringBit)
        {
            return Convert.ToInt32(stringBit, 2);
        }
        /// <summary>
        /// Pretvara binarni niz u pozitivan cijeli broj
        /// </summary>
        /// <param name="stringBit">binarni niz</param>
        /// <returns>cijeli broj</returns>
        private uint stringBitToUInt(string stringBit)
        {
            return Convert.ToUInt32(stringBit, 2);
        }
        /// <summary>
        /// Pretvara cijeli broj u niz bitova
        /// </summary>
        /// <param name="broj">Cijeli broj</param>
        /// <returns>Niz bitova</returns>
        private string intToStringBit(int broj)
        {
            string stringBit = "";
            /*
            BinaryWriter bw = new BinaryWriter(File.Open(@"..\..\Datoteke\Temp.txt", FileMode.Open));
            bw.Write(broj);
            bw.Close();

            BinaryReader br = new BinaryReader(File.Open(@"..\..\Datoteke\Temp.txt", FileMode.Open));
            uint b = br.ReadUInt32();
             * br.Close();
            */

            byte[] bytes = BitConverter.GetBytes(broj);
            uint b = BitConverter.ToUInt32(bytes, 0);
            
            for (int i = 0; i < 32; i++)
            {
                if (b % 2 == 1)
                {
                    stringBit = "1" + stringBit;
                }
                else
                {
                    stringBit = "0" + stringBit;
                }

                b = b / 2;
            }
            for (int i = stringBit.Length; i < 32; i++)
            {
                stringBit = "0" + stringBit;
            }

            return stringBit;
        }     
        /// <summary>
        /// Pretvara realni broj u niz bitova
        /// </summary>
        /// <param name="broj">Cijeli broj</param>
        /// <returns>Niz bitova</returns>
        private string floatToStringBit(float broj)
        {
            string stringBit = "";
            /*
            BinaryWriter bw = new BinaryWriter(File.Open(@"..\..\Datoteke\Temp.txt", FileMode.Open));
            bw.Write(broj);
            bw.Close();

            BinaryReader br = new BinaryReader(File.Open(@"..\..\Datoteke\Temp.txt", FileMode.Open));

            uint b = br.ReadUInt32();
            br.Close();
            */

            byte[] bytes = BitConverter.GetBytes(broj);
            uint b = BitConverter.ToUInt32(bytes, 0);

            for (int i = 0; i < 32; i++)
            {
                if (b % 2 == 1)
                {
                    stringBit = "1" + stringBit;
                }
                else
                {
                    stringBit = "0" + stringBit;
                }

                b = b / 2;
            }
            for (int i = stringBit.Length; i < 32; i++)
            {
                stringBit = "0" + stringBit;
            }

            return stringBit;
        }
        /// <summary>
        /// Pretvara niz bitova u realni broj
        /// </summary>
        /// <param name="stringBit">Niz bitova</param>
        /// <returns>Realni broj</returns>
        private float stringBitToFloat(string stringBit)
        {
            uint num = uint.Parse(stringBitToStringHex(stringBit), System.Globalization.NumberStyles.AllowHexSpecifier);
            byte[] floatVals = BitConverter.GetBytes(num);
            float f = BitConverter.ToSingle(floatVals, 0);

            return f;
        }
        /// <summary>
        /// Pretvara heksadekadski broj u niz bitova
        /// </summary>
        /// <param name="stringHex">heksadekadski broj</param>
        /// <returns>niz bitova</returns>
        private string hexToStringBit(string stringHex)
        {
            string stringBit = "";
            for (int i = 0; i < stringHex.Length; i++)
            {
                switch (stringHex[i])
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
            return stringBit;
        }
        /// <summary>
        /// Pretvori podatak u 4 bajta. Doda mu nule na pocetak ili ga skrati ovisno o potrebi
        /// </summary>
        /// <param name="podatak">Podatak koji pretvaramo</param>
        /// <returns>Podatak u 4 bajta</returns>
        private string pretvoriU4Bajta(string podatak)
        {
            string podatakU4Bajta = "";

            if (podatak.Length < 32)
            {
                for (int i = 0; i < 32 - podatak.Length; i++)
                {
                    podatakU4Bajta += "0";
                }
                podatakU4Bajta += podatak;

                return podatakU4Bajta;
            }
            else if (podatak.Length > 32)
            {
                return podatak.Remove(0, podatak.Length - 32);
            }

            return podatak;
        }
        /// <summary>
        /// Provjerava je li adresa postoji u memoriji ako ne postoji prosiruje memoriju
        /// </summary>
        /// <param name="adresa">Adresa koju provjeravamo</param>
        private void provjeriMemoriju(int adresa)
        {
            if (dataGridView1.Rows.Count - 10 <= adresa / 4)
            {
                DataGridViewRow r;
                DataGridViewTextBoxCell c;

                for (int i = dataGridView1.Rows.Count; i < adresa / 4 + 200; i++)
                {
                    r = new DataGridViewRow();

                    c = new DataGridViewTextBoxCell();
                    c.Value = "00000000000000000000000000000000";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = stringBitToStringHex(intToStringBit(i * 4));
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "";
                    r.Cells.Add(c);

                    c = new DataGridViewTextBoxCell();
                    c.Value = "0";
                    r.Cells.Add(c);

                    dataGridView1.Rows.Add(r);
                }
            }
        }

        /// <summary>
        /// Odredi koja je trenutna naredba na koji 
        /// pokazuje PC te je izvede i poveca PC za 4
        /// </summary>
        private void izvediNaredbu()
        {
            KodRed kr = new KodRed((string)dataGridView1.Rows[stringBitToInt(textBoxPC.Text) / 4].Cells["ColumnLabel"].Value, (string)dataGridView1.Rows[stringBitToInt(textBoxPC.Text) / 4].Cells["ColumnSadrzaj"].Value);

            obojajRedUBijelo(stringBitToInt(textBoxPC.Text) / 4);
            textBoxPC.Text = intToStringBit((stringBitToInt(textBoxPC.Text) + 4));

            switch (kr.Naredba[0])
            {
                ///Naredbe:

                #region aritmeticko-logicke
                //int
                #region ADD
                case "ADD":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = add(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region SUB
                case "SUB":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = sub(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region MUL
                case "MUL":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = mul(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region DIV
                case "DIV":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = div(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region CMP
                case "CMP":
                    cmp(kr.Naredba[1], kr.Naredba[2]);
                    break;
                #endregion

                //float
                #region ADDF
                case "ADDF":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = addf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region SUBF
                case "SUBF":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = subf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region MULF
                case "MULF":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = mulf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region DIVF
                case "DIVF":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = divf(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion    

                #region CMPF
                case "CMPF":
                    cmpf(kr.Naredba[1], kr.Naredba[2]);
                    break;
                #endregion

                //ostale
                #region AND
                case "AND":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = and(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region OR
                case "OR":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = or(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region XOR
                case "XOR":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = xor(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region SHR
                case "SHR":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = shr(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion

                #region SHL
                case "SHL":
                    switch (kr.Naredba[3])
                    {
                        case "R1":
                            textBoxR1.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R2":
                            textBoxR2.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R3":
                            textBoxR3.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R4":
                            textBoxR4.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R5":
                            textBoxR5.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R6":
                            textBoxR6.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                        case "R7":
                            textBoxR7.Text = shl(kr.Naredba[1], kr.Naredba[2]);
                            break;
                    }
                    break;
                #endregion
                #endregion

                #region registarske
                #region MOVE
                case "MOVE":
                    switch (kr.Naredba[2])
                    {
                        case "R1":
                            textBoxR1.Text = move(kr.Naredba[1]);
                            break;
                        case "R2":
                            textBoxR2.Text = move(kr.Naredba[1]);
                            break;
                        case "R3":
                            textBoxR3.Text = move(kr.Naredba[1]);
                            break;
                        case "R4":
                            textBoxR4.Text = move(kr.Naredba[1]);
                            break;
                        case "R5":
                            textBoxR5.Text = move(kr.Naredba[1]);
                            break;
                        case "R6":
                            textBoxR6.Text = move(kr.Naredba[1]);
                            break;
                        case "R7":
                            textBoxR7.Text = move(kr.Naredba[1]);
                            break;
                        case "SR":
                            textBoxSR.Text = move(kr.Naredba[1]);
                            break;
                    }
                    break;
                #endregion

                #region ITF
                case "ITF":
                    switch (kr.Naredba[2])
                    {
                        case "R1":
                            textBoxR1.Text = itf(kr.Naredba[1]);
                            break;
                        case "R2":
                            textBoxR2.Text = itf(kr.Naredba[1]);
                            break;
                        case "R3":
                            textBoxR3.Text = itf(kr.Naredba[1]);
                            break;
                        case "R4":
                            textBoxR4.Text = itf(kr.Naredba[1]);
                            break;
                        case "R5":
                            textBoxR5.Text = itf(kr.Naredba[1]);
                            break;
                        case "R6":
                            textBoxR6.Text = itf(kr.Naredba[1]);
                            break;
                        case "R7":
                            textBoxR7.Text = itf(kr.Naredba[1]);
                            break;
                        case "SR":
                            textBoxSR.Text = itf(kr.Naredba[1]);
                            break;
                    }
                    break;
                #endregion

                #region FTI
                case "FTI":
                    switch (kr.Naredba[2])
                    {
                        case "R1":
                            textBoxR1.Text = fti(kr.Naredba[1]);
                            break;
                        case "R2":
                            textBoxR2.Text = fti(kr.Naredba[1]);
                            break;
                        case "R3":
                            textBoxR3.Text = fti(kr.Naredba[1]);
                            break;
                        case "R4":
                            textBoxR4.Text = fti(kr.Naredba[1]);
                            break;
                        case "R5":
                            textBoxR5.Text = fti(kr.Naredba[1]);
                            break;
                        case "R6":
                            textBoxR6.Text = fti(kr.Naredba[1]);
                            break;
                        case "R7":
                            textBoxR7.Text = fti(kr.Naredba[1]);
                            break;
                        case "SR":
                            textBoxSR.Text = fti(kr.Naredba[1]);
                            break;
                    }
                    break;
                #endregion
                #endregion

                #region memorijeske
                #region LOAD
                case "LOAD":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            textBoxR1.Text = load(kr, 4);
                            break;
                        case "R2":
                            textBoxR2.Text = load(kr, 4);
                            break;
                        case "R3":
                            textBoxR3.Text = load(kr, 4);
                            break;
                        case "R4":
                            textBoxR4.Text = load(kr, 4);
                            break;
                        case "R5":
                            textBoxR5.Text = load(kr, 4);
                            break;
                        case "R6":
                            textBoxR6.Text = load(kr, 4);
                            break;
                        case "R7":
                            textBoxR7.Text = load(kr, 4);
                            break;
                    }
                    break;
                #endregion

                #region LOADH
                case "LOADH":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            textBoxR1.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R2":
                            textBoxR2.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R3":
                            textBoxR3.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R4":
                            textBoxR4.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R5":
                            textBoxR5.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R6":
                            textBoxR6.Text = "0000000000000000" + load(kr, 2);
                            break;
                        case "R7":
                            textBoxR7.Text = "0000000000000000" + load(kr, 2);
                            break;
                    }
                    break;
                #endregion

                #region LOADB
                case "LOADB":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            textBoxR1.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R2":
                            textBoxR2.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R3":
                            textBoxR3.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R4":
                            textBoxR4.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R5":
                            textBoxR5.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R6":
                            textBoxR6.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                        case "R7":
                            textBoxR7.Text = "000000000000000000000000" + load(kr, 1);
                            break;
                    }
                    break;
                #endregion

                #region STORE
                case "STORE":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            store(textBoxR1.Text, kr, 4);
                            break;
                        case "R2":
                            store(textBoxR2.Text, kr, 4);
                            break;
                        case "R3":
                            store(textBoxR3.Text, kr, 4);
                            break;
                        case "R4":
                            store(textBoxR4.Text, kr, 4);
                            break;
                        case "R5":
                            store(textBoxR5.Text, kr, 4);
                            break;
                        case "R6":
                            store(textBoxR6.Text, kr, 4);
                            break;
                        case "R7":
                            store(textBoxR7.Text, kr, 4);
                            break;
                    }
                    break;
                #endregion

                #region STOREH
                case "STOREH":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            store(textBoxR1.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R2":
                            store(textBoxR2.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R3":
                            store(textBoxR3.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R4":
                            store(textBoxR4.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R5":
                            store(textBoxR5.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R6":
                            store(textBoxR6.Text.Substring(16, 16), kr, 2);
                            break;
                        case "R7":
                            store(textBoxR7.Text.Substring(16, 16), kr, 2);
                            break;
                    }
                    break;
                #endregion

                #region STOREB
                case "STOREB":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            store(textBoxR1.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R2":
                            store(textBoxR2.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R3":
                            store(textBoxR3.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R4":
                            store(textBoxR4.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R5":
                            store(textBoxR5.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R6":
                            store(textBoxR6.Text.Substring(24, 8), kr, 1);
                            break;
                        case "R7":
                            store(textBoxR7.Text.Substring(24, 8), kr, 1);
                            break;
                    }
                    break;
                #endregion

                #region PUSH
                case "PUSH":
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            push(textBoxR1.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R2":
                            push(textBoxR2.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R3":
                            push(textBoxR3.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R4":
                            push(textBoxR4.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R5":
                            push(textBoxR5.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R6":
                            push(textBoxR6.Text, stringBitToInt(textBoxR7.Text));
                            break;
                        case "R7":
                            push(textBoxR7.Text, stringBitToInt(textBoxR7.Text));
                            break;
                    }
                    textBoxR7.Text = intToStringBit((stringBitToInt(textBoxR7.Text) + 4));
                    break;
                #endregion

                #region POP
                case "POP":
                    textBoxR7.Text = intToStringBit((stringBitToInt(textBoxR7.Text) - 4));
                    switch (kr.Naredba[1])
                    {
                        case "R1":
                            textBoxR1.Text = pop();
                            break;
                        case "R2":
                            textBoxR2.Text = pop();
                            break;
                        case "R3":
                            textBoxR3.Text = pop();
                            break;
                        case "R4":
                            textBoxR4.Text = pop();
                            break;
                        case "R5":
                            textBoxR5.Text = pop();
                            break;
                        case "R6":
                            textBoxR6.Text = pop();
                            break;
                        case "R7":
                            textBoxR7.Text = pop();
                            break;
                    }
                    break;
                #endregion
                #endregion

                #region upravljacke
                #region JP
                case "JP":
                    textBoxPC.Text = jump(kr.Naredba[1]);
                    break;
                case "JP_C":
                    if (zastavicaC())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_NC":
                    if (!zastavicaC())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_V":
                    if (zastavicaV())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_NV":
                    if (!zastavicaV())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_N":
                    if (zastavicaN())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_NN":
                    if (!zastavicaN())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_M":
                    if (zastavicaN())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_P":
                    if (!zastavicaN())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_Z":
                    if (zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_NZ":
                    if (!zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_EQ":
                    if (zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_NE":
                    if (!zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_ULE":
                    if (zastavicaC() || zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_UGT":
                    if (!zastavicaC() && !zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_ULT":
                    if (zastavicaC())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_UGE":
                    if (!zastavicaC())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_SLE":
                    if (xor(zastavicaN(), zastavicaV()) || zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_SGT":
                    if (!xor(zastavicaN(), zastavicaV()) && !zastavicaZ())
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_SLT":
                    if (xor(zastavicaN(), zastavicaV()))
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                case "JP_SGE":
                    if (!xor(zastavicaN(), zastavicaV()))
                    {
                        textBoxPC.Text = jump(kr.Naredba[1]);
                    }
                    break;
                #endregion

                #region JR
                case "JR":
                    textBoxPC.Text = jumpr(kr.Naredba[1]);
                    break;
                case "JR_C":
                    if (zastavicaC())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_NC":
                    if (!zastavicaC())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_V":
                    if (zastavicaV())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_NV":
                    if (!zastavicaV())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_N":
                    if (zastavicaN())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_NN":
                    if (!zastavicaN())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_M":
                    if (zastavicaN())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_P":
                    if (!zastavicaN())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_Z":
                    if (zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_NZ":
                    if (!zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_EQ":
                    if (zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_NE":
                    if (!zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_ULE":
                    if (zastavicaC() || zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_UGT":
                    if (!zastavicaC() && !zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_ULT":
                    if (zastavicaC())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_UGE":
                    if (!zastavicaC())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_SLE":
                    if (xor(zastavicaN(), zastavicaV()) || zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_SGT":
                    if (!xor(zastavicaN(), zastavicaV()) && !zastavicaZ())
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_SLT":
                    if (xor(zastavicaN(), zastavicaV()))
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                case "JR_SGE":
                    if (!xor(zastavicaN(), zastavicaV()))
                    {
                        textBoxPC.Text = jumpr(kr.Naredba[1]);
                    }
                    break;
                #endregion

                #region CALL
                case "CALL":
                    call(kr.Naredba[1]);
                    break;
                case "CALL_C":
                    if (zastavicaC())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_NC":
                    if (!zastavicaC())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_V":
                    if (zastavicaV())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_NV":
                    if (!zastavicaV())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_N":
                    if (zastavicaN())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_NN":
                    if (!zastavicaN())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_M":
                    if (zastavicaN())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_P":
                    if (!zastavicaN())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_Z":
                    if (zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_NZ":
                    if (!zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_EQ":
                    if (zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_NE":
                    if (!zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_ULE":
                    if (zastavicaC() || zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_UGT":
                    if (!zastavicaC() && !zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_ULT":
                    if (zastavicaC())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_UGE":
                    if (!zastavicaC())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_SLE":
                    if (xor(zastavicaN(), zastavicaV()) || zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_SGT":
                    if (!xor(zastavicaN(), zastavicaV()) && !zastavicaZ())
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_SLT":
                    if (xor(zastavicaN(), zastavicaV()))
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                case "CALL_SGE":
                    if (!xor(zastavicaN(), zastavicaV()))
                    {
                        call(kr.Naredba[1]);
                    }
                    break;
                #endregion

                #region RET
                case "RET":
                    ret();
                    break;
                case "RET_C":
                    if (zastavicaC())
                    {
                        ret();
                    }
                    break;
                case "RET_NC":
                    if (!zastavicaC())
                    {
                        ret();
                    }
                    break;
                case "RET_V":
                    if (zastavicaV())
                    {
                        ret();
                    }
                    break;
                case "RET_NV":
                    if (!zastavicaV())
                    {
                        ret();
                    }
                    break;
                case "RET_N":
                    if (zastavicaN())
                    {
                        ret();
                    }
                    break;
                case "RET_NN":
                    if (!zastavicaN())
                    {
                        ret();
                    }
                    break;
                case "RET_M":
                    if (zastavicaN())
                    {
                        ret();
                    }
                    break;
                case "RET_P":
                    if (!zastavicaN())
                    {
                        ret();
                    }
                    break;
                case "RET_Z":
                    if (zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_NZ":
                    if (!zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_EQ":
                    if (zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_NE":
                    if (!zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_ULE":
                    if (zastavicaC() || zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_UGT":
                    if (!zastavicaC() && !zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_ULT":
                    if (zastavicaC())
                    {
                        ret();
                    }
                    break;
                case "RET_UGE":
                    if (!zastavicaC())
                    {
                        ret();
                    }
                    break;
                case "RET_SLE":
                    if (xor(zastavicaN(), zastavicaV()) || zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_SGT":
                    if (!xor(zastavicaN(), zastavicaV()) && !zastavicaZ())
                    {
                        ret();
                    }
                    break;
                case "RET_SLT":
                    if (xor(zastavicaN(), zastavicaV()))
                    {
                        ret();
                    }
                    break;
                case "RET_SGE":
                    if (!xor(zastavicaN(), zastavicaV()))
                    {
                        ret();
                    }
                    break;
                #endregion

                #region HALT
                case "HALT":
                    halt();
                    break;
                case "HALT_C":
                    if (zastavicaC())
                    {
                        halt();
                    }
                    break;
                case "HALT_NC":
                    if (!zastavicaC())
                    {
                        halt();
                    }
                    break;
                case "HALT_V":
                    if (zastavicaV())
                    {
                        halt();
                    }
                    break;
                case "HALT_NV":
                    if (!zastavicaV())
                    {
                        halt();
                    }
                    break;
                case "HALT_N":
                    if (zastavicaN())
                    {
                        halt();
                    }
                    break;
                case "HALT_NN":
                    if (!zastavicaN())
                    {
                        halt();
                    }
                    break;
                case "HALT_M":
                    if (zastavicaN())
                    {
                        halt();
                    }
                    break;
                case "HALT_P":
                    if (!zastavicaN())
                    {
                        halt();
                    }
                    break;
                case "HALT_Z":
                    if (zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_NZ":
                    if (!zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_EQ":
                    if (zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_NE":
                    if (!zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_ULE":
                    if (zastavicaC() || zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_UGT":
                    if (!zastavicaC() && !zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_ULT":
                    if (zastavicaC())
                    {
                        halt();
                    }
                    break;
                case "HALT_UGE":
                    if (!zastavicaC())
                    {
                        halt();
                    }
                    break;
                case "HALT_SLE":
                    if (xor(zastavicaN(), zastavicaV()) || zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_SGT":
                    if (!xor(zastavicaN(), zastavicaV()) && !zastavicaZ())
                    {
                        halt();
                    }
                    break;
                case "HALT_SLT":
                    if (xor(zastavicaN(), zastavicaV()))
                    {
                        halt();
                    }
                    break;
                case "HALT_SGE":
                    if (!xor(zastavicaN(), zastavicaV()))
                    {
                        halt();
                    }
                    break;
                #endregion
                #endregion
            }
            obojajRed(stringBitToInt(textBoxPC.Text) / 4);
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R1
        /// </summary>
        private void textBoxR1_MouseEnter(object sender, EventArgs e)
        {
            textBoxR1Prikaz.Visible = true;
            textBoxR1Prikaz.Text = "int:  " + stringBitToInt(textBoxR1.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR1.Text).ToString() + "\r\n" 
                                 + "hex:  " + stringBitToStringHex(textBoxR1.Text);
        }
        private void textBoxR1_MouseLeave(object sender, EventArgs e)
        {
            textBoxR1Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R2
        /// </summary>
        private void textBoxR2_MouseEnter(object sender, EventArgs e)
        {
            textBoxR2Prikaz.Visible = true;
            textBoxR2Prikaz.Text = "int:  " + stringBitToInt(textBoxR2.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR2.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR2.Text);
        }
        private void textBoxR2_MouseLeave(object sender, EventArgs e)
        {
            textBoxR2Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R3
        /// </summary>
        private void textBoxR3_MouseEnter(object sender, EventArgs e)
        {
            textBoxR3Prikaz.Visible = true;
            textBoxR3Prikaz.Text = "int:  " + stringBitToInt(textBoxR3.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR3.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR3.Text);
        }
        private void textBoxR3_MouseLeave(object sender, EventArgs e)
        {
            textBoxR3Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R4
        /// </summary>
        private void textBoxR4_MouseEnter(object sender, EventArgs e)
        {
            textBoxR4Prikaz.Visible = true;
            textBoxR4Prikaz.Text = "int:  " + stringBitToInt(textBoxR4.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR4.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR4.Text);
        }
        private void textBoxR4_MouseLeave(object sender, EventArgs e)
        {
            textBoxR4Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R5
        /// </summary>
        private void textBoxR5_MouseEnter(object sender, EventArgs e)
        {
            textBoxR5Prikaz.Visible = true;
            textBoxR5Prikaz.Text = "int:  " + stringBitToInt(textBoxR5.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR5.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR5.Text);
        }
        private void textBoxR5_MouseLeave(object sender, EventArgs e)
        {
            textBoxR5Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R6
        /// </summary>
        private void textBoxR6_MouseEnter(object sender, EventArgs e)
        {
            textBoxR6Prikaz.Visible = true;
            textBoxR6Prikaz.Text = "int:  " + stringBitToInt(textBoxR6.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR6.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR6.Text);
        }
        private void textBoxR6_MouseLeave(object sender, EventArgs e)
        {
            textBoxR6Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra R7
        /// </summary>
        private void textBoxR7_MouseEnter(object sender, EventArgs e)
        {
            textBoxR7Prikaz.Visible = true;
            textBoxR7Prikaz.Text = "int:  " + stringBitToInt(textBoxR7.Text).ToString() + "\r\n"
                                 + "float:  " + stringBitToFloat(textBoxR7.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxR7.Text);
        }
        private void textBoxR7_MouseLeave(object sender, EventArgs e)
        {
            textBoxR7Prikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra PC
        /// </summary>
        private void textBoxPC_MouseEnter(object sender, EventArgs e)
        {
            textBoxPCPrikaz.Visible = true;
            textBoxPCPrikaz.Text = "int:  " + stringBitToInt(textBoxPC.Text).ToString() + "\r\n"
                                 + "hex:  " + stringBitToStringHex(textBoxPC.Text);
        }
        private void textBoxPC_MouseLeave(object sender, EventArgs e)
        {
            textBoxPCPrikaz.Visible = false;
        }

        /// <summary>
        /// Prikazuje sadrzaj registra SR
        /// </summary>
        private void textBoxSR_MouseEnter(object sender, EventArgs e)
        {
            textBoxSRPrikaz.Visible = true;
            textBoxSRPrikaz.Text = "Z = " + textBoxSR.Text[28] + "\t\t V = " + textBoxSR.Text[29] + "\r\n"
                                 + "C = " + textBoxSR.Text[30] + "\t\t N = " + textBoxSR.Text[31];
        }
        private void textBoxSR_MouseLeave(object sender, EventArgs e)
        {
            textBoxSRPrikaz.Visible = false;
        }
        
        /// <summary>
        /// Resetira formu
        /// </summary>
        private void restartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            kraj = false;

            toolStripButton1.Enabled = true;
            toolStripButton2.Enabled = true;

            inicijalizacijaMemorije();
            inicijalizacijaRegistara();

            obojajRed(stringBitToInt(hexToStringBit(textBoxPC.Text)));
        }
        /// <summary>
        /// Zatvori aplikaciju
        /// </summary>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// Izvede jednu naredbu
        /// </summary>
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            izvediNaredbu();
        }
        /// <summary>
        /// Izvodi sve naredbe dok ne obradi naredbu HALT
        /// </summary>
        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            while (!kraj)
            {
                izvediNaredbu();
            }
        }
        /// <summary>
        /// Resetira formu
        /// </summary>
        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            kraj = false;

            toolStripButton1.Enabled = true;
            toolStripButton2.Enabled = true;

            inicijalizacijaMemorije();
            inicijalizacijaRegistara();

            obojajRed(stringBitToInt(hexToStringBit(textBoxPC.Text)));
        }
        /// <summary>
        /// Ucita novi kod
        /// </summary>
        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            try
            {
                FileDialog folder = new OpenFileDialog();
                folder.Filter = "Files of type (*.o)|*.o|(*.txt)|*.txt";
                folder.ShowDialog();
                if (folder.FileName != "")
                {
                    Properties.Settings.Default.path = folder.FileName;

                    kraj = false;

                    toolStripButton1.Enabled = true;
                    toolStripButton2.Enabled = true;

                    inicijalizacijaMemorije();
                    inicijalizacijaRegistara();

                    obojajRed(stringBitToInt(hexToStringBit(textBoxPC.Text)));
                }
            }
            catch
            {
                MessageBox.Show("Došlo je do greške");
            }
        }

    }
}