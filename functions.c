/*NICOLESCU Daniel-Marian-314CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "materie.h"
#include "student.h"
#include "hash.h"

/*
 * data - key to compute hash
 * len - sizeof of the data
 * range - the number of buckets
 * Return the index of the bucket
 */
unsigned int hash_f(const void *data, size_t len, size_t range) {
	unsigned int hash = 0u;
	size_t idx = 0;
	size_t cof = 1;
	size_t rate = 2;
	const char *d = (const char*)data;

	for(; idx < len; ++idx) {
		hash += d[idx] * cof;
		cof *= rate;
	}

	return hash % range;
}

//afisarea cheii int
void show_int_key(void *key, FILE *output)
{
	fprintf(output, "%d", *(int*)key);
}

//afisarea cheii string
void show_string_key(void *key, FILE *output)
{
	fprintf(output, "%s", (char*)key);
}

//afisarea valorii de tip TMaterie
void show_mat_value(void *value, FILE *output)
{
	fprintf(output, "[Materie: %s, Ore_curs: %d, Ore_lab: %d, Credit: %d, Teme: %d]",
	 ((TMaterie*)value)->materie, ((TMaterie*)value)->ore_curs, 
	((TMaterie*)value)->ore_lab, ((TMaterie*)value)->credit, ((TMaterie*)value)->teme);
}

//afisarea valorii de tip TStudent
void show_stud_value(void *value, FILE *output)
{
	fprintf(output, "[Nume: %s, Grupa: %s, Medie: %.2f, Varsta: %d]", 
		((TStudent*)value)->nume, ((TStudent*)value)->grupa, 
		((TStudent*)value)->medie, ((TStudent*)value)->varsta);
}

//afisarea Hash tabel-ului
void Print_Hash(TH *h, size_t nr, FILE *output)
{
	int i;
	TLG p;
	ShowKey show_key = h->show_key;

	for(i = 0; i < nr; i++)
	{
		p = h->v[i];
		fprintf(output, "[%d] :", i);

		for(; p != NULL; p = p->urm)
		{
			ShowValue show_value = ((TInfo*)p->info)->show_value;
			fprintf(output, " (");
			show_key(((TInfo*)p->info)->key, output);
			fprintf(output, " -> ");
			show_value(((TInfo*)p->info)->value, output);
			fprintf(output, ")");
		}

		fprintf(output, "\n");
	}
}

//compararea a 2 chei int
int int_cmp(void* a, void* b)
{
	if(*(int*)a == *(int*)b)
		return 0;
	return 1;
}

//compararea a 2 chei string
int string_cmp(void* a, void* b)
{
	if(!strcmp((char*)a, (char*)b))
		return 0;
	return 1;
}

//eliberarea din memorie a cheii 
void free_key(void *info)
{
	TInfo *aux;

	aux = (TInfo*)info;
	free((TMaterie*)aux->key);
}

//eliberarea din memorie a structurii TMaterie
void free_mat_value(void *info)
{
	TInfo *aux;

	aux = (TInfo*)info;

	free(((TMaterie*)aux->value)->materie);
	free((TMaterie*)aux->value);
}

//eliberarea din memorie a structurii TStudent
void free_stud_value(void *info)
{
	TInfo *aux;

	aux = (TInfo*)info;

	free(((TStudent*)aux->value)->nume);
	free((TStudent*)aux->value);
}

//initializarea Hash tabel-ului
TH* IniTH(size_t nr_buckets, TFHash hash_f, KeyCmp cmp, ShowKey show_key, FreeKey free_key)
{
	TH *h = (TH*)calloc(sizeof(TH),1);
	size_t i;
	if (!h) 
	{	
		printf("eroare alocare hash\n");
		return NULL;
	}

	//am folosit calloc pentru a face pointerii NULL
	h->v = (TLG*)calloc(nr_buckets, sizeof(TLG));

	if(!h->v)
	{
		printf("eroare alocare vector de pointeri TLG in hash\n");
		free(h);
		return NULL;
	}

	h->fh = hash_f;
	h->cmp = cmp;
	h->show_key = show_key;
	h->free_key = free_key;

	return h;
}

//inserarea unui element in Hash table
int InsTH(TH* h, void* a_val, size_t val_dim, void* a_key, size_t key_dim,
	size_t range, ShowValue show_value, FreeValue free_value)
{
	//stabileste bucket-ul corespunzator, apoi apeleaza functia de inserare
	//a unui element in lista generica
	size_t cod = h->fh(a_key, key_dim, range), rez;
	if (cod < 0) return 0;

	rez = InsLG(&(h->v[cod]), h->cmp, a_val, val_dim, a_key, key_dim, 
		show_value, free_value, h->free_key);

	return rez;	
}

