#include <stdlib.h>

#include "hacklerAction.h"

hackletAction *createHackletAction(
	int id,
	int delay,
	process* target,
	int action
){

	hackletAction *a = (hackletAction*) malloc(sizeof(hackletAction));

	a->id = id;
	a->delay = delay;
	a->target = target;
	a->action = action;

	return a;
}