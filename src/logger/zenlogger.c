#include <zen.h>

int zen_elog(const char *fmt, ...)
{
    va_list args;
	va_start(args, fmt);
	ft_fprintf(STDERR_FILENO, "zen: ");
	return (ft_vfprintf(STDERR_FILENO, fmt, args));
}
