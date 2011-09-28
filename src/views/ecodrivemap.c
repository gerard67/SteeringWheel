/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include <clutter/clutter.h>
#include <champlain/champlain.h>
#include "ecodrivemap.h"

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define ECODRIVEMAP_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_ECODRIVEMAP,EcodrivemapPrivate))
struct _EcodrivemapPrivate
{
	ClutterStage *stage;
	ChamplainPathLayer *ecodrivemap_path_layer;
};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// SIGNAL INDICES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
enum {
	CHANGED,
	LAST_SIGNAL
};
static guint ecodrivemap_signals[LAST_SIGNAL] = {0};


/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PROPERTIES INDICES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
enum {
	PROP_0,
	PROP_ECODRIVEMAP_STAGE
};


/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/

/* Init/Construct/Destroy handlers */
static GObject *ecodrivemap_constructor (GType gtype, guint n_properties, GObjectConstructParam *properties);
static void ecodrivemap_finalize (GObject* obj);
static void ecodrivemap_dispose (GObject* obj);
/*Properties handler*/
static void ecodrivemap_set_property(GObject *object,guint property_id, const GValue *value,GParamSpec *pspec);
static void ecodrivemap_get_property(GObject *object,guint property_id, const GValue *value,GParamSpec *pspec);
/*Signals handlers*/
static gboolean on_zoom_out_released(G_GNUC_UNUSED ClutterActor *actor,G_GNUC_UNUSED ClutterButtonEvent *event, ChamplainView *view);
static gboolean on_zoom_in_released(G_GNUC_UNUSED ClutterActor *actor,G_GNUC_UNUSED ClutterButtonEvent *event, ChamplainView *view);
static void on_mapstage_allocation_changed(ClutterActor *actor, const ClutterActorBox  *allocation, ClutterAllocationFlags  flags, gpointer mapactor);
/*other private methods*/
static ClutterActor *make_button (char *text);