//inserarea unui element intr-o lista generica
int InsLG(ALG aL, KeyCmp cmp, void* a_val, size_t val_dim, void* a_key, 
	size_t key_dim, ShowValue show_value, FreeValue free_value, FreeKey free_key)
{
	//alocare celula pentru elementul aux ce urmeaza sa fie inserat
  	TLG aux = (TCelulaG*)malloc(sizeof(TCelulaG));
	TLG p = *aL, ant = NULL;

  	if(!aux) return 0;

  	aux->info = (TInfo*)malloc(sizeof(TInfo));
  	if (!aux->info)
  	{ 
  		free(aux); 
  		return 0;
  	}

  	((TInfo*)aux->info)->value = malloc(val_dim);
  	if(!((TInfo*)aux->info)->value)
  	{
  		free(aux->info);
  		free(aux);
  		return 0;
  	}

  	((TInfo*)aux->info)->key = malloc(key_dim);
  	if(!((TInfo*)aux->info)->key)
  	{
  		free(((TInfo*)aux->info)->value);
  		free(aux->info);
  		free(aux);
  		return 0;
  	}

  	//copierea informatiei
  	memcpy(((TInfo*)aux->info)->value, a_val, val_dim);
  	memcpy(((TInfo*)aux->info)->key, a_key, key_dim);
  	((TInfo*)aux->info)->show_value = show_value;
  	((TInfo*)aux->info)->free_value = free_value;

  	for(; p != NULL; ant = p, p = p->urm)
  	{
  		//in cazul in care exista deja un element cu acea cheie,
  		//il inlocuieste
  		if(!cmp(a_key, ((TInfo*)p->info)->key))
  		{
  			//modificare legaturi
  			aux->urm = p->urm;

  			//in cazul in care trebuie inlocuit capul de lista
  			if(ant == NULL)
  				*aL = aux;
			else
				ant->urm = aux;

			//eliberarea din memorie a elementului inlocuit
			((TInfo*)p->info)->free_value(p->info);
			free_key(p->info);
			free(p->info);
			free(p);
			return 1;
  		}
  	}

  	//cazul in care nu inlocuieste o intrare veche
  	if(ant == NULL)
  	{
  		*aL = aux;
  	}
  	else
  	{
  		ant->urm = aux;
  	}
  	aux->urm = NULL;

  	return 2;
}

//redimensionarea Hash tabel-ului
int ResTH(TH *h, size_t nr_buckets, size_t old_nr_buckets, size_t key_dim)
{
	TLG p, q, ant, ant_q, aux, *temp;
	size_t cod;
	size_t i;

	temp = (TLG*)realloc(h->v, nr_buckets * sizeof(TLG));
	//cazul in care nu memoria e insuficienta pentru a mari numarul
	//de bucket-uri
	if(!temp)
	{
		free_hash(&h, old_nr_buckets);
		return 0;
	}

	h->v = temp;

	for(i = old_nr_buckets; i < nr_buckets; i++)
		h->v[i] = NULL;

	//reintroducerea elementelor in Hash table pentru care
	//functia hash returneaza o alta pozitie decat cea curenta

	//avansare in vectorul de liste
	for(i = 0; i < old_nr_buckets; i++)
	{
		p = h->v[i];
		ant = NULL;

		//avansare in lista
		while(p != NULL)
		{
			cod = h->fh(((TInfo*)p->info)->key, key_dim, nr_buckets);

			//daca noua pozitie e diferita
			if(cod != i)
			{
				//daca e cap de lista
				if(ant == NULL)
				{
					aux = p;
					h->v[i] = h->v[i]->urm;
					p = h->v[i];
				}
				//daca nu e cap de lista
				else
				{
					aux = p;
					ant->urm = p->urm;
					p = p->urm;
				}

				ant_q = NULL;

				//itroducerea in noua lista

				//parcurgere lista
				for(q = h->v[cod]; q != NULL; ant_q = q, q = q->urm);

				//daca devine cap de lista
				if(ant_q == NULL)
				{
					h->v[cod] = aux;
				}
				else
				{
					ant_q->urm = aux;
				}
				aux->urm = NULL;
			}
			//daca pozitia in hash table nu se schimba, se avanseaza in lista
			else
			{
				ant = p;
				p = p->urm;
			}
		}

	}

	return 1;
}

//afisarea valorii ce are cheia key
void find_value_by_key(TH *h, void *key, size_t len, size_t range, FILE *output)
{
	//gasirea bucket-ului corespunzator
	size_t cod = h->fh(key, len, range);
	TLG p = h->v[cod];

	//parcurgere lista
	for(; p != NULL; p = p->urm)
	{
		//daca gaseste elementul cu cheia key, il afiseaza
		if(!h->cmp(key, ((TInfo*)p->info)->key))
		{
			((TInfo*)p->info)->show_value(((TInfo*)p->info)->value, output);
			fprintf(output, "\n");
			return;
		}		
	}
	//daca nu, afiseaza ca nu exista
	fprintf(output, "Nu exista\n");
	return;

}

//stergerea elementului cu cheia key
int del_by_key(TH *h, void *key, size_t len, size_t range)
{
	//gasirea bucket-ului corespunzator
	size_t cod = h->fh(key, len, range);
	TLG p = h->v[cod], ant = NULL, aux;

	while(p != NULL)
	{
		//daca gaseste elementul cu cheia key
		if(!h->cmp(key, ((TInfo*)p->info)->key))
		{
			aux = p;

			//modifica legaturile
			if(ant == NULL)
				h->v[cod] = h->v[cod]->urm;
			else
				ant->urm = p->urm;
			p = p->urm;

			//elibereaza din memorie elementul eliminat
			((TInfo*)aux->info)->free_value(aux->info);
			h->free_key(aux->info);
			free(aux->info);
			free(aux);
			return 1;
		}
		else
		//daca nu gaseste elementul avanseaza in lista
		{
			ant = p;
			p = p->urm;
		}		
	}
	return 0;

}

//eliberarea Hash tabel-ului din memorie
void free_hash(TH **h, size_t nr_buckets)
{
	TLG p, aux;
	size_t i;

	//parcurgere vector de liste
	for(i = 0; i < nr_buckets; i++)
	{
		p = (*h)->v[i];

		//parcurgere lista
		for(; p != NULL;)
		{
			//eliberarea din memorie a valorii si a cheii
			aux = p;
			p = p->urm;
			((TInfo*)aux->info)->free_value(aux->info);
			(*h)->free_key(aux->info);
			free(aux->info);
			free(aux);
		}
	}

	//eliberarea vectorului de liste
	free((*h)->v);
	//eliberarea Hash tabel-ului
	free(*h);
	*h = NULL;
}