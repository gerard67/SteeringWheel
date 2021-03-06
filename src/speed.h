/*
 * speed.h
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */


#ifndef SPEED_CLASS_H_
#define SPEED_CLASS_H_

#include <glib-object.h>


/* /////////////////////////////////////////////////////////////////////////////////////////////////
// GTYPE BOILERPLATE
// ////////////////////////////////////////////////////////////////////////////////////////////////*/
#define SPEED(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_SPEED, Speed))
#define SPEED_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_SPEED, SpeedClass))
#define IS_SPEED(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_SPEED))
#define IS_SPEED_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_SPEED))
#define SPEED_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_SPEED, SpeedClass))
#define TYPE_SPEED (speed_get_type())

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS CONSTANTS DEFINITIONS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define MAX_SPEED 25

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE PROTOTYPE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _SpeedPrivate SpeedPrivate;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// INSTANCE STRUCTURE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _Speed
{
	GObject parent_instance;
	/*< public >*/

	/*< private >*/
	/* may be a pointer to a private structure. _GET_PRIVATE macro in .c is prefered !*/

} Speed;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS STRUCTURE
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct _SpeedClass
{
	GObjectClass parent_class;

	/* class members */

	/* virtual public methods */

} SpeedClass;

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS PROTOTYPES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint speed_get (Speed* sp);
void speed_set (Speed* sp, gint value);

#endif /* SPEED_CLASS_H_ */
