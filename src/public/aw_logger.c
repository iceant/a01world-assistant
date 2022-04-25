//
// Created by pengc on 2022/4/24.
//

#include <aw_logger.h>
#include <stdio.h>
#include <aw_mutex.h>
#include <aw_cond.h>
#include <aw_memory.h>
#include <aw_vector.h>
#include <assert.h>
#include <string.h>
#include <aw_thread.h>
#include <aw_thread_windows.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#if defined(_WIN32)

#include <windows/gettimeofday.h>

#endif

////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_logger_buffer_t {
    aw_size_t capacity;
    aw_size_t size;
    char *buffer;
} aw_logger_buffer_t;

struct aw_logger_t {
    eAwLoggerLevel level;
    aw_int8_t status; // 0:init, 1:running, 2:try_stop, 3:stopped
    FILE *d_file_p;
    aw_vector_t *d_buffers_p;
    aw_logger_buffer_t *d_currentBuffer_p;
    aw_logger_buffer_t *d_nextBuffer_p;
    aw_mutex_t *d_mutex_p;
    aw_cond_t *d_cond_p;
    aw_cond_t *d_stopCond_p;
    aw_thread_t *d_thread_p;
};

////////////////////////////////////////////////////////////////////////////////
////
#define AW_LOGGER_BUFFER_SIZE 4096
#define AW_LOGGER_FLUSH_INTERVAL 3000
#define AW_LOGGER_BUCKET_SIZE 4

////////////////////////////////////////////////////////////////////////////////
////
static aw_logger_buffer_t *aw_logger_buffer_new(aw_size_t capacity) {
    aw_logger_buffer_t *buffer;

    assert(capacity > 0);

    buffer = AW_ALLOC(sizeof(*buffer) + capacity);
    assert(buffer);
    memset(buffer, 0, capacity);

    buffer->capacity = capacity;
    buffer->size = 0;
    buffer->buffer = (char *) (buffer + 1);

    return buffer;
}

static void aw_logger_buffer_free(aw_logger_buffer_t **pBuffer) {
    assert(pBuffer);
    assert(*pBuffer);
    AW_FREE(*pBuffer);
}

static int aw_logger_buffer_append(aw_logger_buffer_t *buffer, void* data, aw_size_t size){
    assert(buffer);
    assert(data);
    assert(size>0);
    if(buffer->size+size<buffer->capacity) {
        memcpy(buffer->buffer + buffer->size, data, size);
        buffer->size+=size;
    }else{
        return -1;
    }
}

static void aw_logger_buffer_reset(aw_logger_buffer_t *buffer) {
    assert(buffer);
    buffer->size = 0;
}

static aw_size_t aw_logger_buffer_available(aw_logger_buffer_t *buffer) {
    assert(buffer);
    return buffer->capacity - buffer->size;
}

static void aw_logger_append(aw_logger_t *logger, const char *data, aw_size_t len) {
    aw_mutex_lock(logger->d_mutex_p);
    if ((logger->d_currentBuffer_p->size + len) < logger->d_currentBuffer_p->capacity) {
        memcpy(logger->d_currentBuffer_p->buffer+logger->d_currentBuffer_p->size, data, len);
        logger->d_currentBuffer_p->size+=len;
    } else {
        aw_vector_push_back(logger->d_buffers_p, logger->d_currentBuffer_p);
        if (logger->d_nextBuffer_p) {
            logger->d_currentBuffer_p = logger->d_nextBuffer_p;
            logger->d_nextBuffer_p = NULL;
        } else {
            logger->d_currentBuffer_p = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
        }
        memcpy(logger->d_currentBuffer_p->buffer+logger->d_currentBuffer_p->size, data, len);
        logger->d_currentBuffer_p->size+=len;
        aw_cond_signal(logger->d_cond_p);
    }
    aw_mutex_unlock(logger->d_mutex_p);
}

static int aw_vasprintf(char **strp, const char *fmt, va_list ap) {
    aw_size_t r = -1, size;

    va_list ap_copy;
    va_copy(ap_copy, ap);
    size = vsnprintf(0, 0, fmt, ap_copy);
    va_end(ap_copy);

    if ((size >= 0) && (size < AW_SIZE_MAX)) {
        *strp = (char *) AW_ALLOC(size + 1); //+1 for null
        if (*strp) {
            r = vsnprintf(*strp, size + 1, fmt, ap);  //+1 for null

            if ((r < 0) || (r > size)) {
                AW_FREE(*strp);
                r = -1;
            }
        }
    } else { *strp = 0; }

    return (r);
}

