/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "speed.h"

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define SPEED_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_SPEED,SpeedPrivate))
struct _SpeedPrivate
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
static guint speed_signals[LAST_SIGNAL] = {0};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void speed_finalize (GObject* obj);
static void speed_dispose (GObject* obj);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Speed, speed, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void speed_class_init (SpeedClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(SpeedPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */

	/* Hook finalization functions */
	g_object_class->dispose = speed_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = speed_finalize; /* class finalization, reverse of class init */

	/*registering signals*/
	speed_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_SPEED,
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

static void speed_init (Speed *self)
{
	/* Retrieve the private data structure */
	SpeedPrivate *priv=SPEED_GET_PRIVATE(self);

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
static void speed_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (speed_parent_class)->finalize (obj);
}

static void speed_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Speed *self = SPEED (obj);
	SpeedPrivate *priv = SPEED_GET_PRIVATE (self);
	G_OBJECT_CLASS (speed_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gint speed_get (Speed* sp)
{
   g_return_val_if_fail (IS_SPEED (sp), NULL);
   SpeedPrivate *priv = SPEED_GET_PRIVATE (sp);
   return priv->value;
}

void speed_set (Speed* sp, gint value){
	g_return_if_fail (IS_SPEED (sp));
	SpeedPrivate *priv = SPEED_GET_PRIVATE (sp);
	value=value>MAX_SPEED?MAX_SPEED:value;
	value=value<0?0:value;
	if(priv->value!=value){
		priv->value=value;
		g_signal_emit(sp,speed_signals[CHANGED],0);
	}
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/


/* provide accessors to the real implementations */

