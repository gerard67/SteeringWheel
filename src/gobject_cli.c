/*
 ============================================================================
 Name        : gobject_tutorial.c
 Author      : Mathieu Petit
 Version     :
 Copyright   : Your copyright notice
 Description : Check gobjects ..
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "enginespeed.h"

int main(int argc,char *argv[]) {

	g_type_init();

	Enginespeed *h1=NULL;
	h1=g_object_new(TYPE_ENGINESPEED,NULL);

	enginespeed_set(h1,10);
	g_print("%d",enginespeed_get(h1));
	g_object_unref (h1);

	return EXIT_SUCCESS;
}
