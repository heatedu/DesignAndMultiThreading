#include<iostream>
#include<vector>
#include <thread>
using namespace std;

static const int MAX_DEPTH = 3;  // controls max parallelism

// Merge two sorted halves [l..mid] and [mid+1..r]
void merge(vector<int>& arr, int l, int mid, int r) {
    vector<int> temp;
    int i = l, j = mid + 1;

    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= r) temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); k++)
        arr[l + k] = temp[k];
}

// Parallel merge sort
void parallelMergeSort(vector<int>& arr, int l, int r, int depth) {
    if (l >= r) return;

    int mid = l + (r - l) / 2;

    // If depth allows, spawn threads
    if (depth < MAX_DEPTH) {
        std::thread leftThread(parallelMergeSort, ref(arr), l, mid, depth + 1);
        std::thread rightThread(parallelMergeSort, ref(arr), mid + 1, r, depth + 1);

        leftThread.join();
        rightThread.join();
    }
    // Otherwise, run sequentially
    else {
        parallelMergeSort(arr, l, mid, depth + 1);
        parallelMergeSort(arr, mid + 1, r, depth + 1);
    }

    merge(arr, l, mid, r);
}

// Driver code
int main() {
    vector<int> arr = { 9, 5, 1, 4, 3, 8, 6, 2, 7 };

    parallelMergeSort(arr, 0, arr.size() - 1, 0);

    for (int x : arr)
        std::cout << x << " ";

    std::cout << std::endl;
    return 0;
}
