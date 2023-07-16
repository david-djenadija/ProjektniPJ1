#pragma once
#include <vector>
template<typename T>
class Tensor {
private:
	std::vector<size_t> dimensions;
	std::vector<T> data;
public:
	Tensor();
	Tensor(const std::vector<size_t>& dims);
	T& operator()(const std::vector<size_t>& indices);
	void resizeData();
	void addDimension(size_t dim);
	size_t computeIndex(const std::vector<size_t>& indices) const;
	void removeDimension(int index);
	void removeElement(const std::vector<size_t>& indices);
	void removeElementAt(size_t index);
};

template<typename T>
inline Tensor<T>::Tensor()
{
}

template<typename T>
inline Tensor<T>::Tensor(const std::vector<size_t>& dims) : dimension(dims)
{
	size_t size = 1;
	for (size_t dim : dimensions) {
		size *= dim;
	}
	data.resize(size);
}

template<typename T>
inline T& Tensor<T>::operator()(const std::vector<size_t>& indices)
{
	size_t index = computeIndex(indices);
	return data[index];
}

template<typename T>
inline void Tensor<T>::resizeData()
{
	size_t size = 1;
	for (size_t dim : dimensions) {
		size *= dim;
	}
	data.resize(size);
}

template<typename T>
inline void Tensor<T>::addDimension(size_t dim)
{
	dimensions.insert(dimensions.begin(), dim);
	data.resize();
}

template<typename T>
inline size_t Tensor<T>::computeIndex(const std::vector<size_t>& indices) const
{
	size_t index = 0;
	size_t stride = 1;
	for (int i = dimensions.size() - 1; i >= 0; --i) {
		index += indices[i] * stride;
		stride *= dimensions[i];
	}
	return index;
}

template<typename T>
inline void Tensor<T>::removeDimension(int index)
{
	size_t stride = 1;
	if (dimensions.size() > 0) {
		int size = dimensions.size();
		for(j = 0;index+1!=j;j++){
			size_t help = dimensions[index];
			dimensions.erase(dimensions.begin() + j);
			for (int d = dimensions.size() - 1; d >= 0; --d) {
				stride *= dimensions[d];
			}
			for (v = stride; v < data.size();v++) {
				data.erase(data.begin() + v);
			}
		}
		data.resize();
		dimensions.resize();
		
		// brisanje elemenata obrisane dimenzije

	}
}

template<typename T>
inline void Tensor<T>::removeElement(const std::vector<size_t>& indices)
{
	size_t index = computeIndex(indices);
	data.erase(data.begin() + index);
}

template<typename T>
inline void Tensor<T>::removeElementAt(size_t index)
{
	data.erase(data.begin() + index);
}
