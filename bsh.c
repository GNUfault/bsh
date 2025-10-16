/*
 * bsh - Basic UNIX-like shell
 * Copyright (C) 2025 Connor Thomson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    signal(SIGINT, SIG_IGN);
    char *line = 0;
    size_t len = 0;

    while (1) {
        if (isatty(0)) {
            char cwd[256], prompt[256];
            getcwd(cwd, sizeof(cwd));
            if (!strcmp(cwd, getenv("HOME"))) {
                snprintf(prompt, sizeof(prompt), "~%s", geteuid() ? "$ " : "# ");
            } else {
                snprintf(prompt, sizeof(prompt), "%s%s", cwd, geteuid() ? "$ " : "# ");
            }
            printf("%s", prompt);
            fflush(stdout);
        }

        if (getline(&line, &len, stdin) < 0) break;
        line[strcspn(line, "\n")] = 0;
        if (!*line) continue;

        char *a[64]; int i = 0;
        for (char *t = strtok(line, " \t"); t && i < 63; t = strtok(0, " \t"))
            a[i++] = t;
        a[i] = 0;

        if (!strcmp(a[0], "cd")) {
            const char *d = (a[1] && *a[1]) ? a[1] : getenv("HOME");
            if (!d || chdir(d)) perror("cd");
            continue;
        }

        if (!strcmp(a[0], "help")) {
            puts("bsh - Barebones Shell by Connor Thomson");
            puts(" Built in commands:");
            puts("  cd - Change directory");
            puts("  help - Get help with bsh");
            continue;
        }

        if (!fork()) {
            signal(SIGINT, SIG_DFL);
            char cmd_path[256];
            snprintf(cmd_path, sizeof(cmd_path), "/bin/%s", a[0]);
            execv(cmd_path, a);
            perror("exec");
            _exit(1);
        }

        wait(0);
    }
}
