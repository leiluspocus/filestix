#include "fdiscovery.h"
#include "fview.h"

int main() {
	int choice = 0; 
	int childProc, status;
	
	printf("- FileStix - \n \n ");
	 
	 
	if ( (childProc = fork()) < 0 ) { perror("fork issue"); }	
	if ( childProc == 0 ) {
		// Child Process - Running the application 
		printf("fils construit la gui");
		construct_gui(); 
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