/*
 ============================================================================
 Name        : gobject_tutorial.c
 Author      : Mathieu Petit
 Version     :
 Copyright   : Your copyright notice
 Description : Check gobjects ..
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <cairo/cairo.h>
#include <math.h>
#include "heading.h"
#include "speed.h"


typedef struct _Compas_data
{
	Heading *h;
	Speed *s;
}Compas_data;


static gboolean MOUSE_PRESSED=FALSE;

void
on_heading_plus(GtkWidget *widget, gpointer user_data){
	if(!gtk_widget_has_focus(widget)){
			gtk_widget_grab_focus(widget);
	}
	heading_set(HEADING(user_data),heading_get(HEADING(user_data))+1);
}

void
on_heading_moins(GtkWidget *widget, gpointer user_data){
	if(!gtk_widget_has_focus(widget)){
			gtk_widget_grab_focus(widget);
	}
	heading_set(HEADING(user_data),heading_get(HEADING(user_data))-1);
}

void
on_speed_plus(GtkWidget *widget, gpointer user_data){
	if(!gtk_widget_has_focus(widget)){
			gtk_widget_grab_focus(widget);
	}
	speed_set(SPEED(user_data),speed_get(SPEED(user_data))+1);
}

void
on_speed_moins(GtkWidget *widget, gpointer user_data){
	if(!gtk_widget_has_focus(widget)){
			gtk_widget_grab_focus(widget);
	}
	speed_set(SPEED(user_data),speed_get(SPEED(user_data))-1);
}

void
on_heading_changed(Heading *obj,GtkLabel *label){
	gchar buf[3];
	g_snprintf(buf,4,"%d",heading_get(obj));
	gtk_label_set_text(label, buf);
}

void
on_speed_changed(Speed *obj,GtkLabel *label){
	gchar buf[3];
	g_snprintf(buf,4,"%d",speed_get(obj));
	gtk_label_set_text(label, buf);
}

void
on_compass_draw_changed(GObject *obj,GtkDrawingArea *drawing){
	gtk_widget_queue_draw(GTK_WIDGET(drawing));
}


gboolean
on_compass_motion(GtkWidget *widget,GdkEventMotion *event,Compas_data *user_data){
	if(MOUSE_PRESSED){
		return set_all_from_compass(widget,(guint)event->x,(guint)event->y,user_data);
	}
	return FALSE;
}

gboolean
on_compass_pressed(GtkWidget *widget,GdkEventButton *event,Compas_data *user_data){
	MOUSE_PRESSED=TRUE;
	return set_all_from_compass(widget,(guint)event->x,(guint)event->y,user_data);
}
gboolean
on_compass_released(GtkWidget *widget,GdkEventButton *event,Compas_data *user_data){
	MOUSE_PRESSED=FALSE;
	return FALSE;
}

gboolean
set_all_from_compass(GtkWidget *widget,guint cx,guint cy,Compas_data *user_data){
	guint width, height;
	gdouble x,y,teta,speed;
	Heading *h1=HEADING(user_data->h);
	Speed *s1=SPEED(user_data->s);
	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);
	x=(int)cx-MIN(width,height)/2.;
	y=(int)cy-MIN(width,height)/2.;
	teta=180/G_PI*acos(x/sqrt(pow(x,2)+pow(y,2)));
	speed=sqrt(pow(x,2)+pow(y,2))*MAX_SPEED/(MIN(width,height)/2.);;

	if(asin(y/sqrt(pow(x,2)+pow(y,2))<0))teta=360-teta;
	heading_set(h1,teta);
	speed_set(s1,speed);
	return FALSE;
}

gboolean
compass_draw(GtkWidget *widget, cairo_t *cr, Compas_data *user_data)
{
	const gint RADIUS=10;
	guint width, height;

	Heading *h1=HEADING(user_data->h);
	Speed *s1=SPEED(user_data->s);

	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);

	cairo_set_source_rgb(cr, 0.69, 0.19, 0);
	cairo_arc (cr,
			(double)(width/2.+((MIN(width,height)/2.)/MAX_SPEED*speed_get(s1))*cos(G_PI/180*heading_get(h1))),
			(double)(height/2.+((MIN(width,height)/2.)/MAX_SPEED*speed_get(s1))*sin(G_PI/180*heading_get(h1))),
			RADIUS,
			0, 2*G_PI);
	cairo_stroke_preserve(cr);
	cairo_set_source_rgb(cr, 0.3, 0.4, 0.6);
	cairo_fill (cr);
	return FALSE;
}

int
main(int argc,char *argv[]) {


	g_type_init();
	Heading *h1=NULL;
	Speed *s1=NULL;
	s1=g_object_new(TYPE_SPEED,NULL);
	h1=g_object_new(TYPE_HEADING,NULL);
	heading_set(h1,1);
	speed_set(s1,1);

	/*GTK construct ...*/
	GtkBuilder *builder;
	GtkWindow *window_heading,*window_compass,*window_speed;
	GtkLabel *label_speed,*label_heading;
	GtkButton *button[4];
	GtkDrawingArea *drawingarea_compass;
	GError *error=NULL;

	gtk_init(&argc, &argv);

	builder=gtk_builder_new();
	if(!gtk_builder_add_from_file(builder,DATADIR "/heading.glade",&error)){
		g_warning("%s",error->message);
		g_free(error);
		return 1;
	}

	/*retrieve the useful widgets from the builder*/
	window_heading = GTK_WINDOW(gtk_builder_get_object(builder,"window_heading"));
	window_speed = GTK_WINDOW(gtk_builder_get_object(builder,"window_speed"));
	window_compass = GTK_WINDOW(gtk_builder_get_object(builder,"window_compass"));
	label_heading = GTK_LABEL(gtk_builder_get_object(builder,"label_heading"));
	label_speed = GTK_LABEL(gtk_builder_get_object(builder,"label_speed"));
	drawingarea_compass = GTK_DRAWING_AREA(gtk_builder_get_object(builder,"drawingarea_compass"));
	button[0]=GTK_BUTTON(gtk_builder_get_object(builder,"button_heading_plus"));
	button[1]=GTK_BUTTON(gtk_builder_get_object(builder,"button_heading_moins"));
	button[2]=GTK_BUTTON(gtk_builder_get_object(builder,"button_speed_plus"));
	button[3]=GTK_BUTTON(gtk_builder_get_object(builder,"button_speed_moins"));

	/* connecting signals */
	/*control signals*/
	g_signal_connect(window_compass,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(window_heading,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(window_speed,"destroy",G_CALLBACK(gtk_main_quit),NULL);

	Compas_data compas_data;
	compas_data.h=h1;
	compas_data.s=s1;
	g_signal_connect(drawingarea_compass,"draw",G_CALLBACK(compass_draw),&compas_data);

	/*signals from the views to the model*/
	g_signal_connect(button[0],"clicked",G_CALLBACK(on_heading_plus),h1);
	g_signal_connect(button[1],"clicked",G_CALLBACK(on_heading_moins),h1);
	g_signal_connect(button[2],"clicked",G_CALLBACK(on_speed_plus),s1);
	g_signal_connect(button[3],"clicked",G_CALLBACK(on_speed_moins),s1);
	g_signal_connect(drawingarea_compass,"button-press-event",G_CALLBACK(on_compass_pressed),&compas_data);
	g_signal_connect(drawingarea_compass,"button-release-event",G_CALLBACK(on_compass_released),&compas_data);
	g_signal_connect(drawingarea_compass,"motion-notify-event",G_CALLBACK(on_compass_motion),&compas_data);

	/*signals from the model to the views*/
	g_signal_connect(h1,"changed",G_CALLBACK(on_heading_changed),label_heading);
	g_signal_connect(s1,"changed",G_CALLBACK(on_speed_changed),label_speed);
	g_signal_connect(h1,"changed",G_CALLBACK(on_compass_draw_changed),drawingarea_compass);
	g_signal_connect(s1,"changed",G_CALLBACK(on_compass_draw_changed),drawingarea_compass);

	/*display the views*/
	gtk_widget_show(GTK_WIDGET(window_compass));
	gtk_widget_show(GTK_WIDGET(window_heading));
	gtk_widget_show(GTK_WIDGET(window_speed));

	/*event loop*/
	gtk_main();

	/*cleaning up*/
	g_object_unref(h1);
	g_object_unref(s1);
	g_object_unref(G_OBJECT(builder));

	return EXIT_SUCCESS;
}
