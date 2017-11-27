# Get Them All
A C implementation of a web page downloader 

Description of program
--------------
Upon typing "make", the Makefile included will compile and link the necessary source files to produce the executable http-client. This program will download a webpage specified in the command line arguments. For example: 

./http-client www.gnu.org 80 /software/make/manual/make.html

will download the page at http://www.gnu.org/software/make/manual/make.html on port 80

Note: the client downloads any file type, not just HTML files. 

Implementation details
--------------
The client opens a socket connection to the host that the user entered as a command line argument. It sets a HTTP 1.0 GET request to the host. The GET request looks like this:

GET /path/file.html HTTP/1.0

[zero or more headers ...]

[a blank line]

The client will parse the response from the host server. The response for a successful request looks like this:

HTTP/1.0 200 OK

Date: Fri, 21 Nov 2017 20:51:39 GMT

Content-Type: text/html

Content-Length: 1354
 
<html>

<body>

<h1>Happy New Millennium!</h1>

(more file contents)

        .
        .
        .
        
</body>

</html>


The socket is closed when the entire file has been received.

Expected Output
--------------
The file that the target URL points to will be downloaded to the http-client directory. 

In the event of an unsuccessful GET request, the program prints the first line of the response (e.g. HTTP/1.0 404 NOT FOUND) and terminates.

