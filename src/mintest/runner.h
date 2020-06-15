
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

typedef struct {
    int id;
    char name[50];
    double start;
} process;


int main(int argc, char** argv) {

    int counter;
    int specific = -1;

    printf("Program name is: %s", argv[0]); 

    if(argc == 1) {
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name"); 
    }
    else if(argc >= 2) { 
        printf("\nNumber Of Arguments Passed: %d",argc); 
        printf("\n----Following Are The Command Line Arguments Passed---- \n"); 
        for(counter = 0;counter < argc; counter++)  {
            printf("argv[%d]: %s \n", counter, argv[counter]);
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

        if (fork() == 0) {
            alarm(2);
            if (all_tests[specific].function() >= 0) {
                printf("%s: [PASS]\n", all_tests[specific].name);
                return 0;
            } else {
                return 1;
            }
        }

        int st;
        wait(&st);
        if (WIFEXITED(st)) {
            if (WEXITSTATUS(st) == 0) {
                pass_count++;
            }
        } else if (WIFSIGNALED(st)) {
            printf("%s: [ERRO] %s \n", all_tests[specific].name, strsignal(WTERMSIG(st)));
        } 

        printf("%d/%d tests passed\n", pass_count, size);
        printf("\n\n=====================\n");

    } else {

        int size = sizeof(all_tests)/sizeof(test_data);
        process *dictionary = malloc(size * sizeof(process));
        printf("\nRunning %d tests:\n", size);
        printf("=====================\n\n");
        int pass_count = 0;

        for (int i = 0; i < size; i++) {
            pid_t child = fork();
            process action;
            clock_t start = clock();
            action.start = start;
            action.id = child;
            sprintf(action.name, "%s", all_tests[i].name);
            dictionary[i] = action;
            if (child == 0) {
                alarm(2);
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
            clock_t end = clock();
            float seconds = (float)(end - dictionary[i].start) / CLOCKS_PER_SEC;

            for (int i = 0; i < size; i++) {
                if (child == dictionary[i].id) {
                    if (WIFEXITED(st)) {
                        if (WEXITSTATUS(st) == 0) {
                            printf("%s: [PASS] %f [s] \n", dictionary[i].name, seconds);
                            pass_count++;
                        } else if (WEXITSTATUS(st) == 1) {
                            printf("%s: [FAIL] %f [s] \n", dictionary[i].name, seconds);
                        } 
                    } else if (WIFSIGNALED(st)) {
                        if (strcmp(strsignal(WTERMSIG(st)), "Alarm clock") == 0) {
                            printf("%s: [ERRO] [TIME] %f [s] \n", dictionary[i].name, seconds);
                        } else {
                            printf("%s: [ERRO] %s %f [s] \n", dictionary[i].name, strsignal(WTERMSIG(st)), seconds);
                        }
                    }
                }
            }
        }

        clock_t end = clock();
        // float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("%d/%d tests passed\n", pass_count, size);

        // printf("In %lf seconds", seconds);
        printf("\n\n=====================\n");

    }


    return 0;
}
