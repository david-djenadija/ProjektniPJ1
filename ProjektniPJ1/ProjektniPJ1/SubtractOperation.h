#pragma once
#include "Operation.h"
template<typename T, int... Args>
class SubtractOperation : virtual public Operation<T, Args...> {
public:
	SubtractOperation();
	Tensor<T, Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2) const override{
		return op1 - op2;
	};

};

template<typename T, int... Args>
inline SubtractOperation<T, Args...>::SubtractOperation() : Operation<T, Args...>("B-")
{
}
