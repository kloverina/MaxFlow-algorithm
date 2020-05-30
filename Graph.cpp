#include "Graph.h"
#include<string>
#include <fstream>
#include <sstream>
#define LAST_VERTEX 'T'
#define START_VERTEX 'S'
using std::cout;
using std::endl;


/////////////////  PUBLIC METHODS  /////////////////////

//reads data from file
void Graph::getFrom(std::string path)
{
	std::ifstream file(path.c_str());
	std::string line;
	std::string flag_LastV; //checks if the last vertex is 'T'
	bool first_line = true;
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
			if (first_line)
				if (from.at(0) != START_VERTEX)
					throw std::invalid_argument("First vertex must be 'S'!");
			AddEdge(from.at(0), to.at(0), stoi(weight));
			first_line = false;
			flag_LastV = to;
		}

		if (!EdgeList.get_size())
			throw std::invalid_argument("File is empty!");
		if (flag_LastV != "T")
			throw std::invalid_argument("Last vertex must be 'T'!");
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
void Graph::AddEdge(char Start_Vertex, char End_Vertex, size_t capacity)
{
	if (!VertexMap.contains(Start_Vertex))
		VertexMap.insert(Start_Vertex,VertexMap.get_size());
	if (!VertexMap.contains(End_Vertex))
		VertexMap.insert(End_Vertex, VertexMap.get_size());
	EdgeList.push_back(new Edge(0, capacity, VertexMap.find(Start_Vertex), VertexMap.find(End_Vertex)));
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
		if (cur_edge->Start_Vertex == source)
		{
			//flow becames equal to capacity
			cur_edge->flow = cur_edge->capacity;

			//counts excess flow for the nearest vertex to v(start vertex)
			VertexList.at(cur_edge->End_Vertex)->e_flow += cur_edge->flow;

			//ads edge from v(end vertex) to s with zero capacity
			EdgeList.push_back(new Edge(-cur_edge->flow, 0, cur_edge->End_Vertex, source));
		}
	}
	int Start_Vertex;
	while ((Start_Vertex = OverFlowVertex()) != -1)
	{
		if (!Push(Start_Vertex))
			Relabel(Start_Vertex);
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
bool Graph::Push(int vertex)
{
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto current_edge = iterator->next();
		if (current_edge->Start_Vertex == vertex)
		{
			//if the flow is equal to capacity 
			if (current_edge->flow == current_edge->capacity)
				continue;

			//Pushing is possible only if height of the vertex is less then height of an overflowed vertex
			auto cur_vertex = VertexList.at(vertex);
			if (cur_vertex->height > VertexList.at(current_edge->End_Vertex)->height)
			{
				
				//pushed flow = the least from remaining flow on the edge or exess flow
				int flow;
				if (current_edge->capacity - current_edge->flow < cur_vertex->e_flow)
					flow = current_edge->capacity - current_edge->flow;
				else  flow = cur_vertex->e_flow;
				cur_vertex->e_flow -= flow;
				VertexList.at(current_edge->End_Vertex)->e_flow += flow;

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
void Graph::Relabel(int vertex)
{
	int max_height = INT_MAX;

	//finding neighbor-vertex with the least height
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto cur_edge = iterator->next();
		if (cur_edge->Start_Vertex == vertex)
		{
			if (cur_edge->flow == cur_edge->capacity)
				continue;
			auto cur_vertex = VertexList.at(cur_edge->End_Vertex);
			if (cur_vertex->height < max_height)
			{
				max_height = cur_vertex->height;
				VertexList.at(vertex)->height = max_height + 1;
			}
		}
	}
}


//creates a reverse edge 
void Graph::ReverseEdgeFlow(Edge*& current_edge, int flow)
{
	size_t Start_Vertex = current_edge->End_Vertex;
	size_t End_Vertex = current_edge->Start_Vertex;
	auto iterator = EdgeList.create_list_iterator();
	while (iterator->has_next())
	{
		auto reverse_edge = iterator->next();
		if (reverse_edge->End_Vertex == End_Vertex && reverse_edge->Start_Vertex == Start_Vertex)
		{
			reverse_edge->flow -= flow;
			return;
		}
	}
	EdgeList.push_back(new Edge(0, flow, Start_Vertex, End_Vertex));
}