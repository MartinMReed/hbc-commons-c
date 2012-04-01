/**
 * Copyright (c) 2010 Martin M Reed
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "thread_queue.h"

#include <stdio.h>
#include <stdlib.h>

/**
 *
 */
hbc::thread_queue::thread_queue(int thread_count) {
  
  this->thread_count = thread_count;
  threads = (pthread_t**) calloc(thread_count, sizeof(pthread_t*));
  
  pthread_mutex_init(&mutex, 0);
}

/**
 *
 */
hbc::thread_queue::~thread_queue() {

  detach_all();
  
  thread_count = 0;
  
  delete threads;
  threads = 0;
  
  pthread_mutex_destroy(&mutex);
  
  while (!runnables.empty()) {
  
    runnable* runnable = runnables.front();
    runnables.pop();
    delete runnable;
  }
}

/**
 *
 */
void hbc::thread_queue::join() {

  for (int i = 0; i < thread_count;) {
  
    pthread_mutex_lock(&mutex);
    pthread_t* thread = threads[i];
    pthread_mutex_unlock(&mutex);
    
    if (!thread) {
    
      i++;
      continue;
    }
    
    pthread_join(*thread, 0);
    
    // if we found a thread, go back to the
    // front to see if there are any new threads.
    // if there are not, then we will just keep hitting
    // the continue-if block above and eventually exit out.
    i = 0;
  }
}

/**
 *
 */
void* hbc::run(void* cookie) {

  thread_queue* queue = (thread_queue*) cookie;
  
  runnable* runnable;
  
  while (runnable = queue->pop()) {
  
    runnable->run();
    
    delete runnable;
    runnable = 0;
  }
  
  queue->detach(pthread_self());
}

/**
 *
 */
void hbc::thread_queue::push(runnable* runnable) {

  if (runnable == 0) {
  
    throw "runnable may not be null";
  }
  
  pthread_mutex_lock(&mutex);
  
  runnables.push(runnable);
  
  for (int i = 0; i < thread_count; i++) {
  
    if (threads[i]) {
    
      continue;
    }
    
    printf("creating new thread at pos %i.\n", i);
    threads[i] = new pthread_t();
    pthread_create(threads[i], 0, &hbc::run, this);
  }
  
  pthread_mutex_unlock(&mutex);
}

/**
 *
 */
hbc::runnable* hbc::thread_queue::pop() {
  
  runnable* runnable = 0;
  
  pthread_mutex_lock(&mutex);
  
  if (!runnables.empty()) {
  
    runnable = runnables.front();
    runnables.pop();
  }
  
  pthread_mutex_unlock(&mutex);
  
  return runnable;
}

/**
 *
 */
void hbc::thread_queue::detach(pthread_t id) {
  
  pthread_mutex_lock(&mutex);
  
  for (int i = 0; i < thread_count; i++) {
  
    if (!threads[i] || *threads[i] != id) {
    
      continue;
    }
    
    if (id != pthread_self()) {
    
      pthread_detach(*threads[i]);
      pthread_join(*threads[i], 0);
    }
    
    delete threads[i];
    threads[i] = 0;
  }
  
  pthread_mutex_unlock(&mutex);
}

/**
 *
 */
void hbc::thread_queue::detach_all() {
  
  pthread_mutex_lock(&mutex);
  
  for (int i = 0; i < thread_count; i++) {
  
    if (!threads[i]) {
    
      continue;
    }
    
    detach(*threads[i]);
  }
  
  pthread_mutex_unlock(&mutex);
}
