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
#include "example-class.h"
#include "heading.h"

/*constructor overload -  sort of*/
Heading* heading_new (gchar* initital_message){
		Heading* h;
		h=g_object_new(TYPE_HEADING,NULL);
		heading_add_message(h,initital_message);
		return h;
}

int main(void) {

	g_type_init();

	Heading *h1=NULL;
	h1=g_object_new(TYPE_HEADING,NULL);

	/*option 2 : with constructor overload*/
	Heading *h2=NULL;
	h2=heading_new("mon premier message");
	heading_add_message(h2,"second message");

	g_print("last message : %s\n", heading_get_last_message(h2));
	g_print("lentgh h2 = %d\n", heading_get_char_count(h2));
	g_print("lentgh h1 = %d\n", heading_get_char_count(h1));

	/* Calling a super-method (if heading extends tutorial) */
	/* tutorial_somemethod (TUTORIAL (h2), "some parameter"); */
	g_object_notify(G_OBJECT (h2),"toto");

	/* Access a super-member (if heading extends tutorial) */
	/* TUTORIAL (h)->somepublicfield = 1; */

	g_object_unref (h1);
	g_object_unref (h2);

	return EXIT_SUCCESS;
}
