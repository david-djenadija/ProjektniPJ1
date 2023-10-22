#pragma once
#pragma once
#include "Operation.h"
template<typename T, int... Args>
class MultiplicationOperation : virtual public Operation<T, Args...> {
public:
	MultiplicationOperation();
	Tensor<T, Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2)const override {
		return op1 * op2;
	};

};

template<typename T, int... Args>
inline MultiplicationOperation<T, Args...>::MultiplicationOperation() : Operation<T, Args...>("B*")
{
}
