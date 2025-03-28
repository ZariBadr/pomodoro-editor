#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// SIGINT  - 2  CTRL+C
// SIGKILL - 9  Kill process
// SIGALRM - 14 Timer signal
// SIGTERM - 15 Ask process to terminate cleanly

pid_t child_pid = -1;

void handle_alarm(int sig) {
    if (child_pid > 0) {
        printf("\n⏰ Time is Up! Exiting the editor...\n");
        kill(child_pid, SIGTERM); // You can use SIGKILL for forced kill
    }
}

int main() {
    struct sigaction act;
    act.sa_handler = handle_alarm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child: open the nano editor
        execlp("nano", "nano", "my_notes.txt", NULL);
        perror("execlp failed");
        exit(1);
    } else if (pid > 0) {
        // Parent: set timer and wait
        child_pid = pid;
        printf("🍅 Pomodoro started! You have a few minutes to focus...\n");
        alarm(30); // 30 seconds for testing

        wait(NULL); // Wait for child (nano) to finish
        printf("📝 Editor closed.\n");
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}

