#include "fdiscovery.h"

int main() {
	int choice = 0;
	
	printf("- FileStix -");
	printf("Press 1 to scan, 2 to send, -1 to exit");
	scanf("%d", &choice);
	
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
	
	return EXIT_SUCCESS;
}