static int aw_asprintf(char **strp, const char *fmt, ...) {
    aw_size_t r = -1, size;

    va_list args;
    va_start(args, fmt);
    size = vsnprintf(0, 0, fmt, args);
    va_end(args);

    if ((size >= 0) && (size < AW_SIZE_MAX)) {
        *strp = (char *) AW_ALLOC(size + 1); //+1 for null
        if (*strp) {
            va_start(args, fmt);
            r = vsnprintf(*strp, size + 1, fmt, args);  //+1 for null
            va_end(args);

            if ((r < 0) || (r > size)) {
                AW_FREE(*strp);
                r = -1;
            }
        }
    } else { *strp = 0; }

    return (r);
}

////////////////////////////////////////////////////////////////////////////////
//// THREAD
void aw_logger_thread_fn(void *args) {

    aw_logger_t *logger = (aw_logger_t *) args;
    logger->status = 1; // running


    aw_logger_buffer_t *newBuffer1 = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
    aw_logger_buffer_t *newBuffer2 = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
    aw_vector_t *buffersToWrite = aw_vector_new(AW_LOGGER_BUCKET_SIZE);

    while (logger->status < 2 /*try_stop*/) {
        aw_mutex_lock(logger->d_mutex_p);
        if (aw_vector_isEmpty(logger->d_buffers_p)) {
            aw_cond_waitForMilliSecond(logger->d_cond_p, logger->d_mutex_p, AW_LOGGER_FLUSH_INTERVAL);
        }
        aw_vector_push_back(logger->d_buffers_p, logger->d_currentBuffer_p);
        logger->d_currentBuffer_p = newBuffer1;
        newBuffer1 = NULL;

        // SWAP
        aw_vector_t *buffersTemp = buffersToWrite;
        buffersToWrite = logger->d_buffers_p;
        logger->d_buffers_p = buffersTemp;

        if (!logger->d_nextBuffer_p) {
            logger->d_nextBuffer_p = newBuffer2;
            newBuffer2 = NULL;
        }
        aw_mutex_unlock(logger->d_mutex_p);

        // output buffersToWrite to file
        for (aw_size_t i = 0; i < aw_vector_size(buffersToWrite); i++) {
            aw_logger_buffer_t *buffer = aw_vector_get(buffersToWrite, i);
            fwrite(buffer->buffer, buffer->size, 1, logger->d_file_p);
            buffer->size = 0;
        }
        fflush(logger->d_file_p);

        // re-fill newBuffer1 and newBuffer2
        if (newBuffer1 == NULL) {
            if (!aw_vector_isEmpty(buffersToWrite)) {
                newBuffer1 = aw_vector_pop(buffersToWrite);
            } else {
                newBuffer1 = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
            }
        }
        if (newBuffer2 == NULL) {
            if (!aw_vector_isEmpty(buffersToWrite)) {
                newBuffer1 = aw_vector_pop(buffersToWrite);
            } else {
                newBuffer1 = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
            }
        }
    }

    // flush output
    for (aw_size_t i = 0; i < aw_vector_size(buffersToWrite); i++) {
        aw_logger_buffer_t *buffer = aw_vector_get(buffersToWrite, i);
        fwrite(buffer->buffer, buffer->size, 1, logger->d_file_p);
        aw_logger_buffer_free(&buffer);
    }
    fflush(logger->d_file_p);
    aw_vector_free(&buffersToWrite);

    // flush latest message before stop
    aw_mutex_lock(logger->d_mutex_p);
    // latest write buffer
    aw_vector_push_back(logger->d_buffers_p, logger->d_currentBuffer_p);
    logger->d_currentBuffer_p = NULL;
    buffersToWrite = logger->d_buffers_p;
    logger->d_buffers_p = NULL;
    aw_mutex_unlock(logger->d_mutex_p);

    if (buffersToWrite) {
        for (aw_size_t i = 0; i < aw_vector_size(buffersToWrite); i++) {
            aw_logger_buffer_t *buffer = aw_vector_get(buffersToWrite, i);
            fwrite(buffer->buffer, buffer->size, 1, logger->d_file_p);
            aw_logger_buffer_free(&buffer);
        }
        fflush(logger->d_file_p);
        aw_vector_free(&buffersToWrite);
    }

    if (newBuffer1!=NULL) {
        aw_logger_buffer_free(&newBuffer1);
    }
    if (newBuffer2!=NULL) {
        aw_logger_buffer_free(&newBuffer2);
    }

    logger->status = 3; // stopped
    aw_cond_broadcast(logger->d_stopCond_p);
}


////////////////////////////////////////////////////////////////////////////////
////
aw_logger_t *aw_logger_new(aw_logger_ops ops) {
    aw_logger_t *p;
    AW_NEW(p);
    assert(p);

    p->level = ops.level;
    p->d_file_p = fopen(ops.file, "ab+");
    p->d_buffers_p = aw_vector_new(AW_LOGGER_BUCKET_SIZE);
    p->d_currentBuffer_p = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);
    p->d_nextBuffer_p = aw_logger_buffer_new(AW_LOGGER_BUFFER_SIZE);

    p->d_mutex_p = aw_mutex_new();
    p->d_cond_p = aw_cond_new();
    p->d_stopCond_p = aw_cond_new();

    p->status = 0; // NOT START
    p->d_thread_p = NULL;

    return p;
}

