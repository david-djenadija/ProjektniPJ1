#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>
template<typename T, int... Args>
class Tensor {
private:
	std::vector<size_t> dimensions;
	std::vector<T> data;
public:
	Tensor();
	Tensor(double scalar);
	T& operator()(const std::vector<size_t>& indices);
	void resizeData() ;
	size_t computeIndex(const std::vector<size_t>& indices) const;
	void removeElement(const std::vector<size_t>& indices);
	void removeElementAt(size_t index);
	void addElement(const T& element);
	size_t getStride() const;
	void clearDimensions() { dimensions.clear(); };
	void clearData() { data.clear(); };
	size_t getDimension(int i) const;
	size_t getDimensionSize() const;
	size_t getDataSize() const;
	std::vector<T> getData() const;
	std::vector<size_t> getDimensions()const;
	T getDataElement(size_t i) const;
	void pushData(T dat) { data.push_back(dat); };
	friend std::ostream& operator<<(std::ostream& os, const Tensor<T, Args...> a) {
		for (size_t i = 0; i < a.getDataSize(); i++) {
			os << a.getDataElement(i);
		}
		return os;

	};
	friend std::ofstream& operator<<(std::ofstream& file, const Tensor<T, Args...>& a) {
		for (size_t i = 0; i < a.getDimensionSize(); i++) {
			file << a.getDimension(i) << " ";
		}
		file << std::endl;

		for (size_t i = 0; i < a.getDataSize(); i++) {
			file << a.getDataElement(i) << " ";
		}
		return file;
	};
	friend std::ifstream& operator>>(std::ifstream& file, Tensor<T, Args...>& data) {
		char ch;
		std::vector<int> dimensionsData;
		while (file.get(ch) && ch != '\n') {
			if (ch != ' ') {
				dimensionsData.push_back(std::stoi(std::string(1, ch)));
			}
		}
		if ((dimensionsData.size() != data.getDimensionSize())) {
			//std::cout << "prva" << dimensionsData.size() << "druga" << data.getDimensionSize() << std::endl;
			//for (size_t i = 0; i < dimensionsData.size(); i++)std::cout << dimensionsData[i];
			throw std::out_of_range("Invalid tensor in file!\n"); 
		}
		else {
			for (size_t i = 0; i < data.getDimensionSize(); i++)
			{
				if (data.getDimension(i) != dimensionsData[i])
					throw std::out_of_range("Invalid tensor in file!\n");
			}
		}

		//dio za ucitavanje date
		std::vector<T> dataData;
		while (file.get(ch) && ch != '\n') {
			if (ch != ' ') {
				dataData.push_back(std::stod(std::string(1, ch)));
			}
		}
		data.clearData();
		for (size_t i = 0; i < dataData.size(); i++) {
			data.pushData(dataData[i]);
		}
		return file;

	};
	Tensor<T, Args...> operator+ (const Tensor<T, Args...> a) const;
	Tensor<T, Args...> operator- (const Tensor<T, Args...> a) const;
	Tensor<T, Args...> operator* (const Tensor<T, Args...> a) const;
	Tensor<T, Args...> operator+ (const double a) const;
	Tensor<T, Args...> operator* (const double a) const;
	Tensor<T, Args...>& operator=(const Tensor<T, Args...>& other);
};





