#pragma once
#include "Operation.h"
template<typename T>
class CalculationGraph {
private:
	int rang;
	Operation<T>* arr;
	int* ms;
};