### `Main repository goal`

This repository goal is to create a multithreaded server using only the standard linux socket libraries.
 
 #### `How to launch`
 0. Clone this repository, write the following commands and run the server.
 
 ```
 mkdir build && cd build
 cmake .. &&  make 
 ./server
```
 1.  As the client you can use Telnet. Download it and connect to the server using the following commands.  
  ```
 telnet localhost 8888
```
  
 3. Using Telnet write the following commands:
```
 <seq> <initial digit> <inteval>
 <initial digit> ∈ [0 : 9999]
 <interval> ∈ [-9999 : 9999]
 
 For example:
   seq1 8989 -2323
   seq2 9999 9999
   seq3 10 -1
   export seq
```