/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Ecodrivemap, ecodrivemap, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
ecodrivemap_class_init (EcodrivemapClass *klass)
{
	GObjectClass *g_object_class;
	GParamSpec *pspec;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(EcodrivemapPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook constructors */
	g_object_class->constructor=ecodrivemap_constructor;

	/* Hook getter/setters methods  (Setup the default handler for virtual method) */
	g_object_class->set_property=ecodrivemap_set_property;
	g_object_class->get_property=ecodrivemap_get_property;

	/* Hook finalization functions */
	g_object_class->dispose = ecodrivemap_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = ecodrivemap_finalize; /* class finalization, reverse of class init */

	/* registering parameters */
	pspec=g_param_spec_object("MapStage",
				"Ecodrive map stage",
				"Set ecodrivemap clutter stage",
				CLUTTER_TYPE_STAGE,
				G_PARAM_CONSTRUCT_ONLY|G_PARAM_READWRITE);
	g_object_class_install_property(g_object_class,PROP_ECODRIVEMAP_STAGE,pspec);

	/*registering signals*/
	ecodrivemap_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_ECODRIVEMAP,
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
ecodrivemap_init (Ecodrivemap *self)
{
	g_return_if_fail (IS_ECODRIVEMAP (self));
	/* Retrieve the private data structure */
	EcodrivemapPrivate *priv=ECODRIVEMAP_GET_PRIVATE(self);

	/*
	 * Initialize all public and private members to reasonable default values.
	 */

	/* Initialize public fields */

	/* Initialize private fields */
	priv->stage=NULL;
	priv->ecodrivemap_path_layer=champlain_path_layer_new();

}

static GObject *
ecodrivemap_constructor (GType gtype, guint n_properties, GObjectConstructParam *properties)
{
	GObject *obj;
	/* Chain up to the parent constructor */
	{	obj = G_OBJECT_CLASS (ecodrivemap_parent_class)->constructor (gtype, n_properties, properties);	}

	/*-------------------------------------------------------------*/
	/* update the object state depending on constructor properties */
	/*-------------------------------------------------------------*/

	/*retrieve self after class init, instance init and contruction properties init*/
	Ecodrivemap *self = ECODRIVEMAP (obj);
	g_return_if_fail (IS_ECODRIVEMAP (self));
	EcodrivemapPrivate *priv=ECODRIVEMAP_GET_PRIVATE(self);

	/*initilize Constructor local variables*/
	ChamplainView *ecodrivemap_actor;
	ChamplainPathLayer *ecodrivemap_path_layer;
	ChamplainMapSourceFactory *ecodrivemap_factory;
	ClutterActor *buttongroup,*button[2];
	ClutterColor stage_color = { 0xff, 0xff, 0xff, 0xff };
	ClutterStage *stage=priv->stage;

	/*preparing the stage*/
	clutter_stage_set_color(CLUTTER_STAGE(stage),&stage_color);

	/*preparing the button group actors*/
	buttongroup = clutter_group_new ();
	button[0]=make_button("Zoom In");
	button[1]=make_button("Zoom Out");
	clutter_actor_set_position (buttongroup,10,10);
	clutter_actor_set_reactive(button[0], TRUE);
	clutter_actor_set_reactive(button[1], TRUE);
	clutter_actor_set_position(button[1],clutter_actor_get_width(button[0])+10,0);
	clutter_container_add_actor(CLUTTER_CONTAINER(buttongroup),button[0]);
	clutter_container_add_actor(CLUTTER_CONTAINER(buttongroup),button[1]);

	/*preparing the map actor*/
	ecodrivemap_actor = CHAMPLAIN_VIEW(champlain_view_new());
	ecodrivemap_path_layer=priv->ecodrivemap_path_layer;
	ecodrivemap_factory = champlain_map_source_factory_dup_default();
	champlain_view_set_map_source(ecodrivemap_actor,champlain_map_source_factory_create(ecodrivemap_factory,CHAMPLAIN_MAP_SOURCE_OSM_MAPNIK));
	champlain_path_layer_set_stroke_width(ecodrivemap_path_layer,5.0);
	champlain_view_add_layer(ecodrivemap_actor,CHAMPLAIN_LAYER(ecodrivemap_path_layer));

	/*set actors on the stage*/
	clutter_container_add_actor (CLUTTER_CONTAINER(stage), CLUTTER_ACTOR(ecodrivemap_actor));
	clutter_container_add_actor(CLUTTER_CONTAINER(stage),buttongroup);

	/*connecting self signals*/
	g_signal_connect(stage,"allocation-changed",G_CALLBACK(on_mapstage_allocation_changed),ecodrivemap_actor);
	g_signal_connect(button[1],"button-release-event", G_CALLBACK(on_zoom_out_released),ecodrivemap_actor);
	g_signal_connect(button[0],"button-release-event", G_CALLBACK(on_zoom_in_released),ecodrivemap_actor);

	return obj;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
ecodrivemap_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (ecodrivemap_parent_class)->finalize (obj);
}

static void
ecodrivemap_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Ecodrivemap *self = ECODRIVEMAP (obj);
	EcodrivemapPrivate *priv = ECODRIVEMAP_GET_PRIVATE (self);
	G_OBJECT_CLASS (ecodrivemap_parent_class)->dispose (obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
void ecodrivemap_path_add_node(Ecodrivemap *self,gdouble lat, gdouble lng){
	g_return_if_fail (IS_ECODRIVEMAP (self));
	EcodrivemapPrivate *priv = ECODRIVEMAP_GET_PRIVATE (self);
	champlain_path_layer_add_node(priv->ecodrivemap_path_layer,CHAMPLAIN_LOCATION(champlain_coordinate_new_full(lat,lng)));
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static ClutterActor *
make_button (char *text)
{
  ClutterActor *button, *button_bg, *button_text;
  ClutterColor white = { 0xff, 0xff, 0xff, 0xff };
  ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
  gfloat width, height;

  button = clutter_group_new ();

  button_bg = clutter_rectangle_new_with_color (&white);
  clutter_container_add_actor (CLUTTER_CONTAINER (button), button_bg);
  clutter_actor_set_opacity (button_bg, 0xcc);

  button_text = clutter_text_new_full ("Sans 10", text, &black);
  clutter_container_add_actor (CLUTTER_CONTAINER (button), button_text);
  clutter_actor_get_size (button_text, &width, &height);

  clutter_actor_set_size (button_bg, width + 10 * 2, height + 10 * 2);
  clutter_actor_set_position (button_bg, 0, 0);
  clutter_actor_set_position (button_text, 10, 10);

  return button;
}

/* Signals handlers*/
static gboolean
on_zoom_in_released(G_GNUC_UNUSED ClutterActor *actor,G_GNUC_UNUSED ClutterButtonEvent *event, ChamplainView *view)
{
	champlain_view_zoom_in(view);
	return TRUE;
}

static gboolean
on_zoom_out_released(G_GNUC_UNUSED ClutterActor *actor,G_GNUC_UNUSED ClutterButtonEvent *event, ChamplainView *view)
{
	champlain_view_zoom_out(view);
	return TRUE;
}

static void
on_mapstage_allocation_changed (ClutterActor *actor, const ClutterActorBox  *allocation, ClutterAllocationFlags  flags, gpointer mapactor){
	clutter_actor_set_size(CLUTTER_ACTOR(mapactor),clutter_actor_box_get_width(allocation),clutter_actor_box_get_height(allocation));
}



/* Properties handler */
static void
ecodrivemap_set_property(GObject *object,guint property_id, const GValue *value,GParamSpec *pspec){
	Ecodrivemap *self=ECODRIVEMAP(object);
	g_return_if_fail (IS_ECODRIVEMAP (self));
	EcodrivemapPrivate *priv = ECODRIVEMAP_GET_PRIVATE (self);
	switch(property_id){
	case PROP_ECODRIVEMAP_STAGE:
		priv->stage=CLUTTER_STAGE(g_value_get_object(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void
ecodrivemap_get_property(GObject *object,guint property_id, const GValue *value,GParamSpec *pspec){
	  Ecodrivemap *self = ECODRIVEMAP(object);
	  g_return_if_fail (IS_ECODRIVEMAP (self));
	  EcodrivemapPrivate *priv = ECODRIVEMAP_GET_PRIVATE (self);
	  switch (property_id)
	    {
	    case PROP_ECODRIVEMAP_STAGE:
	      g_value_set_object(value, priv->stage);
	      break;
	    default:
	      /* We don't have any other property... */
	      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	      break;
	    }
}
