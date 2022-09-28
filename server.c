#include <stdio.h>  // console input/output, perror
#include <stdlib.h> // exit
#include <string.h> // string manipulation
#include <netdb.h>  // getnameinfo

// socket programming
#include <sys/socket.h> // socket APIs
#include <netinet/in.h> // sockaddr_in - for storing the address of the server
#include <unistd.h>     // open, close

#include <signal.h> // signal handling

#define SIZE 1024  // buffer size
#define PORT 2728  // port number
#define BACKLOG 10 // number of pending connections queue will hold

/**
 * @brief Generates file URL based on route
 * @param route requested route
 * @param fileurl generated url
 */
void getFileURL(char *route, char *fileURL);

/**
 * @brief Sets *MIME to the mime type of file
 * @param file file URL
 * @param mime mime type of file
 */
void getMimeType(char *file, char *mime);

// Handles console interrupts
void handleSignal(int signal);

int serverSocket; // server socket
int clientSocket; // client socket
char *request;

int main()
{

  signal(SIGINT, handleSignal); // register signal handler

  serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Socket of type IPv4 using TCP protocol

  struct sockaddr_in serverAddress;                       // Server Socket Address
  serverAddress.sin_family = AF_INET;                     // IPv4
  serverAddress.sin_port = htons(PORT);                   // port number
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost

  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // bind socket to address
  {
    printf("Error: The server is not bound to the address.\n");
    return 1;
  }

  int listening = listen(serverSocket, BACKLOG); // listen for connections
  if (listening < 0)
  {
    printf("Error: The server is not listening.\n");
    return 1;
  }

  char hostBuffer[NI_MAXHOST];    // client's remote name
  char serviceBuffer[NI_MAXSERV]; // port the client is connect to

  // get the client's remote name and port
  int error = getnameinfo(
      (struct sockaddr *)&serverAddress,
      sizeof(serverAddress),
      hostBuffer,
      sizeof(hostBuffer),
      serviceBuffer,
      sizeof(serviceBuffer),
      0);

  if (error != 0)
  {
    printf("Error: %s\n", gai_strerror(error)); // print error message
    return 1;
  }

  printf("\nServer is listening on http://%s:%s/\n\n", hostBuffer, serviceBuffer);

  while (1)
  {
    request = (char *)malloc(SIZE * sizeof(char));   // buffer to store data (request)
    char method[10], route[100];                     // HTTP request method and path
    clientSocket = accept(serverSocket, NULL, NULL); // accept connection from client
    read(clientSocket, request, SIZE);               // read data from client
    sscanf(request, "%s %s", method, route);         // parse HTTP request
    printf("%s %s", method, route);                  // print data received from client
    free(request);

    // only GET method supported
    if (strcmp(method, "GET") != 0)
    {
      const char response[] = "HTTP/1.1 400 Bad Request\r\n\n"; // HTTP response
      send(clientSocket, response, sizeof(response), 0);        // send HTTP response
    }
    else
    {
      char fileURL[100];
      getFileURL(route, fileURL);       // generate file URL
      FILE *file = fopen(fileURL, "r"); // open file
      if (!file)
      {
        const char response[] = "HTTP/1.1 404 Not Found\r\n\n"; // HTTP response
        send(clientSocket, response, sizeof(response), 0);      // send HTTP response
      }
      else
      {
        char mimeType[32];
        getMimeType(fileURL, mimeType); // generate mime type from file URL
        char resHeader[SIZE];           // HTTP response header
        sprintf(resHeader, "HTTP/1.1 200 OK\r\nContent-Type : %s\r\n\n", mimeType);
        int headerSize = strlen(resHeader);

        printf(" %s", mimeType);

        // Calculate file size
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *resBuffer = (char *)malloc(fsize + headerSize); // buffer to store data (response)
        strcpy(resBuffer, resHeader);                         // copy HTTP header to response buffer
        char *fileBuffer = resBuffer + headerSize;            // pointer to file buffer
        fread(fileBuffer, fsize, 1, file);                    // read file into file buffer

        send(clientSocket, resBuffer, fsize + headerSize, 0); // send HTTP header
        free(resBuffer);
      }
      fclose(file);
    }
    close(clientSocket);
    printf("\n");
  }
}

void getFileURL(char *route, char *fileURL)
{
  // if route has parameters, remove them
  char *question = strrchr(route, '?');
  if (question)
    *question = '\0';

  // if route is empty, set it to index.html
  if (route[strlen(route) - 1] == '/')
  {
    strcat(route, "index.html");
  }

  // get filename from route
  strcpy(fileURL, "htdocs");
  strcat(fileURL, route);

  // if filename does not have an extension, set it to .html
  const char *dot = strrchr(fileURL, '.');
  if (!dot || dot == fileURL)
  {
    strcat(fileURL, ".html");
  }
}

void getMimeType(char *file, char *mime)
{
  const char *dot = strrchr(file, '.'); // position in string with dot character

  if (dot == NULL) // if dot is NULL, set mime type to text/html
    strcpy(mime, "text/html");

  else if (strcmp(dot, ".html") == 0)
    strcpy(mime, "text/html");

  else if (strcmp(dot, ".css") == 0)
    strcpy(mime, "text/css");

  else if (strcmp(dot, ".js") == 0)
    strcpy(mime, "application/js");

  else if (strcmp(dot, ".jpg") == 0)
    strcpy(mime, "image/jpeg");

  else if (strcmp(dot, ".png") == 0)
    strcpy(mime, "image/png");

  else if (strcmp(dot, ".gif") == 0)
    strcpy(mime, "image/gif");

  else
    strcpy(mime, "text/html");
}

void handleSignal(int signal)
{
  if (signal == SIGINT)
  {
    printf("\nShutting down server...\n");

    close(clientSocket); // close client socket
    close(serverSocket); // close server socket

    if (request != NULL) // free request buffer
      free(request);

    exit(0);
  }
}