# QRcode

Serial (RS232 or USB-to-serial) QRCode reader device interface, using Qt, in Linux.

Used to read from serial port (it can be anything there, not just a qrcode reader) and prints the whole message for analisys of a protocol, for instance.

If assigned to, it translates a base64 encoded string to an hex string.

## Tools

There are two excelent sites:

https://base64.guru/converter/encode/hex  -  convert to and from hex and base64 strings (among other options);

https://www.the-qrcode-generator.com/  -  encodes any string into a qrcode (and allows you to save it);

## Basics

The serial reading class thread might be of interest, as it is mostly a blocking "read" from standard Linux "unistd.h", but still set up with a reasonable time-out, placed inside a "while" loop, that exits if there is at least one character in the input buffer (so the thread might be interrupted almost at any time). Then, if the terminator character is found in the buffer, a callback function is used for serving the complete input string.

There is also a higher abstraction layer class for the serial port, that defines that callback function as pure virtual, so that another class, now from the application layer, might inherit from it and implement that same callback.

Enjoy!

## Statistics



cloc|github.com/AlDanial/cloc v 1.82  T=0.05 s (191.7 files/s, 39516.8 lines/s)
--- | ---

Language|files|blank|comment|code
:-------|-------:|-------:|-------:|-------:
C++|5|133|89|805
Qt|1|0|0|549
C/C++ Header|4|121|229|135
--------|--------|--------|--------|--------
SUM:|10|254|318|1489
