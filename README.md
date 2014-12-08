OpenKeyLogger
=============

Simple keylogger for Windows with HTML output and FTP upload.

##Installation
###Standalone (local storage)
OpenKeyLogger should compile on any Windows machine without additional configuration. Just make sure to comment out the `#define USE_FTP` line in _keylogger.h_.
###With SFML (FTP storage)
OpenKeyLogger uses the Simple and Fast Multimedia Library (SFML) to upload logs to a FTP server. To enable this feature, make sure to: 

1. define USE_FTP in keylogger.h
2. link to these libraries 
    3. ws2_32.lib
	4. sfml-main.lib
	5. sfml-system-s.lib
	6. sfml-network-s.lib

For instructions on how to compile SFML, see http://sfml-dev.org/tutorials/2.0/compile-with-cmake.php

##TODO
- refactor private methods
- name keylogfile with date
- lifetime
