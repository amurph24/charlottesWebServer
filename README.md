# charlottesWebServer
HTTP web server!

## TODO
not necessarily in this order:
 - [ ] more complete arg parsing
   - [x] ~~add ip as optional arg~~ (default should get current ip)
   - [ ] error checking on passed args
   - [ ] help message/usage
 - [ ] limit request resources to those in "html" folder
   - [ ] no requested resource should default to "index.html"
 - [ ] safety checks on buffers
 - [ ] smarter compilation in makefile (file by file, instead of all files always)
 - [ ] special behaviour for certain ports (e.g. port 7 should echo request message)
 - [ ] conform to HTTP 1.1 protocols
