/*-
 * Copyright (c) 2011 Robert N. M. Watson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */

#include <sys/types.h>

#include <stdio.h>

#include <tesla/tesla_util.h>
#include <tesla/tesla_state.h>

#include "mwc_defs.h"
#include "syscalls.h"

/*
 * Test program for the 'mwc' assertion.  Run a number of times with various
 * event sequences and see how it works out.
 */

static void
test(int scope)
{

	printf("\nScope: %s\n", scope == TESLA_SCOPE_GLOBAL ? "global" :
	    "per-thread");

	mwc_init(scope);
	mwc_setaction_debug();	/* Use printf(), not assert(). */

	printf("Simulating syscall without check or use; should pass\n");
	syscall(NOOP);

	printf("Simulating syscall with successful check, no use; should "
	    "pass\n");
	syscall(CHECK_ONLY);

	printf("Simulating syscall, unsuccessful check, no use; should "
	    "pass\n");
	syscall(CHECK_ASSERT);

	printf("Simulating syscall, unsuccessful check, use; should fail\n");
	syscall(BADCHECK_ASSERT);

	printf("Simulating syscall, use without check; should fail\n");
	syscall(ASSERT_ONLY);

	printf("Simulating syscall, check on wrong vnode, use; should "
	    "fail\n");
	syscall(WRONG_VNODE);

	printf("Simulating syscall, use with check from last run; should "
	    "fail\n");
	syscall(LASTRULE);

	printf("Simulating syscall, checking two vnodes and then using "
	    "them; should pass\n");
	syscall(TWOPASS);

	printf("Using same cred twice to make sure both keys used; "
	    "should fail.\n");
	syscall(DOUBLECRED);

	printf("Using same vnode twice to make sure both keys used; "
	    "should fail.\n");
	syscall(DOUBLEVNODE);

	mwc_destroy();
}

int
main(int argc, char *argv[])
{

	test(TESLA_SCOPE_GLOBAL);
	test(TESLA_SCOPE_PERTHREAD);

	return (0);
}
