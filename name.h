/* =========================================================================

   Deklaration der Datenstrukturen und Funktionen
   zur Verwaltung von Namen

   ========================================================================= */

#ifndef NAME_H
#define NAME_H


extern char **name_globals_array;

/* --------------------------   Datatypes   -------------------------------- */

#define NAME_HASH_MIN 1024
#define NAME_GLOBALS_DELTA 1024


typedef struct NameListElementStruct *NameSet;

typedef struct NameListElementStruct *NameListElement;

typedef int Name;

struct NameListElementStruct
{
	int name;
	unsigned long hash;
	NameListElement next;
};

/* --------------------------   Functions   -------------------------------- */

#define name_cstring(name) (name_globals_array[name])

int name_create(char *name);



#endif
