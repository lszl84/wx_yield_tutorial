#include <vector>

template <class T>
struct ChunkedSort
{
    ChunkedSort(std::vector<T> &&array) : arr(std::move(array)) {}

    bool finished()
    {
        return i >= arr.size() - 1;
    }

    int getProgress(int scale)
    {
        return i * scale / (arr.size() - 1);
    }

    void processChunk()
    {
        for (int j = 0; j < arr.size() - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }

        i++;
    }

    std::vector<T> arr;
    int i = 0;
};