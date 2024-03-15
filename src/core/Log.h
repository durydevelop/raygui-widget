
#ifndef DGUIWIDGET_LOG_H
#define DGUIWIDGET_LOG_H

enum LogTag { DLOG_INFO, DLOG_ERROR, DLOG_DEBUG, DLOG_TRACE, DLOG_WARNING };
void Log(LogTag, const char *, ...);

#endif