### `Main repository goal`

This repository goal is to create a multithreaded server using only the standard linux socket libraries.
 
 #### `How to launch`
 0. Clone this repo, write these commands and run the server exe.
 
 ```
 mkdir build && cd build
 cmake .. &&  make 
 ./server
```
 1.  As the client you can use Telnet. Download it and connect to the server using the following commands.  
  ```
 telnet localhost 8888
```
  
 3.  Write the second commands using the telnet:
```
 <seq> <initial digit> <inteval>
   
 For example:
   seq1 8989 -2323
   seq2 1000 1000
   seq3 -9999 9999
```
 4. Write the next command to get all sequences
 ```
  export seq
```
