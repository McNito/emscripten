#include "pthread_impl.h"
#include "syscall.h"

#ifndef __EMSCRIPTEN__
static pthread_once_t check_robust_once;
static int check_robust_result;

static void check_robust()
{
	void *p;
	size_t l;
	check_robust_result = -__syscall(SYS_get_robust_list, 0, &p, &l);
}
#endif

int pthread_mutexattr_setrobust(pthread_mutexattr_t *a, int robust)
{
	if (robust > 1U) return EINVAL;
	if (robust) {
#ifndef __EMSCRIPTEN__
		pthread_once(&check_robust_once, check_robust);
		if (check_robust_result) return check_robust_result;
#endif
		a->__attr |= 4;
		return 0;
	}
	a->__attr &= ~4;
	return 0;
}
