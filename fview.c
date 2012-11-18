#include "fview.h" 

int application_menu() {
	int choice;
	printf("Press 1 to scan, 2 to send, -1 to exit");
	scanf("%d", &choice);
	return choice;
}

void launch_function(int choice) {	
	switch(choice) {
		case 1:
			discovery();	
			break;
		case 2:
			send_file();
			break;
		default:
			printf("Unknown choice…");
			break;
	}
}

void OnButton(GtkWidget *pSendFileButton, GPointer data)
{
    GtkWidget *pDialog;
    GtkWidget *pChild;
    gint iPageNum;
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
    sDialogText = g_strdup_printf("C'est la page %d\n"
        "Le label est \"%s\"\n"
        "Le label de l'onglet est \"%s\"\n"
        "Le label du menu est \"%s\"\n",
        iPageNum,
        sLogsLabel,
        sTabLabel,
        sMenuLabel);

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
	
    GtkWidget *pWindow;
    GtkWidget *pVBox;
    GtkWidget *pNotebook;
    GtkWidget *pSendFileButton;
	GtkWidget *pFileTextView; 
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
    
    // Creation of the "Logs" Panel
    sLogsLabel = g_strdup_printf("Logs of the application");
    sTabLabel = g_strdup_printf("Logs");

    pLabel = gtk_label_new(sLogsLabel);
    pTabLabel = gtk_label_new(sTabLabel);

    /* Insertion de la page */
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pLabel, pTabLabel);

    g_free(sLogsLabel);
    g_free(sTabLabel);
    
    gchar *sFilesReceivedLabel;
    // Creation of the "Received Files" panel
    sFilesReceivedLabel = g_strdup_printf("Files received");
    sTabLabel = g_strdup_printf("Downloads");

    pLabel = gtk_label_new(sFilesReceivedLabel);
    pTabLabel = gtk_label_new(sTabLabel);

    /* Insertion de la page */
    gtk_notebook_append_page(GTK_NOTEBOOK(pNotebook), pLabel, pTabLabel);

    g_free(sLogsLabel);
    g_free(sTabLabel);
    
    /* Activation du menu popup */
    gtk_notebook_popup_enable(GTK_NOTEBOOK(pNotebook));

    g_signal_connect(G_OBJECT(pSendFileButton), "clicked", G_CALLBACK(OnButton), pNotebook);

    gtk_widget_show_all(pWindow);

    gtk_main();



}
	