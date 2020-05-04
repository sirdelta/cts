/**
 * 
 * @file ipcTools.c
 * Shared memory segments, semaphores and message queues
 * for collaborative processes synchronization.
 * 
 * Implementation of functions defined in @see ipcTools.h
 * 
 * ********************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/ipcTools.h"

/**
 * Generate a System V IPC key from a file specified by a file path
 * and a project number.
 * The file must exist and be accessible. The project number must
 * be other than zero.
 * 
 * @param path the pathname of a file associated with the key
 * @param prjt_id the projetc id associated with the key
 * 
 * @return the generated key if success, -1 otherwise
 */
key_t convertkey (char * path, int prjt_id) {
    key_t key; 
    // if (strncmp (path, "", strlen (path)) == 0)
	if (strncmp (path, "", 2) == 0)
        path = DEFAULT_PRJT_PATH;
    if (prjt_id < 0)
        prjt_id = DEFAULT_PRJT_ID;
    if ((key = ftok (path, prjt_id) == -1))
		return -1;
	return key;
}

/**
 * Allocate a semaphore.
 * If a semaphore is already associated with the given key, it is
 * simply returned (not reallocated).
 * 
 * @param key the key associated with the semaphore
 * @param valInit the initial value of the semaphore
 * 
 * @return the generated id of the sempahore if success, -1 otherwise
 */
int semalloc (key_t key, int valInit) {
	int semid = semget (key, 1, 0);
	if (semid == -1) {	/* The semaphore does not exist yet. */
		semid = semget (key, 1, IPC_CREAT|IPC_EXCL|0600);
		if (semid == -1)
			return -1;
		if (valInit < 0)
			valInit = DEFAULT_SEM_VAL_INIT;
		if (semctl (semid, 0, SETVAL, valInit) == -1) {
			semctl (semid, 0, IPC_RMID, 0);
			return -1;
		}
	}
	return semid;
}

/**
 * Destroy the semaphore associated with the given id.
 * 
 * @param semid the semaphore's id
 * 
 * @return 0 if success, -1 otherwise
 */
int semfree (int semid) {
	return semctl (semid, 0, IPC_RMID, 0);
}

/**
 * Used to implement the P operation on semaphores (decrementation).
 */
static struct sembuf sP = {0,-1,0};

/**
 * Used to implement the V operation on semaphores (incrementation).
 */
static struct sembuf sV = {0, 1,0};

/**
 * Block on a semaphore.
 * 
 * @param semid the semaphore's id
 */
void P (int semid) {
	semop (semid, &sP, 1);
}

/**
 * Unlock a semaphore.
 * 
 * @param semid the semaphore's id
 */
void V (int semid) {
	semop (semid, &sV, 1);
}

/**
 * Allocate a shared memory area.
 * If a shared memory is already allocated with the specified key,
 * its address will be returned and not reallocated.
 * 
 * @param key the key associated with the shared memory
 * @param size the size of the memory segment
 * 
 * @return the adress of the shared memory segment, 0 otherwise
 */
void * shmalloc (key_t key, int size) {
    void * addr;
	int alreadyCreat = 0;	/*	Used for specified if its already allocated
								0: not yet,	1: yes	*/
	int shmid = shmget (key, 1, 0600);
	if (shmid == -1) {	/* The shared memory does not exist yet. */
		alreadyCreat = 1;
		shmid = shmget (key, size, IPC_CREAT|IPC_EXCL|0600);
		if (shmid == -1)
			return 0;
	}
	addr = shmat (shmid, 0, 0);
	if (addr == (void *) -1)  {
		if (alreadyCreat)
			shmctl (shmid, IPC_RMID, 0);
		return 0;
	}
    return addr;
}

/**
 * Destroy the shared memory with the key specified as input.
 * 
 * @param key the key associated with the shared memory
 * 
 * @return 0 if success, -1 otherwise
 */
int shmfree (key_t key) {
	int shmid = shmget (key, 0, 0600);
	return shmctl (shmid, IPC_RMID, NULL);
}

/**
 * Allocate a message queue.
 * If a message queue is already associated with the given key,
 * the existing id is returned (not reallocated).
 * 
 * @param key the key associated with the message queue
 * 
 * @return the associated id of the message queue, -1 otherwise
 */
int msgalloc (key_t key) {
    int msgqid = msgget (key, 0);
	if (msgqid == -1) {	/* The message queue does not exist yet. */
		msgqid = msgget (key, IPC_CREAT|IPC_EXCL|0777);
	}
	return msgqid;
}

/**
 * Destroy the message queue associated with the given id.
 * 
 * @param msgqid the id associated with the message queue.
 * 
 * @return 0 if success, -1 otherwise
 */
int msgfree (int msgqid) {
    return msgctl (msgqid, IPC_RMID, NULL);
}

/**
 * The statically allocated buffer to handle messages
 * in message-queue-related functions
 */
static struct { long type; char msg[MAX_MSG_SIZE]; } buffer;

/**
 * Send a message in a message queue with the specified id in input.
 * 
 * @param msgqid the id associated with the message queue.
 * @param msg the sended message in the message queue
 * @param size the size of the message
 * 
 * @return 0 if success, -1 otherwise
 */
int msgsend (int msgqid, char * msg, int size) {

	if (size > MAX_MSG_SIZE) {
		perror ("Sended message size too long");
		return -1;
	}
	buffer.type = 1;
	strncpy (buffer.msg, msg, size);
	return msgsnd (msgqid, &buffer, size, 0);
}

/**
 * Receive a message from a message queue with the corresponding id
 * specified as input.
 * 
 * @param msgqid the id associated with the message queue.
 * @param msg the received message from the message queue
 * @param size the size of the message
 * 
 * @return the number of byte read if success, -1 otherwise
 */
int msgrecv (int msgqid, char * msg, int size) {
	int rcv;
	if (size > MAX_MSG_SIZE) {
		perror ("Received message size too long");
		return -1;
	}
	buffer.type = 1;
	rcv = msgrcv (msgqid, &buffer, size, 1, 0);
	if (rcv != -1)
		strncpy (msg, buffer.msg, size);
    return rcv;
}