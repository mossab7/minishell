#ifndef EXECUTE_H
#define EXECUTE_H
#include <sys/types.h>
#include <sys/wait.h>
#include <zen.h>
#include <fcntl.h>
int execute_ast(t_ast *node, t_env *env);
#endif // EXECUTE_H

