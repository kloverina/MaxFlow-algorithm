#include "Graph.h"
#include<string>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;


/////////////////  PUBLIC METHODS  /////////////////////

//reads data from file
void Graph::getFrom(std::string path)
{
	std::ifstream file(path.c_str());
	std::string line;
	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::string from, to, weight, other;
			std::istringstream iss(line);
			iss >> from >> to >> weight >> other;
			if (from.size() > 1 || from.empty() || to.size() > 1 || to.empty()
				|| weight.empty() || !other.empty())
				throw std::invalid_argument("Wrong data format in the file!");
			AddEdge(from.at(0), to.at(0), stoi(weight));
		}
		if (!EdgeList.get_size())
			throw std::invalid_argument("File is empty!");
		
	}
	else 
		throw std::logic_error("No file was found!");
	MaxFlow();
}


//returns MaxFlow
size_t Graph::getMaxFlow()
{
	if (VertexList.isEmpty())
		throw std::invalid_argument("Vertex list was empty!");
	if (VertexList.at(VertexList.get_size() - 1)->e_flow == 0)
		throw std::invalid_argument("There is no way from source to runoff!");
	return VertexList.at(VertexList.get_size() - 1)->e_flow;
}


/////////////////  PRIVATE METHODS  /////////////////////

//ads vertex to VertexMap and edge between them to EdgeList
void Graph::AddEdge(char u, char v, size_t capacity)
{
	if (!VertexMap.contains(u))
		VertexMap.insert(u,VertexMap.get_size());
	if (!VertexMap.contains(v))
		VertexMap.insert(v, VertexMap.get_size());
	EdgeList.push_back(new Edge(0, capacity, VertexMap.find(u), VertexMap.find(v)));
}

//counts maximum flow
void Graph::MaxFlow()
{
	for (int i = 0; i < VertexMap.get_size(); i++)  //create list of vertex
		VertexList.push_back(new Vertex(0, 0)); 
	
	//intialising the graph
	size_t source = 0; // index of the source
	VertexList.at(source)->height = VertexList.get_size();
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto cur_edge = iterator->next();
		if (cur_edge->u == source)
		{
			//flow becames equal to capacity
			cur_edge->flow = cur_edge->capacity;

			//counts excess flow for the nearest vertex to v
			VertexList.at(cur_edge->v)->e_flow += cur_edge->flow;

			//ads edge from v to s with zero capacity
			EdgeList.push_back(new Edge(-cur_edge->flow, 0, cur_edge->v, source));
		}
	}
	int u;
	while ((u = OverFlowVertex()) != -1)
	{
		if (!Push(u))
			Relabel(u);
	}
}

// returns the index of the overflowed vertex
int Graph::OverFlowVertex() 
{
	auto iterator = VertexList.create_list_iterator();
	int i = 1;
	iterator->next();
	while (iterator->has_next() && i < VertexList.get_size() - 1)
	{
		if (iterator->next()->e_flow > 0)
		{
			return i;
		}
		i++;
	}
	return -1;
}

// Pushes the flow from the overflowing vertex u
bool Graph::Push(int u)
{
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto current_edge = iterator->next();
		if (current_edge->u == u)
		{
			//if the flow is equal to capacity 
			if (current_edge->flow == current_edge->capacity)
				continue;

			//Pushing is possible only if height of the vertex is less then height of an overflowed vertex
			auto cur_vertex = VertexList.at(u);
			if (cur_vertex->height > VertexList.at(current_edge->v)->height)
			{
				
				//pushed flow = the least from remaining flow on the edge or exess flow
				int flow;
				if (current_edge->capacity - current_edge->flow < cur_vertex->e_flow)
					flow = current_edge->capacity - current_edge->flow;
				else  flow = cur_vertex->e_flow;
				cur_vertex->e_flow -= flow;
				VertexList.at(current_edge->v)->e_flow += flow;

				//adding remaining flow
				current_edge->flow += flow;
				ReverseEdgeFlow(current_edge, flow);
				return true;
			}
		}
	}
	return false;
}


//lifts the vertex
void Graph::Relabel(int u)
{
	int max_height = INT_MAX;

	//finding neighbor-vertex with the least height
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto cur_edge = iterator->next();
		if (cur_edge->u == u)
		{
			if (cur_edge->flow == cur_edge->capacity)
				continue;
			auto cur_vertex = VertexList.at(cur_edge->v);
			if (cur_vertex->height < max_height)
			{
				max_height = cur_vertex->height;
				VertexList.at(u)->height = max_height + 1;
			}
		}
	}
}


//creates a reverse edge 
void Graph::ReverseEdgeFlow(Edge*& current_edge, int flow)
{
	size_t u = current_edge->v;
	size_t v = current_edge->u;
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto reverse_edge = iterator->next();
		if (reverse_edge->v == v && reverse_edge->u == u)
		{
			reverse_edge->flow -= flow;
			return;
		}
	}
	EdgeList.push_back(new Edge(0, flow, u, v));
}