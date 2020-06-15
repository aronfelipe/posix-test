
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

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

    printf("%d \n", specific);

    if (specific != -1) {

        int size = 1;
        printf("Running %d test:\n", size);
        printf("=====================\n\n");
        int pass_count = 0;

        if (fork() == 0) {
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
            printf("Return: %d \n", WEXITSTATUS(st));
            if (WEXITSTATUS(st) == 0) {
                pass_count++;
            }
        } 

        printf("%d/%d tests passed\n", pass_count, size);
        printf("\n\n=====================\n");

    } else {

        int size = sizeof(all_tests)/sizeof(test_data);
        printf("Running %d tests:\n", size);
        printf("=====================\n\n");
        int pass_count = 0;

        for (int i = 0; i < size; i++) {
            if (fork() == 0) {
                if (all_tests[i].function() >= 0) {
                    printf("%s: [PASS]\n", all_tests[i].name);
                    return 0;
                } else {
                    return 1;
                }
            }
        }

        for (int i = 0; i < size; i++) {
            int st;
            wait(&st);
            if (WIFEXITED(st)) {
                printf("Return: %d \n", WEXITSTATUS(st));
                if (WEXITSTATUS(st) == 0) {
                    pass_count++;
                }
            } else if (WIFSIGNALED(st)) {
                printf("[ERRO]: %s \n", strsignal(WTERMSIG(st)));
            } 

        }
        
        printf("%d/%d tests passed\n", pass_count, size);
        printf("\n\n=====================\n");

    }


    return 0;
}
