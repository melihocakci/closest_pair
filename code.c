#include <stdio.h>
#include <stdlib.h>

// noktalar için struct
typedef struct POINT
{
    int x;
    int y;
} point;

// nokta çiftleri için struct
typedef struct PAIR
{
    point p1;
    point p2;
    int dist; // mesafenin karesi
} pair;

// kıyaslama için iki nokta arası mesafe hesaplama algoritması
int calc_dist(point p1, point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// y eksenine göre sıralama
void quicksort_y(point *list, int first, int last)
{
    int i, j, pivot;
    point tmp;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (list[i].y <= list[pivot].y && i < last)
                i++;
            while (list[j].y > list[pivot].y)
                j--;
            if (i < j)
            {
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }

        tmp = list[pivot];
        list[pivot] = list[j];
        list[j] = tmp;
        quicksort_y(list, first, j - 1);
        quicksort_y(list, j + 1, last);
    }
}

// x eksenine göre sıralama
void quicksort_x(point *list, int first, int last)
{
    int i, j, pivot;
    point tmp;

    if (first < last)
    {
        pivot = first;
        i = first;
        j = last;

        while (i < j)
        {
            while (list[i].x <= list[pivot].x && i < last)
                i++;
            while (list[j].x > list[pivot].x)
                j--;
            if (i < j)
            {
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }

        tmp = list[pivot];
        list[pivot] = list[j];
        list[j] = tmp;
        quicksort_x(list, first, j - 1);
        quicksort_x(list, j + 1, last);
    }
}

// nokta listesi, listenin başlangıç ve bitiş indislerini alıp en yakın nokta çiftini döndürür
pair find_dmin(point *list, int min, int max)
{
    pair dmin;          // en yakın çift
    if (max - min == 1) // 2 nokta kalma durumu
    {
        dmin.p1 = list[min];
        dmin.p2 = list[max];
        dmin.dist = calc_dist(dmin.p1, dmin.p2);
    }
    else if (max - min == 2) // 3 nokta kalma durumu
    {
        int dist1 = calc_dist(list[min], list[min + 1]);     // 1. çift
        int dist2 = calc_dist(list[min + 1], list[min + 2]); // 2. çift
        int dist3 = calc_dist(list[min + 2], list[min]);     // 3. çift

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
    else // 4 ya da daha fazla nokta kalma durumu
    {
        // fonksiyonun rekürsif olarak çağrılması
        int mid = min + (max - min) / 2;
        pair dl = find_dmin(list, min, mid);     // sol tarafın en yakın çifti
        pair dr = find_dmin(list, mid + 1, max); // sağ tarafın en yakın çifti

        // sol ve sağ dmin karşılaştırması
        if (dl.dist < dr.dist)
        {
            dmin = dl;
        }
        else
        {
            dmin = dr;
        }

        // merkeze yakın olan noktaların listesinin çıkartılması
        int i, j, k = 0;
        point *arr = (point *)malloc(sizeof(point) * (max - min + 1)); // merkeze yakın olan noktaların listesi
        for (i = min; i <= max; i++)
        {
            if ((list[i].x - list[mid].x) * (list[i].x - list[mid].x) <= dmin.dist)
            {
                arr[k] = list[i];
                k++;
            }
        }

        quicksort_y(arr, 0, k - 1);

        // dmin'den küçük olan çiftlerin saptanması
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
    // sample.txt dosyasını açma
    char *dosya = (char *)malloc(sizeof(char) * 20);
    printf("Degerlerin alinacagi dosya adi: ");
    scanf("%s", dosya);
    FILE *fp = fopen(dosya, "r");
    if (fp == NULL)
    {
        printf("Dosya bulunamadi");
        return 0;
    }

    // dosyadan isteye kaydetme
    int i, k;
    printf("Alinacak koordinat sayisi: ");
    scanf("%d", &k);
    point *list = (point *)malloc(sizeof(point) * k); // nokta listesi
    for (i = 0; i < k; i++)
    {
        fscanf(fp, "%d %d", &list[i].x, &list[i].y);
    }
    quicksort_x(list, 0, k - 1);

    // dmin bulma
    pair dmin = find_dmin(list, 0, k - 1); // en yakın çift

    free(list);
    fclose(fp);
    // sonucu yazdırma
    printf("En yakin ikili:\n(%d,%d)\n(%d,%d)", dmin.p1.x, dmin.p1.y, dmin.p2.x, dmin.p2.y);
}