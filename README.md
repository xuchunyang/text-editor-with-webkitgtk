## About
A simple rich text editor using Gtk+/WebKit

## Build && Run
```shell
$ cd src/
$ gcc main.c `pkg-config --cflags --libs gtk+-3.0 webkitgtk-3.0`
$ ./a.out test.html
```

## TODO 
- [x] Use GtkApplication class
- [x] Create window UI (menubar, toolbar and gmenu) with XML and GtkBuilder
- [x] Renderer html using WebKit1 API
- [x] Set html editable and change text style with js (Rich text editor)
- [ ] Port to WebKit2 API
- [ ] Use CSS to customize background

## Possible Questions
* How to debugg Gtk+ app?

* What is the main difference between WebKitGTK+ API and WebKit2Gtk+ API?

* What are the advantages and disadvantages of using WebKit to create text editor?  
