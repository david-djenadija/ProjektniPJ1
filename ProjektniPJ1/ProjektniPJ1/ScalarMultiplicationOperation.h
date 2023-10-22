#pragma once
#include "Operation.h"
template<typename T, int... Args>
class ScalarMultiplicationOperation : virtual public Operation<T, Args...> {
private:
	
public:
	ScalarMultiplicationOperation();
	Tensor<T, Args...> operator()(Tensor<T, Args...> op1, T scalar) {
		return op1 * scalar;
	};
	Tensor<T, Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2)const override {
		return op1 * op2;
	};
};

template<typename T, int... Args>
inline ScalarMultiplicationOperation<T, Args...>::ScalarMultiplicationOperation() : Operation<T, Args...>("BS*")
{
}
