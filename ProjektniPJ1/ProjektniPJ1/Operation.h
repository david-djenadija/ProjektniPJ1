#pragma once
#include <vector>
#include "Tensor.h"	
#include <string>
template<typename T, int... Args>
class Operation {
protected:
	std::string Id;

public:
	Operation() {};
	Operation(std::string Id1);
	virtual Tensor<T, Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2)const = 0;
	std::string getId() const { return this->Id; };
};


template<typename T,int... Args>
inline Operation<T,Args...>::Operation(std::string Id1): Id(Id1)
{
}


