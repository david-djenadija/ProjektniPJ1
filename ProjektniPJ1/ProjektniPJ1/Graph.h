#pragma once
#include "GraphNode.h"
#include <vector>
#include <iostream>
#include <cctype>
#include <string>
#include <fstream>
#include <stack>
template<typename T, int... Args>
class Graph {
private:
	std::vector<GraphNode<T, Args...>> elements;
	std::vector<std::vector<int>> ms;
public:
	Graph(decltype(elements) el, decltype(ms) msc);
	Graph() {};
	bool addNode(const GraphNode<T, Args...>& node);
	bool addConnection(const size_t i,const size_t j);
	size_t getElementsSize() const;
	size_t getMsSize() const;
	int getConnection(size_t i,size_t j) const;
	Tensor<T, Args...> calculate(const size_t i) ; 
	GraphNode<T, Args...> getNode(size_t i) const;
	void ispisVeza() const;
	friend std::ofstream& operator<<(std::ofstream& file, const Graph<T, Args...>& a) {
		for (size_t i = 0; i < a.getElementsSize(); i++) {
			if (a.getNode(i).isTensor()) {
				file << "T" << std::endl;
				file << a.getNode(i).getTensorData() << std::endl;
			}
			else if (a.getNode(i).isOperation())
				file << a.getNode(i).getOperationData().getId() << std::endl;
			else if (a.getNode(i).isScalar())
				file << a.getNode(i).getScalar() << std::endl;
		}
		file << std::endl;
		file << 'M' << std::endl;
		for (size_t i = 0; i < a.getElementsSize(); i++) {
			for (size_t j = 0; j < a.getElementsSize(); j++)
				file << a.getConnection(i, j) << " ";
			file << std::endl;
		}
		return file;
	};
	friend std::ifstream& operator>>(std::ifstream& file, Graph<T, Args...>& data) {
		char ch;
		while (file.get(ch)) {
			if (ch == 'B') {
				char operationId;
				file >> operationId;
				if (operationId == '+') {
					data.addNode(GraphNode<T, Args...>(std::make_shared<AddOperation<T, Args...>>(AddOperation<T, Args...>())));
				}
				else if (operationId == '-') {
					data.addNode(GraphNode<T, Args...>(std::make_shared<SubtractOperation<T, Args...>>(SubtractOperation<T, Args...>())));
				}
				else if (operationId == '*') {
					data.addNode(GraphNode<T, Args...>(std::make_shared<MultiplicationOperation<T, Args...>>(MultiplicationOperation<T, Args...>())));
				}
				else if (operationId == '!') {
					data.addNode(GraphNode<T, Args...>(std::make_shared<NegationOperation<T, Args...>>(NegationOperation<T, Args...>())));
				}
				else if (operationId == 'S') {
					//skalarne operacije
					char pomocnichar;
					file >> pomocnichar;
					if (pomocnichar == '+') {
						data.addNode(GraphNode<T, Args...>(std::make_shared<ScalarAddOperation<T, Args...>>(ScalarAddOperation<T, Args...>())));
					}
					else if (pomocnichar == '*') {
						data.addNode(GraphNode<T, Args...>(std::make_shared<ScalarMultiplicationOperation<T, Args...>>(ScalarMultiplicationOperation<T, Args...>())));
					}
				}
			}
			else if (ch == 'M') {
				//ucitavanje matrice susjednosti

				for (size_t i = 0; i < data.getElementsSize(); i++) {
					for (size_t j = 0; j < data.getElementsSize(); j++) {
						int connection;
						file >> connection;
						if (connection > 0)
							data.addConnection(i, j);
					}
				}
			}
			else if (ch == 'T') {
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				Tensor<T, Args...>* pok = new Tensor<T, Args...>();
				file >> *pok;
				data.addNode(GraphNode<T, Args...>(std::make_shared<Tensor<T, Args...>>(*pok)));
			}
		}

		return file;
	};
	void findAllPaths(int start, int end, std::vector<std::vector<int>>& allPaths, std::vector<int>& currentPath, std::vector<bool>& visited) {
		visited[start] = true;
		currentPath.push_back(start);

		if (start == end) {
			allPaths.push_back(currentPath);
		}
		else {
			for (int neighbor = 0; neighbor < ms[start].size(); ++neighbor) {
				if (ms[start][neighbor] && !visited[neighbor]) {
					findAllPaths(neighbor, end, allPaths, currentPath, visited);
				}
			}
		}

		visited[start] = false;
		currentPath.pop_back();
	}

