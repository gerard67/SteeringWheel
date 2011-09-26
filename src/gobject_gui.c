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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <regex.h>

#include <math.h>

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>

#include "enginespeed.h"
#include "vehiclespeed.h"

/*parameters to read serial scanbus*/
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1
volatile int STOP=FALSE;


typedef struct _Dashboard_data
{
	Enginespeed *h;
	Vehiclespeed *s;
	RsvgHandle *cursor_big;
	RsvgHandle *cursor_small;
	RsvgHandle *dashboard_back;
	RsvgHandle *dashboard_top;
}Dashboard_data;


static gboolean MOUSE_PRESSED=FALSE;

int
read_canbus(Dashboard_data *dashboard_data )
{
	Enginespeed *h1=dashboard_data->h;
	Vehiclespeed *s1=dashboard_data->s;

	int fd,res,regcom;
	regex_t reg_speed;
	size_t nmatch = 4;
	regmatch_t pmatch[4];
	struct termios oldtio,newtio;
	char buf[255];//="$FMS1,8057.66,539281,338.6,24,864,20,0,28.8,0,0,0,0,54.8,68,1.04,0,4410,0,0,12140";
	char str_esp[5]="1",str_vsp[5]="1";

	//gdk_threads_enter();

	printf("-- TESTING USB-TO-SERIAL PORT --\n");
	fd = open(MODEMDEVICE, O_RDONLY | O_NOCTTY);
	if(fd < 0 ){perror("open_port: Unable to open modem - ");}
	else{

		/*compiling regular expression*/
		if (0 != (regcom=regcomp(&reg_speed,"^\\$FMS1(,[0-9]+[.]?[0-9]*){3},([0-9]+)[.]?[0-9]*,([0-9]+)[.]?[0-9]*,.*$",REG_EXTENDED))){
			printf("regcomp() failed, returning nonzero (%d)\n", regcomp);
			exit(EXIT_FAILURE);
		};

		tcgetattr(fd,&oldtio);
		bzero(&newtio,sizeof(newtio));
		newtio.c_cflag=BAUDRATE|CRTSCTS|CS8|CLOCAL|CREAD;

		newtio.c_iflag = IGNPAR | ICRNL;

		newtio.c_oflag = 0;

		newtio.c_lflag = ICANON;

		newtio.c_cc[VINTR]    = 0;
		newtio.c_cc[VQUIT]    = 0;
		newtio.c_cc[VERASE]   = 0;
		newtio.c_cc[VKILL]    = 0;
		newtio.c_cc[VEOF]     = 4;
		newtio.c_cc[VTIME]    = 0;
		newtio.c_cc[VMIN]     = 1;
		newtio.c_cc[VSWTC]    = 0;
		newtio.c_cc[VSTART]   = 0;
		newtio.c_cc[VSTOP]    = 0;
		newtio.c_cc[VSUSP]    = 0;
		newtio.c_cc[VEOL]     = 0;
		newtio.c_cc[VREPRINT] = 0;
		newtio.c_cc[VDISCARD] = 0;
		newtio.c_cc[VWERASE]  = 0;
		newtio.c_cc[VLNEXT]   = 0;
		newtio.c_cc[VEOL2]    = 0;


		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&newtio);


		while (STOP==FALSE) {
			res = read(fd,buf,255);
			buf[res]=0;
			if(0!=(regcom=regexec(&reg_speed, buf, nmatch, pmatch, 0))){
				//printf("Frame received does not match speeds : \"%s\"",buf);
			}else{
				strncpy(str_vsp,buf+pmatch[2].rm_so,pmatch[2].rm_eo-pmatch[2].rm_so);
				strncpy(str_esp,buf+pmatch[3].rm_so,pmatch[3].rm_eo-pmatch[3].rm_so);
				str_esp[pmatch[3].rm_eo-pmatch[3].rm_so]='\0';
				str_vsp[pmatch[2].rm_eo-pmatch[2].rm_so]='\0';
				printf("%d->%d\n",pmatch[3].rm_so,pmatch[3].rm_eo);
				printf("%s",buf);
				printf("vitesse camion : \"%s\" , vitesse moteur : \"%s\"\n",str_vsp,str_esp);
				enginespeed_set(h1,atoi(str_esp));
				vehiclespeed_set(s1,atoi(str_vsp));
			}

			if (buf[0]=='z') STOP=TRUE;
		}
		tcsetattr(fd,TCSANOW,&oldtio);

		regfree(&reg_speed);
	}

	//gdk_threads_leave();
	return(1);
}

