# charlottesWebServer
HTTP web server!

## TODO
not necessarily in this order:
 - [x] more complete arg parsing
   - [x] ~~add ip as optional arg~~ (default should get current ip)
   - [x] error checking on passed args
   - [x] help message/usage
 - [x] ~~limit request resources to those in "html" folder~~ check for sneaky resource requests
   - [x] return requested files
   - [x] no requested resource should default to "index.html"
 - [ ] safety checks on buffers
 - [x] smarter compilation in makefile (file by file, instead of all files always)
 - [x] special behaviour for certain ports
   - [x] port 10007 will echo the HTTP request received
 - [ ] conform to HTTP 1.1 protocols
 - [ ] proper testing
   - [x] automated unit tests
   - [ ] call and response with make test - need to verify file contents
   - [x] check for memory leaks
 - [x] threading
