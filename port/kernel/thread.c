/****************************************************************************
 * apps/external/zblue/port/kernel/thread.c
 *
 *   Copyright (C) 2020 Xiaomi InC. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <kernel.h>
#include <inttypes.h>
#include <sys/prctl.h>

#include <nuttx/kmalloc.h>

typedef struct
{
	void *argv[4];
} k_thread_main_t;

static int nxthread_create(FAR const char *name, uint8_t ttype, int priority,
		FAR void *stack, int stack_size, main_t entry, FAR char * const argv[])
{
	FAR struct task_tcb_s *ttcb;
	pid_t pid;
	int ret;

	ttcb = (FAR struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!ttcb)
		return -ENOMEM;

	ttcb->cmn.flags = ttype;

	ret = nxtask_init(ttcb, name, priority, NULL, stack_size, entry, argv);
	if (ret < OK) {
		kmm_free(ttcb);
		return ret;
	}

	pid = ttcb->cmn.pid;

	nxtask_activate(&ttcb->cmn);

	return (int)pid;
}

k_tid_t k_current_get(void)
{
	extern struct k_work_q k_sys_work_q;
	pid_t pid = getpid();

	if (pid == k_sys_work_q.thread.pid)
		return &k_sys_work_q.thread;

	return (k_tid_t)(intptr_t)pid;
}

int k_thread_name_set(struct k_thread *thread, const char *value)
{
	return prctl(PR_SET_NAME_EXT, value, thread->pid);
}

static int k_thread_main(int argc, FAR char *argv[])
{
	k_thread_main_t *_main;
	void *_argv[4];

	_main = (k_thread_main_t *)((uintptr_t)strtoul(argv[2], NULL, 0));
	if (_main == NULL)
		return -EINVAL;

	memcpy(_argv, _main->argv, sizeof(_argv));

	kmm_free(_main);

	((k_thread_entry_t)_argv[0])(_argv[1], _argv[2], _argv[3]);
	return 0;
}

k_tid_t k_thread_create(struct k_thread *new_thread,
		k_thread_stack_t *stack,
		size_t stack_size, k_thread_entry_t entry,
		void *p1, void *p2, void *p3,
		int prio, uint32_t options, k_timeout_t delay)
{
	k_thread_main_t *_main;
	void *argv[3];
	char arg1[16];
	int ret;

	_main = kmm_malloc(sizeof(*_main));
	if (_main == NULL)
		return (k_tid_t)(intptr_t)-ENOMEM;

	_main->argv[0] = entry;
	_main->argv[1] = p1;
	_main->argv[2] = p2;
	_main->argv[3] = p3;

	snprintf(arg1, 16, "0x%" PRIxPTR, (uintptr_t)_main);

	argv[0] = "";
	argv[1] = arg1;
	argv[2] = NULL;

	ret = nxthread_create("zephyr", TCB_FLAG_TTYPE_KERNEL, prio,
			stack, stack_size, k_thread_main, (FAR char * const *)argv);
	if (ret < 0)
		return (k_tid_t)-1;

	new_thread->pid = ret;

	return (k_tid_t)ret;
}

int k_is_in_isr(void)
{
	return false;
}
