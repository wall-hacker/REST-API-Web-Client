#ifndef _COMMANDS_
#define _COMMANDS_

std::string getLastLine(char *s);

void getBooks(char *s);

void getBook(char *s, std::string id);

std::string treatRegisterError(char *s);

std::string treatLoginError(char *s);

std::string ExtractCookie(char *s);

std::string ExtractToken(char *s);

void register_command(char **cookies, int sockfd);

void login_command(char **cookies, int sockfd);

void enter_library_command(char **cookies, int sockfd);

void get_books_command(char **cookies, int sockfd);

void get_book_command(char **cookies, int sockfd);

void add_book_command(char **cookies, int sockfd);

void delete_book_command(char **cookies, int sockfd);

void logout_command(char **cookies, int sockfd);

#endif
