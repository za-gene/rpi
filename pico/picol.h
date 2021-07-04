#pragma once
#include <stdio.h>

// forward declarations
struct picolCmd;
struct picolCallFrame;

struct picolInterp {
	int level; /* Level of nesting */
	struct picolCallFrame *callframe;
	struct picolCmd *commands;
	char *result;
};

//struct picolInterp; /* forward declaration */

typedef int (*picolCmdFunc)(struct picolInterp *i, int argc, char **argv, void *privdata);

void picolInitInterp(struct picolInterp *i);
void picolRegisterCoreCommands(struct picolInterp *i);
char* picolEchoedFgets(char* s, int size, FILE *stream);
int picolEval(struct picolInterp *i, char *t);
int picolRegisterCommand(struct picolInterp *i, char *name, picolCmdFunc f, void *privdata);
