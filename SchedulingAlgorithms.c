#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_REQUESTS 100

// Function prototypes
void fcfs(int requests[], int n, int head);
void sstf(int requests[], int n, int head);
void scan(int requests[], int n, int head, int disk_size, int direction);
void cscan(int requests[], int n, int head, int disk_size);
void clook(int requests[], int n, int head, int direction);
void printSequence(int sequence[], int n, int total_movement);
void printMenu();
int getRequests(int requests[]);

int main() {
    int requests[MAX_REQUESTS];
    int n, head, disk_size, direction, choice;

    printf("=================================================\n");
    printf("      DISK SCHEDULING ALGORITHMS SIMULATOR      \n");
    printf("=================================================\n\n");

    // Get input
    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size (cylinder range 0 to size-1): ");
    scanf("%d", &disk_size);

    n = getRequests(requests);

    do {
        printMenu();
        printf("Enter your choice (1-6): ");
        scanf("%d", &choice);

        printf("\n");

        switch(choice) {
            case 1:
                printf("*** FIRST COME FIRST SERVE (FCFS) ***\n");
                fcfs(requests, n, head);
                break;

            case 2:
                printf("*** SHORTEST SEEK TIME FIRST (SSTF) ***\n");
                sstf(requests, n, head);
                break;

            case 3:
                printf("Enter direction (0 for left, 1 for right): ");
                scanf("%d", &direction);
                printf("*** SCAN (Elevator Algorithm) ***\n");
                scan(requests, n, head, disk_size, direction);
                break;

            case 4:
                printf("*** C-SCAN (Circular SCAN) ***\n");
                cscan(requests, n, head, disk_size);
                break;

            case 5:
                printf("Enter direction (0 for left, 1 for right): ");
                scanf("%d", &direction);
                printf("*** C-LOOK ***\n");
                clook(requests, n, head, direction);
                break;

            case 6:
                printf("Thank you for using the Disk Scheduling Simulator!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

        if(choice >= 1 && choice <= 5) {
            printf("\nPress Enter to continue...");
            getchar(); getchar();
        }

    } while(choice != 6);

    return 0;
}

int getRequests(int requests[]) {
    int n;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    printf("Enter the request sequence: ");
    for(int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("\nRequest Queue: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n\n");

    return n;
}

void printMenu() {
    printf("\n=================================================\n");
    printf("             SCHEDULING ALGORITHMS              \n");
    printf("=================================================\n");
    printf("1. FCFS (First Come First Serve)\n");
    printf("2. SSTF (Shortest Seek Time First)\n");
    printf("3. SCAN (Elevator Algorithm)\n");
    printf("4. C-SCAN (Circular SCAN)\n");
    printf("5. C-LOOK\n");
    printf("6. Exit\n");
    printf("=================================================\n");
}

void fcfs(int requests[], int n, int head) {
    int sequence[MAX_REQUESTS + 1];
    int total_movement = 0;
    int current = head;

    sequence[0] = head;

    printf("Starting at head position: %d\n", head);
    printf("Order of service: %d", head);

    for(int i = 0; i < n; i++) {
        total_movement += abs(current - requests[i]);
        current = requests[i];
        sequence[i + 1] = current;
        printf(" -> %d", current);
    }

    printf("\n\nTotal head movement: %d cylinders\n", total_movement);
    printf("Average seek time: %.2f cylinders\n", (float)total_movement / n);
}

void sstf(int requests[], int n, int head) {
    int sequence[MAX_REQUESTS + 1];
    int visited[MAX_REQUESTS] = {0};
    int total_movement = 0;
    int current = head;
    int count = 0;

    sequence[0] = head;
    printf("Starting at head position: %d\n", head);
    printf("Order of service: %d", head);

    while(count < n) {
        int min_distance = 9999;
        int next_request = -1;

        // Find the closest unvisited request
        for(int i = 0; i < n; i++) {
            if(!visited[i]) {
                int distance = abs(current - requests[i]);
                if(distance < min_distance) {
                    min_distance = distance;
                    next_request = i;
                }
            }
        }

        visited[next_request] = 1;
        total_movement += min_distance;
        current = requests[next_request];
        sequence[count + 1] = current;
        printf(" -> %d", current);
        count++;
    }

    printf("\n\nTotal head movement: %d cylinders\n", total_movement);
    printf("Average seek time: %.2f cylinders\n", (float)total_movement / n);
}

void scan(int requests[], int n, int head, int disk_size, int direction) {
    int sequence[MAX_REQUESTS + 2];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int left_count = 0, right_count = 0;
    int total_movement = 0;
    int seq_index = 0;

    // Separate requests into left and right of head
    for(int i = 0; i < n; i++) {
        if(requests[i] < head) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }

    // Sort arrays
    for(int i = 0; i < left_count - 1; i++) {
        for(int j = 0; j < left_count - i - 1; j++) {
            if(left[j] < left[j + 1]) {
                int temp = left[j];
                left[j] = left[j + 1];
                left[j + 1] = temp;
            }
        }
    }

    for(int i = 0; i < right_count - 1; i++) {
        for(int j = 0; j < right_count - i - 1; j++) {
            if(right[j] > right[j + 1]) {
                int temp = right[j];
                right[j] = right[j + 1];
                right[j + 1] = temp;
            }
        }
    }

    sequence[seq_index++] = head;
    printf("Starting at head position: %d\n", head);
    printf("Direction: %s\n", direction ? "Right" : "Left");
    printf("Order of service: %d", head);

    int current = head;

    if(direction == 1) { // Moving right
        // Service requests to the right
        for(int i = 0; i < right_count; i++) {
            total_movement += abs(current - right[i]);
            current = right[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }

        // Go to end and reverse
        if(current != disk_size - 1) {
            total_movement += abs(current - (disk_size - 1));
            current = disk_size - 1;
            printf(" -> %d", current);
        }

        // Service requests to the left
        for(int i = 0; i < left_count; i++) {
            total_movement += abs(current - left[i]);
            current = left[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }
    } else { // Moving left
        // Service requests to the left
        for(int i = 0; i < left_count; i++) {
            total_movement += abs(current - left[i]);
            current = left[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }

        // Go to beginning and reverse
        if(current != 0) {
            total_movement += abs(current - 0);
            current = 0;
            printf(" -> %d", current);
        }

        // Service requests to the right
        for(int i = 0; i < right_count; i++) {
            total_movement += abs(current - right[i]);
            current = right[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }
    }

    printf("\n\nTotal head movement: %d cylinders\n", total_movement);
    printf("Average seek time: %.2f cylinders\n", (float)total_movement / n);
}

void cscan(int requests[], int n, int head, int disk_size) {
    int sequence[MAX_REQUESTS + 2];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int left_count = 0, right_count = 0;
    int total_movement = 0;
    int seq_index = 0;

    // Separate requests into left and right of head
    for(int i = 0; i < n; i++) {
        if(requests[i] < head) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }

    // Sort arrays
    for(int i = 0; i < left_count - 1; i++) {
        for(int j = 0; j < left_count - i - 1; j++) {
            if(left[j] > left[j + 1]) {
                int temp = left[j];
                left[j] = left[j + 1];
                left[j + 1] = temp;
            }
        }
    }

    for(int i = 0; i < right_count - 1; i++) {
        for(int j = 0; j < right_count - i - 1; j++) {
            if(right[j] > right[j + 1]) {
                int temp = right[j];
                right[j] = right[j + 1];
                right[j + 1] = temp;
            }
        }
    }

    sequence[seq_index++] = head;
    printf("Starting at head position: %d\n", head);
    printf("Order of service: %d", head);

    int current = head;

    // Service requests to the right
    for(int i = 0; i < right_count; i++) {
        total_movement += abs(current - right[i]);
        current = right[i];
        sequence[seq_index++] = current;
        printf(" -> %d", current);
    }

    // Go to end
    if(current != disk_size - 1) {
        total_movement += abs(current - (disk_size - 1));
        current = disk_size - 1;
        printf(" -> %d", current);
    }

    // Jump to beginning (no service on return)
    total_movement += abs(current - 0);
    current = 0;
    printf(" -> %d", current);

    // Service requests from beginning
    for(int i = 0; i < left_count; i++) {
        total_movement += abs(current - left[i]);
        current = left[i];
        sequence[seq_index++] = current;
        printf(" -> %d", current);
    }

    printf("\n\nTotal head movement: %d cylinders\n", total_movement);
    printf("Average seek time: %.2f cylinders\n", (float)total_movement / n);
}

void clook(int requests[], int n, int head, int direction) {
    int sequence[MAX_REQUESTS + 1];
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int left_count = 0, right_count = 0;
    int total_movement = 0;
    int seq_index = 0;

    // Separate requests into left and right of head
    for(int i = 0; i < n; i++) {
        if(requests[i] < head) {
            left[left_count++] = requests[i];
        } else {
            right[right_count++] = requests[i];
        }
    }

    // Sort arrays
    for(int i = 0; i < left_count - 1; i++) {
        for(int j = 0; j < left_count - i - 1; j++) {
            if(left[j] > left[j + 1]) {
                int temp = left[j];
                left[j] = left[j + 1];
                left[j + 1] = temp;
            }
        }
    }

    for(int i = 0; i < right_count - 1; i++) {
        for(int j = 0; j < right_count - i - 1; j++) {
            if(right[j] > right[j + 1]) {
                int temp = right[j];
                right[j] = right[j + 1];
                right[j + 1] = temp;
            }
        }
    }

    sequence[seq_index++] = head;
    printf("Starting at head position: %d\n", head);
    printf("Direction: %s\n", direction ? "Right" : "Left");
    printf("Order of service: %d", head);

    int current = head;

    if(direction == 1) { // Moving right
        // Service requests to the right
        for(int i = 0; i < right_count; i++) {
            total_movement += abs(current - right[i]);
            current = right[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }

        // Jump to leftmost request
        if(left_count > 0) {
            total_movement += abs(current - left[0]);
            current = left[0];
            sequence[seq_index++] = current;
            printf(" -> %d", current);

            // Service remaining left requests
            for(int i = 1; i < left_count; i++) {
                total_movement += abs(current - left[i]);
                current = left[i];
                sequence[seq_index++] = current;
                printf(" -> %d", current);
            }
        }
    } else { // Moving left
        // Service requests to the left (in descending order)
        for(int i = left_count - 1; i >= 0; i--) {
            total_movement += abs(current - left[i]);
            current = left[i];
            sequence[seq_index++] = current;
            printf(" -> %d", current);
        }

        // Jump to rightmost request
        if(right_count > 0) {
            total_movement += abs(current - right[right_count - 1]);
            current = right[right_count - 1];
            sequence[seq_index++] = current;
            printf(" -> %d", current);

            // Service remaining right requests (in descending order)
            for(int i = right_count - 2; i >= 0; i--) {
                total_movement += abs(current - right[i]);
                current = right[i];
                sequence[seq_index++] = current;
                printf(" -> %d", current);
            }
        }
    }

    printf("\n\nTotal head movement: %d cylinders\n", total_movement);
    printf("Average seek time: %.2f cylinders\n", (float)total_movement / n);
}

void printSequence(int sequence[], int n, int total_movement) {
    printf("Seek sequence: ");
    for(int i = 0; i < n; i++) {
        printf("%d", sequence[i]);
        if(i < n - 1) printf(" -> ");
    }
    printf("\nTotal head movement: %d cylinders\n", total_movement);
}
