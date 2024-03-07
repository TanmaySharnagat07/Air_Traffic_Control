#include <stdio.h>
#include <stdlib.h>

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

FlightSchedule *createSchedule(int flightId, Time departureTime, Time eta)
{
    FlightSchedule *newSchedule = (FlightSchedule *)malloc(sizeof(FlightSchedule));
    newSchedule->flightId = flightId;
    newSchedule->departureTime = departureTime;
    newSchedule->eta = eta;
    newSchedule->next = NULL;
}

int liesBetween(Time etaStart, Time etaEnd, Time GivenETA)
{
    int ans = 0;
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

void insertFlightPlan(Bucket *bucketList, int flightId, Time departTime, Time ETA)
{
    FlightSchedule *newSchedule = createSchedule(flightId, departTime, ETA);
    Bucket *temp = bucketList;
    Bucket *prev = bucketList;
    int flag = 1;
    while (temp && flag == 1)
    {
        if (liesBetween(temp->etaStart, temp->etaEnd, ETA) == 1)
        {
            flag = 0;
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }

    // Add new bucket to the list
    if (prev && temp == NULL && flag == 1)
    {
        Time bETAstart = prev->etaStart ;
        bETAstart.hrs++;
        Time bETAend = prev->etaEnd;
        bETAend.hrs++;
        Bucket *newBucket = createBucket(prev->bucketId + 1, bETAstart, bETAend);
        prev->next = newBucket ;
        newBucket->flightSchedule = newSchedule;
        printf("New bucket is created\n");
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
    return;
}

Bucket *insertBucket(Bucket *bucketList, int bucketId, Time etaStart, Time etaEnd)
{
    Bucket *newBucket = createBucket(bucketId, etaStart, etaEnd);
    if (bucketList == NULL)
    {
        bucketList = newBucket;
    }
    else if (timeDiff(bucketList->etaStart, newBucket->etaEnd) >= 0)
    {
        newBucket->next = bucketList;
        bucketList = newBucket;
    }
    else
    {
        Bucket *temp = bucketList;
        Bucket *prev = bucketList;
        int flag = 1;
        while (flag && temp)
        {
            if (timeDiff(etaStart, temp->etaStart) >= 0 && timeDiff(etaStart, temp->etaStart) < 60 && timeDiff(temp->etaEnd, etaEnd) >= 0 && timeDiff(temp->etaEnd, etaEnd) < 60)
            {
                flag = 0;
            }
            prev = temp;
            temp = temp->next;
        }
        prev->next = newBucket;
        newBucket->next = temp;
    }
    return bucketList;
}

void deleteFlightPlan(Bucket *bucketList, int flightId)
{
    Bucket *temp = bucketList;
    int flag = 1;
    while (temp && flag)
    {
        FlightSchedule *curr = temp->flightSchedule;
        if(curr && curr->flightId == flightId){
            flag = 0;
            FlightSchedule* ptr = curr ;
            temp->flightSchedule = curr->next;
            free(ptr);
        }
        else{
            FlightSchedule* prev = temp->flightSchedule;
            while(curr && flag){
                if(curr->flightId == flightId){
                    flag = 0;
                    FlightSchedule* ptr = curr ;
                    prev->next = curr->next;
                    free(ptr);
                }
                else{
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
        temp = temp->next;
    }
    if(flag == 1)
    {
        printf("Flight Id %d does not exist\n", flightId);
    }
    printf("\n");
    return;
}
int main()
{

    Time etaS;
    etaS.hrs = 2;
    etaS.min = 00;
    Time etaE;
    etaE.hrs = 2;
    etaE.min = 59;
    Bucket *newB = createBucket(90, etaS, etaE);
    etaS.hrs = 1;
    etaS.min = 00;
    etaE.hrs = 1;
    etaE.min = 59;
    newB = insertBucket(newB, 91, etaS, etaE);
    etaS.hrs = 3;
    etaS.min = 00;
    etaE.hrs = 3;
    etaE.min = 59;
    newB = insertBucket(newB, 92, etaS, etaE);
    Time dT;
    dT.hrs = 1;
    dT.min = 30;
    insertFlightPlan(newB, 110, dT, dT);
    dT.hrs = 2;
    dT.min = 30;
    insertFlightPlan(newB, 111, dT, dT);
    dT.hrs = 1;
    dT.min = 36;
    insertFlightPlan(newB, 112, dT, dT);
    dT.hrs = 2;
    dT.min = 24;
    insertFlightPlan(newB, 113, dT, dT);
    dT.hrs = 4;
    dT.min = 24;
    insertFlightPlan(newB, 114, dT, dT);
    dT.hrs = 4;
    dT.min = 25;
    insertFlightPlan(newB, 115, dT, dT);
    Print(newB);
    showStatus(newB, 110);
    showStatus(newB, 114);
    deleteFlightPlan(newB, 110);
    Print(newB);
    return 0;
}