template<typename T>
class Tensor<T> {
private:
	std::vector<size_t> dimensions;
	std::vector<T> data;
public:
	Tensor() {};
	T& operator()(const std::vector<size_t>& indices) {
		size_t index = computeIndex(indices);
		return data[index];
	};
	void resizeData() {
		size_t size = 1;
		for (size_t dim : dimensions) {
			size *= dim;
		}
		data.resize(size);
	};
	void addDimension(size_t dim);
	void pushDimension(size_t dim);
	void pushData(T dat); 
	size_t computeIndex(const std::vector<size_t>& indices) const {
		size_t index = 0;
		size_t stride = 1;
		for (int i = dimensions.size() - 1; i >= 0; --i) {
			index += indices[i] * stride;
			stride *= dimensions[i];
		}
		return index;
	};
	void removeDimension(int index) ; 
	void removeElement(const std::vector<size_t>& indices) {
		size_t index = computeIndex(indices);
		data.erase(data.begin() + index);
	}; 
	void removeElementAt(size_t index) {
		data.erase(data.begin() + index);
	};   
	void addElement(const T& element) {
		if (data.size() < getStride()) {
			data.push_back(element);
		}
		else return;
	}; 
	void clearDimensions() { dimensions.clear(); };
	void clearData() { data.clear(); };
	size_t getStride() const {
		size_t pr = 1;
		for (size_t i = 0; i < dimensions.size(); i++) {
			pr *= dimensions[i];
		}
		return pr;
	}; 
	size_t getDimension(int i) const {
		return dimensions[i];
	};
	size_t getDimensionSize() const {
		return dimensions.size();
	};
	size_t getDataSize() const {
		return data.size();
	};
	std::vector<T> getData() const { return data; };  
	std::vector<size_t> getDimensions()const {
		return dimensions;
	}; 
	T getDataElement(size_t i) const { return data[i]; };
	friend std::ofstream& operator<<(std::ofstream& file, const Tensor<T>& a) {
		for (size_t i = 0; i < a.getDimensionSize(); i++) {
			file << a.getDimension(i) << " ";
		}
		file << std::endl;

		for (size_t i = 0; i < a.getDataSize(); i++) {
			file << a.getDataElement(i) << " ";
		}
		return file;
	};
	friend std::ifstream& operator>>(std::ifstream& file, Tensor<T>& data) {
		char ch;
		std::vector<int> dimensionsData;
		while (file.get(ch) && ch != '\n') {
			if (ch!=' ') { 
				dimensionsData.push_back(std::stoi(std::string(1, ch)));
			}
		}
		data.clearDimensions();
		
		for (size_t i =0; i <dimensionsData.size(); i++) {
			data.pushDimension(dimensionsData[i]);
		}
		//dio za ucitavanje date
		std::vector<T> dataData;
		while (file.get(ch) && ch != '\n') {
			if (ch != ' ') {
				dataData.push_back(std::stod(std::string(1,ch)));
			}
		}
		data.clearData();
		for (size_t i = 0; i < dataData.size(); i++) {
			data.pushData(dataData[i]);
		}
		return file;

	};
	Tensor<T> operator+ (const Tensor<T> a) const {
		Tensor<T> pom;
		for (size_t i = 0; i < this->getDimensionSize(); i++)pom.pushDimension(this->getDimension(i));
		for (int i = 0; i < ((this->getDataSize() < a.getDataSize()) ? this->getDataSize() : a.getDataSize()); i++) {
			pom.addElement(a.getDataElement(i) + this->data[i]);
		}
		return pom;
	};
	Tensor<T> operator- (const Tensor<T> a) const {
		Tensor<T> pom;
		for (size_t i = 0; i < this->getDimensionSize(); i++)pom.pushDimension(this->getDimension(i));
		for (size_t i = 0; i < ((this->getDataSize() < a.getDataSize()) ? this->getDataSize() : a.getDataSize()); i++) {
			pom.addElement(this->data[i] - a.getDataElement(i));
		}
		return pom;
	};
	Tensor<T> operator* (const Tensor<T> a) const {
		Tensor<T> pom;
		for (size_t i = 0; i < this->getDimensionSize(); i++)pom.pushDimension(this->getDimension(i));
		for (int i = 0; i < ((this->getDataSize() < a.getDataSize()) ? this->getDataSize() : a.getDataSize()); i++) {
			pom.addElement(this->data[i] * a.getDataElement(i));
		}
		return pom;
	};
	Tensor<T> operator+ (const double a) const {
		Tensor<T> pom;
		for (size_t i = 0; i < this->getDimensionSize(); i++)pom.pushDimension(this->getDimension(i));
		for (int i = 0; i < this->getDataSize(); i++) {
			pom.addElement(this->data[i] + a);
		}
		return pom;
	};
	Tensor<T> operator* (const double a) const {
		Tensor<T> pom;
		for (size_t i = 0; i < this->getDimensionSize(); i++)pom.pushDimension(this->getDimension(i));
		for (int i = 0; i < this->getDataSize(); i++) {
			pom.addElement(this->data[i] * a);
		}
		return pom;
	};
	Tensor<T>& operator=(const Tensor<T>& other) {
		data = other.getData();
		dimensions = other.getDimensions();
		return *this;
	};
};

template<typename T,int... Args>
inline Tensor<T,Args...>::Tensor()
{
	dimensions = { Args... };
}

template<typename T, int ...Args>
inline Tensor<T, Args...>::Tensor(double scalar):Tensor()
{
	for (size_t i = 0; i < getStride(); i++) {
		data.push_back(scalar);
	}
}


template<typename T,int... Args>
inline T& Tensor<T,Args...>::operator()(const std::vector<size_t>& indices)
{
	size_t index = computeIndex(indices);
	return data[index];
}

