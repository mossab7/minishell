#ifndef EXECUTE_H
#define EXECUTE_H
#include <sys/types.h>
#include <sys/wait.h>
#include <zen.h>
#include <fcntl.h>
int execute_command_line(t_ast *ast);
#endif // EXECUTE_H

