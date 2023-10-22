#pragma once
#include "Operation.h"
template<typename T, int... Args>
class NegationOperation : virtual public Operation<T, Args...> {
private:
	const T scalar = -1;
public:
	NegationOperation();
	Tensor<T, Args...>* operator()(Tensor<T, Args...> op1) {
		return op1 * scalar;
	};
	Tensor<T, Args...> operator()(Tensor<T, Args...> op1, Tensor<T, Args...> op2)const override {
		return op1 * op2;
	};
};

template<typename T, int... Args>
inline NegationOperation<T, Args...>::NegationOperation() : Operation<T, Args...>("B!")
{
}
