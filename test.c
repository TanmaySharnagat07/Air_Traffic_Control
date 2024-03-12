#include <stdio.h>
#include <stdlib.h>
static int BucketIDStart = 500;
typedef struct Time
{
    int hrs;
    int min;
} Time;

typedef struct FlightScheduleTag
{
    int flightId;
    Time departureTime;
    Time eta;
    struct FlightScheduleTag *next;

} FlightSchedule;

typedef struct BucketTag
{
    int bucketId;
    Time etaStart;
    Time etaEnd;
    FlightSchedule *flightSchedule;
    struct BucketTag *next;
} Bucket;

int timeDiff(Time a, Time b)
{
    int diff = 0;
    diff = (a.hrs * 60 + a.min) - (b.hrs * 60 + b.min);
    return diff;
}

int maxTime(Time a, Time b)
{
    int diff = timeDiff(a, b);
    if (diff == 0)
    {
        return 0;
    }
    else if (diff > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

Bucket *createBucket(int bucketId, Time etaStart, Time etaEnd);
FlightSchedule *createFlightSchedule(int flightId, Time departureTime, Time eta);
void Print(Bucket *bucket);
void showStatus(Bucket *buketList, int flightId);
void insertFlightPlan(Bucket **bucketList, int flightId, Time departureTime, Time ETA);
Bucket *insertBucket(Bucket *bucketList, Bucket *newBucket);
void deleteFlightPlan(Bucket **bucketList, int flightId);
void reArrangeBucket(Bucket **bucket);

Bucket *createBucket(int bucketId, Time etaStart, Time etaEnd)
{
    Bucket *newBucket = (Bucket *)malloc(sizeof(Bucket));
    newBucket->bucketId = bucketId;
    newBucket->etaStart = etaStart;
    newBucket->etaEnd = etaEnd;
    newBucket->flightSchedule = NULL;
    newBucket->next = NULL;
    return newBucket;
}

FlightSchedule *createFlightSchedule(int flightId, Time departureTime, Time eta)
{
    FlightSchedule *newSchedule = (FlightSchedule *)malloc(sizeof(FlightSchedule));
    newSchedule->flightId = flightId;
    newSchedule->departureTime = departureTime;
    newSchedule->eta = eta;
    newSchedule->next = NULL;
    return newSchedule;
}

int liesBetween(Time etaStart, Time etaEnd, Time GivenETA)
{
    int ans = 0;
    // if(etaEnd.hrs == 0) etaEnd.hrs = 24 ;
    if (timeDiff(GivenETA, etaStart) >= 0 && timeDiff(GivenETA, etaStart) < 60 && timeDiff(etaEnd, GivenETA) >= 0 && timeDiff(etaEnd, GivenETA) < 60)
    {
        ans = 1;
    }
    return ans;
}

void Print(Bucket *bucket)
{
    Bucket *temp = bucket;
    while (temp)
    {
        printf("\n");
        printf("Bucket Id: %d\n", temp->bucketId);
        printf("ETA Start: %d:%d\n", temp->etaStart.hrs, temp->etaStart.min);
        printf("ETA End: %d:%d\n", temp->etaEnd.hrs, temp->etaEnd.min);
        FlightSchedule *temp2 = temp->flightSchedule;
        while (temp2)
        {
            printf("Flight Id: %d\n", temp2->flightId);
            printf("Departure Time: %d:%d\n", temp2->departureTime.hrs, temp2->departureTime.min);
            printf("ETA: %d:%d\n", temp2->eta.hrs, temp2->eta.min);
            temp2 = temp2->next;
        }
        temp = temp->next;
        printf("\n");
    }
}

void showStatus(Bucket *bucketList, int flightId)
{
    Bucket *temp = bucketList;
    int flag = 1;
    while (temp && flag)
    {
        FlightSchedule *temp2 = temp->flightSchedule;
        while (temp2 && flag)
        {
            if (temp2->flightId == flightId)
            {
                printf("Flight Id: %d\n", temp2->flightId);
                printf("Departure Time: %d:%d\n", temp2->departureTime.hrs, temp2->departureTime.min);
                printf("ETA: %d:%d\n", temp2->eta.hrs, temp2->eta.min);
                flag = 0;
            }
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    if (flag)
    {
        printf("Flight Id %d does not exist\n", flightId);
    }
    printf("\n");
    return;
}
Bucket *insertBucket(Bucket *bucketList, Bucket *newBucket)
{

    if (bucketList == NULL)
    {
        bucketList = newBucket;
    }
    else if (bucketList->etaStart.hrs > newBucket->etaEnd.hrs)
    {
        newBucket->next = bucketList;
        bucketList = newBucket;
    }
    else
    {
        Bucket *temp = bucketList;
        Bucket *prev;
        int flag = 1;
        while (temp && flag)
        {
            if (temp->etaEnd.hrs < newBucket->etaStart.hrs)
            {
                prev = temp;
                temp = temp->next;
            }
            else
            {
                flag = 0;
            }
        }
        if (flag && !temp)
        {
            newBucket->next = temp;
            prev->next = newBucket;
        }
        else if (temp)
        {
            prev->next = newBucket;
            newBucket->next = temp;
        }
    }
    return bucketList;
}

void insertFlightPlan(Bucket **bucketList, int flightId, Time departTime, Time ETA)
{
    FlightSchedule *newSchedule = createFlightSchedule(flightId, departTime, ETA);
    if (*bucketList == NULL)
    {
        Time etaS, etaE;
        etaS.hrs = ETA.hrs;
        etaS.min = 0;
        etaE.hrs = ETA.hrs;
        etaE.min = 59;
        Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
        *bucketList = insertBucket(*bucketList, newBucket);
        (*bucketList)->flightSchedule = newSchedule;
    }
    else
    {
        Bucket *temp = *bucketList;
        Bucket *prev;
        int flag = 1;
        while (temp && flag)
        {
            if (liesBetween(temp->etaStart, temp->etaEnd, ETA))
                flag = 0;
            else
            {
                prev = temp;
                temp = temp->next;
            }
        }

        if (flag == 1)
        {
            printf("New bucket added with bucketID %d\n", BucketIDStart);
            Time etaS, etaE;
            etaS.hrs = ETA.hrs;
            etaS.min = 0;
            etaE.hrs = ETA.hrs;
            etaE.min = 59;
            Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
            newBucket->flightSchedule = newSchedule;
            *bucketList = insertBucket(*bucketList, newBucket);
        }

        else
        {
            if (temp->flightSchedule == NULL)
            {
                temp->flightSchedule = newSchedule;
            }
            else if (maxTime(newSchedule->departureTime, temp->flightSchedule->departureTime) < 0)
            {
                newSchedule->next = temp->flightSchedule;
                temp->flightSchedule = newSchedule;
            }
            else
            {
                FlightSchedule *prev = temp->flightSchedule;
                FlightSchedule *curr = temp->flightSchedule;
                while (curr && maxTime(newSchedule->departureTime, curr->departureTime) >= 0)
                {
                    prev = curr;
                    curr = curr->next;
                }
                prev->next = newSchedule;
                newSchedule->next = curr;
            }
        }
    }
    return;
}
void deleteFlightPlan(Bucket **bucketList, int flightId)
{
    Bucket *temp = *bucketList;
    int flag = 1;
    while (temp && flag)
    {
        FlightSchedule *curr = temp->flightSchedule;
        if (curr && curr->flightId == flightId)
        {
            flag = 0;
            FlightSchedule *ptr = curr;
            temp->flightSchedule = curr->next;
            free(ptr);
        }
        else
        {
            FlightSchedule *prev = temp->flightSchedule;
            while (curr && flag)
            {
                if (curr->flightId == flightId)
                {
                    flag = 0;
                    FlightSchedule *ptr = curr;
                    prev->next = curr->next;
                    free(ptr);
                }
                else
                {
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
        temp = temp->next;
    }
    if (flag == 1)
    {
        printf("Flight Id %d does not exist\n", flightId);
    }
    printf("\n");
    return;
}

// void reArrangeBucket(Bucket **bucket)
// {
//     Time t;
//     int hr, min;
//     printf("Enter current time hours and minutes\n");
//     scanf("%d %d", &hr, &min);
//     t.hrs = hr;
//     t.min = min;
//     Time zeroTime;
//     zeroTime.hrs = 0;
//     zeroTime.min = 0;
//     Bucket *temp = createBucket(0,zeroTime,zeroTime);
//     Bucket *prev = NULL;
//     Bucket *curr = *bucket;
//     FlightSchedule *next = NULL;

//     FlightSchedule *current;
//     FlightSchedule *store = NULL;
//     FlightSchedule *Temp = NULL;

//     while (curr)
//     {
//         FlightSchedule *store = NULL;
//         FlightSchedule *storeStart;
//         curr->etaStart.min = t.min;
//         curr->etaEnd.hrs = (curr->etaEnd.hrs + 1)%24;
//         curr->etaEnd.min = t.min - 1;
//         Temp = curr->flightSchedule;

//         FlightSchedule *previous = createFlightSchedule(0, zeroTime, zeroTime);
//         FlightSchedule *prevTail = previous;

//         while (Temp != NULL)
//         {   FlightSchedule *next = Temp->next;
//             if(Temp->eta.min >= t.min)
//             {
//                 prevTail->next = Temp;
//                 prevTail = prevTail->next;
//                 Temp->next ;
//             }
//             else if(Temp->eta.min < t.min)
//             {
//                 if(store == NULL)
//                 {
//                     store = Temp;
//                     storeStart = store;
//                 }
//                 else
//                 {
//                     store->next = Temp;
//                     store = store->next;
//                 }

//             }
//             Temp = next ;
//         }
//         store->next = NULL;
//         prevTail->next = NULL;
//         curr->flightSchedule = NULL;
//         curr->flightSchedule = previous->next;
//         if (prev == NULL)
//         {
//             temp->flightSchedule = storeStart;
//         }
//         else
//         {   FlightSchedule *a = prev->flightSchedule;
//             while(a->next)
//             {
//                 a = a->next;
//             }
//             a->next  = storeStart;
//         }
//         prev = curr;
//         curr = curr->next;

//     }
// }

void reArrangeBucket(Bucket **bucket)
{
    Time t;
    int hr, min;
    printf("Enter current time hours and minutes\n");
    scanf("%d %d", &hr, &min);
    t.hrs = hr;
    t.min = min;
    Time notTime;
    notTime.hrs = -1;
    notTime.min = -1;
    Bucket *tempBucket = createBucket(-1, notTime, notTime);
    Bucket *curr = *bucket;
    Bucket *prev = NULL;

    while (curr)
    {
        curr->etaStart.min = t.min;
        curr->etaEnd.hrs = (curr->etaEnd.hrs + 1) % 24;
        curr->etaEnd.min = t.min - 1;
        FlightSchedule *temp = curr->flightSchedule;

        // dummy stores flightschedule in range
        FlightSchedule *dummy = createFlightSchedule(-11, notTime, notTime);
        FlightSchedule *dummyTail = dummy;

        // store stores fligtschedule not in range
        FlightSchedule *store = createFlightSchedule(-1, notTime, notTime), *storeTail = store;
        while (temp)
        {
            FlightSchedule *next = temp->next;
            if (temp->eta.min >= t.min)
            {
                dummyTail->next = temp;
                dummyTail = dummyTail->next;
            }
            else
            {
                storeTail->next = temp;
                storeTail = storeTail->next;
            }
            temp->next = NULL;
            temp = next;
        }
        curr->flightSchedule = NULL;
        curr->flightSchedule = dummy->next;

        if(prev == NULL){
            tempBucket->flightSchedule = store->next ;
        }
        else{
            FlightSchedule *a = prev->flightSchedule;
            while(a->next)
            {
                a = a->next;
            }
            a->next = store->next ;
        }
        free(store);
        prev = curr;
        curr = curr->next;
    }
}

int main()
{
    Bucket *bucketList = NULL;
    int flightId;
    Time departTime;
    Time ETA;
    FILE *fptr;
    fptr = fopen("data.txt", "r");
    if (fptr == NULL)
    {
        printf("File not found\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (!feof(fptr))
        {
            fscanf(fptr, "%d %d %d %d %d", &flightId, &departTime.hrs, &departTime.min, &ETA.hrs, &ETA.min);
            insertFlightPlan(&bucketList, flightId, departTime, ETA);
        }
    }
    Print(bucketList);
    fclose(fptr);
    reArrangeBucket(&bucketList);
    Print(bucketList);
    return 0;
}