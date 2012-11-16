#include "fdiscovery.h"

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
	

int main() {
	int choice = 0;
	int childProc, status;
	
	printf("- FileStix - \n \n ");
	
	if ( (childProc = fork()) < 0 ) { perror("fork issue"); }
	
	if ( childProc == 0 ) {
		// Child Process - Running the application
		choice = application_menu();
		while ( choice != -1 ) {
			launch_function(choice);
		}
	}
	else {
		// Father process - Listening to received files
		while ( waitpid(childProc, &status, WNOHANG) == 0 ) {
			receive_file();
		}
        if ( WIFEXITED(status) ) 
        	printf("childProc exited with exit status %d.\n", WEXITSTATUS(status));
        else 
        	perror("ChildProc has not terminated correctly.\n");
	}
	
	return EXIT_SUCCESS;
}