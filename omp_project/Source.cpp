#include<iostream>
#include <vector>
#include <chrono>
#include <string>
#include<omp.h>
/*ѕирамидальна€ сортировка MPI*/

void print(int arr[], int size) {
	for (int i = 0; i < 10; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << "\n";
}

void heapify(int arr[], int n, int root)
{
	int largest = root; // корень - самый большой элемент
	int l = 2 * root + 1; // слева = 2*корень + 1
	int r = 2 * root + 2; // справа = 2*корень + 2

	// ≈сли левый дочерний элемент больше корневого
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// ≈сли правый дочерний элемент больше, чем самый большой на данный момент
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// ≈сли самый большой не €вл€етс€ корневым
	if (largest != root)
	{
		//помен€ть местами корневой и самый большой
		std::swap(arr[root], arr[largest]);
		// –екурсивное нагромождение поддерева
		heapify(arr, n, largest);
	}
}

void sortSimple(int arr[], int n)
{
	// создать кучу
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i);
	}

	// извлечение элементов из кучи один за другим
	for (int i = n - 1; i >= 0; i--)
	{
		// ѕереместить текущий корень в конец
		std::swap(arr[0], arr[i]);

		// снова вызовите max heapify дл€ уменьшенной кучи
		heapify(arr, i, 0);
	}
}


void sort(int arr[], int n)
{
	// создать кучу
#pragma omp parallel for
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i);
	}

	// извлечение элементов из кучи один за другим
#pragma omp parallel for
	for (int i = n - 1; i >= 0; i--)
	{
		// ѕереместить текущий корень в конец
		std::swap(arr[0], arr[i]);

		// снова вызовите max heapify дл€ уменьшенной кучи
		heapify(arr, i, 0);
	}
}

void fillArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = rand() % size;
	}
}

int main() {
	int arrSize = 0;
	std::cout << "Enter array size: ";
	std::cin >> arrSize;
	

	int *arr = new int[arrSize];
	fillArray(arr, arrSize);


	auto start = std::chrono::high_resolution_clock::now();
	sort(arr, arrSize);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;

	
	//выводит врем€ в нано сек.
	std::cout << "Parallel Time in nano: " << duration.count() << std::endl;


	auto startS = std::chrono::high_resolution_clock::now();
	sortSimple(arr, arrSize);
	auto endS = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> durationS = endS - startS;

	//выводит врем€ в нано сек.
	std::cout << "Time in nano: " << durationS.count() << std::endl;

	return 0;
}