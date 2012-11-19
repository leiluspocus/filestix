#include "commonheaders.h"
#include <gtk/gtk.h>


void construct_gui(int argc, char **argv);

int application_menu() ;

void launch_function(int choice);

void OnSend(GtkWidget *pButton, gpointer data);
void OnScan(GtkWidget *pScanDevicesButton, gpointer data);
void notifyViewNewFileReceived(char*fileName); 