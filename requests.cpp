#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "nlohmann/json.hpp"
#include "helpers.h"
#include "requests.h"

using namespace std;

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
       sprintf(line, "Cookie: ");
       for(int i = 0; i < cookies_count; i++) {
           strcat(line, cookies[i]);
           strcat(line, "; ");
       }
       compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_get_auth_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
       sprintf(line, "Authorization: Bearer ");
       for(int i = 0; i < cookies_count; i++) {
           strcat(line, cookies[i]);
       }
       compute_message(message, line);
    }

    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, string body_data,
                            int body_data_fields_count, char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", body_data.length());
    compute_message(message, line);

    if (cookies != NULL) {
       sprintf(line, "Cookie: ");
       for(int i = 0; i < cookies_count; i++) {
           strcat(line, cookies[i]);
           strcat(line, "; ");
       }
       compute_message(message, line);
    }

    compute_message(message, "");

    memset(line, 0, LINELEN);
    strcat(message, body_data.c_str());

    free(line);
    return message;
}

char *compute_post_auth_request(char *host, char *url, char* content_type, string body_data,
                            int body_data_fields_count, char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    sprintf(line, "Content-Length: %ld", body_data.length());
    compute_message(message, line);

    if (cookies != NULL) {
       sprintf(line, "Authorization: Bearer ");
       for(int i = 0; i < cookies_count; i++) {
           strcat(line, cookies[i]);
       }
       compute_message(message, line);
    }

    compute_message(message, "");

    memset(line, 0, LINELEN);
    strcat(message, body_data.c_str());

    free(line);
    return message;
}

char *compute_delete_auth_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
       sprintf(line, "Authorization: Bearer ");
       for(int i = 0; i < cookies_count; i++) {
           strcat(line, cookies[i]);
       }
       compute_message(message, line);
    }

    compute_message(message, "");

    free(line);
    return message;
}
