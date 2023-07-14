#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;
int hash(int x) {
  return x % 100;
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int* hashArr = (int*)malloc(sizeof(int) * n);
    if (hashArr == NULL) {
        printf("Cannot allocate memory\n");
        exit(-1);
    }

    extraMemoryAllocated += sizeof(int) * n;
    for (int i = 0; i < n; i++) {
        hashArr[i] = hash(arr[i]);
    }

    
    free(hashArr);
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);

        
        int leftSize = m - l + 1;
        int rightSize = r - m;
        int* leftArr = (int*)malloc(sizeof(int) * leftSize);
        int* rightArr = (int*)malloc(sizeof(int) * rightSize);

        if (leftArr == NULL || rightArr == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        extraMemoryAllocated += sizeof(int) * (leftSize + rightSize);

        // Copy data to temporary arrays
        memcpy(leftArr, pData + l, sizeof(int) * leftSize);
        memcpy(rightArr, pData + m + 1, sizeof(int) * rightSize);

       
        int i, j, k;
        int* leftHashes = (int*)malloc(sizeof(int) * leftSize);
        int* rightHashes = (int*)malloc(sizeof(int) * rightSize);

        if (leftHashes == NULL || rightHashes == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        extraMemoryAllocated += sizeof(int) * (leftSize + rightSize);

        for (i = 0; i < leftSize; i++) {
            leftHashes[i] = hash(leftArr[i]);
        }

        for (j = 0; j < rightSize; j++) {
            rightHashes[j] = hash(rightArr[j]);
        }

        i = 0;   
        j = 0;   
        k = l;   

        while (i < leftSize && j < rightSize) {
            if (leftHashes[i] <= rightHashes[j]) {
                pData[k] = leftArr[i];
                i++;
            }
            else {
                pData[k] = rightArr[j];
                j++;
            }
            k++;
        }

       
        while (i < leftSize) {
            pData[k] = leftArr[i];
            i++;
            k++;
        }

        
        while (j < rightSize) {
            pData[k] = rightArr[j];
            j++;
            k++;
        }

        // Cleanup
        free(leftArr);
        free(rightArr);
        free(leftHashes);
        free(rightHashes);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}