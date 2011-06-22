/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "example-class.h"


/*
    http://library.gnome.org/devel/gobject/2.21/gobject-Type-Information.html#G-DEFINE-TYPE--CAPS

    A convenience macro for type implementations, which declares a class
    initialization function, an instance initialization function (see GTypeInfo
    for information about these) and a static variable named t_n_parent_class
    pointing to the parent class. Furthermore, it defines a *_get_type()
    function. See G_DEFINE_TYPE_EXTENDED() for an example.
*/
G_DEFINE_TYPE (Vehicule, vehicule, G_TYPE_OBJECT)

/* Define the private structure in the .c file */
#define VEHICULE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_VEHICULE,VehiculePrivate))
struct _VehiculePrivate
{
	gint vitesse;
};

/* Init functions */
static void vehicule_class_init (VehiculeClass *klass)
{
	g_return_if_fail (klass != NULL);
	g_type_class_add_private(klass,sizeof(VehiculePrivate));
	/* Setup the default handler for virtual method */
	klass->do_action_virt = vehicule_do_action_virt_default;
}

static void vehicule_init (Vehicule *self)
{
	g_return_if_fail (self != NULL);
	g_print("maman_bar_init() - init object\n");

	/*
	 * Initialize all public and private members to reasonable default values.
	 */

	/* Initialize public fields */
	self->flags=20;

	/* Initialize private fields */
	VehiculePrivate *priv;
	self->priv=priv=VEHICULE_GET_PRIVATE(self);
	self->priv->vitesse=10;

	/* If you need specific construction properties to complete initialization,
     * delay initialization completion until the property is set.
     */

}

/* Object non-virtual method */
void vehicule_set_vitesse(Vehicule *self,gint vitesse){
	g_return_if_fail(self!=NULL);
	g_return_if_fail(IS_VEHICULE(self));
	self->priv->vitesse=vitesse;
}

gint vehicule_get_vitesse(Vehicule *self){
	g_return_if_fail(self!=NULL);
	g_return_if_fail(IS_VEHICULE(self));
	return self->priv->vitesse;
}

/* Object virtual method call - performs the override */
void vehicule_do_action_virt (Vehicule *self, gchar *msg) {
     /* First test that 'self' is of the correct type */
    g_return_if_fail (IS_VEHICULE (self));
    g_print("vehicule_do_action_virt() -> ");
    VEHICULE_GET_CLASS (self)->do_action_virt(self, msg);
}

/* Object virtual method default action (can be overridden) */
void vehicule_do_action_virt_default(Vehicule *self, gchar *msg){
	 g_print("vehicule_do_action_virt_default() - %s\n", msg );
}

