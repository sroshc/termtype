# ***termtype***

**A terminal typing game**

-Has a cool terminal manipulation library (doesn't use ncurses or any 3rd party terminal libraries)

-Will work in terminals that support ANSI escape codes in macOS and Linux

-Not compatible on Windows 

How to run:

    git clone https://github.com/sroshc/termtype.git
    cd termtype
    make
    ./termtype


./termtype accepts 2 integer terminal arguments, the number of words, and
the top X most used words in english that the words should be picked out from


For example: 
    
    ./termtype 10 200

This will give you the 10 of the top 200 most used words in English to type


- maybe wont work on macOS actually
