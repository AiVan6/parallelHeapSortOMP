#include<iostream>
#include <vector>
#include <chrono>
#include <string>
#include<omp.h>
/*������������� ���������� MPI*/

void print(int arr[], int size) {
	for (int i = 0; i < 10; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << "\n";
}

void heapify(int arr[], int n, int root)
{
	int largest = root; // ������ - ����� ������� �������
	int l = 2 * root + 1; // ����� = 2*������ + 1
	int r = 2 * root + 2; // ������ = 2*������ + 2

	// ���� ����� �������� ������� ������ ���������
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// ���� ������ �������� ������� ������, ��� ����� ������� �� ������ ������
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// ���� ����� ������� �� �������� ��������
	if (largest != root)
	{
		//�������� ������� �������� � ����� �������
		std::swap(arr[root], arr[largest]);
		// ����������� ������������� ���������
		heapify(arr, n, largest);
	}
}

void sortSimple(int arr[], int n)
{
	// ������� ����
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i);
	}

	// ���������� ��������� �� ���� ���� �� ������
	for (int i = n - 1; i >= 0; i--)
	{
		// ����������� ������� ������ � �����
		std::swap(arr[0], arr[i]);

		// ����� �������� max heapify ��� ����������� ����
		heapify(arr, i, 0);
	}
}


void sort(int arr[], int n)
{
	// ������� ����
#pragma omp parallel for
	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(arr, n, i);
	}

	// ���������� ��������� �� ���� ���� �� ������
#pragma omp parallel for
	for (int i = n - 1; i >= 0; i--)
	{
		// ����������� ������� ������ � �����
		std::swap(arr[0], arr[i]);

		// ����� �������� max heapify ��� ����������� ����
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

	
	//������� ����� � ���� ���.
	std::cout << "Parallel Time in nano: " << duration.count() << std::endl;


	auto startS = std::chrono::high_resolution_clock::now();
	sortSimple(arr, arrSize);
	auto endS = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> durationS = endS - startS;

	//������� ����� � ���� ���.
	std::cout << "Time in nano: " << durationS.count() << std::endl;

	return 0;
}