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
#include "heading.h"

int main(int argc,char *argv[]) {

	g_type_init();

	Heading *h1=NULL;
	h1=g_object_new(TYPE_HEADING,NULL);

	heading_set(h1,10);
	g_print("%d",heading_get(h1));
	g_object_unref (h1);

	return EXIT_SUCCESS;
}
