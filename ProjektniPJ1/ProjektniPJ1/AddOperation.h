#pragma once
#include "Operation.h"
template<typename T,int... Args>
class AddOperation : virtual public Operation<T, Args...> {
public:
	AddOperation();
	Tensor<T,Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2)const override {
		return op1 + op2;
	};
	
};

template<typename T,int... Args>
inline AddOperation<T, Args...>::AddOperation() : Operation<T,Args...>("B+")
{
}

