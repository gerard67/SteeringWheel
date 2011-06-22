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
	GQueue*		queue;
	guint		index;
	gboolean 	active;
};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void heading_finalize (GObject* obj);
static void heading_dispose (GObject* obj);
static void real_heading_write (Heading* self, gchar* text);
static gint real_heading_get_char_count (Heading* self);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Heading, heading, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void heading_class_init (HeadingClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(HeadingPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */
	klass->write = real_heading_write;
	klass->get_char_count = real_heading_get_char_count;

	/* Hook finalization functions */
	g_object_class->dispose = heading_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = heading_finalize; /* class finalization, reverse of class init */
}

static void heading_init (Heading *self)
{
	/* Retrieve the private data structure */
	HeadingPrivate *priv;
	priv=HEADING_GET_PRIVATE(self);

	/*
	 * Initialize all public and private members to reasonable default values.
	 */
	/* Initialize public fields */
	self->value = 0;
	/* Initialize private fields */
	priv->queue = g_queue_new();
	priv->index = 0;
	priv->active = TRUE;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void heading_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (heading_parent_class)->finalize (obj);
}

static void heading_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */

	Heading *self = HEADING (obj);
	HeadingPrivate *priv;

	priv = HEADING_GET_PRIVATE (self);

	g_queue_foreach (priv->queue, (GFunc) g_object_unref, NULL);
	g_queue_free (priv->queue);

	G_OBJECT_CLASS (heading_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
gchar* heading_get_last_message (Heading* he)
{
   g_return_val_if_fail (IS_HEADING (he), NULL);
   HeadingPrivate * priv = HEADING_GET_PRIVATE (he);
   return g_queue_peek_tail(priv->queue);
}

void heading_add_message (Heading* he, gchar* message){
	g_return_if_fail (IS_HEADING (he));
	HeadingPrivate * priv = HEADING_GET_PRIVATE (he);
	g_queue_push_tail (priv->queue,message);
	he->value += strlen(g_queue_peek_tail(priv->queue));
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/

static void real_heading_write (Heading* self, gchar* text)
{
	g_print("real write\n");
}

static gint real_heading_get_char_count (Heading* self)
{
	return self->value;
}

/* provide accessors to the real implementations */
void heading_write (Heading* self, gchar* text) {return HEADING_GET_CLASS (self)->write (self, text);}
gint heading_get_char_count (Heading* self){return HEADING_GET_CLASS (self)->get_char_count (self);}

