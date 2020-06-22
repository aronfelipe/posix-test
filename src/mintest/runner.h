
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
    int id;
    char name[50];
    double start;
    int out;
} process;

int main(int argc, char** argv) {

    int counter;
    int specific = -1;
    int sv_stdout = dup(1);

    printf("Program name is: %s", argv[0]); 

    if(argc == 1) {
        printf("\nAll tests will be executed"); 
    }
    else if(argc >= 2) { 
        for(counter = 0;counter < argc; counter++)  {
            if (counter == 1) {
                printf("\nTest to be executed: %s \n", argv[counter]);
            }
            for (int i = 0; i < sizeof(all_tests)/sizeof(test_data); i++) {
                if (strcmp(all_tests[i].name, argv[counter]) == 0) {
                    specific = i;
                }
            }
        }
    }

    if (specific != -1) {

        int size = 1;
        printf("Running %d test:\n", size);
        printf("=====================\n\n");
        int pass_count = 0;
        
	    clock_t start, end;
        start = clock();

        char filename[50];
        sprintf(filename, "%s", all_tests[specific].name);
        int out = open(filename, O_WRONLY | O_CREAT | O_APPEND , 0700);

        if (fork() == 0) {
            alarm(2);
            dup2(out, 1);

            if (all_tests[specific].function() >= 0) {
                return 0;
            } else {
                return 1;
            }
        }

        int st;
        wait(&st);
        end = clock();
        double seconds = ((double) (end - start)) / CLOCKS_PER_SEC;

        if (WIFEXITED(st)) {
            if (WEXITSTATUS(st) == 0) {
                dup2(out, 1);
                printf("%s: [PASS] %f [s] \n", all_tests[specific].name, seconds);
                pass_count++;
            } else if (WEXITSTATUS(st) == 1) {
                dup2(out, 1);
                printf("%s: [FAIL] %f [s] \n", all_tests[specific].name, seconds);
            } 
        } else if (WIFSIGNALED(st)) {
            if (strcmp(strsignal(WTERMSIG(st)), "Alarm clock") == 0) {
                dup2(out, 1);
                printf("%s: [ERRO] [TIME] %f [s] \n", all_tests[specific].name, seconds);
            } else {
                dup2(out, 1);
                printf("%s: [ERRO] %s %f [s] \n", all_tests[specific].name, strsignal(WTERMSIG(st)), seconds);
            }
        }
        
        dup2(sv_stdout, 1);
        int c;
        FILE *file;
        file = fopen(all_tests[specific].name, "r");
        if (file) {
            while ((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
        }
        remove(all_tests[specific].name);

        printf("%d/%d tests passed\n", pass_count, size);
        printf("\n\n=====================\n");

    } else {

        int size = sizeof(all_tests)/sizeof(test_data);
        process *dictionary = malloc(size * sizeof(process));
        printf("\nRunning %d tests:\n", size);
        printf("=====================\n\n");
        int pass_count = 0;
	    clock_t start, end;

        for (int i = 0; i < size; i++) {
            pid_t child = fork();
            process action;
            start = clock();
            action.start = start;
            action.id = child;
            sprintf(action.name, "%s", all_tests[i].name);
            char filename[50];
            sprintf(filename, "%s", all_tests[i].name);
            action.out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0700);
            dictionary[i] = action;
            
            if (child == 0) {
                alarm(2);
                dup2(action.out, 1);
                if (all_tests[i].function() >= 0) {
                    return 0;
                } else {
                    return 1;

                }
            }
        }

        for (int i = 0; i < size; i++) {
            int st;
            pid_t child = wait(&st);
            end = clock();
            double seconds = (double)(end - dictionary[i].start) / CLOCKS_PER_SEC;

            for (int i = 0; i < size; i++) {
                if (child == dictionary[i].id) {
                    if (WIFEXITED(st)) {
                        if (WEXITSTATUS(st) == 0) {
                            dup2(dictionary[i].out, 1);
                            printf("%s: [PASS] %f [s] \n", dictionary[i].name, seconds);
                            pass_count++;
                        } else if (WEXITSTATUS(st) == 1) {
                            dup2(dictionary[i].out, 1);
                            printf("%s: [FAIL] %f [s] \n", dictionary[i].name, seconds);
                        } 
                    } else if (WIFSIGNALED(st)) {
                        if (strcmp(strsignal(WTERMSIG(st)), "Alarm clock") == 0) {
                            dup2(dictionary[i].out, 1);
                            printf("%s: [ERRO] [TIME] %f [s] \n", dictionary[i].name, seconds);
                        } else {
                            dup2(dictionary[i].out, 1);
                            printf("%s: [ERRO] %s %f [s] \n", dictionary[i].name, strsignal(WTERMSIG(st)), seconds);
                        }
                    }
                }
            }
        }

        dup2(sv_stdout, 1);

        for (int i = 0; i < size; i++) {
            int c;
            FILE *file;
            file = fopen(dictionary[i].name, "r");
            if (file) {
                while ((c = getc(file)) != EOF)
                    putchar(c);
                fclose(file);
            }
            remove(dictionary[i].name);
        }

        printf("%d/%d tests passed\n", pass_count, size);

        printf("\n\n=====================\n");

    }

    return 0;
}
