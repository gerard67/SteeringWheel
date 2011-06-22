/*
 * example-class.h
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */


#ifndef EXAMPLE_CLASS_H_
#define EXAMPLE_CLASS_H_

#include <glib-object.h>

/*boilerplate code*/
#define TYPE_VEHICULE            (vehicule_get_type ())
#define VEHICULE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_VEHICULE, Vehicule))
#define VEHICULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_VEHICULE, VehiculeClass))
#define IS_VEHICULE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_VEHICULE))
#define IS_VEHICULE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_VEHICULE))
#define VEHICULE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_VEHICULE, VehiculeClass))

typedef struct _Vehicule Vehicule;
typedef struct _VehiculeClass VehiculeClass;
/*structure to host private members */
typedef struct _VehiculePrivate VehiculePrivate;

/*instance definition*/
struct _Vehicule
{
	GObject parent_instance;
	/*< public >*/
	guint32 flags;
	/*< private >*/
	VehiculePrivate *priv;
};

/*class definition*/
struct _VehiculeClass
{
	GObjectClass parent_class;

	/* class members */

	/* virtual public methods */
	void (*do_action_virt) (Vehicule *self, gchar *msg);
};

/* This method returns the GType associated with our new object type. */
GType	vehicule_get_type (void);

/*
 * Non-virtual public methods
 */
void vehicule_set_vitesse(Vehicule *self, gint vitesse /*, other params*/);
gint vehicule_get_vitesse(Vehicule *self);

/* Virtual method call declaration */
void vehicule_do_action_virt(Vehicule *self, gchar *msg /*, other params*/);
/* Virtual method default 'super' class method */
void vehicule_do_action_virt_default(Vehicule *self, gchar *msg);


#endif /* EXAMPLE_CLASS_H_ */