void
on_enginespeed_plus(GtkWidget *widget, Enginespeed *h){
	if(!gtk_widget_has_focus(widget)){gtk_widget_grab_focus(widget);}
	enginespeed_set(h,enginespeed_get(h)+50);
}

void
on_enginespeed_moins(GtkWidget *widget, Enginespeed *h){
	if(!gtk_widget_has_focus(widget)){gtk_widget_grab_focus(widget);}
	enginespeed_set(h,enginespeed_get(h)-50);
}

void
on_vehiclespeed_plus(GtkWidget *widget, Vehiclespeed *s){
	if(!gtk_widget_has_focus(widget)){gtk_widget_grab_focus(widget);}
	vehiclespeed_set(s,vehiclespeed_get(s)+1);
}

void
on_vehiclespeed_moins(GtkWidget *widget, Vehiclespeed *s){
	if(!gtk_widget_has_focus(widget)){gtk_widget_grab_focus(widget);}
	vehiclespeed_set(s,vehiclespeed_get(s)-1);
}

void
on_enginespeed_changed(Enginespeed *h,GtkLabel *label){
	gchar buf[4];
	g_snprintf(buf,5,"%d",enginespeed_get(h));
	gtk_label_set_text(label, buf);
}

void
on_vehiclespeed_changed(Vehiclespeed *s,GtkLabel *label){
	gchar buf[3];
	g_snprintf(buf,4,"%d",vehiclespeed_get(s));
	gtk_label_set_text(label, buf);
}

void
on_dashboard_draw_changed(GObject *obj,GtkDrawingArea *drawing){
	gtk_widget_queue_draw(GTK_WIDGET(drawing));
}

