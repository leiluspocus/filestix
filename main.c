#include "fdiscovery.h"
#include "fview.h"

int main(int argc, char **argv) {
	int choice = 0; 
	int childProc, status;
	char* filename;
	printf("- FileStix - \n \n ");
	 
	 
	if ( (childProc = fork()) < 0 ) { perror("fork issue"); }	
	if ( childProc == 0 ) {
		// Child Process - Running the application 
		printf("fils construit la gui");
		construct_gui(argc, argv); 
	}
	else {
		// Father process - Listening to received files
		while ( waitpid(childProc, &status, WNOHANG) == 0 ) {
			filename = receive_file();
			if ( filename != "" ) {
				printf("%s received <<<", filename);	
			}
		}
        if ( WIFEXITED(status) ) 
        	printf("childProc exited with exit status %d.\n", WEXITSTATUS(status));
        else 
        	perror("ChildProc has not terminated correctly.\n");
	}
	
	return EXIT_SUCCESS;
}