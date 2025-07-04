# charlottesWebServer
HTTP web server!

## How to use
1. Install dependencies - libpthread, gcc (other compilers _\*probably\*_ work too),
and build tools: git, make.

On Debian/Ubuntu you can use apt:
```
sudo apt install git make gcc libpthread-stubs0-dev
```

2. Download and compile source - run:
```
git clone https://github.com/amurph24/charlottesWebServer.git
cd charlottesWebServer
```
then:
```
make
```
If there are build errors, you may need to change the `LIBS` variable in the makefile.
Something like `-lpthread` --> `-pthread` will sometimes help.

3. run the executable - it's found in the `build` dir:
```
./build/cwserver 8000
```

You can use `make run` to run the server in the background,
and then kill **all** cwserver instances with `make kill`.

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
 - [ ] cl args
     - [ ] option for ip address (or at the very least, localhost)