gboolean
dashboard_draw(GtkWidget *widget, cairo_t *cr, Dashboard_data *user_data)
{
	const gint RADIUS=6;
	gdouble width, height,drawing_scale;
	guint draw_height,draw_width;
	Enginespeed *h1=user_data->h;
	Vehiclespeed *s1=user_data->s;

	width=307;
	height=178;
	draw_width = gtk_widget_get_allocated_width (widget);
	draw_height = gtk_widget_get_allocated_height (widget);
	drawing_scale=MIN(draw_width/width,draw_height/height);

	cairo_scale(cr,drawing_scale,drawing_scale);

	cairo_save(cr);
	rsvg_handle_render_cairo(user_data->dashboard_back,cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_translate(cr,103.,110.);
	cairo_rotate(cr,(244/160.*vehiclespeed_get(s1)-122)*G_PI/180);
	cairo_translate(cr,-16.5/2.,-177./2.);
	rsvg_handle_render_cairo(user_data->cursor_big,cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_translate(cr,237.,73.);
	cairo_rotate(cr,(237/3000.*enginespeed_get(h1)-40)*G_PI/180);
	cairo_translate(cr,-16.5/2.,-105.5/2.);
	rsvg_handle_render_cairo(user_data->cursor_small,cr);
	cairo_restore(cr);

	cairo_save(cr);
	rsvg_handle_render_cairo(user_data->dashboard_top,cr);
	cairo_restore(cr);

	return FALSE;
}

int
main(int argc,char *argv[]) {

		GError *error=NULL;
		GThread *readcanbus;

		/*Gobject constructs*/
		g_type_init();
		Enginespeed *h1=NULL;
		Vehiclespeed *s1=NULL;
		s1=g_object_new(TYPE_VEHICLESPEED,NULL);
		h1=g_object_new(TYPE_ENGINESPEED,NULL);
		enginespeed_set(h1,1);
		vehiclespeed_set(s1,1);

		/*GTK construct ...*/
		GtkBuilder *builder;
		GtkWindow *window_enginespeed,*window_dashboard,*window_vehiclespeed;
		GtkLabel *label_vehiclespeed,*label_enginespeed;
		GtkButton *button[4];
		GtkDrawingArea *drawingarea_dashboard;

		if(!g_thread_supported()){
			g_thread_init(NULL);
		}

		gdk_threads_init();

		gdk_threads_enter();

		gtk_init(&argc, &argv);

		builder=gtk_builder_new();
		if(!gtk_builder_add_from_file(builder,DATADIR "/dashboard.glade",&error)){
			g_warning("%s",error->message);
			g_free(error);
			exit(EXIT_FAILURE);
		}

		/*retrieve the useful widgets from the builder*/
		window_enginespeed = GTK_WINDOW(gtk_builder_get_object(builder,"window_enginespeed"));
		window_vehiclespeed = GTK_WINDOW(gtk_builder_get_object(builder,"window_vehiclespeed"));
		window_dashboard = GTK_WINDOW(gtk_builder_get_object(builder,"window_dashboard"));
		label_enginespeed = GTK_LABEL(gtk_builder_get_object(builder,"label_enginespeed"));
		label_vehiclespeed = GTK_LABEL(gtk_builder_get_object(builder,"label_vehiclespeed"));
		drawingarea_dashboard = GTK_DRAWING_AREA(gtk_builder_get_object(builder,"drawingarea_dashboard"));
		button[0]=GTK_BUTTON(gtk_builder_get_object(builder,"button_enginespeed_plus"));
		button[1]=GTK_BUTTON(gtk_builder_get_object(builder,"button_enginespeed_moins"));
		button[2]=GTK_BUTTON(gtk_builder_get_object(builder,"button_vehiclespeed_plus"));
		button[3]=GTK_BUTTON(gtk_builder_get_object(builder,"button_vehiclespeed_moins"));


		/* connecting signals */
		/*control signals*/
		g_signal_connect(window_dashboard,"destroy",G_CALLBACK(gtk_main_quit),NULL);
		g_signal_connect(window_enginespeed,"destroy",G_CALLBACK(gtk_main_quit),NULL);
		g_signal_connect(window_vehiclespeed,"destroy",G_CALLBACK(gtk_main_quit),NULL);

		Dashboard_data dashboard_data;
		dashboard_data.h=h1;
		dashboard_data.s=s1;
		dashboard_data.cursor_big=rsvg_handle_new_from_file(DATADIR "/counter_cursor_big.svg",&error);
		dashboard_data.cursor_small=rsvg_handle_new_from_file(DATADIR "/counter_cursor_small.svg",&error);
		dashboard_data.dashboard_back=rsvg_handle_new_from_file(DATADIR "/dashboard_back.svg",&error);
		dashboard_data.dashboard_top=rsvg_handle_new_from_file(DATADIR "/dashboard_top.svg",&error);
		g_signal_connect(drawingarea_dashboard,"draw",G_CALLBACK(dashboard_draw),&dashboard_data);

		/*signals from the views to the model*/
		g_signal_connect(button[0],"clicked",G_CALLBACK(on_enginespeed_plus),h1);
		g_signal_connect(button[1],"clicked",G_CALLBACK(on_enginespeed_moins),h1);
		g_signal_connect(button[2],"clicked",G_CALLBACK(on_vehiclespeed_plus),s1);
		g_signal_connect(button[3],"clicked",G_CALLBACK(on_vehiclespeed_moins),s1);

		/*signals from the model to the views*/
		g_signal_connect(h1,"changed",G_CALLBACK(on_enginespeed_changed),label_enginespeed);
		g_signal_connect(s1,"changed",G_CALLBACK(on_vehiclespeed_changed),label_vehiclespeed);
		g_signal_connect(h1,"changed",G_CALLBACK(on_dashboard_draw_changed),drawingarea_dashboard);
		g_signal_connect(s1,"changed",G_CALLBACK(on_dashboard_draw_changed),drawingarea_dashboard);

		/*create another thread to read canbus*/

		readcanbus=g_thread_create(read_canbus,(gpointer)&dashboard_data,FALSE, &error);
		if(!readcanbus){
			g_print("Error %s\n",error->message);
		}

		/*display the views*/
		gtk_widget_show(GTK_WIDGET(window_enginespeed));
		gtk_widget_show(GTK_WIDGET(window_vehiclespeed));
		gtk_widget_show(GTK_WIDGET(window_dashboard));

		/*event loop*/
		gtk_main();

		gdk_threads_leave();

		/*cleaning up*/
		g_object_unref(h1);
		g_object_unref(s1);
		g_object_unref(G_OBJECT(builder));
		g_object_unref(dashboard_data.cursor_big);
		g_object_unref(dashboard_data.cursor_small);
		g_object_unref(dashboard_data.dashboard_back);
		g_object_unref(dashboard_data.dashboard_top);

		exit(EXIT_SUCCESS);
}
