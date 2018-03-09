/*NICOLESCU Daniel-Marian-314CB*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#include "materie.h"
#include "student.h"
#include "hash.h"

#define LOAD_FACTOR_THRESHOLD 0.75

int main()
{
	TH *hash_table = NULL;
	char inithash[9];
	char operatie[20];
	char tip_cheie;
	size_t nr_buckets;
	size_t old_nr_buckets;
	size_t nr_operatii;
	size_t i;
	size_t dim;
	void *key;
	void *key_to_find;
	void *del_key;
	char struct_type[5];
	//numarul de elemente inserate
	size_t k = 0;
	float load_factor;
	int check;

	//deschidere fisiere
	FILE *input = fopen("input.in", "rt");
	if(!input)
	{
		printf("Deschidere fisier esuata");
		return -1;
	}

	FILE *output = fopen("output.out", "wt");
	if(!output)
	{
		printf("Deschidere fisier esuata");
		return -1;
	}

	//alocare de memorie pentru cele doua structuri, TMaterie si TStudent
	TMaterie *struct_materie = (TMaterie*)malloc(sizeof(TMaterie));
	if(!struct_materie) return 1;

	TStudent *struct_student = (TStudent*)malloc(sizeof(TStudent));
	if(!struct_student) return 1;

	fscanf(input, "%s", inithash);
	fscanf(input, " %c", &tip_cheie);
	fscanf(input, " %zu", &nr_buckets);

	if(strcmp(inithash, "inithash"))
	{
		printf("eroare");
		return -1;
	}

	//initializarea Hash tabel-ului in functie de tipul cheii
	//si determinarea dimensiunii cheii
	if(tip_cheie == 's')
	{
		hash_table = IniTH(nr_buckets, hash_f, string_cmp, show_string_key, free_key);
		dim = 51 * sizeof(char);
	}
	if(tip_cheie == 'd')
	{
		hash_table = IniTH(nr_buckets, hash_f, int_cmp, show_int_key, free_key);
		dim = sizeof(int);
	}

	if(hash_table == NULL) return -1;

	fscanf(input, "%zu\n", &nr_operatii);

	for(i = 0; i < nr_operatii; i++)
	{ 
		load_factor = (float)k / nr_buckets;

		//redimensionare Hash table in cazul in care load factor-ul
		//este egal sau depaseste limita
		if(load_factor >= LOAD_FACTOR_THRESHOLD)
		{
			old_nr_buckets = nr_buckets;
			nr_buckets = 2 * nr_buckets;

			//ResTH intoarce 0 in cazul in care memoria este insuficienta
			//pentru redimensionare, 1, in cazul in care redimensionarea reuseste
			check = ResTH(hash_table, nr_buckets, old_nr_buckets, dim);
			if(check == 0)
				return -1;
		}

		fscanf(input, "%s", operatie);

		if(!strcmp(operatie, "insert"))
		{
			//alocare de memorie pentru cheie
			if(tip_cheie == 's')
			{
				key = calloc(51, sizeof(char));
				fscanf(input, "%s", (char*)key);
			}
			if(tip_cheie == 'd')
			{	
				key = calloc(1, sizeof(int));
				fscanf(input, "%d", (int*)key);
			}

			fscanf(input, "%s", struct_type);

			if(!strcmp(struct_type, "mat"))
			{
				char materie[50];
				size_t len_materie;

				//mai intai citirea materiei se face intr-o variabila alocata static,
				//apoi se determina lungimea cuvantului, se aloca dinamic memorie pentru
				//struct_materie->materie, apoi se copiaza informatia in structura. Astfel,
				//nu se va aloca memorie inutil
				fscanf(input, "%s", materie);

				len_materie = strlen(materie);

				struct_materie->materie = (char*)malloc((len_materie + 1) * sizeof(char));
				if(!struct_materie->materie)
				{
					free(struct_materie);
					return 1;
				}

				memcpy(struct_materie->materie, materie, len_materie + 1);

				fscanf(input, "%d", &struct_materie->ore_curs);
				fscanf(input, "%d", &struct_materie->ore_lab);
				fscanf(input, "%d", &struct_materie->credit);
				fscanf(input, "%d", &struct_materie->teme);

				//inserarea elementului cu informatia din TMaterie
				//InsTH intoarce 0 daca nu s-a introdus elementul, 1 daca elementul 
				//a inlocuit intrarea veche, 2 daca elementul a fost inserat
				//la sfarsitul listei 
				check = InsTH(hash_table, struct_materie, sizeof(TMaterie), key,
				 			dim,  nr_buckets, show_mat_value, free_mat_value);
				
				if(check == 0)
				{
					free_hash(&hash_table, nr_buckets);
					return -1;
				}
				if(check == 2)
					++k;
			}
			if(!strcmp(struct_type, "stud"))
			{
				char nume[50];
				size_t len_nume;

				//citirea numelui e similara cu cea a materiei din structura TMaterie
				fscanf(input, "%s", nume);

				len_nume = strlen(nume);

				struct_student->nume = (char*)malloc((len_nume  + 1) * sizeof(char));

				memcpy(struct_student->nume, nume, len_nume + 1);

				fscanf(input, "%f", &struct_student->medie);
				fscanf(input, "%s", struct_student->grupa);
				fscanf(input, "%d", &struct_student->varsta);

				check = InsTH(hash_table, struct_student, sizeof(TStudent), key, dim, nr_buckets, show_stud_value, free_stud_value);
				if(check == 0)
					return -1;
				if(check == 2)
					++k;
			}
			//eliberarea memoriei alocate pentru cheie
			free(key);
		}
		else if(!strcmp(operatie, "print"))
		{
			//afisare Hash table
			Print_Hash(hash_table, nr_buckets, output);
		}
		else if(!strcmp(operatie, "find"))
		{
			if(tip_cheie == 's')
			{
				key_to_find = calloc(51, sizeof(char));
				fscanf(input, "%s", (char*)key_to_find);
			}
			if(tip_cheie == 'd')
			{	
				key_to_find = calloc(1, sizeof(int));
				fscanf(input, "%d", (int*)key_to_find);
			}

			//afisarea valorii corsepunzatoare cheii key_to_find
			find_value_by_key(hash_table, key_to_find, dim, nr_buckets, output);

			//eliberarea memoriei alocate pentru cheie
			free(key_to_find);
		}
		else if(!strcmp(operatie, "delete"))
		{
			if(tip_cheie == 's')
			{
				del_key = calloc(51, sizeof(char));
				fscanf(input, "%s", (char*)del_key);
			}
			if(tip_cheie == 'd')
			{	
				del_key = calloc(1, sizeof(int));
				fscanf(input, "%d", (int*)del_key);
			}

			//stergerea elemntului a carui cheie este del_key
			//del_by_key intoarce 0 daca nu s-a gasit elementul cu cheia
			//del_key, 1 daca elementul a fost gasit si sters
			check = del_by_key(hash_table, del_key, dim, nr_buckets);
			if(check)
				--k;

			//eliberarea memoriei alocate pentru cheie
			free(del_key);
		}

	}

	//inchidere fisiere
	fclose(input);
	fclose(output);

	//eliberarea din memorie a structurilor
	//(eliberarea memoriei alocate prin struct_materie->materie si
	//struct_student->nume se face atunci cand se elibereaza 
	//elementele, prin functiile free_mat_value si free_stud_value)
	free(struct_materie);
	free(struct_student);
	
	//eliberarea Hash tabel-ului din memorie
	free_hash(&hash_table, nr_buckets);

	return 0;
}