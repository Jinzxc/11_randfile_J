#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int randin;

int rand() {
    int in;
    int reed = read(randin, &in, sizeof(in));

    if(reed == -1) {
        printf("Error when reading from randin->errno: %d msg: %s.\n", errno, strerror(errno));
        return -1;
    }

    return in;
}

void print_array(int* arr, int len) {
    int i;
    for(i = 0; i < len; i++) {
        printf("\trandom %d: %d\n", i, arr[i]);
    }

    printf("\n");
}

int main() {
    randin = open("/dev/random", O_RDONLY);
    if(randin == -1) {
        printf("Error when opening randin->errno: %d msg: %s.\n", errno, strerror(errno));
        return 0;
    }

    int arr[10];
    int i;

    printf("Generating randoom numbers:\n");

    for(i = 0; i < 10; i++) {
        int get = rand();
        if(get == -1) {
            return 0;
        }
        arr[i] = get;
    }

    print_array(arr, 10);

    printf("\nWriting numbers to file...\n");
    int randout = open("randfile.txt", O_CREAT | O_WRONLY, 0664);

    if(randout == -1) {
        printf("Error when opening randout->errno: %d msg: %s.\n", errno, strerror(errno));
        return 0;
    }

    int whrite = write(randout, arr, sizeof(arr));
    if(whrite == -1) {
        printf("Error when writing to randout->errno: %d msg: %s.\n", errno, strerror(errno));
        return 0;
    }
    printf("Wrote %d bytes\n", whrite);

    close(randout);

    printf("\nReading numbers from file...\n");
    int randback = open("randfile.txt", O_RDONLY);
    int arr2[10];

    int reed = read(randback, arr2, sizeof(arr2));
    if(reed == -1) {
        printf("Error when reading from randback->errno: %d msg: %s.\n", errno, strerror(errno));
        return 0;
    }
    printf("Read %d bytes\n", reed);

    printf("\nVerification that written values were the same:\n");

    print_array(arr2, 10);

    close(randin);
    close(randback);

    return 0;
}