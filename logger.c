
#include <asterisk.h>
#include <asterisk/logger.h>
#include <asterisk/astobj2.h>

#include "logger.h"

struct quectel_logger {
	unsigned int enabled:1;
};

static struct quectel_logger* default_logger;

static void quectel_logger_dtor(void* obj)
{
	struct quectel_logger* logger = obj;
	logger = logger;
	// nothing
}

EXPORT_DEF int quectel_logger_alloc()
{
	default_logger = ao2_alloc_options(
		sizeof(struct quectel_logger),
		quectel_logger_dtor,
		AO2_ALLOC_OPT_LOCK_RWLOCK
	);
	if (!default_logger)
		return 0;

	// default_logger->enabled = 1;
	return 1;
}

EXPORT_DEF void quectel_logger_dealloc()
{
	ao2_cleanup(default_logger);
	default_logger = NULL;
}

EXPORT_DEF void quectel_enable_logger()
{
	ao2_wrlock(default_logger);
	default_logger->enabled = 1;
	ao2_unlock(default_logger);
}

EXPORT_DEF void quectel_disable_logger()
{
	ao2_wrlock(default_logger);
	default_logger->enabled = 0;
	ao2_unlock(default_logger);
}

EXPORT_DEF void quectel_logger_write(const char* format, ...)
{
	ao2_rdlock(default_logger);
	if (!default_logger->enabled) {
		return;
	}
	ao2_unlock(default_logger);
	
	#define BUFSIZE 10240
	static char buf[BUFSIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, BUFSIZE, format, args);
	va_end(args);
	#undef BUFSIZE

	ast_verbose(buf);
}

