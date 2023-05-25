#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <check.h>


START_TEST(test_first) {
    
    int num_threads = 6;
    int m = 2;

    FILE* dev_null = fopen("/dev/null", "w");
    int stdout_fd = dup(fileno(stdout));
    freopen("/dev/null", "w", stdout);

    FILE* pipe = popen("./main", "w");
    fprintf(pipe, "%d\n%d\n", num_threads, m);
    fclose(pipe);

    fflush(stdout);
    dup2(stdout_fd, fileno(stdout));
    close(stdout_fd);

    fclose(dev_null);

    FILE* output = fopen("output.txt", "r");

    int numbers[num_threads / 3];
    int count_numbers[num_threads / 3];
    int total_count = 0;


    for (int i = 0; i < num_threads / 3; i++)
    {
        numbers[i] = i + num_threads / 3 + 1;
        count_numbers[i] = 0;
    }

    char line[10];

    while (fgets(line, m*num_threads*10, output) != NULL) {
        total_count++;
        int number = atoi(line);
        for (int i = 0; i < num_threads / 3; i++)
        {
            if (number == numbers[i])
                count_numbers[i]++;
        }
        
    }
    fclose(output); 
    
    ck_assert(total_count == m * (num_threads / 3) * 3);
    for (int i = 0; i < num_threads / 3; i++)
    {
        ck_assert(count_numbers[i] == 3 * m); 
    }
    
}
END_TEST

START_TEST(test_second) {
    
    int num_threads = 150;
    int m = 10;

    FILE* dev_null = fopen("/dev/null", "w");
    int stdout_fd = dup(fileno(stdout));
    freopen("/dev/null", "w", stdout);

    FILE* pipe = popen("./main", "w");
    fprintf(pipe, "%d\n%d\n", num_threads, m);
    fclose(pipe);

    fflush(stdout);
    dup2(stdout_fd, fileno(stdout));
    close(stdout_fd);

    fclose(dev_null);

    FILE* output = fopen("output.txt", "r");

    int numbers[num_threads / 3];
    int count_numbers[num_threads / 3];
    int total_count = 0;


    for (int i = 0; i < num_threads / 3; i++)
    {
        numbers[i] = i + num_threads / 3 + 1;
        count_numbers[i] = 0;
    }

    char line[10];

    while (fgets(line, m*num_threads*10, output) != NULL) {
        total_count++;
        int number = atoi(line);
        for (int i = 0; i < num_threads / 3; i++)
        {
            if (number == numbers[i])
                count_numbers[i]++;
        }
        
    }
    fclose(output); 
    
    ck_assert(total_count == m * (num_threads / 3) * 3);
    for (int i = 0; i < num_threads / 3; i++)
    {
        ck_assert(count_numbers[i] == 3 * m); 
    }
    
}
END_TEST

START_TEST(test_third) {
    
    int num_threads = 100;
    int m = 5;

    FILE* dev_null = fopen("/dev/null", "w");
    int stdout_fd = dup(fileno(stdout));
    freopen("/dev/null", "w", stdout);

    FILE* pipe = popen("./main", "w");
    fprintf(pipe, "%d\n%d\n", num_threads, m);
    fclose(pipe);

    fflush(stdout);
    dup2(stdout_fd, fileno(stdout));
    close(stdout_fd);

    fclose(dev_null);

    FILE* output = fopen("output.txt", "r");

    int numbers[num_threads / 3];
    int count_numbers[num_threads / 3];
    int total_count = 0;


    for (int i = 0; i < num_threads / 3; i++)
    {
        numbers[i] = i + num_threads / 3 + 1;
        count_numbers[i] = 0;
    }

    char line[10];

    while (fgets(line, m*num_threads*10, output) != NULL) {
        total_count++;
        int number = atoi(line);
        for (int i = 0; i < num_threads / 3; i++)
        {
            if (number == numbers[i])
                count_numbers[i]++;
        }
        
    }
    fclose(output); 
    
    ck_assert(total_count == m * (num_threads / 3) * 3);
    for (int i = 0; i < num_threads / 3; i++)
    {
        ck_assert(count_numbers[i] == 3 * m); 
    }
    
}
END_TEST



int main(void) {
    Suite *suite = suite_create("My Suite");
    TCase *tc = tcase_create("My Test Case");
    SRunner *runner = srunner_create(suite);

    tcase_add_test(tc, test_first);
    tcase_add_test(tc, test_second);
    tcase_add_test(tc, test_third);

    suite_add_tcase(suite, tc);

    srunner_run_all(runner, CK_NORMAL);

    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed == 0 ? 0 : 1;
}
