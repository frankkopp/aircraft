# FlyByWire Simulations - C++ WASM Development Guidelines

## Purpose
Reading and troubleshooting other developers code is hard. Also, new developers 
have a hard time to get started with developing new features and functionalities 
in the FlyByWire Simulations code base. This document aims to provide a set of
guidelines for the C++ WASM development in the FlyByWire Simulations code base 
which aims to readability,  maintainability of the code base which is easily 
accessible for new developers.

## Commenting
Good commenting is a very important part of writing readable and maintainable
software. This is especially true in a multi-developer project as the FlyByWire
Open-Source project certainly is. 

In general other developers should not have to read the actual code to understand
what classes, methods and members do and why they exist in the first place. 

In C++ in general, it should be sufficient to read the header file to understand 
the purpose and usage of a class or method/function. There should be little need 
to read the code in the definition files (cpp) to know what a class or any of its
members does or how it should be used. 
This is especially important for any public members of a class.
                                            
If you write comments think of the following questions:

- What? Why? Usage? How?
- What does the code do?
  - Give a short general description of what the code does
- Why does the code do it?
  - Explain the purpose of the code 
  - e.g. why is this function needed?
  - e.g. why is this variable needed?
- Hot to use the code?
  - Explain how to use the code
  - e.g. how to use the method/function?
- How does the code do it (optional)?
  - Although good code speaks for itself it is helpful to explain in very
    broad strokes how the code does it if the code has more complex logic
  - e.g. how does the function work?

Modern IDEs like VSCode or CLion provide a lot of features to help developers 
by simply hovering over a class, function, variable, etc. to get a quick 
look at the documentation. Have this in mind when writing comments.

It is good practice to comment the code as you write it. Often it is easier to
visualize what code should be written by writing parts of the documentation 
first. E.g. writing the header files first by declaring members and document 
these often helps to achieve a better and faster implementation.  

## Logging

TODO                                                          
- MSFS does not easily allow to attach a debugger for C++
- MSFS has no permanent logging
- Logging should not be excessive but allow to see where the code is at
- Logging should print any warning and errors to the console to make it easier 
  to find issues later.  

## C++ Code Style

## Tips

### Debugging
 


