/*
	Assignment 1 DAA: Submission for Question 2. Ronak Sisodia 2014B3A70696H
*/
#include<stdio.h>
#include<stdlib.h>

#define INT_MAX 1e9


// Function used for qsort comparison
int comp(const void *p, const void *q) 
{
    int l = *(const int *)p;
    int r = *(const int *)q;
    return (l - r);
}


// Function to sort the two elements
void swap_them(long int *a, long int *b)
{
	long int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(long int arr[], int l, int r, int x)
{
	// Search for x in arr[l..r] and move it to end
	int i,j;
	for (i=l; i<r; i++)
		if (arr[i] == x)
		break;
	swap_them(&arr[i], &arr[r]);

	// Partition Algorithm
	i = l;
	for (j = l; j <= r - 1; j++)
	{
		if (arr[j] <= x)
		{
			swap_them(&arr[i], &arr[j]);
			i++;
		}
	}
	swap_them(&arr[i], &arr[r]);
	return i;
}

int search_Median(long int arr[], int n)
{
	// Using qsort to sort 5 elements. Returning the middle element. That is median
	qsort((void*)arr, n, sizeof(arr[0]), comp);
	return arr[n/2];
}

int nSmallest(long int arr[], int l, int r, int k)
{
	if (k > 0 && k <= r - l + 1)
	{
		long int n = r-l+1;
		long int i, median[(n+4)/5]; 
		for (i=0; i<n/5; i++)
			median[i] = search_Median(arr+l+i*5, 5);
		if (i*5 < n) 
		{
			median[i] = search_Median(arr+l+i*5, n%5);
			i++;
		}
		// Finding mom: Median Of Median
		int mom = (i == 1)? median[i-1]:nSmallest(median, 0, i-1, i/2);
		
		// Position of pivot(MOM) in sorted array
		int position = partition(arr, l, r, mom);
		
		if (position-l == k-1)
			return arr[position];
		
		//Recurence to left subaaray
		if (position-l > k-1) 
			return nSmallest(arr, l, position-1, k);
		
		//Recurence to Right subarray
		return nSmallest(arr, position+1, r, k-position+l-1);
	}
	return INT_MAX;
}

void quickSort(long int arr[], int l, int h)
{
	if (l < h)
	{
		int n = h-l+1;
		
		// Finding the elemnt to pivot about
		int med = nSmallest(arr, l, h, n/2);
		
		// Finding the position of pivot
		int p = partition(arr, l, h, med);
		
		// Quicksort on the left subarray
		quickSort(arr, l, p - 1);
		
		// QuickSort on right subarray
		quickSort(arr, p + 1, h);
	}
}

int main()
{
	long int i,k;
	
	// Total no of elements
	scanf("%ld",&k);
	
	long int arr[k];
	// Input for elements
	for(i=0;i<k;i++)
		scanf("%ld",&arr[i]);
	long int n = sizeof(arr)/sizeof(arr[0]);
	
	// QuickSort for complete array
	quickSort(arr, 0, n-1);
	
	// Printing the final output. By now array are sorted
	for(i=0;i<n;i++)
		printf("%ld ",arr[i]);
		
	return 0;
}
