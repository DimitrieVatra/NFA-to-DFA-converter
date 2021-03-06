#include "stdafx.h"
#include<iostream>
#include<fstream>
using namespace std;
ifstream f("date.in");
int ***adiac;
int NrNoduri, NrLitere,Start;

void Afisare()
{
	for (int i = 0; i < NrLitere; i++)
	{
		for (int j = 0; j < NrNoduri; j++)
		{
			for (int k = 0; k < NrNoduri; k++)
			{
				cout << adiac[k][j][i] << ' ';
			}cout << '\n';
		}cout << '\n';
	}
	cout << "Lista:\n";
	for (int i = 0; i < NrLitere; i++)
	{
		cout << i << '\n';
		for (int j = 0; j < NrNoduri; j++)
		{
			for (int k = 0; k < NrNoduri; k++)
			{
				if (adiac[k][j][i])cout << k << ' ' << j << '\n';
			}
		}
		cout << '\n';
	}
}
void MoveNod(int To, int From, int BothDirections = 0)
{
	for (int i = 0; i < NrNoduri; i++)
	{
		for (int j = 0; j < NrLitere; j++)
		{
			{
				if (BothDirections && adiac[i][To][j] == 0)
					adiac[i][To][j] = adiac[i][From][j];
				if (adiac[To][i][j] == 0)
					adiac[To][i][j] = adiac[From][i][j];
			}
		}
	}
}
void DeleteNod(int Nod)
{
	char ***Copy = new char **[NrNoduri - 1];
	for (int i = 0; i < NrNoduri; i++)
	{
		Copy[i] = new char*[NrNoduri];
		for (int j = 0; j < NrNoduri; j++)
		{
			Copy[i][j] = new char[NrLitere];
			for (int k = 0; k < NrLitere; k++)
			{
				Copy[i][j][k] = adiac[i][j][k];
			}
		}
	}
	NrNoduri--;
	delete adiac;
	adiac = new int**[NrNoduri];
	for (int i = 0; i < NrNoduri; i++)
	{
		adiac[i] = new int*[NrNoduri];
		for (int j = 0; j < NrNoduri; j++)
		{
			adiac[i][j] = new int[NrLitere];
			for (int k = 0; k < NrLitere; k++)
			{
				adiac[i][j][k] = 0;
			}
		}
	}
	for (int i = 0, i2 = 0; i < NrNoduri; i++, i2++, i2 += (i2 == Nod))
		for (int j = 0, j2 = 0; j < NrNoduri; j++, j2++, j2 += (j2 == Nod))
			for (int k = 0; k < NrLitere; k++)
				adiac[i][j][k] = Copy[i2][j2][k];
}
void DoubleNod(int Nod)
{
	char ***Copy = new char **[NrNoduri + 1];
	for (int i = 0; i < NrNoduri; i++)
	{
		Copy[i] = new char*[NrNoduri];
		for (int j = 0; j < NrNoduri; j++)
		{
			Copy[i][j] = new char[NrLitere];
			for (int k = 0; k < NrLitere; k++)
			{
				Copy[i][j][k] = adiac[i][j][k];
			}
		}
	}

	NrNoduri++;
	delete adiac;
	adiac = new int**[NrNoduri];
	for (int i = 0; i < NrNoduri; i++)
	{
		adiac[i] = new int*[NrNoduri];
		for (int j = 0; j < NrNoduri; j++)
		{
			adiac[i][j] = new int[NrLitere];
			for (int k = 0; k < NrLitere; k++)
			{
				adiac[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < NrNoduri; i++)
		for (int j = 0; j < NrNoduri; j++)
			for (int k = 0; k < NrLitere; k++)
				adiac[i][j][k] = Copy[i][j][k];
	MoveNod(Nod, NrNoduri - 1, 1);//Maybe reverted
}
void NFALtoNFA()
{
	bool ok = true;
	while (ok)
	{
		ok = false;
		for (int i = 0; i < NrNoduri; i++)
			for (int j = 0; j < NrNoduri && i < NrNoduri; j++)
				if (adiac[i][j][0])
				{
					adiac[i][j][0] = 0;
					ok = true;
					MoveNod(i, j);
				}
	}
	ok = true;
	//Afisare();
	while (ok)
	{
		ok = false;
		for (int i = 1; i < NrNoduri; i++)
		{
			bool SpreIVinNoduri = false;
			for (int j = 0; j < NrNoduri; j++)
			{
				for (int k = 0; k < NrLitere; k++)
					if (adiac[j][i][k]) { SpreIVinNoduri = true; goto ItDoes; }
				
			}
			ItDoes:;
			if (!SpreIVinNoduri)
			{
				DeleteNod(i);
				ok = true;
				break;
			}
		}
	}
}
int UsedID = 0;
class Nod
{
public:
	int ID;
	int Stari[100];
	int NrStari;
	Nod ***Adiacente;
	int *NrAdiac;
	Nod(int Parent)
	{
		ID = UsedID++;
		NrStari = 1;
		Stari[0] = Parent;
		Adiacente = new Nod**[NrLitere];
		for (int i = 0; i < NrLitere; i++)
			Adiacente[i] = NULL;
		NrAdiac = new int[NrLitere];
		for (int i = 0; i < NrLitere; i++)
			NrAdiac[i] = 0;
	}
	Nod(Nod *Parent,int Litera)
	{
		ID = -1;
		NrStari = Parent->NrAdiac[Litera];
		if (NrStari == 0)return;
		ID = UsedID++;
		for (int i = 0; i < Parent->NrAdiac[Litera]; i++)
		{
			Stari[i] = Parent->Adiacente[Litera][i]->ID;
		}
		NrAdiac = new int[NrLitere];
		for (int i = 0; i < NrLitere; i++)
			NrAdiac[i] = 0;
		for (int i = 0; i < 10; i++)
		{
			if (Parent->Adiacente[Litera][i] != NULL)
			{
				for (int k = 0; k < NrLitere; k++)
				{
					for (int j = 0; j < Parent->Adiacente[Litera][i]->NrAdiac[k];j++)
					{
						//adaugam la adiacente prin litera k unde i=nodul cu care Parent e adiacent,  k, litera prin care vom crea adiacnta
						LinkTo(Parent->Adiacente[Litera][i]->Adiacente[k][j],k);
					}
				}
			}
			
		}
	}
	void LinkTo(Nod *To,int Litera)
	{
		if (To == NULL)return;
		Nod **copie = new Nod*[10];
		for (int i = 0; i < 10; i++)copie[i] = NULL;
		if (Adiacente[Litera] = NULL)
		{
			copie[0] = To;
		}
		else
		{
			for (int j = 0; j < 10; j++)
				copie[j] = Adiacente[Litera][j];
			delete Adiacente[Litera];
		}
		Adiacente[Litera] = copie;
		NrAdiac[Litera]++;
	}
};

int cmp(const void *x, const void *y)
{
	return *(int*)x - *(int*)y;
}
Nod* Search(Nod *nod)
{
	qsort(nod->Stari, nod->NrStari, 4, cmp);
	for (int i = 0; i < NrNoduri; i++)
	{
		bool allthesame = true;
		qsort(Noduri[i]->Stari, Noduri[i]->NrStari, 4, cmp);
		for (int j = 0; j < Noduri[i]->NrStari; j++)
		{
			if (nod->Stari[j] != Noduri[i]->Stari[j])
				allthesame = false;
		}
		if (allthesame)return Noduri[i];
	}
	return NULL;
}
Nod **Noduri;
int NrNoduri = 0;
void NFAtoDFA()
{
	Noduri = new Nod*[100];
	NrNoduri = NrLitere;
	for (int i = 0; i < NrLitere; i++)
		Noduri[i] = new Nod(i);
	for (int i = 0; i < NrLitere; i++)
	{
		for (int j = 0; j < NrNoduri; j++)
		{
			for (int k = 0; k < NrNoduri; k++)
			{
				if (adiac[j][k][i])Noduri[j]->LinkTo(Noduri[k], i);
			}
		}
	}
	for (int i = 0; i < NrLitere; i++)
	{
		for (int j = 0; j < NrLitere; j++)
		{
			Nod *newnod = new Nod(Noduri[j], i);
			if (Search(newnod) != NULL)
			{
				Noduri[NrNoduri++] = newnod;
			}
		}
	}
}
void Citire()
{
	f >> NrNoduri >> NrLitere;
	adiac = new int**[NrNoduri];
	for (int i = 0; i < NrNoduri; i++)
	{
		adiac[i] = new int*[NrNoduri];
		for (int j = 0; j < NrNoduri; j++)
		{
			adiac[i][j] = new int[NrLitere];
			for (int k = 0; k < NrLitere; k++)
			{
				adiac[i][j][k]=0;
			}
		}
	}
	
	for (int i = 0; i < NrLitere; i++)
	{
		int NrLitera;
		f >> NrLitera;
		for (int j = 0; j < NrLitera; j++)
		{
			int a, b;
			f >> a >> b;
			adiac[a][b][i] = 1;
		}
	}
}

int main()
{
	Citire();
	cout << "NFA lambda to NFA\n";
	NFALtoNFA();
	Afisare();
	cout << "NFA to DFA\n";
	NFAtoDFA();
    Afisare();
	int x; cin >> x;
    return 0;
}

