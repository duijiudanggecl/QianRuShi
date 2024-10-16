/*********************************************************************************
 *      Copyright:  
 *                  
 *
 *       Filename:  
 *    Description:  
 *                 
 *        Version: 
 *         Author:  swiftchen
 *      ChangeLog:  
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#include "logger.h"

/*
 * Program name variable is provided by the libc
 */
extern const char* __progname;
#define PROGRAM_NAME __progname

#define FILENAME_LEN           32
#define LOG_ROLLBACK_SIZE      4    /* Default rollback log size  */
#define LOG_ROLLBACK_NONE      0      /* Set rollback size to 0 will not rollback  */

static unsigned long log_rollback_size = LOG_ROLLBACK_SIZE*1024;

/*
 * Logger internal sctructure
 */
typedef struct logger_s {
    FILE   *fp;
    int     loglevel;
    int     use_stdout;
    char    file[FILENAME_LEN];
} logger_t;

static struct logger_s g_logger;

static const char* LOG_LEVELS[] = {
    LOG_STRING_ERROR,
    LOG_STRING_WARN,
    LOG_STRING_INFO,
    LOG_STRING_DEBUG
};

/*
 * initial logger system
 */
int logger_init(char *filename, int loglevel)
{
    logger_term();

    g_logger.loglevel = loglevel>LOG_LEVEL_MAX ? LOG_LEVEL_MAX : loglevel;

    /* $filename is NULL or match "stdout"  will use standard output */
    if( !filename || !strcasecmp(filename, "stdout"))
    {
        g_logger.use_stdout = 1;
        g_logger.fp = stderr;
    }
    else
    {
        g_logger.use_stdout = 0;
        g_logger.fp = fopen(filename, "a");
        if( !g_logger.fp )
        {
            fprintf(stderr, "Failed to open file '%s': %s", filename, strerror(errno));
            return -1;
        }
        strcpy(g_logger.file, filename);
    }

    return 0;
}

/*
 * terminate logger system
 */
void logger_term(void)
{
    if( !g_logger.fp )
    {
        return ;
    }

    if( !g_logger.use_stdout )
    {
        fclose(g_logger.fp);
    }

    g_logger.use_stdout = 0;
    g_logger.fp = NULL;

    return ;
}

/*
 * Logging functions
 */
void log_generic(const int level, const char* format, va_list args)
{
    char message[256];
    struct tm* current_tm;
    time_t time_now;

    vsprintf(message, format, args);

    time(&time_now);
    current_tm = localtime(&time_now);

    int res = fprintf(g_logger.fp,
            "%s : %02i:%02i:%02i [%s] %s\n"
                , PROGRAM_NAME
                , current_tm->tm_hour
                , current_tm->tm_min
                , current_tm->tm_sec
                , LOG_LEVELS[level]
                , message );

    fflush(g_logger.fp);
}

void log_error(char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_ERROR, format, args);
    va_end(args);
    check_and_rollback();
}

void log_warn(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_WARN) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_WARN, format, args);
    va_end(args);
    check_and_rollback();
}

void log_info(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_INFO) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_INFO, format, args);
    va_end(args);
    check_and_rollback();
}

void log_debug(char *format, ...)
{
    if (g_logger.loglevel < LOG_LEVEL_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
    check_and_rollback();
}

static void check_and_rollback(void)
{
    if( !g_logger.fp )
        return ;
 
    if (log_rollback_size != LOG_ROLLBACK_NONE)
    {
        long curOffset = ftell(g_logger.fp);
 
        if ((curOffset != -1) && (curOffset >= log_rollback_size))
        {
            char cmd[512];
            snprintf(cmd, sizeof(cmd), "cp -f %s %s.roll", g_logger.file, g_logger.file);
            system(cmd);
 
            if (-1 == fseek(g_logger.fp, 0L, SEEK_SET))
                fprintf(g_logger.fp, "log rollback fseek failed \n");
 
            rewind(g_logger.fp);
            truncate(g_logger.file, 0);
        }
    }
}

