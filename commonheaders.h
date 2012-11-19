#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h> 
#include <sys/types.h>  
#include <sys/wait.h>

  
#include <errno.h>
#include <fcntl.h>
#include <sys/sendfile.h> 
#include <sys/stat.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

