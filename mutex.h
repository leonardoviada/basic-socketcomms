/*
 * mutex.h
 * This file is part of DelpozzoSoft
 *
 * Copyright (C) 2016 - Delpozzo - Cuneo (CN)
 *
 * DelpozzoSoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * DelpozzoSoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DelpozzoSoft. If not, see <http://www.gnu.org/licenses/>.
 */

/*--------------------------------------------------------------------
  mutex.h 
	v 2.1.1	(Ottobre 2020)
----------------------------------------------------------------------
Libreria di interfaccia per la gestione di semplici semafori binari
per mutua esclusione.

ITIS Delpozzo - Cuneo (CN)
Dipartimento Informatica 
Author: Tosello G

Se modificate questo sorgente per favore fatene
avere una copia agli autori originali, cosi' da 
permettere l'allineamento ed evitare la diffusione
di cloni o fork che non garantiscano la compatibilita'
verso il preesistente.

Grazie.                     			L'autore
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef __MUTEX_H
#define	__MUTEX_H

/* prototipi */
int mutex_create (key_t ipc_key, int base_value);
int mutex_find (key_t ipc_key);
int mutex_trylock(int sem_id);
int mutex_lock(int sem_id);
int mutex_unlock(int sem_id);
int mutex_remove(int sem_id);

/* strutture per le chiamate a semop() per modificare il contenuto del mutex */
struct sembuf sem_lock = {
		0,		    /* numero del mutex */
		-1,		    /* togliere uno al valore nel mutex */
		SEM_UNDO };	/* quando esce dal processo ripristina il valore a uno */
struct sembuf sem_trylock = {
		0,		    /* numero del mutex */
		-1,		    /* togliere uno al valore nel mutex */
		SEM_UNDO | IPC_NOWAIT };	/* quando esce dal processo ripristina il valore a uno
                                    /* se non riesce subito a decrementare il semaforo esce
                                        con errore */
struct sembuf sem_unlock = {
		0,		    /* numero del mutex */
		+1,		    /* aggiungere uno al valore nel mutex */
		0 };	    /* nessun flag */

/* mutex_create()      crea un mutex
RICEVE: l'identificativo nel sistema dell'insieme semafori e il valore al quale inizializzare
RESTITUISCE: un intero che identifica il semaforo 
ATTENZIONE: questa funzione NON è atomica*/
int mutex_create (key_t ipc_key, int base_value) {
  int ret, sem_id;

    /* semget       crea un semaforo 
    ipc_key      --> chiave univoca associata al gruppo di strutture IPC
    1            --> numero di strutture contenute nell'insieme
    3o parametro --> operazione da compiere messe in OR:
 		      IPC_CREAT :crea l'insieme di semafori 
		      IPC_EXCL	:restituisce un valore non valido nel caso in cui
				la struttura esista gia'	
		      0666: accesso consentito a tutti */
    sem_id = semget(ipc_key, 1, IPC_CREAT|IPC_EXCL|0666);
    if (sem_id == -1) 
        return (sem_id);  

    /* semctl       inizializzazione semaforo 
    sem_id        --> id del semaforo
    0             --> numero del semaforo da inizializzare
    SETVAL        --> operazione di impostazione del valore del semaforo
    base_value    --> valore di inizializzazione */
    struct sembuf sem_setbase =  {
               base_value,    /* valore per SETVAL */
               0,    /* non usato */
               0    }; /* non usato */
    ret = semctl(sem_id, 0, SETVAL, sem_setbase); 
    if (ret == -1)
		return (ret); 
 
    return (sem_id);
 }

/* mutex_find()
   RICEVE: l'identificativo nel sistema dell'insieme di oggetti IPC 
   RESTITUISCE: un intero che identifica il semaforo */
int mutex_find (key_t ipc_key) {
    /*  semget 
    ipc_key      --> chiave univoca associata al gruppo di strutture IPC
    il 2o parametro è ignorato; 
    il 3o parametro indica che non si compiono operazioni sul semaforo */
    return semget(ipc_key, 1, 0);
 }

/* mutex_lock()
   RICEVE: id del semaforo
   RESTITUISCE: 0 in caso di successo */
int mutex_lock(int sem_id) {
     /*  semop 
         sem_id     --> identificativo del semaforo
         &sem_lock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_lock, 1);
}

/* mutex_trylock()
   RICEVE: id del semaforo
   RESTITUISCE: 0 in caso di successo */
int mutex_trylock(int sem_id) {
     /*  semop 
         sem_id     --> identificativo del semaforo
         &sem_lock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_trylock, 1);
}

/* mutex_unlock()
   RICEVE: id del semaforo
   RESTITUISCE: 0 in caso di successo */
int mutex_unlock(int sem_id) {
     /*  semop 
         sem_id     --> identificativo del semaforo
         &sem_unlock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_unlock, 1);
 }

/* mutex_remove()
   RICEVE: id del semaforo
   RESTITUISCE: 0 in caso di successo */
int mutex_remove(int sem_id) {
  /* semctl       
     sem_id       --> id del semaforo
     2o parametro --> ignorato
     IPC_RMID     --> operazione di RIMOZIONE del semaforo */
  return semctl(sem_id,	0, IPC_RMID);	
}

#endif	/* __MUTEX_H */

