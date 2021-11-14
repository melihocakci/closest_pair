#include <stdio.h>
#include <stdlib.h>

typedef struct POINT
{
    int x;
    int y;
} point;

typedef struct PAIR
{
    point p1;
    point p2;
    int dist;
} pair;

int calc_dist(point p1, point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

void quicksort(point *number, int first, int last)
{
    int i, j, pivot;
    point temp;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (number[i].y <= number[pivot].y && i < last)
                i++;
            while (number[j].y > number[pivot].y)
                j--;
            if (i < j)
            {
                temp = number[i];
                number[i] = number[j];
                number[j] = temp;
            }
        }

        temp = number[pivot];
        number[pivot] = number[j];
        number[j] = temp;
        quicksort(number, first, j - 1);
        quicksort(number, j + 1, last);
    }
}

pair find_dmin(point *list, int min, int max)
{
    pair dmin;
    if (max - min == 1)
    {
        dmin.p1 = list[min];
        dmin.p2 = list[max];
        dmin.dist = calc_dist(dmin.p1, dmin.p2);
    }
    else if (max - min == 2)
    {
        int dist1 = calc_dist(list[min], list[min + 1]);
        int dist2 = calc_dist(list[min + 1], list[min + 2]);
        int dist3 = calc_dist(list[min + 2], list[min]);

        if (dist1 < dist2 && dist1 < dist3)
        {
            dmin.dist = dist1;
            dmin.p1 = list[min];
            dmin.p2 = list[min + 1];
        }
        else if (dist2 < dist3)
        {
            dmin.dist = dist2;
            dmin.p1 = list[min + 1];
            dmin.p2 = list[min + 2];
        }
        else
        {
            dmin.dist = dist3;
            dmin.p1 = list[min];
            dmin.p2 = list[min + 2];
        }
    }
    else
    {
        int mid = min + (max - min) / 2;
        pair dl = find_dmin(list, min, mid);
        pair dr = find_dmin(list, mid + 1, max);

        if (dl.dist < dr.dist)
        {
            dmin = dl;
        }
        else
        {
            dmin = dr;
        }

        int i, j, k = 0;
        point *arr = (point *)malloc(sizeof(point) * (max - min + 1));
        for (i = min; i <= max; i++)
        {
            if ((list[i].x - list[mid].x) * (list[i].x - list[mid].x) <= dmin.dist)
            {
                arr[k] = list[i];
                k++;
            }
        }

        quicksort(arr, 0, k - 1);

        for (i = 0; i < k - 1; i++)
        {
            j = i + 1;
            while ((arr[i].y - arr[j].y) * (arr[i].y - arr[j].y) < dmin.dist && j < k)
            {
                if (calc_dist(arr[i], arr[j]) < dmin.dist)
                {
                    dmin.p1 = arr[i];
                    dmin.p2 = arr[j];
                    dmin.dist = calc_dist(arr[i], arr[j]);
                }
                j++;
            }
        }
        free(arr);
    }
    return dmin;
}

int main()
{
    FILE *fp = fopen("sample.txt", "r");
    int i;
    point list[10];
    for (i = 0; i < 10; i++)
    {
        fscanf(fp, "%d %d", &list[i].x, &list[i].y);
    }

    pair dmin = find_dmin(list, 0, 9);

    printf("%d %d\n%d %d", dmin.p1.x, dmin.p1.y, dmin.p2.x, dmin.p2.y);
}