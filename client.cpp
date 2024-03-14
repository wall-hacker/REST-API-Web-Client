#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "helpers.h"
#include "requests.h"
#include "commands.h"

using namespace std;
using json = nlohmann::json;

int sockfd;
string command;

int main(int argc, char *argv[]) {
    char *cookies[0];
    cookies[0] = (char *)malloc(100);

    while (1) {
        sockfd = open_connection("34.254.242.81", 8080, AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("open_connection");
            exit(EXIT_FAILURE);
        }

        getline(cin, command);

        if (command == "register") {
            register_command(cookies, sockfd);
        } else if (command == "login") {
            login_command(cookies, sockfd);
        } else if (command == "enter_library") {
            enter_library_command(cookies, sockfd);
        } else if (command == "get_books") {
            get_books_command(cookies, sockfd);
        } else if (command == "get_book") {
            get_book_command(cookies, sockfd);
        } else if (command == "add_book") {
            add_book_command(cookies, sockfd);
        } else if (command == "delete_book") {
            delete_book_command(cookies, sockfd);
        } else if (command == "logout") {
            logout_command(cookies, sockfd);
        } else if (command == "exit") {
            break;
        } else {
            cout << "Invalid command!\n";
            continue;
        }

        close_connection(sockfd);
    }

    free(cookies[0]);
    return 0;
}
