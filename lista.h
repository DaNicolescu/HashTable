/*NICOLESCU Daniel-Marian-314CB*/

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

//typedef-uri pentru:

//functia hash
typedef unsigned int (*TFHash)(const void *, size_t, size_t);
typedef int (*KeyCmp)(void*, void*);
typedef void (*ShowKey)(void*, FILE*);
typedef void (*FreeKey)(void*);
typedef void (*ShowValue)(void *, FILE*);
typedef void (*FreeValue)(void *);

//structura celula
typedef struct celulag
{
  void *info;
  struct celulag *urm;
} TCelulaG, *TLG, **ALG;

//structura info
typedef struct
{
  ShowValue show_value;
  FreeValue free_value;
  void *value;
  void *key;
} TInfo;

#endif
