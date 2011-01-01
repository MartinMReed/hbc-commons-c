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

#ifndef HBC_THREAD_QUEUE_H
#define HBC_THREAD_QUEUE_H

#include "runnable.h"
using namespace hbcn_thread;

#include <pthread.h>

#include <queue>
using namespace std;

namespace hbcn_thread {
  
  void* run(void* cookie);
  
  class thread_queue {
  
    friend void* run(void* cookie);
    
    public:
    
    thread_queue(int thread_count);
    ~thread_queue();
    
    void push(runnable* runnable);
    runnable* pop();
    
    void join();
    
    private:
    
    int thread_count;
    pthread_t** threads;
    
    pthread_mutex_t mutex;
    
    queue<runnable*> runnables;
    
    void detach(pthread_t id);
    void detach_all();
  };
}

#endif
