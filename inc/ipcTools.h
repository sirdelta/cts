/**
 * 
 * @file ipcTools.h
 * Shared memory segments, semaphores and message queues
 * for collaborative processes synchronization.
 * 
 * This file declares high level functions to allocate and
 * de-allocate shared memory, semaphores and message queues.
 * 
 * ********************************************************* */
#ifndef __ipcTools_H
	#define __ipcTools_H

	#include <sys/types.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <sys/msg.h>

	/**
	 * Used as the default pathname to generate a System V IPC key.
	 */
	#define DEFAULT_PRJT_PATH "./etc/password/"

	/**
	 * Used as the default project id to generate a System V IPC key.
	 */
	#define DEFAULT_PRJT_ID 1

	/**
	 * Used as the default value assigned to a semaphore when it is allocated.
	 */
	#define DEFAULT_SEM_VAL_INIT 0

	/**
	 * The maximum size of a message (buffer statically allocated).
	 */
	#define MAX_MSG_SIZE 1024

	/**
	 * Generate a System V IPC key from a file specified by a file path
	 * and a project number. The file must exist and be accessible.
	 * The project number must be other than zero.
	 * 
	 * @param path the pathname of a file associated with the key
	 * @param prjt_id the projetc id associated with the key
	 * 
	 * @return the generated key if success, -1 otherwise
	 */
	key_t convertkey (char * path, int prjt_id);

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
	int semalloc (key_t key, int valInit);

	/**
	 * Destroy the semaphore associated with the given id.
	 * 
	 * @param semid the semaphore's id
	 * 
	 * @return 0 if success, -1 otherwise
	 */
	int semfree (int semid);

	/**
	 * Block on a semaphore.
	 * 
	 * @param semid the semaphore's id
	 */
	void P (int semid);

	/**
	 * Unlock a semaphore.
	 * 
	 * @param semid the semaphore's id
	 */
	void V (int semid);

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
	void * shmalloc (key_t key, int size);

	/**
	 * Deallocate the memory area associated with the given key.
	 * If the area has already been freed (or never allocated),
	 * do nothing.
	 * 
	 * @param key the key associated with the shared memory
	 * 
	 * @return 0 if success, -1 otherwise
	 */
	int shmfree (key_t key);

	/**
	 * Allocate a message queue.
	 * If a message queue is already associated with the given key,
	 * the existing id is returned (not reallocated).
	 * 
	 * @param key the key associated with the message queue
	 * 
	 * @return the associated id of the message queue, -1 otherwise
	 */
	int msgalloc (key_t key);

	/**
	 * Destroy the message queue associated with the given id.
	 * 
	 * @param msgqid the id associated with the message queue.
	 * 
	 * @return 0 if success, -1 otherwise
	 */
	int msgfree (int msgqid);

	/**
	 * Send a message in a message queue with the specified id in input.
	 * 
	 * @param msgqid the id associated with the message queue.
	 * @param msg the sended message in the message queue
	 * @param size the size of the message
	 * 
	 * @return 0 if success, -1 otherwise
	 */
	int msgsend (int msgqid, char * msg, int size);

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
	int msgrecv (int msgqid, char * msg, int size);

#endif