# C Web Server

A simple static web server made using *socket programming* in C.


## Getting started

In order to build the server, [GNC GCC](https://gcc.gnu.org/) must be installed in your system.

[GNU Make](https://www.gnu.org/software/make/) can also be installed if you want to use the `make` command instead.


## Building and running the server

To compile the programme with the recommended flags, run:
``` Bash
gcc -Wall -g -o server server.c
```

To run the server:
``` Bash
./server
```

Using `make`, we can build and run the server with ease.

To build the server:
``` Bash
make build
```

To build and run the server:
``` Bash
make run
```

To terminate the server, you can hit <kbd>Ctrl</kbd> + <kbd>C</kbd> within the terminal.

## Serving web pages

You can create and place static web pages in the [htdocs](/htdocs/) folder which will be served when the server is running.

## Author

W D N C Weerasiri - 2020/CS/203 - 20002033