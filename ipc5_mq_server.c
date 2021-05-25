/*


token

Proce-1 <message Queue-1/Created/reading > Rreading from PRoc-2 

Proce2 <Message Queue -1/ create/write >  Writing into mesg1  

POSIX message queues allow for an efficient, priority-driven IPC mechanism with multiple readers and writers.

mqd_t

Like pipes and FIFOs, all message queue operations are performed based on message queue descriptors (an mqd_t). 


mqd_t mq_open (const char *name, int oflag); // opening already created queue
mqd_t mq_open (const char *name, int oflag, mode_t mode,
               struct mq_attr *attr); // for new queue creation

 The mq_open function is for opening a POSIX queue. The first parameter specifies the queue name, The second parameter is a flag which can be O_RDONLY for receiving messages, O_WRONLY, for sending messages and O_RDWR for both send and receive operations on the queue. More values can be OR'ed to this flag. You can specify O_NONBLOCK to use the queue in a non-blocking mode. By default, mq_send would block if the queue was full and mq_receive would block if there was no message in the queue. But if O_NONBLOCK is specified in oflag, the call would return in those cases immediately with errno set to EAGAIN.

If you specify O_CREAT as a part of oflag, the queue is created, if it does not, already, exist. If you specify O_EXCL along with O_CREAT, and the queue exists, mq_open fails with errno set to EEXIST. If O_CREAT is specified in oflag, the second form of mq_open has to be used with two additional parameters. In that case, mode specifies permissions for the queue and the pointer to struct mq_attr gives the attributes for the message queue. If this pointer is NULL, a queue with default attributes is created.

struct mq_attr {
    long mq_flags;       /* Flags: 0 or O_NONBLOCK 
    long mq_maxmsg;      /* Max. # of messages on queue 
    long mq_msgsize;     /* Max. message size (bytes) 
    long mq_curmsgs;     /* # of messages currently in queue 
};  


int mq_close (mqd_t mqdes);

The mq_close call closes the message queue descriptor, mqdes.



int mq_send (mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
             unsigned int msg_prio);

mq_send is for sending a message to the queue referred by the descriptor mqdes. The msg_ptr points to the message buffer. msg_len is the size of the message, which should be less than or equal to the message size for the queue. msg_prio is the message priority, which is a non-negative number specifying the priority of the message. Messages are placed in the queue in the decreasing order of message priority, with the older messages for a priority coming before newer messages. If the queue is full, mq_send blocks till there is space on the queue, unless the O_NONBLOCK flag is enabled for the message queue, in which case mq_send returns immediately with errno set to EAGAIN.


ssize_t mq_receive (mqd_t mqdes, char *msg_ptr, size_t msg_len, 
                    unsigned int *msg_prio);

mq_receive receives a message from the queue referred by the descriptor mqdes. The oldest of the highest priority is deleted from the queue and passed to the process in the buffer pointed by msg_ptr. msg_len is the length of buffer in bytes and it must be greater than the maximum message size, the mq_msgsize attribute, for the queue. If the pointer msg_prio is not null, the priority of the received message is stored in the integer pointed by it. The default behavior of mq_receive is to block if there is no message in the queue. However, if the O_NONBLOCK flag is enabled for the queue, and the queue is empty, mq_receive returns immediately with errno set to EAGAIN. On success, mq_receive returns the number of bytes received in the buffer pointed by msg_ptr.


int mq_unlink(const char *queue_name);

mq_unlink removes the queue with the name queue_name.


int mq_timedsend (mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
                  unsigned int msg_prio, const struct timespec *abs_timeout);



ssize_t mq_timedreceive (mqd_t mqdes, char *msg_ptr, size_t msg_len, 
                         unsigned int *msg_prio, 
                         const struct timespec *abs_timeout);


struct timespec {
    time_t tv_sec;        /* seconds 
    long   tv_nsec;       /* nanoseconds 
}; 

 */
0- REQUEST
1 - RESPONSE

/* Task _1
 
REad from input string write --> buff
create a unique client quename -> qname
cmd = 0;

P_A to P_B (Read)
P_A1 to P_B (READ)

...

P_A5


---- pb to client (p_a, p_a1)

P-B ( Reading Queue name + Read buff +print)
    Take input string -> buff
     cmd =1



struct{
	char qname[] // client name

	char buff[] // message

	int cmd; // 0 or 1


}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/sp-example-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main (int argc, char **argv)
{
    mqd_t qd_server, qd_client;   // queue descriptors
    long token_number = 1; // next token to be given to client

    printf ("Server: Hello, World!\n");

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    mq_unlink(SERVER_QUEUE_NAME);
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    char in_buffer [MSG_BUFFER_SIZE];
    char out_buffer [MSG_BUFFER_SIZE];

    while (1) {
        // get the oldest message with highest priority
        if (mq_receive (qd_server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Server: mq_receive");
            exit (1);
        }

        printf ("Server: message received. %s\n",in_buffer);

        // send reply message to client
	// client Queue name
	// open client queue -- write mode
        if ((qd_client = mq_open (in_buffer, O_WRONLY)) == 1) {
            perror ("Server: Not able to open client queue");
            continue;
        }

        sprintf (out_buffer, "%ld", token_number);

        if (mq_send (qd_client, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
            perror ("Server: Not able to send message to client");
            continue;
        }

        printf ("Server: response sent to client.\n");
        token_number++;
    }
}
