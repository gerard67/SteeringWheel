/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "vehiclespeed.h"

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define VEHICLESPEED_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_VEHICLESPEED,VehiclespeedPrivate))
struct _VehiclespeedPrivate
{
	gint value;
};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// SIGNAL INDICES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
enum {
  CHANGED,
  LAST_SIGNAL
};
static guint vehiclespeed_signals[LAST_SIGNAL] = {0};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void vehiclespeed_finalize (GObject* obj);
static void vehiclespeed_dispose (GObject* obj);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Vehiclespeed, vehiclespeed, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void vehiclespeed_class_init (VehiclespeedClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(VehiclespeedPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */

	/* Hook finalization functions */
	g_object_class->dispose = vehiclespeed_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = vehiclespeed_finalize; /* class finalization, reverse of class init */

	/*registering signals*/
	vehiclespeed_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_VEHICLESPEED,
					G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
					NULL /* closure */,
					NULL /* accumulator */,
					NULL /* accumulator data */,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE /* return_type */,
					0     /* n_params */,
					NULL  /* param_types */
			);
}

static void vehiclespeed_init (Vehiclespeed *self)
{
	/* Retrieve the private data structure */
	VehiclespeedPrivate *priv=VEHICLESPEED_GET_PRIVATE(self);

	/*
	 * Initialize all public and private members to reasonable default values.
	 */
	/* Initialize public fields */

	/* Initialize private fields */
	priv->value = 0;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void vehiclespeed_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (vehiclespeed_parent_class)->finalize (obj);
}

static void vehiclespeed_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Vehiclespeed *self = VEHICLESPEED (obj);
	VehiclespeedPrivate *priv = VEHICLESPEED_GET_PRIVATE (self);
	G_OBJECT_CLASS (vehiclespeed_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint vehiclespeed_get (Vehiclespeed* sp)
{
   g_return_val_if_fail (IS_VEHICLESPEED (sp), NULL);
   VehiclespeedPrivate *priv = VEHICLESPEED_GET_PRIVATE (sp);
   return priv->value;
}

void vehiclespeed_set (Vehiclespeed* sp, gint value){
	g_return_if_fail (IS_VEHICLESPEED (sp));
	VehiclespeedPrivate *priv = VEHICLESPEED_GET_PRIVATE (sp);
	value=value>MAX_VEHICLESPEED?MAX_VEHICLESPEED:value;
	value=value<0?0:value;
	if(priv->value!=value){
		priv->value=value;
		g_signal_emit(sp,vehiclespeed_signals[CHANGED],0);
	}
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/


/* provide accessors to the real implementations */

