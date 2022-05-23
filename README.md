# <center> Introduction </center>


This project is difficult for me because I am learning c++ without anyone's help.
I'm writing this to practice my c++, and if the project goes well, I'll post the release of the program.

You can use the code in this project, or even the entire project, for your own purposes, but be sure to reference this repository when using code from it.
I trust you.

For me, the word "module" and the word dynamic load library (dll) are one and the same. And yes, I know that these are slightly different concepts.



## <center> content </center>


* [JSONER Project](#-jsoner-project-)
    * [JSONER](#-file-jsoner-)
    * [class window](#-window-class-)
* [Filer](#-filer-)
* [JSON](#-json-module-)
* [XML](#XML)
* [Author](#author)


# <center> JSONER project </center>


This project was very easy for me because I already have experience with winapi applications.
JSONER consists of the following files:
* JSONER.cpp - main file. From this file begining all app 
* JSONER.h - header file of JSONER.cpp
* Window.h and Window.cpp - files for window creating and etc acts  with window
* Menu_Logic.h and Menu_Logic.cpp - window menu handler files
* Dialog_Creator.h and Dialog_Creator.cpp - files for dialog boxes
* Headers.h - just file...
* Resource.h and JSONER.rc - window resources files
* other files

# <center> file JSONER </center>

From this file started program because this file have a main function.  
In main function created a mainWindow and setting window handlers.  
It is main file of project.

# <center> Window class </center>


This class create the graphics windows. Constructor takes one element - hInstance. It is needed to register and create the window.
Method init create the window and returns false on error. Other methods are used for window control.


# <center> Filer </center>


Dependencie of JSONER. Compiling to Filer.dll file. This module work with file system and read or write a json and xml. Creates the window for creating new project or openes already created project.


# <center> JSON module </center>


Compiling to JSON.dll located in the `components` folder. This module read and write json. I separated it into a separate module (dll) because it was more convenient for me. Interaction with it occurs only through Filer.



## <center>XML</center>

Module for xml reading and writing. Uses through Filer or directly. This library use libxml.dll iconv.dll and zlib.dll. It dll's located in the `libraries/libs` folder. 


## <center>Author</center>

* **Author-name**: Ashabaly
* **Pseudonym**: As-Almas
* **Company-name**: coming soon (if possible)
* **License**:  No license
