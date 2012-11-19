#include "fsending.h"

/*
 * Listenning mode for the client
 */
char* receive_file() {
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
	char filename[9] = "dummyfile";
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // bind socket to port 0x1001 of the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_psm = htobs(0x1001);
	
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.l2_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);

    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
   	    printf("Lets write some shit: %d ", bytes_read);
   	    FILE *fp = fopen (filename, "r");
   	    if ( fp != NULL ) {
   	    	fwrite(buf, bytes_read, 1, fp);
   	    	fclose(fp); 
    		close(client);
    		close(s);	
    		return filename;
   	    }
   	    else {
   	    	// ERROR	
      		fprintf(stderr, "unable to open %s: %s\n", filename, strerror(errno));
      		exit(1);
   	    }
    }

    // close connection
    close(client);
    close(s);	
    return "";
}

/*
 * Sending files mode for the client
 */ 
void send_file() {
	struct sockaddr_l2 addr = { 0 };
    int s, status, offset;
    char *message = "hello!";
    char dest[18] = "00:19:88:0B:1B:79"; 
    
	char filename[9] = "dummyfile";
    // Variables for the sending of the file
    
 	struct stat stat_buf; /* argument to fstat */
    int fd, rc;
    
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);

    // set the connection parameters (who to connect to)
    addr.l2_family = AF_BLUETOOTH;
    addr.l2_psm = htobs(0x1001);
    str2ba( dest, &addr.l2_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        
   		 /* open the file to be sent */
    	 fd = open(filename, O_RDONLY);
    	 if (fd == -1) {
      		fprintf(stderr, "unable to open %s: %s\n", filename, strerror(errno));
      		exit(1);
    	 }

   		 /* get the size of the file to be sent */
   		 fstat(fd, &stat_buf);

   		 /* copy file using sendfile */
 		 offset = 0;
    	 rc = sendfile (s, fd, &offset, stat_buf.st_size);
    	 if (rc == -1) {
      	 	fprintf(stderr, "error from sendfile: %s\n", strerror(errno));
      		exit(1);
    	 }
    	 if (rc != stat_buf.st_size) {
      		fprintf(stderr, "incomplete transfer from sendfile: %d of %d bytes\n", rc,
              (int)stat_buf.st_size);
      		exit(1);
    	 }

    	 /* close descriptor for file that was sent */
    	 close(fd); 
    }

    if( status < 0 ) perror("uh oh");

    close(s);	
}