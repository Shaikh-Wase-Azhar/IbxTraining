#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/sp-example-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

struct Q{

    char qname[64];
    char buff[100];
    int cmd;
};


int main (int argc, char **argv)
{
    struct Q s1;
    memset(&s1,0 ,sizeof(struct Q));
    printf("Client:Enter the message for Reciever:");
    //gets(&s1.buff);
    scanf("%[^\n\t]s",s1.buff);

    char client_queue_name [64];
    mqd_t qd_server, qd_client;   // queue descriptors


   //  create the client queue for receiving messages from server
    sprintf (s1.qname, "/sp-example-client-%d", getpid ());

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_client = mq_open (s1.qname, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Client: mq_open (client)");
        exit (1);
    }

    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDWR)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }

    char in_buffer [MSG_BUFFER_SIZE];

    printf ("Ask %s for a token (Press <ENTER>):\n",s1.qname);

    char temp_buf [10];

    while (fgets (temp_buf, 2, stdin)) {

        // send message to server
        if (mq_send (qd_server, (const char *)&s1, sizeof(s1), 0) == -1) {
            perror ("Client: Not able to send message to server");
            continue;
        }

        // receive response from server

        if (mq_receive (qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Client: mq_receive");
            exit (1);
        }
        // display token received from server
        printf ("Client: Token received from server: %s\n\n", in_buffer);

        printf ("Ask for a token (Press ): ");
    }


    if (mq_close (qd_client) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }

    if (mq_unlink (s1.qname) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: bye\n");

    exit (0);
}