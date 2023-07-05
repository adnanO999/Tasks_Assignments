#create a Vertex and Face datatype using structure

#create a function readHeader that is used to know how many line the header contains in order to skip them
this function could be eliminated and integrated as while loop in the remaining functions
however this will help to get directly the number of lines

#create a function findFeatureNbr that find the the number of vertices and faces and return an array containing both values
this is done by finding the strings "element vertex " and "element face " then taking the substring that starts when the string ends
in other terms taking substring (that contains the nbr of vertices/faces) from the line and converting it to integrated

if the string is present line.find() will return the index at which the string is present else it will return -1

finally the file is closed and the array containing both counts is returned


#create a function that retuns a vector of vertices
by inspecting the ply file we notice that a vertex is composed of 3 float values

if we start reading from the begining of the file we need to skip the header to reach the float values
therefore the first while loop is used to iterate over the header

the second while loop creates a string stream that is used to separate the 3 floats
instead of creating a stream we could use regex expression to split the line on " "
(by researching it was found that the stream can be used to extract formatted data from a string) 

for each vertex we push it in the vertices vector
(creating a vector of vertices)

finaly we retun the vector of vertices

#we repeat the same procedure for getting faces however the difference is that we have a vector of int instead of 3 floats
the vector of int is useful because a face could have different number of parameters depending on its shape

we can skip the first number and not include it when returning the face structure
this can be done simply by using a continue statement if we are in the first iteration
or by reading and not pusishing the number in the vector of indices

#a ply library is available to read ply file directly. We need to install it and use it
include <ply/ply.h>

Bonus:

#for the thread part I created a simple thread that calls the getVertices function and wait for 1 second after loading the vertices
the same is done using a second thread to get the faces

#for the abstract class we need to define a pure virtual function that retrieves the vertices
and implement it in the derived class
 