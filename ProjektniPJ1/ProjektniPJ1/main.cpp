#include "Tensor.h"
#include "Operation.h"
#include "AddOperation.h"
#include "SubtractOperation.h"
#include <vector>
#include <iostream>
#include "GraphNode.h"
#include <memory>
#include <type_traits>
#include "Graph.h"
#include <fstream>
int main() {
	Tensor<int, 1, 2, 3> t;
	Tensor<int, 1, 2, 3> m;
	std::vector<Tensor<int, 1, 2, 3>> vek;
	t.addElement(1);
	t.addElement(2);
	t.addElement(3);
	//std::cout << t.getDataElement(1);
	m.addElement(3);
	m.addElement(3);
	m.addElement(3);
	vek.push_back(t);
	vek.push_back(m);
	SubtractOperation<int, 1, 2, 3> m1;
	AddOperation<int, 1, 2, 3> m0;
	MultiplicationOperation<int, 1, 2, 3> m2;

	//std::shared_ptr<SubtractOperation<int, 1, 2, 3>> pom = std::shared_ptr<SubtractOperation<int, 1, 2, 3>>(&m1);
	//GraphNode<int, 1, 2, 3> cvor(pom);
	//std::shared_ptr<GraphNode<int, 1, 2, 3>> pom = std::make_shared<GraphNode<int, 1, 2, 3>>(std::make_shared<SubtractOperation<int, 1, 2, 3>>(m1));
	//GraphNode<int, 1, 2, 3 > pom(std::make_shared<SubtractOperation<int, 1, 2, 3>>(m1));
	double broj = 5;
	GraphNode<int, 1, 2, 3 > pom(std::make_shared<Tensor<int, 1, 2, 3>>(t));
	GraphNode<int, 1, 2, 3 > pom1(std::make_shared<Tensor<int, 1, 2, 3>>(m));
	GraphNode<int, 1, 2, 3 > pom2(std::make_shared<Tensor<int, 1, 2, 3>>(Tensor<int, 1, 2, 3>(broj)));
	GraphNode<int, 1, 2, 3 > pom3(std::make_shared<AddOperation<int, 1, 2, 3>>(m0));
	GraphNode<int, 1, 2, 3 > pom4(std::make_shared<SubtractOperation<int, 1, 2, 3>>(m1));
	GraphNode<int, 1, 2, 3 > pom5(std::make_shared<MultiplicationOperation<int, 1, 2, 3>>(m2));
	Graph<int, 1, 2, 3> graf;
	graf.addNode(pom5);
	graf.addNode(pom4);
	graf.addNode(pom);
	graf.addNode(pom1);
	graf.addNode(pom3);
	graf.addNode(pom2);

	graf.addConnection(0, 1);

	graf.addConnection(0, 4);
	graf.addConnection(1, 2);
	graf.addConnection(1, 3);
	graf.addConnection(4, 3);
	graf.addConnection(4, 5);
	//graf.ispisVeza();

	//std::cout << graf.calculate(0).getDataElement(0);
	std::ofstream FileOut;
	FileOut.open("output.txt");
	FileOut << graf;
	FileOut.close();
	//graf.ispisVeza();
	std::ifstream FileIn;
	FileIn.open("output.txt");
	Graph<int, 1, 2, 3> graf1;
	FileIn >> graf1;
	FileIn.close();
	FileOut.open("output1.txt");
	FileOut << graf1;
	FileOut.close();
	std::vector<std::vector<int>> allPaths = graf.findAllPaths(0, 3);

	//std::cout << graf.findDerivativeOfMultiplePaths(allPaths);
	//graf1.ispisVeza();
	//std::cout << pomocni.getDataElement(0);
	//std::cout << cvor.isSubtractOperation();
	return 0;
}