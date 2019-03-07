#pragma once

#include <vector>

class Node;

struct Edge {
	Node& n1;
	Node& n2;
	int cost;
};

class Node {

public:
	Node(float _x, float _y);

	//connects two nodes with an edge of 'edgeCost'
	void addNode(Node& n, int edgeCost);

	//determines the opposing node on an edge.
	//if the edge does not contain the current node, it will return the current node
	Node getOtherNode(Edge e);

	//links two nodes together via and edge. Updates both nodes edges vector
	inline void addEdge(Edge e) { edges.push_back(e); }

	//setters
	inline void setX(float _x) { x = _x; }
	inline void setY(float _y) { y = _y; }
	inline void setPosition(float _x, float _y) { x = _x; y = _y; }
	inline void setCost(int c) { cost = c; }
	inline void setPrev(Node* n) { prev = n; }
	inline void setOnPath(bool pathStatus) { onPath = pathStatus; }
	inline void setVisited(bool v) { visited = v; }
	inline void setObstacle(bool o) { obstacle = o; }

	//getters
	inline bool isOnPath() const { return onPath; }
	inline bool isVisited() const { return visited; }
	inline bool isObstacle() const { return obstacle; }
	inline int getId() const { return id; }
	inline float getX() const { return x; }
	inline float getY() const { return y; }
	inline int getCost() const { return cost; }
	inline Node* getPrev() { return prev; }
	inline std::vector<Edge> getEdges() { return edges; }


protected:
	//vector containing all edges the connect connects to.
	//this can be used to create a graph with any number of connectivity
	std::vector<Edge> edges;
	//id used to compare nodes.
	const int id;

	int cost;
	bool onPath;
	bool visited;
	bool obstacle;
	float x, y;
	Node* prev;

};