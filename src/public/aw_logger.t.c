//
// Created by pengc on 2022/4/25.
//

#include <aw_logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <aw_thread.h>
#include <aw_thread_windows.h>

void writeThread(void* args){
    aw_logger_t * logger = (aw_logger_t*)args;
    for(int i=0; i<10000000; i++){
        AW_LOGGER_INFO(logger, "VALUE IS: %d", i);
        if(i>100){
            aw_logger_level(logger, kAW_LOGGER_OFF);
        }
    }
}

int main(int argc, char** argv){
    aw_logger_ops loggerOps;
    loggerOps.level=kAW_LOGGER_INFO;
    loggerOps.file="d:/aw_logger.log";

    printf("start logger...\n");
    aw_logger_t * logger = aw_logger_new(loggerOps);
    aw_logger_start(logger);
    printf("logger started...\n");

    printf("create thread...\n");
    aw_thread_new_ops threadNewOps;
    threadNewOps.stack_size=512;
    threadNewOps.thread_fn=writeThread;
    threadNewOps.args = logger;
    threadNewOps.platform_ops=NULL;

    aw_thread_t * thread = aw_thread_new(threadNewOps);
    printf("thread started...\n");

    aw_thread_join(thread);
    aw_thread_free(&thread);
    printf("thread freed...\n");

    aw_logger_try_stop(logger);
    aw_logger_free(&logger);
    printf("logger freed...\n");
    return 0;
}