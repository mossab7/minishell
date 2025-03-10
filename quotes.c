/*
CONSTS:
	NONE    = 0
	SQOUTED = 1
	DQOUTED = 2

- [1] = Make a program that takes in a content and produces a mask depending on the qoutation of that content.


Example:
	./program "Hello world"
	out:
		Hello world
		00000000000
		Because the content is neither double qouted nor single qouted

	./program "'Hello' world"
	out:
		Hello world
		111110000
		Because the content is single qouted only but only the first word is.. also we strip the qoutes..
	./program "\"'Hello' world\""
	out:
		'Hello' world
		2222222222222
		Because the content is double qouted from the start to the end. also the single qoutes are escaped because they are inside double qoutes so we dont strip them
		but the double qoutes are stripped tho
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define SCAP 1024 * 4
#define SQ '\''
#define DQ '\"'
#define PT '^'
typedef unsigned char ubyte;
typedef char byte;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct range
{
	size_t start;
	size_t end;
}	range;

typedef struct stack {
	u8 items[SCAP];
	u64 size;
} stack;

typedef stack array;

void	stack_push(stack *s, u8 what)
{
	assert((s->size < SCAP) && "Bumped the stack");
	s->items[s->size++] = what;
}

#define	array_push stack_push
int main(int ac, char **av)
{
	if (ac < 2)
		return (1);

	char *content = av[1];
	//printf("content: |%s|\n", content);
	char *final = malloc(strlen(content));
	range r = {0};
	stack dstack = {0};
	stack sstack = {0};
	array mask = {0};

	// FIRST: Capture everything that is not quoted.
	for (r.end = (strlen(content) - 1); r.start <= r.end;)
	{
		if (content[r.start] != SQ && content[r.start] != DQ && !isspace(content[r.start]))
		{
			final[r.start] = content[r.start];
			r.start++;
			array_push(&mask, 0);
			continue ;
		}
		//printf("Stopped at %zu -> %c\n", r.start, content[r.start]);
		break ;
	}
	final[r.start] = 0;
	u8 inside_what = content[r.start];
	size_t idx = r.start;
	u8 stat = (inside_what == SQ) ? 1 : 2;
	r.end = (strlen(content) - 1);
	while (content[r.end] != inside_what && r.end > r.start)
		r.end--;
	for (; r.start <= r.end;)
	{
		if (inside_what == SQ)
		{
			if (content[r.start] == DQ)
			{
				final[idx++] = content[r.start++];
				array_push(&mask, stat);
			}
			else
			{
				if (content[r.start] != SQ)
				{
					final[idx++] = content[r.start];
					array_push(&mask, stat);
				}
				r.start++;
			}
		}
		else if (inside_what == DQ)
		{
			if (content[r.start] == SQ)
			{
				final[idx++] = content[r.start++];
				array_push(&mask, stat);
			}
			else
			{
				if (content[r.start] != DQ)
				{
					final[idx++] = content[r.start];
					array_push(&mask, stat);
				}
				r.start++;
			}
		}
	}
	// printf("# %s\n", final);
	// printf("  ");
	// for (size_t i = 0; i < mask.size; i++)
	// 	printf("%u", mask.items[i]);
	// printf("\n");
	// puts("");
	return (0);
}
