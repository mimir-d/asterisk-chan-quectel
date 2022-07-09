#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include "export.h"

EXPORT_DECL int quectel_logger_alloc();
EXPORT_DECL void quectel_logger_dealloc();

EXPORT_DECL void quectel_enable_logger();
EXPORT_DECL void quectel_disable_logger();

EXPORT_DECL void quectel_logger_write(const char* format, ...);

#endif

