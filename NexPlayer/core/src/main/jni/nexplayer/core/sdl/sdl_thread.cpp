//
// Created by txy on 18-6-4.
//
extern "C" {
#include "sdl_thread.h"
#include <stdio.h>
#include <assert.h>

#if !defined(__APPLE__)
// using ios implement for autorelease
static void *SDL_RunThread(void *data)
{
    SDL_Thread *thread = (SDL_Thread *)data;
    pthread_setname_np(pthread_self(), thread->name);
    thread->retval = thread->func(thread->data);
    return NULL;
}

SDL_Thread *SDL_CreateThreadEx(SDL_Thread *thread, int (*fn)(void *), void *data, const char *name)
{
    thread->func = fn;
    thread->data = data;
    strlcpy(thread->name, name, sizeof(thread->name) - 1);
    int retval = pthread_create(&thread->id, NULL, SDL_RunThread, thread);
    if (retval) {
        return NULL;
    }

    return thread;
}

SDL_Thread *SDL_CreateThreadEx2(SDL_Thread *thread, int (*fn)(void *), void *data, const char *name, const char *log_name)
{
    thread->func = fn;
    thread->data = data;
    strlcpy(thread->name, name, sizeof(thread->name) - 1);
    memset(thread->logname, 0, 32);
    if(log_name != NULL && strlen(log_name) > 0) {
        sprintf(thread->logname, "[%s][%s]", log_name, name);
    } else {
        strlcpy(thread->logname, name, sizeof(thread->name) - 1);
    }
    int retval = pthread_create(&thread->id, NULL, SDL_RunThread, thread);
    if (retval) {
        return NULL;
    }

    return thread;
}
#endif

int SDL_SetThreadPriority(SDL_ThreadPriority priority)
{
    struct sched_param sched;
    int policy;
    pthread_t thread = pthread_self();

    if (pthread_getschedparam(thread, &policy, &sched) < 0) {
        return -1;
    }
    if (priority == SDL_THREAD_PRIORITY_LOW) {
        sched.sched_priority = sched_get_priority_min(policy);
    } else if (priority == SDL_THREAD_PRIORITY_HIGH) {
        sched.sched_priority = sched_get_priority_max(policy);
    } else {
        int min_priority = sched_get_priority_min(policy);
        int max_priority = sched_get_priority_max(policy);
        sched.sched_priority = (min_priority + (max_priority - min_priority) / 2);
    }
    if (pthread_setschedparam(thread, policy, &sched) < 0) {
        return -1;
    }
    return 0;
}

void SDL_WaitThread(SDL_Thread *thread, int *status)
{
    assert(thread);
    if (!thread)
        return;

    pthread_join(thread->id, NULL);

    if (status)
        *status = thread->retval;
}

void SDL_DetachThread(SDL_Thread *thread)
{
    assert(thread);
    if (!thread)
        return;

    pthread_detach(thread->id);
}
}

