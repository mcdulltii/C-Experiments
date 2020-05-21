#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int
main(int argc, char **argv)
{
    pid_t pid;
    int status;
    posix_spawnattr_t attr;

    status = posix_spawnattr_init(&attr);
    if (status != 0) { 
        perror("can't init spawnattr"); 
        exit(status); 
    }

    status = posix_spawnattr_setflags(&attr, POSIX_SPAWN_START_SUSPENDED);
    if (status != 0) { 
        perror("can't set flags"); 
        exit(status); 
    }

    status = posix_spawn(&pid, "./hello", NULL, &attr, NULL, NULL);
    if (status != 0) {
        printf("posix_spawn: %s\n", strerror(status));
        exit(status);
    }

    printf("Child pid: %i\n", pid);
            
    sleep(3);
    
    kill(pid, SIGCONT);

    return 0;
}