template<typename T,int... Args>
inline void Tensor<T,Args...>::resizeData()
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
	//data.resize(getStride());
}

template<typename T>
inline void Tensor<T>::pushDimension(size_t dim)
{
	dimensions.push_back(dim);
}

template<typename T>
inline void Tensor<T>::pushData(T dat)
{
	data.push_back(dat);
}

template<typename T, int... Args>
inline size_t Tensor<T, Args...>::computeIndex(const std::vector<size_t>& indices) const
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
inline void Tensor<T>::removeDimension(int index) //greska treba obrisati prvu for petlju
{
	size_t stride = 1;
	if (dimensions.size() > 0) {
		int size = dimensions.size();
		
			size_t help = dimensions[index];
			dimensions.erase(dimensions.begin() + index);
			for (int d = dimensions.size() - 1; d >= 0; --d) {
				stride *= dimensions[d];
			}
			for (int v = stride; v < data.size();v++) {
				data.erase(data.begin() + v);
			}
		
		data.shrink_to_fit();
		dimensions.shrink_to_fit();
	}
}

template<typename T, int... Args>
inline void Tensor<T, Args...>::removeElement(const std::vector<size_t>& indices) //uslov
{
	size_t index = computeIndex(indices);
	data.erase(data.begin() + index);
}

template<typename T,int... Args>
inline void Tensor<T, Args...>::removeElementAt(size_t index) //uslov
{
	data.erase(data.begin() + index);
}

template<typename T, int... Args>
inline void Tensor<T, Args...>::addElement(const T& element)
{
	if (data.size() < getStride()) {
		data.push_back(element);
	}
	else return;
}

template<typename T, int... Args>
inline size_t Tensor<T,Args...>::getStride() const
{
	size_t pr = 1;
	for (size_t i = 0; i < dimensions.size(); i++) {
		pr *= dimensions[i];
	}
	return pr;
}

template<typename T, int ...Args>
inline size_t Tensor<T, Args...>::getDimension(int i) const //uslov
{
	return dimensions[i];
}

template<typename T, int ...Args>
inline size_t Tensor<T, Args...>::getDimensionSize() const
{
	return dimensions.size();
}

template<typename T, int ...Args>
inline size_t Tensor<T, Args...>::getDataSize() const
{
	return data.size();
}

template<typename T, int ...Args>
inline std::vector<T> Tensor<T, Args...>::getData() const
{
	return data;
}

template<typename T, int ...Args>
inline std::vector<size_t> Tensor<T, Args...>::getDimensions() const
{
	return dimensions;
}

template<typename T, int ...Args>
inline T Tensor<T, Args...>::getDataElement(size_t i) const 
{
	return data[i];
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Tensor<T, Args...>::operator+(const Tensor<T, Args...> a) const
{
	Tensor<T, Args...> pom;
	for (int i = 0; i < ((this->getDataSize() < a.getDataSize()) ? this->getDataSize() : a.getDataSize()); i++) {
		pom.addElement(a.getDataElement(i) + this->data[i]);
	}
	return pom;
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Tensor<T, Args...>::operator-(const Tensor<T, Args...> a) const
{
	Tensor<T, Args...> pom;
	for (size_t i = 0; i < ((this->getDataSize() < a.getDataSize()) ? this->getDataSize() : a.getDataSize()); i++) {
		pom.addElement(this->data[i]- a.getDataElement(i));
	}
	return pom;
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Tensor<T, Args...>::operator*(const Tensor<T, Args...> a) const
{
	Tensor<T, Args...> pom;
	for (int i = 0; i < ((this->getDataSize()<a.getDataSize())?this->getDataSize():a.getDataSize()); i++) {
		pom.addElement(this->data[i] * a.getDataElement(i));
	}
	return pom;
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Tensor<T, Args...>::operator+(const double a) const
{
	Tensor<T, Args...> pom;
	for (int i = 0; i < this->getDataSize() ; i++) {
		pom.addElement(this->data[i] + a);
	}
	return pom;
}

template<typename T, int ...Args>
inline Tensor<T, Args...> Tensor<T, Args...>::operator*(const double a) const
{
	Tensor<T, Args...> pom;
	for (int i = 0; i < this->getDataSize(); i++) {
		pom.addElement(this->data[i] * a);
	}
	return pom;
}

template<typename T, int ...Args>
inline Tensor<T, Args...>& Tensor<T, Args...>::operator=(const Tensor<T, Args...>& other)
{
	data = other.getData();
	dimensions = other.getDimensions();
	return *this;
}
