# Notes

**Compiled on**
- *Intel Pentium*  

**Toolchain vendor**
- *GNU GCC 5.1.0 TDM (MingW)*  
- *Compiler and linker flags can be found in "compile.bat"*   

**SFML Version**
- *2.5.0*  

**Operating System**
- *Windows 7 x86*   

# Current Goals

- [x] Start development on a simple, extendable TCP Server and Client using the SFML Network library - Project started
- [ ] Develop a starting foundation for connection, authentication and data transmission (80/100%) - Needs testing 
- [ ] Have the TCP service running on an internal separate thread (0/100%)
- [ ] Make some sort of homebrew authentication (25/100%) - Simple GUUID assignment, unencrypted, needs work
- [ ] Solidify the internal mechanism, make it private and develop an interface for extended objects to use (0/100%)


# What this is

This is an attempt at making a useful network library for educational and personal use. Code in here will collect as I learn new perspectives, systems, methodologies and ideas. I will be using the SFML Network library to create a framework for an easy intergration of data exchange into programs without having to worry about the initial setup. I would like the server and client extendable with a smooth interface to allow for easy modification of data processing depending on Server or Client criteria. In time I hope to really single out the core mechanisms of data exchange and handle everything in it's own thread with a safe way of passing and retrieving data.
