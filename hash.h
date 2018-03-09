/*NICOLESCU Daniel-Marian-314CB*/

#ifndef _HASH_H_
#define _HASH_H_
 
//structura hash     
typedef struct
{
	TFHash fh;
	TLG *v;
	KeyCmp cmp;
	ShowKey show_key;
	FreeKey free_key;

} TH;


//prototipurile functiilor:

//functia hash
unsigned int hash_f(const void*, size_t, size_t);
void free_key(void*);
int int_cmp(void*, void*);
int string_cmp(void*, void*);
void show_int_key(void*, FILE*);
void show_string_key(void*, FILE*);
void show_mat_value(void*, FILE*);
void show_stud_value(void*, FILE*);
void free_mat_value(void*);
void free_stud_value(void*);
void Print_Hash(TH*, size_t, FILE*);
//initiere Hash table
TH* IniTH(size_t, TFHash, KeyCmp, ShowKey, FreeKey);
//inserare element in Hash table
int InsTH(TH*, void*, size_t, void*, size_t, size_t, ShowValue, FreeValue);
//inserare element in lista generica determinata de InsTH
int InsLG(ALG, KeyCmp, void*, size_t, void*, size_t, ShowValue, FreeValue, FreeKey);
//redimensionare Hash table
int ResTH(TH*, size_t, size_t, size_t);
void find_value_by_key(TH*, void*, size_t, size_t, FILE*);
int del_by_key(TH*, void*, size_t, size_t);
void free_hash(TH**, size_t);


#endif