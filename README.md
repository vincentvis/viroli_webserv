# viroli webserv
The second to last big team project for our curriculum at [Codam](https://www.codam.nl/). 
Creating a basic webserver to handle GET/POST/DELETE requests and learn about programming with sockets. 

## Installation and usage
```
git clone git@github.com:vincentvis/viroli_webserv.git viroli_webserv
cd viroli_webserv
cmake -S . -B build
cmake --build build
```

Run the server with default configuration and browse its testing websites.
```
cd build
./webserv
```

Open your favorite browser and enter in its address bar:
```
http://localhost:8248/
OR
http://localhost:8248/servernames/koda.com/index.html
```