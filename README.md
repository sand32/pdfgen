# pdfgen
A simple HTML-to-PDF TCP server proof-of-concept

# Build
This project doesn't use qmake or Qt Creator in any way, because... why? Instead it uses the much more popular CMake. Assuming CMake is in your PATH, modify the `build/RunCMake.sh` script to point to the directory containing your Qt SDK and then run that script. It'll generate makefiles from which you can run `make && make install` to build the project. The CMake part of the process will take care of generating Qt's magical moc files which will then be included in your build. If you make any changes that require regeneration of those moc files, re-run the script.

# Reasoning
This project was created after writing a PDF rendering service in Node.js which utilized PhantomJS and being thoroughly disappointed by it. In addition, I then found out that PhantomJS was faltering in its maintenance status (likely in response to similar efforts via Chrome-Headless), so I decided to go to the source of the PDF rendering functionality and utilize Qt directly (which PhantomJS uses under the hood).

# Post Mortem
It's pretty unusual to use Qt outside of a client-side GUI context from what I've seen, but it worked fairly well. I still had to use a QApplication instance due to my necessary usage of the QWebEngine module which relies on it, otherwise it would've beeen preferrable to use QCoreApplication to avoid bringing in a bunch of GUI stuff.

In addition to the QWebEngine, I also use the QTcpServer to open a TCP socket. Normally, just using Qt for network communication would be a bit overkill, but I already needed it for the PDF rendering, so why not?

Outside of the simple Node-based test I've written, this project hasn't really been put through its paces yet, and unless I need something like this again, it may never be.

# Areas for Improvement
This project is just a rough POC and as such, there are many possible improvements to be made. Some that come to mind:
- TLS connections
- AuthN/Z (wouldn't have been necessary for my purposes, but still)
- Proper logging
- Expose metrics for monitoring (likely in Prometheus format)
- Multithreaded document rendering utilizing QThreadPool in some way (perhaps utilizing the QtConcurrent module)
- Better handling of client tracking (this is probably the inadequacy most revealing of my lack of inexperience working at this level)
