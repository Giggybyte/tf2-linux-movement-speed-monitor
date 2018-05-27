/*
 * tf2 velocity montior
 * by giggybyte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/uio.h>
#include <math.h>

void check_for_root(void)
{
    int uid = getuid();
    if (uid != 0)
    {
        printf("I need root to read TF2 memory.\n");
        exit(1);
    }
}

int check_for_tf2(void)
{
    DIR *proc_dir = opendir("/proc/");
    struct dirent *curr_dir;
    int pid = -1;
    while ((curr_dir = readdir(proc_dir)))
    {
        char comm_filename[64] = {'\0'};
        strcat(comm_filename, "/proc/");
        strcat(comm_filename, curr_dir->d_name);
        strcat(comm_filename, "/comm");
        FILE *comm_filepointer = fopen(comm_filename, "r");
        if (comm_filepointer != NULL)
        {
            char *line = NULL;
            size_t len = 0;
            getline(&line, &len, comm_filepointer);
            if (strcmp(line, "hl2_linux\n") == 0)
            {
                pid = atoi(curr_dir->d_name);
                break;
            }
        }
    }
    if (pid == -1)
    {
        printf("tf2 (hl2_linux) was not found. Is it running?\n");
        exit(1);
    }
    return pid;
}

char *read_mem(int pid, void *addr, size_t n)
{
    struct iovec local[1];
    struct iovec remote[1];
    char *buf = malloc(n);
    local[0].iov_base = buf;
    local[0].iov_len = n;
    remote[0].iov_base = addr;
    remote[0].iov_len = n;
    process_vm_readv(pid, local, 1, remote, 1, 0);
    return buf;
}

char *get_client_addr(void)
{
    // Am I a horrible person for using exec like this?
    // Yes.
    // Do I feel like parsing the maps file in C?
    // No.
    system("./get_client_mem.sh");
    FILE *result = fopen("start_address", "r");
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, result);
    return line;
}

int main(void)
{
    check_for_root();
    int tf2_pid = check_for_tf2();
    printf("Found tf2 pid: %d\n", tf2_pid);
    char *start = get_client_addr();
    printf("Found starting address of client.so: %s", start);
    sleep(3);
    long int start_addr = strtol(start, NULL, 16);
    union
    {
        float f;
        unsigned char c[4];
    } x, y;
    for (;;)
    {
        char *x_char = read_mem(tf2_pid, (void *) start_addr+0x218B844, sizeof(float));
        char *y_char = read_mem(tf2_pid, (void *) start_addr+0x218B848, sizeof(float));
        for (int i = 0; i < 4; i++)
        {
            x.c[i] = x_char[i];
            y.c[i] = y_char[i];
        }
        //printf("  x.f: %f\n", x.f);
        //printf("  y.f: %f\n", y.f);
        //printf("x.f^2: %f\n", x.f*x.f);
        //printf("y.f^2: %f\n", y.f*y.f);
        printf("%f\n", sqrtf( (x.f*x.f) + (y.f*y.f) ));
    } 
}
