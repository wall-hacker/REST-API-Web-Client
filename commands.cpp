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

json j;
char *response, *request;
string send_data, cookie, token, username, password, id, path;

// for getting the last line of the server response
string getLastLine(char *s) {
    string str(s);
    size_t pos = str.find_last_of("\n");
    return str.substr(pos + 1);
}

// parse the json object from the server response
void getBooks(char *s) {
    string str(s);
    size_t pos = str.find_first_of("[");
    json j = json::parse(str.substr(pos));
    cout << j.dump(4) << '\n';
}

// parse the json object from the server response
void getBook(char *s, string id) {
    string str(s);
    size_t pos = str.find_first_of("{");
    json j = json::parse(str.substr(pos));
    if (j.find("error") == j.end()) {
        j["id"] = id;
        cout << j.dump(4) << endl;
        return;
    }
    cout << "No book with that id found!\n";
    return;
}

// checks the json response for register errors
string treatRegisterError(char *s) {
    string str(s);
    size_t pos = str.find_first_of("{");
    if (pos == string::npos)
        return "good";
    return "bad";
}

// checks the json response for login errors
string treatLoginError(char *s) {
    string str(s);
    size_t pos = str.find_first_of("{");
    if (pos == string::npos)
        return "good";
    json aux = json::parse(str.substr(pos));
    return aux["error"].dump();
}

// parses the json response for the login cookie
string ExtractCookie(char *s) {
    string str(s);
    size_t pos = str.find("Set-Cookie: ");
    if (pos == string::npos)
        return "";
    pos += 12;
    size_t pos2 = str.find(";", pos);
    return str.substr(pos, pos2 - pos);
}

// parses the json response for the enter_library token
string ExtractToken(char *s) {
    string str(getLastLine(s));
    json j = json::parse(str);
    if (j.find("token") == j.end())
        return "";
    return j["token"].dump();
}

void register_command(char **cookies, int sockfd)
{
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);
    // check if username and password have spaces
    if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
        cout << "Username and password must not contain spaces\n";
        return;
    }
    j["username"] = username;
    j["password"] = password;
    send_data = j.dump().c_str();
    request = compute_post_request("34.254.242.81:8080", "/api/v1/tema/auth/register", "application/json", send_data, 2, NULL, 0);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    if (treatRegisterError(response) == "good") {
        cout << "Register successful\n";
        return;
    }
    cout << "Register failed, account already exists\n";
    return;
}

void login_command(char **cookies, int sockfd) {
    cout << "username=";
    getline(cin, username);
    cout << "password=";
    getline(cin, password);
    j["username"] = username;
    j["password"] = password;
    send_data = j.dump().c_str();
    request = compute_post_request("34.254.242.81:8080", "/api/v1/tema/auth/login", "application/json", send_data, 2, NULL, 0);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    if (treatLoginError(response) == "good") {
        cout << "Login successful\n";
        // get the cookie from the response and store it
        cookie = ExtractCookie(response);
        return;
    }
    // remove the quotes from the error message
    string error = treatLoginError(response);
    error = error.substr(1, error.size() - 2);
    treatLoginError(response).c_str();
    cout << "Login failed: " << error << '\n';
    return;
}

void enter_library_command(char **cookies, int sockfd) {
    strcpy(cookies[0], cookie.c_str());
    request = compute_get_request("34.254.242.81:8080", "/api/v1/tema/library/access", "application/json", cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    token = ExtractToken(response);
    if (token == "") {
        cout << "Enter library failed, login first!\n";
        return;
    }
    // get the token from the response and store it without the quotes
    token = token.substr(1, token.size() - 2);
    cout << "Enter library successful\n";
    return;
}

void get_books_command(char **cookies, int sockfd) {
    // check if the user is logged in and entered in the library
    if (token == "") {
        cout << "Get books failed, enter the library first\n";
        return;
    }
    strcpy(cookies[0], token.c_str());
    request = compute_get_auth_request("34.254.242.81:8080", "/api/v1/tema/library/books", "application/json", cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    getBooks(response);
    return;
}

void get_book_command(char **cookies, int sockfd) {
    cout << "id=";
    getline(cin, id);
    // check if the user is logged in and entered in the library
    if (token == "") {
        cout << "Get book failed, enter the library first\n";
        return;
    }
    strcpy(cookies[0], token.c_str());
    path = "/api/v1/tema/library/books/" + id;
    request = compute_get_auth_request("34.254.242.81:8080", (char *)path.c_str(), "application/json", cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    getBook(response, id);
    return;
}

void add_book_command(char **cookies, int sockfd) {
    // check if the user is logged in and entered in the library
    if (token == "") {
        cout << "Add book failed, enter the library first\n";
        return;
    }
    string title, author, genre, page_count, publisher;
    cout << "title=";
    getline(cin, title);
    cout << "author=";
    getline(cin, author);
    cout << "genre=";
    getline(cin, genre);
    cout << "publisher=";
    getline(cin, publisher);
    cout << "page_count=";
    getline(cin, page_count);
    // check if page_count is a number
    for (int i = 0; i < page_count.size(); i++) {
        if (page_count[i] < '0' || page_count[i] > '9') {
            cout << "Page count must be a number!\n";
            return;
        }
    }
    j["title"] = title;
    j["author"] = author;
    j["genre"] = genre;
    j["page_count"] = stoi(page_count);
    j["publisher"] = publisher;
    send_data = j.dump().c_str();
    strcpy(cookies[0], token.c_str());
    request = compute_post_auth_request("34.254.242.81:8080", "/api/v1/tema/library/books", "application/json", send_data, 1, cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    if (treatRegisterError(response) == "good") {
        cout << "Add book successful\n";
        return;
    }
    cout << "Add book failed\n";
    return;
}

void delete_book_command(char **cookies, int sockfd) {
    cout << "id=";
    getline(cin, id);
    // check if the user is logged in and entered in the library
    if (token == "") {
        cout << "Delete book failed, enter the library first\n";
        return;
    }
    strcpy(cookies[0], token.c_str());
    path = "/api/v1/tema/library/books/" + id;
    request = compute_delete_auth_request("34.254.242.81:8080", (char *)path.c_str(), "application/json", cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    if (treatRegisterError(response) == "good") {
        cout << "Delete book successful\n";
        return;
    }
    cout << "No book with that id found!\n";
    return;
}

void logout_command(char **cookies, int sockfd) {
    strcpy(cookies[0], cookie.c_str());
    request = compute_get_request("34.254.242.81:8080", "/api/v1/tema/auth/logout", "application/json", cookies, 1);
    send_to_server(sockfd, request);
    response = receive_from_server(sockfd);
    if (treatRegisterError(response) == "good") {
        cout << "Logout successful\n";
        // reset the cookie and token
        cookie = "";
        token = "";
        return;
    }
    cout << "You are not logged in!\n";
    return;
}