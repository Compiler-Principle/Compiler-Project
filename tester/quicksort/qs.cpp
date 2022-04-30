#include <iostream>


void swap(int list[], int i, int j){
    int temp = list[i];
    list[i] = list[j];
    list[j] = temp;
}

int partition(int list[], int low, int high){
    int pivot = list[low];
    int j = high + 1;
    for (int i = high; i > low; i--) {
        if (list[i] >= pivot) {
            j--;
            swap(list, i, j);
        }
    }
    swap(list, low, j - 1);
    return j - 1;
}

void quickSort(int list[], int low, int high){
    if (low < high) {
        int p = partition(list, low, high);
        quickSort(list, low, p - 1);
        quickSort(list, p + 1, high);
    }
}

int main(){
//    int list[] = {5, 4, 67, 2, 1};
    int N;
    int inputs[10005];
    std::cin >> N;
    for (int i = 0; i < N; i++) {
        std::cin >> inputs[i];
    }
    quickSort(inputs, 0, N - 1);
    for (int i = 0; i < N; i++) {
        std::cout << inputs[i] << std::endl;
    }
    std::cout << std::endl;
    return 0;
}