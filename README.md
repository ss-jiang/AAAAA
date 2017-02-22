# Nginx Clone Webserver
A webserver written in C++ using the Boost API. This webserver supports a common
RequestHandler interface/abstract base class that can be used to easily implement
any handler.

**Authors**:
* Ryan Peterman
* Aditya Raju
* Yilei Zhang

## Important Commands
To compile:

``` make ```

To run:

``` ./web-server <config-file> ```

To test:

``` make test ```

To get boost (Ubuntu):

``` sudo apt-get install libboost-all-dev ```

## Config File Format
``` bash
# port the webserver is listening on
port <port-num>;

# how to connected a url to a handler
path <url-mapping> <handler-name> {
    <additional parameters as expected by handler>
}

# Handler to be called if no url matches
default <handler-name> {}
```