void aw_logger_free(aw_logger_t **pLogger) {
    assert(pLogger && *pLogger);

    aw_logger_t *logger = *pLogger;

    aw_mutex_lock(logger->d_mutex_p);
    while (logger->status != 3) {
        aw_cond_wait(logger->d_stopCond_p, logger->d_mutex_p);
    }
    aw_mutex_unlock(logger->d_mutex_p);

    aw_thread_join(logger->d_thread_p);

    if (logger->d_nextBuffer_p) {
        aw_logger_buffer_free(&logger->d_nextBuffer_p);
    }
    if (logger->d_currentBuffer_p) {
        aw_logger_buffer_free(&logger->d_currentBuffer_p);
    }
    if(logger->d_buffers_p) {
        aw_vector_free(&logger->d_buffers_p);
    }

    fclose(logger->d_file_p);
    logger->level = kAW_LOGGER_OFF;


    aw_cond_free(&logger->d_stopCond_p);
    aw_cond_free(&logger->d_cond_p);
    aw_mutex_free(&logger->d_mutex_p);

    AW_FREE(*pLogger);
}

eAwLoggerLevel aw_logger_level(aw_logger_t *logger, eAwLoggerLevel newLevel) {
    assert(logger);
    eAwLoggerLevel oldLevel = logger->level;
    logger->level = newLevel;
    return oldLevel;
}

#define AW_LOGGER_FMT ""

// 20210603T08:02:45.125770Z 23241(线程ID) INFO [方法]文件名:行号 - 正文
void aw_logger_log(aw_logger_t *logger, eAwLoggerLevel level, const char *function, const char *file, int line,
                   const char *fmt, ...) {
    aw_size_t bodyBufferSize;
    char *bodyBuffer;
    aw_size_t logBufferSize;
    char *logBuffer;
    aw_size_t result;

    // timestamp
    struct timeval tv;
    struct tm *tm_info;
//    aw_uint32_t millisec;
    char timeBuffer[20]={0};

    assert(logger);
    if (logger->level < level) return;

    va_list args;
    va_start(args, fmt);
    bodyBufferSize = aw_vasprintf(&bodyBuffer, fmt, args);
    va_end(args);

    if (bodyBufferSize<0 || bodyBuffer == NULL) return;

    // timestamp
    time_t tt;
    tt = time(NULL);
    tm_info = localtime(&tt);
    gettimeofday(&tv, NULL);
//    millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
//    if (millisec>=1000) { // Allow for rounding up to nearest second
//        millisec -=1000;
//        tv.tv_sec++;
//    }
    strftime(timeBuffer, 20, "%Y-%m-%dT%H:%M:%S", tm_info);

    aw_uint32_t threadId = aw_thread_self_id();

    // 20210603T08:02:45.125770Z 23241(线程ID) INFO [方法]文件名:行号 - 正文

    const char *LEVEL = (logger->level == kAW_LOGGER_TRACE) ? "TRACE" :
            ((logger->level == kAW_LOGGER_DEBUG) ? "DEBUG" :
                ((logger->level == kAW_LOGGER_INFO) ? "INFO" :
                    ((logger->level == kAW_LOGGER_WARN) ? "WARN":
                        ((logger->level ==kAW_LOGGER_ERROR)? "ERROR":
                            ((logger->level ==kAW_LOGGER_FATAL)? "FATAL": "UNKNOWN") ))));

    logBufferSize = aw_asprintf(&logBuffer, "%s.%dZ %06d %s [%s]%s:%d - %s\n",
                                 timeBuffer,
                                 tv.tv_usec,
                                 threadId,
                                 LEVEL,
                                 function,
                                 file,
                                 line,
                                 bodyBuffer);

    aw_logger_append(logger, logBuffer, logBufferSize);
}

void aw_logger_start(aw_logger_t *logger) {
    assert(logger);
    aw_thread_new_ops ops;
    ops.stack_size = 4096;
    ops.thread_fn = aw_logger_thread_fn;
    ops.args=logger;

    aw_thread_windows_ops windowsOps;
    windowsOps.initflag = 0;
    windowsOps.security = NULL;
    windowsOps.thrdaddr=NULL;
    ops.platform_ops = &windowsOps;

    logger->d_thread_p = aw_thread_new(ops);
    assert(logger->d_thread_p);
}

void aw_logger_try_stop(aw_logger_t *logger) {
    assert(logger);
    logger->status = 2; // try stop
}