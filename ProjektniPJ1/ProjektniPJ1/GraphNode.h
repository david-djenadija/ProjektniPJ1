#pragma once
#include <memory>
#include <type_traits>
#include "Operation.h"
#include "AddOperation.h"
#include "SubtractOperation.h"
#include "ScalarAddOperation.h"
#include "ScalarMultiplicationOperation.h"
#include "MultiplicationOperation.h"
#include "NegationOperation.h"
template<typename T, int... Args>
class GraphNode {
private:
	double scalar;
	std::shared_ptr<Tensor<T, Args...>> TensorData;
	std::shared_ptr<Operation<T,Args...>> OperationData;
public:
	GraphNode(std::shared_ptr<Tensor<T, Args...>> element)
	{
		auto ptr = std::dynamic_pointer_cast<Tensor<T, Args...>>(element);
		if (ptr != nullptr)
		{
			TensorData = element;
			OperationData = nullptr;
			scalar = 0;
		}
		else
		{
			TensorData = nullptr;
		}
	};
	GraphNode(double scalar1) {
		scalar = scalar1;
		TensorData = nullptr;
		OperationData = nullptr;
	};
	GraphNode(std::shared_ptr<Operation<T, Args...>> op) {
		OperationData = op;
		TensorData = nullptr;
		scalar = 0;
	};
	bool isAddOperation() const {
		auto ptr = std::dynamic_pointer_cast<AddOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}else return false;
	};
	bool isSubtractOperation() const {
		auto ptr = std::dynamic_pointer_cast<SubtractOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}else return false;
	};
	bool isScalarAddOperation() const {
		auto ptr = std::dynamic_pointer_cast<ScalarAddOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}
		else return false;
	};
	bool isMultiplicationOperation() const {
		auto ptr = std::dynamic_pointer_cast<MultiplicationOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}
		else return false;
	};
	bool isScalarMultiplicationOperation() const {
		auto ptr = std::dynamic_pointer_cast<ScalarMultiplicationOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}
		else return false;
	};
	bool isNegationOperation() const {
		auto ptr = std::dynamic_pointer_cast<NegationOperation<T, Args...>> (OperationData);
		if (ptr != nullptr) {
			return true;
		}
		else return false;
	};
	bool isOperation() const {
		if (OperationData != nullptr)return true;
		return false;
	};
	Tensor<T, Args...>& getTensorData() const
	{
		return *(this->TensorData);
	};
	Operation<T, Args...>& getOperationData() const {
		return *(this->OperationData);
	};
	double getScalar() const {
		return scalar;
	};
	bool isTensor() const
	{
		if (TensorData != nullptr)
			return true;
		return false;
	};
	bool isScalar() const {
		if (scalar != 0)return true;
		return false;
	}
};