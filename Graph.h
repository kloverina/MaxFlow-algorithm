#pragma once
#include <iostream>
#include "Map.h"
#include "List.h"



class Graph
{

	// Edge of graph
	struct Edge
	{
		int flow;
		size_t capacity;
		size_t Start_Vertex; //pointer to start vertex
		size_t End_Vertex; //pointer to finish vertex
		Edge(int flow, size_t capacity, size_t Start_Vertex, size_t End_Vertex)
		{
			this->flow = flow;
			this->capacity = capacity;
			this->Start_Vertex = Start_Vertex;
			this->End_Vertex = End_Vertex;
		}

	};

	//Vertex of the Graph
	struct Vertex
	{
		size_t height; //height of the vertex 
		size_t e_flow; //excess pre-flow
		Vertex(size_t h, size_t e_flow)
		{
			this->height = h;
			this->e_flow = e_flow;
		}

	};

	List<Vertex*> VertexList;
	List<Edge*> EdgeList;
	Map<char, size_t> VertexMap;

	void AddEdge(char, char, size_t);
	void MaxFlow();
	bool Push(int ); // push function
	void Relabel(int ); //lift function
	void ReverseEdgeFlow(Edge*& , int ); // flow reversal function
	int OverFlowVertex(); //overflow check function, returns the index of the full vertex

public:
	~Graph()
	{
		VertexMap.clear();
		VertexList.clear();
		EdgeList.clear();
	}
	Graph() {};
	void getFrom(std::string); //reads data from file
	size_t getMaxFlow(); //returns MaxFlow
	
};

