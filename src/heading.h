/*
 * heading.h
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */


#ifndef HEADING_CLASS_H_
#define HEADING_CLASS_H_

#include <glib-object.h>


/* /////////////////////////////////////////////////////////////////////////////////////////////////
// GTYPE BOILERPLATE
// ////////////////////////////////////////////////////////////////////////////////////////////////*/
#define HEADING(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_HEADING, Heading))
#define HEADING_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_HEADING, HeadingClass))
#define IS_HEADING(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_HEADING))
#define IS_HEADING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_HEADING))
#define HEADING_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_HEADING, HeadingClass))
#define TYPE_HEADING (heading_get_type())


/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CONSTANTS DEFINITIONS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define HEADING_STEP 10

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE PROTOTYPE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _HeadingPrivate HeadingPrivate;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// INSTANCE STRUCTURE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _Heading
{
	GObject parent_instance;
	/*< public >*/

	/*< private >*/
	/* may be a pointer to a private structure. _GET_PRIVATE macro in .c is prefered !*/

} Heading;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS STRUCTURE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _HeadingClass
{
	GObjectClass parent_class;

	/* class members */

	/* virtual public methods */

} HeadingClass;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS PROTOTYPES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint
heading_get (Heading* he);
void
heading_set (Heading* he, gint value);

#endif /* HEADING_CLASS_H_ */
