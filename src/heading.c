/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "heading.h"

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define HEADING_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_HEADING,HeadingPrivate))
struct _HeadingPrivate
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
static guint heading_signals[LAST_SIGNAL] = {0};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void heading_finalize (GObject* obj);
static void heading_dispose (GObject* obj);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Heading, heading, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
heading_class_init (HeadingClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(HeadingPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */

	/* Hook finalization functions */
	g_object_class->dispose = heading_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = heading_finalize; /* class finalization, reverse of class init */

	/*registering signals*/
	heading_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_HEADING,
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
heading_init (Heading *self)
{
	/* Retrieve the private data structure */
	HeadingPrivate *priv=HEADING_GET_PRIVATE(self);

	/*
	 * Initialize all public and private members to reasonable default values.
	 */
	/* Initialize public fields */

	/* Initialize private fields */
	priv->value = 100;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
heading_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (heading_parent_class)->finalize (obj);
}

static void
heading_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Heading *self = HEADING (obj);
	HeadingPrivate *priv = HEADING_GET_PRIVATE (self);
	G_OBJECT_CLASS (heading_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint
heading_get (Heading* he)
{
   g_return_val_if_fail (IS_HEADING (he), -1);
   HeadingPrivate *priv = HEADING_GET_PRIVATE (he);
   return priv->value;
}

void
heading_set (Heading* he, gint value){
	g_return_if_fail (IS_HEADING (he));
	HeadingPrivate *priv = HEADING_GET_PRIVATE (he);
	value%=360;
	if(value<0){value=360+value;}
	int reste=value%HEADING_STEP;
	reste=reste>HEADING_STEP/2?reste-HEADING_STEP:reste;
	value-=reste;

	if(priv->value!=value){
		priv->value=value;
		g_signal_emit(he,heading_signals[CHANGED],0);
	}
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/


/* provide accessors to the real implementations */

