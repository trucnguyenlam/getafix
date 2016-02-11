/* =========================================================================

   Definition der Datenstrukturen und Funktionen
   zur Verwaltung von Namen

   ====================================================================== */

#include <stdio.h>

#include "common.h"
#include "name.h"
#include "variables.h"

char **name_globals_array;
long name_globals_alloc;
long name_globals_used;
long name_current_size=1;

NameListElement *name_globals_hash;

/* ----------------------------------------------------------------------- */
/* ------------------------------  Name  --------------------------------- */
/* ----------------------------------------------------------------------- */

int name_create(char *new_name)
{
	int cmp = 1, ret;
	NameListElement last_entry;
	NameListElement next_entry;
	NameListElement new_entry;
	unsigned long hash = 1, temp;
	char *c;

	//	return 1;

	for (c = new_name; *c; c++) hash = (hash + *c) << 1;
	hash = hash % 200003;

	/* Einfuegeposition suchen */

	last_entry = NULL;


printf("gennaro namecrearte4-\n");
 temp=hash % name_current_size;
printf("gennaro namecrearte5-\n");
 next_entry = name_globals_hash[temp];
printf("gennaro namecrearte5\n");

	while (next_entry)
	{
		cmp = strcmp(new_name, name_globals_array[next_entry->name]);
		if (cmp >= 0) break;
		last_entry = next_entry;
		next_entry = next_entry->next;
	}
printf("gennaro namecrearte6\n");
	/* new_name bereits vorhanden => diesen zurueckliefern */
	if (!cmp) return next_entry->name;

	/* new_name noch nicht vorhanden */
	ret = name_globals_used;

	if (ret == name_globals_alloc)
	{
		/* neuen Speicher reservieren falls noetig */

		name_globals_array = common_realloc(name_globals_array,
		    (name_globals_alloc += NAME_GLOBALS_DELTA) * sizeof(char*),
		    NAME_GLOBALS_DELTA * sizeof(char*));
	}

	name_globals_array[ret] = common_strdup(new_name);
	name_globals_used++;

	/* Rehashing? */

	if (ret >= name_current_size * 3/4)
	{
		NameListElement l1 = NULL, l2 = NULL;
		long newsizemask = name_current_size * 2 - 1;
		int i;

		name_globals_hash = common_realloc(name_globals_hash,
		    name_current_size * 2 * sizeof(void*),
		    name_current_size * sizeof(void*));

		for (i = 0; i < name_current_size; i++)
		{
		    l1 = l2 = NULL;
		    last_entry = name_globals_hash[i];
		    while (last_entry)
		    {
			next_entry = last_entry->next;
			if ((last_entry->hash & newsizemask)
					== (unsigned long)i)
			{
				last_entry->next = l1;
				l1 = last_entry;
			}
			else
			{
				last_entry->next = l2;
				l2 = last_entry;
			}
			last_entry = next_entry;
		    }

		    last_entry = NULL;
		    while (l1)
		    {
			next_entry = l1->next;
			l1->next = last_entry;
			last_entry = l1;
			l1 = next_entry;
		    }

		    name_globals_hash[i] = last_entry;

		    last_entry = NULL;
		    while (l2)
		    {
			next_entry = l2->next;
			l2->next = last_entry;
			last_entry = l2;
			l2 = next_entry;
		    }

		    name_globals_hash[i + name_current_size] = last_entry;
		}

		name_current_size *= 2;

		last_entry = NULL;
		next_entry = name_globals_hash[hash % name_current_size];

		/* Position neu bestimmen */
		while (next_entry)
		{
		    cmp = strcmp(new_name,name_globals_array[next_entry->name]);
		    if (cmp >= 0) break;
		    last_entry = next_entry;
		    next_entry = next_entry->next;
		}
	}

	/* => Neuen Eintrag in Hash-Liste erzeugen */
 
	new_entry = common_malloc(sizeof(struct NameListElementStruct));
	new_entry->name = ret;
	new_entry->hash = hash;

	hash = hash % name_current_size;

	if (!last_entry)
	{
		/* ganz vorne in Liste einfuegen */
		new_entry->next = name_globals_hash[hash];
		name_globals_hash[hash] = new_entry;
	}
	else
	{
		/* sonst mittlere oder Endposition */
		new_entry->next  = next_entry;
		last_entry->next = new_entry;
	}

	return ret;
}






