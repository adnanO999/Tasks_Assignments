#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

// the code and logic are explained in readme file

struct Vertex
{
    float x, y, z;
};

struct Face
{
    std::vector<int> indices;
};
// create abstract class
class AbstractVertex
{
public:
    // create pure virtual functions
    virtual std::vector<Vertex> getVertices(std::string fileName) = 0;
};

// create derived class
class Vertex1 : public AbstractVertex
{
public:
    // implement pure virtual function
    std::vector<Vertex> getVertices(std::string fileName) override
    {
    }
};

// function to read the header and extract the number of lines to be skipped
int readHeader(std::string fileName)
{
    int lineCount = 0;
    std::ifstream myFile(fileName);

    if (!myFile.is_open())
        return -1;

    std::string line;
    while (std::getline(myFile, line))
    {
        lineCount++;
        if (line == "end_header")
            break;
    }

    return lineCount;
}

// function to get nbr of vertices faces

int *findFeatureNbr(std::string fileName)
{

    int *featureNbr = new int[2];
    std::ifstream myFile(fileName);
    int lineInHeader = readHeader(fileName);

    if (!myFile.is_open())
    {
        featureNbr[0] = -1;
        featureNbr[1] = -1;
        return featureNbr;
    }

    std::string line;
    std::string vertex_elt = "element vertex ";
    std::string face_elt = "element face ";
    int vertexCount = 0;
    int faceCount = 0;

    while (lineInHeader > 0 && std::getline(myFile, line))
    {

        lineInHeader--;

        int index_v = line.find(vertex_elt);
        int index_f = line.find(face_elt);

        if (index_v != -1)
            vertexCount = std::stoi(line.substr(vertex_elt.length()));

        if (index_f != -1)
            faceCount = std::stoi(line.substr(face_elt.length()));

        featureNbr[0] = vertexCount;
        featureNbr[1] = faceCount;
    }

    myFile.close();
    return featureNbr;
}

// main functions
// same concept that is explained in readme file
std::vector<Vertex> getVertices(std::string fileName)
{
    std::ifstream myFile(fileName);
    std::vector<Vertex> vertices;
    int lineToStart = readHeader(fileName);
    int *featureNbr = findFeatureNbr(fileName);
    int verticesNbr = featureNbr[0];

    if (!myFile.is_open())
        return vertices;

    else
    {

        std::string line;

        while (lineToStart > 0 && std::getline(myFile, line))
            lineToStart--;

        while (verticesNbr > 0 && std::getline(myFile, line))
        {
            std::istringstream vertexCoordinates(line);
            Vertex vertex;
            if (vertexCoordinates >> vertex.x >> vertex.y >> vertex.z)
            {
                vertices.push_back(vertex);
            }
            verticesNbr--;
        }
    }
    return vertices;
}

std::vector<Face> getFaces(std::string fileName)
{
    std::ifstream myFile(fileName);
    std::vector<Face> faces;
    int *featureNbr = findFeatureNbr(fileName);
    int verticesNbr = featureNbr[0];
    int facesNbr = featureNbr[1];
    int lineToStart = readHeader(fileName) + verticesNbr;

    if (!myFile.is_open())
        return faces;

    else
    {
        std::string line;

        while (lineToStart > 0 && std::getline(myFile, line))
            lineToStart--;

        while (facesNbr > 0 && std::getline(myFile, line))
        {
            std::istringstream faceIndices(line);
            Face face;
            int index;

            while (faceIndices >> index)
                face.indices.push_back(index);

            faces.push_back(face);
            verticesNbr--;
        }
    }
    return faces;
}

int main()
{
    // testing all functions
    int *featureNbr = findFeatureNbr("test.ply");
    std::cout << featureNbr[0] << " " << featureNbr[1] << std::endl;

    int lineCOunt = readHeader("test.ply");
    std::cout << lineCOunt << std::endl;

    // with thread
    std::thread loadVertices([&]()
                             { std::vector<Vertex> vertices = getVertices("test.ply"); 
                             std::this_thread::sleep_for(1s); });

    std::thread loadFaces([&]()
                          { std::vector<Face> faces = getFaces("test.ply"); 
                             std::this_thread::sleep_for(1s); });

    loadVertices.join();
    loadFaces.join();

    // without a thread
    std::vector<Vertex>
        vertices = getVertices("test.ply");
    for (Vertex vertex : vertices)
    {
        std::cout << "Vertex: " << vertex.x << ", " << vertex.y << ", " << vertex.z << std::endl;
    }

    std::vector<Face> faces = getFaces("test.ply");
    for (Face face : faces)
    {
        std::cout << "Face: ";

        for (int index : face.indices)
            std::cout << index << " ";

        std::cout << std::endl;
    }
    //////////////////////////////////////////
}