/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "enginespeed.h"

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define ENGINESPEED_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_ENGINESPEED,EnginespeedPrivate))
struct _EnginespeedPrivate
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
static guint enginespeed_signals[LAST_SIGNAL] = {0};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void enginespeed_finalize (GObject* obj);
static void enginespeed_dispose (GObject* obj);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Enginespeed, enginespeed, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
enginespeed_class_init (EnginespeedClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(EnginespeedPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */

	/* Hook finalization functions */
	g_object_class->dispose = enginespeed_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = enginespeed_finalize; /* class finalization, reverse of class init */

	/*registering signals*/
	enginespeed_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_ENGINESPEED,
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

static void
enginespeed_init (Enginespeed *self)
{
	/* Retrieve the private data structure */
	EnginespeedPrivate *priv=ENGINESPEED_GET_PRIVATE(self);

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
static void
enginespeed_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (enginespeed_parent_class)->finalize (obj);
}

static void
enginespeed_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Enginespeed *self = ENGINESPEED (obj);
	EnginespeedPrivate *priv = ENGINESPEED_GET_PRIVATE (self);
	G_OBJECT_CLASS (enginespeed_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint
enginespeed_get (Enginespeed* he)
{
   g_return_val_if_fail (IS_ENGINESPEED (he), -1);
   EnginespeedPrivate *priv = ENGINESPEED_GET_PRIVATE (he);
   return priv->value;
}

void
enginespeed_set (Enginespeed* he, gint value)
{
	g_return_if_fail (IS_ENGINESPEED (he));
	EnginespeedPrivate *priv = ENGINESPEED_GET_PRIVATE (he);
	value=value>MAX_ENGINESPEED?MAX_ENGINESPEED:value;
	value=value<0?0:value;
	if(priv->value!=value){
		priv->value=value;
		g_signal_emit(he,enginespeed_signals[CHANGED],0);
	}
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/


/* provide accessors to the real implementations */

