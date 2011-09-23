/*
 * example_class.c
 *
 *  Created on: Jun 20, 2011
 *      Author: mathieu
 */

#include "locationdata.h"
#include <stdlib.h>

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE STRUCTURE + ACCESS MACRO
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
#define LOCATIONDATA_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj),TYPE_LOCATIONDATA,LocationdataPrivate))
struct _LocationdataPrivate
{
	MYSQL *conn;
	gdouble replayspeed;
	gdouble longitude;
	gdouble latitude;
};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// SIGNAL INDICES
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
enum {
  CHANGED,
  LAST_SIGNAL
};
static guint locationdata_signals[LAST_SIGNAL] = {0};

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES */
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void locationdata_finalize (GObject* obj);
static void locationdata_dispose (GObject* obj);
static MYSQL * connect_to_database(char *host,char *dbname, char *password);

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITION
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
G_DEFINE_TYPE (Locationdata, locationdata, G_TYPE_OBJECT)

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
locationdata_class_init (LocationdataClass *klass)
{
	GObjectClass	*g_object_class;
	/*add a private structure*/
	g_type_class_add_private(klass,sizeof(LocationdataPrivate));

	/*get the parent class */
	g_object_class = G_OBJECT_CLASS(klass);

	/* Hook overridable methods  (Setup the default handler for virtual method) */

	/* Hook finalization functions */
	g_object_class->dispose = locationdata_dispose; /* instance destructor, reverse of instance init */
	g_object_class->finalize = locationdata_finalize; /* class finalization, reverse of class init */

	/*registering signals*/
	locationdata_signals[CHANGED]=
			g_signal_new(
					"changed",
					TYPE_LOCATIONDATA,
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
locationdata_init (Locationdata *self)
{
	/* Retrieve the private data structure */
	LocationdataPrivate *priv=LOCATIONDATA_GET_PRIVATE(self);

	/*
	 * Initialize all public and private members to reasonable default values.
	 */

	/* Initialize public fields */

	/* Initialize private fields */
	priv->conn=connect_to_database("localhost","mathieu","");
	priv->replayspeed=1.;
	priv->longitude=0.;
	priv->latitude=0.;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// DESTRUCTORS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
static void
locationdata_finalize (GObject* obj)
{
	/* Reverse what was allocated by class init */
	G_OBJECT_CLASS (locationdata_parent_class)->finalize (obj);
}

static void
locationdata_dispose (GObject* obj)
{
	/* Reverse what was allocated by instance init */
	Locationdata *self = LOCATIONDATA (obj);
	LocationdataPrivate *priv = LOCATIONDATA_GET_PRIVATE (self);
	mysql_close(priv->conn);
	G_OBJECT_CLASS (locationdata_parent_class)->dispose(obj);
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/
void
locationdata_startreplay (Locationdata* ld)
{
	g_return_if_fail (IS_LOCATIONDATA (ld));
	LocationdataPrivate *priv = LOCATIONDATA_GET_PRIVATE (ld);
	MYSQL_RES *result;
	MYSQL_ROW row;

	struct tm current;
	/*actual process time*/
	time_t starttime=NULL, currenttime=NULL;
	/*replay times*/
	time_t startreplay=NULL, currentreplay=NULL;

	int i;

	mysql_query(priv->conn,"SELECT distinct(appdate),vehicle_id,speed,altitude,latitude,longitude FROM `mathieu`.`history` WHERE vehicle_id='52' AND appdate<'2011-05-28';");
	result = mysql_store_result(priv->conn);

	while((row=mysql_fetch_row(result))){
		if(starttime==NULL){
			strptime(row[0],"%Y-%m-%d %H:%M:%S",&current);
			startreplay=mktime(&current);
			starttime=time(NULL);
		}
		strptime(row[0],"%Y-%m-%d %H:%M:%S",&current);
		currentreplay=mktime(&current);
		while((currenttime-starttime)<(currentreplay-startreplay)/priv->replayspeed){
			currenttime=time(NULL);
			sleep(1);
		}
		//update my coordinates
		priv->latitude=atof(row[4]);
		priv->longitude=atof(row[5]);

		//time to signal changes...
		g_signal_emit(ld,locationdata_signals[CHANGED],0);
	}
	mysql_free_result(result);
}

void
locationdata_setsreplayspeed (Locationdata *ld,gdouble replayspeed)
{
	g_return_if_fail (IS_LOCATIONDATA (ld));
	LocationdataPrivate *priv = LOCATIONDATA_GET_PRIVATE (ld);
	priv->replayspeed=replayspeed;
}
gdouble
locationdata_get_current_latitude(Locationdata *ld){
	g_return_if_fail (IS_LOCATIONDATA (ld));
	LocationdataPrivate *priv = LOCATIONDATA_GET_PRIVATE (ld);
	return priv->latitude;
}
gdouble
locationdata_get_current_longitude(Locationdata *ld){
	g_return_if_fail (IS_LOCATIONDATA (ld));
	LocationdataPrivate *priv = LOCATIONDATA_GET_PRIVATE (ld);
	return priv->longitude;
}

/* /////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE / STATIC METHODS
// ///////////////////////////////////////////////////////////////////////////////////////////////*/

static MYSQL *
connect_to_database(char *host,char *dbname, char *password)
{
	MYSQL *conn;
	conn=mysql_init(NULL);
	if(conn==NULL){
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	if (mysql_real_connect(conn, host, dbname, password, NULL, 0, NULL, 0) == NULL) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	return conn;
}
/* provide accessors to the real implementations */

