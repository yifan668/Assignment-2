// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    printf("execve=0x%p\n", execve);

    // Re-exec the child process, checking if we socket passed in arguments
    // Checking if the point of execution is from exec as we are pasing two arguments
    if(argc>1){
        printf("reexecing server\n");
        server_fd = atoi(argv[1]);
        printf("server_fd=%d\n",server_fd);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
        return 0;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start server socket for listening
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Establish connection with client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int  pid = fork();

    if (pid < 0)
    {
        printf("fork error!");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        //set user as nobody
        setuid(65534);
        //print uid
        printf("%d",getuid());
        char socket_fd[11];
        snprintf(socket_fd,10,"%d\n",server_fd);
        char *args[] = {"./server.o",socket_fd,NULL};
        printf("before exec server_fd %d\n",server_fd);
        execv("./server.o",args);
        exit(0);
        return 0;
    }
    else(pid > 0) // parent will execute this part , id > 0
    {  // waits for all child process to finish
        wait(NULL);
    }
    return 0;
}
