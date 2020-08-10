/****************************************************************************
 * apps/external/zblue/port/subsys/bluetooth/shell/bt_shell.c
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

#include <zephyr.h>
#include <shell/shell.h>

#include <syslog.h>
#include <nuttx/syslog/syslog.h>

#define CONFIG_SHELL_STR_SIZE 32

extern const struct shell_static_entry shell_bt_cmds[];

void shell_hexdump_line(const struct shell *shell, unsigned int offset,
		const uint8_t *data, size_t len)
{
	int i;

	shell_fprintf(shell, SHELL_NORMAL, "%08X: ", offset);

	for (i = 0; i < SHELL_HEXDUMP_BYTES_IN_LINE; i++) {
		if (i > 0 && !(i % 8)) {
			shell_fprintf(shell, SHELL_NORMAL, " ");
		}

		if (i < len) {
			shell_fprintf(shell, SHELL_NORMAL, "%02x ",
					data[i] & 0xFF);
		} else {
			shell_fprintf(shell, SHELL_NORMAL, "   ");
		}
	}

	shell_fprintf(shell, SHELL_NORMAL, "|");

	for (i = 0; i < SHELL_HEXDUMP_BYTES_IN_LINE; i++) {
		if (i > 0 && !(i % 8)) {
			shell_fprintf(shell, SHELL_NORMAL, " ");
		}

		if (i < len) {
			char c = data[i];

			shell_fprintf(shell, SHELL_NORMAL, "%c",
					isprint((int)c) ? c : '.');
		} else {
			shell_fprintf(shell, SHELL_NORMAL, " ");
		}
	}

	shell_print(shell, "|");
}

void shell_hexdump(const struct shell *shell, const uint8_t *data, size_t len)
{
	const uint8_t *p = data;
	size_t line_len;

	while (len) {
		line_len = MIN(len, SHELL_HEXDUMP_BYTES_IN_LINE);

		shell_hexdump_line(shell, p - data, p, line_len);

		len -= line_len;
		p += line_len;
	}
}

void shell_help(const struct shell *shell)
{
	int i;

	syslog(LOG_INFO, "Bluetooth shell commands:\n");

	for (i = 0; shell_bt_cmds[i].syntax; i++) {
		syslog(LOG_INFO, "[%02d]: bt %s : %s\n", i,
				shell_bt_cmds[i].syntax, shell_bt_cmds[i].help);
	}
}

void shell_fprintf(const struct shell *shell, enum shell_vt100_color color,
		const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	nx_vsyslog(LOG_INFO, fmt, &args);
	va_end(args);
}

int main(int argc, FAR char *argv[])
{
	const struct shell_static_entry *cmds = &shell_bt_cmds[0];

	if (argc < 2)
		goto bail;

	argc -= 1;

	for (; cmds->syntax; cmds++)
		if (!strncmp(cmds->syntax, argv[1], CONFIG_SHELL_STR_SIZE))
			break;

	if (cmds->syntax == NULL ||
			cmds->handler == NULL ||
			cmds->args.mandatory > argc)
		goto bail;

	return cmds->handler(NULL, argc, &argv[1]);

bail:
	shell_help(NULL);

	return 0;
}
