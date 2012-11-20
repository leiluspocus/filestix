#include "fview.h" 

GtkWidget *pFileTextView; 
gchar *sFilesReceivedLabel;
GtkWidget *pNotebook;

GtkWidget *pWindow;
GtkWidget *pVBox;
GtkWidget *pSendFileButton;
GtkWidget *pScanDevicesButton;
    

void notifyViewNewFileReceived(char*fileName) {
	GtkWidget *pDialog;
	gchar* sDialogText;
	int argc=0;
	char ** argv;
	gtk_init(&argc, &argv);
	
	gtk_notebook_set_current_page (GTK_NOTEBOOK(pNotebook), 3);
	
    sDialogText = g_strdup_printf("File received from \n" 
        "%s\"\n", 
        fileName);

    pDialog = gtk_message_dialog_new (NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        sDialogText);

    gtk_dialog_run(GTK_DIALOG(pDialog));

    gtk_widget_destroy(pDialog);

    g_free(sDialogText);	
	
}
    
void OnScan(GtkWidget *pScanDevicesButton, gpointer data) {
    GtkWidget *pDialog;
    GtkWidget *pChild;
    gint iPageNum;
    const gchar *sLogsLabel;
    const gchar *sTabLabel;
    const gchar *sMenuLabel;
    gchar *sDialogText;
    char **hostsReachable;
	int i=0;
    /* Recuperation de la page active */
    iPageNum = gtk_notebook_get_current_page(GTK_NOTEBOOK(data));
    /* Recuperation du widget enfant */
    pChild = gtk_notebook_get_nth_page(GTK_NOTEBOOK(data), iPageNum);

    /* Recuperation du label */
    sLogsLabel = gtk_label_get_text(GTK_LABEL(pChild));
    /* Recuperation du label de l'onglet */
    sTabLabel = gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(data), pChild);
    /* Recuperation du label du menu pop-up */
    sMenuLabel = gtk_notebook_get_menu_label_text(GTK_NOTEBOOK(data), pChild);


	// Calling the method "Scan" of the "fsending" file.
	hostsReachable = discovery();
	
    /* Creation du label de la boite de dialogue */
    char * message = (char*) malloc(400*sizeof(char));
    strcpy(message, hostsReachable[0]);
    strcat(message, "\n");
    if ( hostsReachable != NULL ) { 
	    for ( i=1 ; i < sizeof(hostsReachable) ; ++i ) {
	    	strcat(message, hostsReachable[i]);
	    	strcat(message, "\n");
	    }
    }
    else {
    	message = "No hosts available";	
    }
    sDialogText = g_strdup_printf("There are %d hosts around \n" 
        "%s\"\n",
        sizeof(hostsReachable), 
        message);

    pDialog = gtk_message_dialog_new (NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        sDialogText);

    gtk_dialog_run(GTK_DIALOG(pDialog));

    gtk_widget_destroy(pDialog);

    g_free(sDialogText);	
    free(hostsReachable);
    free(message);
}

void OnSend(GtkWidget *pSendFileButton, gpointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pChild;
    gint iPageNum;
    int res;
    const gchar *sLogsLabel;
    const gchar *sTabLabel;
    const gchar *sMenuLabel;
    gchar *sDialogText;

    /* Recuperation de la page active */
    iPageNum = gtk_notebook_get_current_page(GTK_NOTEBOOK(data));
    /* Recuperation du widget enfant */
    pChild = gtk_notebook_get_nth_page(GTK_NOTEBOOK(data), iPageNum);

    /* Recuperation du label */
    sLogsLabel = gtk_label_get_text(GTK_LABEL(pChild));
    /* Recuperation du label de l'onglet */
    sTabLabel = gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(data), pChild);
    /* Recuperation du label du menu pop-up */
    sMenuLabel = gtk_notebook_get_menu_label_text(GTK_NOTEBOOK(data), pChild);

    /* Creation du label de la boite de dialogue */
    res = send_file();
    if ( res != -1 ) {
   		sDialogText = g_strdup_printf("I've sent the file");
    }
    else {
    	sDialogText = g_strdup_printf("Error ! Are you sure that this file exists?");
    }
    pDialog = gtk_message_dialog_new (NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        sDialogText);

    gtk_dialog_run(GTK_DIALOG(pDialog));

    gtk_widget_destroy(pDialog);

    g_free(sDialogText);
} 


void construct_gui(int argc, char **argv) {
	
    gint i;

    gtk_init(&argc,&argv);

    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
    gtk_window_set_title(GTK_WINDOW(pWindow), "GtkNotebook");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
    g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	// Send a file common to all the views
    pSendFileButton = gtk_button_new_with_label("Send a file");
    gtk_box_pack_start(GTK_BOX(pVBox), pSendFileButton, FALSE, FALSE, 0);

	pFileTextView = gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(pVBox), pFileTextView, FALSE, FALSE, 0);
    
    /* Creation of the GtkNotebook */
    pNotebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(pVBox), pNotebook, TRUE, TRUE, 0);
    /* Position des onglets : en bas */
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(pNotebook), GTK_POS_BOTTOM);
    /* Ajout des boutons de navigation */
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(pNotebook), TRUE);
 
    GtkWidget *pLabel;
    GtkWidget *pTabLabel;
    gchar *sLogsLabel;
    gchar *sTabLabel;
     
    // Creation of the "Received Files" panel 
    sTabLabel = g_strdup_printf("Hosts Reachable");
    pTabLabel = gtk_label_new(sTabLabel);
    pScanDevicesButton = gtk_button_new_with_label("Scan..."); 
    
    /* Insertion de la page */ 
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pScanDevicesButton, pTabLabel);
    g_free(sTabLabel);

    
    // Creation of the "Received Files" panel
    sFilesReceivedLabel = g_strdup_printf("Files received");
    sTabLabel = g_strdup_printf("Downloads");

    pLabel = gtk_label_new(sFilesReceivedLabel);
    pTabLabel = gtk_label_new(sTabLabel);

    /* Insertion de la page */
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pLabel, pTabLabel);

    g_free(sFilesReceivedLabel);
    g_free(sTabLabel);
    
    // Creation of the "Logs" Panel
    sLogsLabel = g_strdup_printf("Logs of the application");
    sTabLabel = g_strdup_printf("Logs");

    pLabel = gtk_label_new(sLogsLabel);
    pTabLabel = gtk_label_new(sTabLabel);

    /* Insertion de la page */
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pLabel, pTabLabel);

    g_free(sLogsLabel);
    g_free(sTabLabel);
    
    
    
    /* Activation du menu popup */
    gtk_notebook_popup_enable(GTK_NOTEBOOK(pNotebook));

    g_signal_connect(G_OBJECT(pSendFileButton), "clicked", G_CALLBACK(OnSend), pNotebook);
    g_signal_connect(G_OBJECT(pScanDevicesButton), "clicked", G_CALLBACK(OnScan), pNotebook);

    gtk_widget_show_all(pWindow);

    gtk_main();



}
	