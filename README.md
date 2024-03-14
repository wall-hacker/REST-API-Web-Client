```
   ____  _____ ____ _____      _    ____ ___     
  |  _ \| ____/ ___|_   _|    / \  |  _ \_ _|    
  | |_) |  _| \___ \ | |     / _ \ | |_) | |     
  |  _ <| |___ ___) || |    / ___ \|  __/| |     
  |_| \_\_____|____/ |_|   /_/   \_\_|  |___|    
 __        __   _        ____ _ _            _   
 \ \      / /__| |__    / ___| (_) ___ _ __ | |_ 
  \ \ /\ / / _ \ '_ \  | |   | | |/ _ \ '_ \| __|
   \ V  V /  __/ |_) | | |___| | |  __/ | | | |_ 
    \_/\_/ \___|_.__/   \____|_|_|\___|_| |_|\__|
```
---
Structure && Implementation
---
```
In the client.cpp file i have implemented the openening and colsing of the connection to the server and the command parsing logic, using an infinite loop and an if-else chain.
```
```
In the requests.cpp file i have implemented the HTTP request sending logic, using the socket API.
```
```
In the commands.cpp file i have implemented the logic sending the HTTP requests to the server, using the requests.cpp file and the logic for for parsing the commands and the responses from the server. The functions in this file are called from the client.cpp file. They are pretty self explanatory. One thing to note is that in order for the enter_library and login commands to work, the client must save the cookie received from the server after the login command. The cookie is saved in the client.cpp file as a global variable. And in order for the get_books, get_book, add_book and delete_book commands to work, the client must save the jwt token received from the server after the enter_library command. The jwt token is also saved in the client.cpp file as a global variable.
```
---
Usage
---
- RUN CLIENT
    - \$ make 
    - \$ ./client
- CLIENT COMMANDS
    - login \<username> \<password>
    - register \<username> \<password>
    - enter_library
    - get_books
    - get_book \<id>
    - add_book \<title> \<author> \<genre> \<publisher> \<page_count>
    - delete_book \<id>
    - logout
    - exit
---
Note
---
- The client is implemented using the lab 10 code as a base.
- I decided to use the json-c++ nlohmann library for parsing the json responses from the server, beacuse it is very easy to use, it is very well documented, it is a header only library and i wanted to use c++ for the easier string manipulation.
---
Feedback && Comments
---
- Very interesting and enjoyable project. I learned a lot about HTTP and RESTful APIs. I also learned how to use the json-c++ nlohmann library.
- I would like to see more projects like this one in the future.