	std::vector<std::vector<int>> findAllPaths(int start, int end) {
		std::vector<std::vector<int>> allPaths;
		std::vector<int> currentPath;
		std::vector<bool> visited(elements.size(), false);

		findAllPaths(start, end, allPaths, currentPath, visited);

		return allPaths;
	}
	std::string findDerivativeOfPath(std::vector<int>& path) {
		std::string result;
		if (path.size() >= 2) {
			if (elements[path[0]].isAddOperation()|| elements[path[0]].isScalarAddOperation()) {
				path.erase(path.begin());
				return "1*" + findDerivativeOfPath(path);
			}
			else if (elements[path[0]].isSubtractOperation()) {
				int i;
				for ( i = 0; i < elements.size(); i++) {
					if (ms[path[0]][i] == 1)break;
				}
				if (i == path[1]) {
					path.erase(path.begin());
					return "1*" + findDerivativeOfPath(path);
				}
				else {
					path.erase(path.begin());
					return "-1*" + findDerivativeOfPath(path);
				}
			}
			else if (elements[path[0]].isMultiplicationOperation() || elements[path[0]].isScalarMultiplicationOperation()) {
				size_t i;
				for (i = 0; i < elements.size(); i++) {
					if (ms[path[0]][i] && i != path[1])break;
				}
				path.erase(path.begin());
				return "i"+std::to_string(i)+"*" + findDerivativeOfPath(path);
			}
			else if (elements[path[0]].isNegationOperation()) {
				path.erase(path.begin());
				return "-1*" + findDerivativeOfPath(path);
			}
		}
		else if (path.size() == 1) {
			if (elements[path[0]].isTensor() || elements[path[0]].isScalar())return "1";
		}
		else return "";
	}
	std::string findDerivativeOfMultiplePaths(std::vector<std::vector<int>> paths) {
		std::string pom="";
		size_t i;
		for (i = 0; i < paths.size(); i++) {
			pom += findDerivativeOfPath(paths[i]) ;
			size_t pos = pom.find("1*1");
			if (pos != std::string::npos) {
				pom.replace(pos, 3, "1");
			}
			if (i != paths.size() - 1)pom += "+";
		}
		return pom;
	}
	std::vector <std::vector<std::string>> jacobianMatrix(std::vector<std::vector<int>> paths) {
		std::vector < std::vector<std::string>> pom;
		
		for (size_t i = 0; i < elements.size(); i++) {
			std::vector<std::string> innerVector;
			for (size_t j = 0; j < elements.size(); j++) {
				
				if (i == j)innerVector.push_back("1");
				else
				innerVector.push_back(findDerivativeOfMultiplePaths(findAllPaths(i,j)));
			}
			pom.push_back(innerVector);
		}
		return pom;
	}
};

template<typename T, int ...Args>
inline Graph<T, Args...>::Graph(decltype(elements) el, decltype(ms) msc):elements(el),ms(msc)
{
}

template<typename T, int ...Args>
inline bool Graph<T, Args...>::addNode(const GraphNode<T, Args...>& node)
{
	elements.push_back(node);
	for (auto& each : ms) {
		each.push_back(0);
	}
	std::vector<int> pom;
	for (size_t pomvar = 0; pomvar < elements.size(); pomvar++) {
		pom.push_back(0);
	}
	ms.push_back(pom);
	
	
	return true;
}

template<typename T, int ...Args>
inline bool Graph<T, Args...>::addConnection(const size_t i, const size_t j)
{
	if (i < elements.size() && j < elements.size()) {
		ms[i][j] = 1;
		return true;
	}
	else return false;
}

template<typename T, int ...Args>
inline size_t Graph<T, Args...>::getElementsSize() const
{
	return elements.size();
}

template<typename T, int ...Args>
inline size_t Graph<T, Args...>::getMsSize() const
{
	return ms.size();
}

template<typename T, int ...Args>
inline int Graph<T, Args...>::getConnection(size_t i, size_t j) const
{
	return ms[i][j];
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Graph<T, Args...>::calculate(const size_t i) 
{
	Tensor<T, Args...> pom12;
	if (i < elements.size()) {
		Tensor<T, Args...> pom(0);
		Tensor<T, Args...> pomnozi(1);
		if (elements[i].isTensor())return elements[i].getTensorData();
		else if (elements[i].isScalar())return Tensor<T,Args...> (elements[i].getScalar());
		else { //Operacije
			size_t ind;
			if (elements[i].isAddOperation() || elements[i].isScalarAddOperation()) {
				for (ind = 0; ind < elements.size(); ind++) {
					if (ms[i][ind] && i != ind) {
						pom = pom + calculate(ind);
					}
				}
				return pom;
			}
			else if (elements[i].isSubtractOperation()) {

				for (ind = 0; ind < elements.size(); ind++) {
					if (ms[i][ind])break;
				}
				pom = calculate(ind);
				for (; ind < elements.size(); ind++) {
					if (ms[i][ind] && i != ind) {
						pom = pom - calculate(ind);
					}
				}return pom;
			}
			else if (elements[i].isNegationOperation()) {
				for (ind = 0; ind < elements.size(); ind++) {
					if (ms[i][ind])break;
				}
				return calculate(ind) * (-1);
			}
			else if (elements[i].isScalarMultiplicationOperation() || elements[i].isMultiplicationOperation()) {

				for (ind = 0; ind < elements.size(); ind++) {
					if (ms[i][ind] && i != ind) {
						pomnozi = pomnozi * calculate(ind);

					}
				}return pomnozi;
			}
			else return pom12;
		}
	}
	else return pom12;
}

template<typename T, int ...Args>
inline GraphNode<T, Args...> Graph<T, Args...>::getNode(size_t i) const
{
	return elements[i];
}

template<typename T, int... Args>
inline void Graph<T, Args...>::ispisVeza() const
{
	for (auto each : ms) {
		for (auto elem : each) {
			std::cout << elem;
		}
		std::cout << std::endl;
	}
}
