#include <stdio.h>

typedef struct POINT
{
    int x;
    int y;
} point;

typedef struct PAIR
{
    int index1;
    int index2;
    int distance;
} pair;

int dist(point p1, point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

pair find_dmin(point *list, int min, int max)
{
    pair dmin;
    if (max - min == 1)
    {
        dmin.index1 = min;
        dmin.index2 = max;
        dmin.distance = dist(list[min], list[max]);
    }
    else if (max - min == 2)
    {
        int dist1 = dist(list[min], list[min + 1]);
        int dist2 = dist(list[min + 1], list[min + 2]);
        int dist3 = dist(list[min + 2], list[min]);

        if (dist1 < dist2 && dist1 < dist3)
        {
            dmin.distance = dist1;
            dmin.index1 = min;
            dmin.index2 = min + 1;
        }
        else if (dist2 < dist3)
        {
            dmin.distance = dist2;
            dmin.index1 = min + 1;
            dmin.index2 = min + 2;
        }
        else
        {
            dmin.distance = dist3;
            dmin.index1 = min;
            dmin.index2 = min + 2;
        }
    }
    else
    {
        int mid = min + (max - min) / 2;
        pair dl = find_dmin(list, min, mid);
        pair dr = find_dmin(list, mid + 1, max);
        if (dl.distance < dr.distance)
        {
            dmin = dl;
        }
        else
        {
            dmin = dr;
        }
        int i = mid, j;
        while ((mid - i) * (mid - i) <= dmin.distance && i >= min)
        {
            j = mid + 1;
            while ((j - mid) * (j - mid) <= dmin.distance && j <= max)
            {
                if (list[i].y - list[j].y < dmin.distance && (list[i].y - list[j].y) > (-1) * dmin.distance)
                {
                    if (dist(list[i], list[j]) < dmin.distance)
                    {
                        dmin.distance = dist(list[i], list[j]);
                        dmin.index1 = i;
                        dmin.index2 = j;
                    }
                }
                j++;
            }
            i--;
        }
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

    pair result = find_dmin(list, 0, 9);

    int i1 = result.index1, i2 = result.index2;

    printf("%d %d\n%d %d", list[i1].x, list[i1].y, list[i2].x, list[i2].y);
}