#include <stdio.h>
#include <stdlib.h>

typedef struct Time
{
    int hrs;
    int min;
} Time;

typedef struct FlightSchedule
{
    int flightId;
    Time departureTime;
    Time eta;
    struct FlightSchedule *next;

} FlightSchedule;

typedef struct Bucket
{
    int bucketId;
    Time etaStart;
    Time etaEnd;
    FlightSchedule *flightSchedule;
    struct Bucket *next;
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

void Print(Bucket *bucket){
    Bucket* temp = bucket;
    while(temp){
        printf("Bucket Id: %d\n", temp->bucketId);
        printf("ETA Start: %d:%d\n", temp->etaStart.hrs, temp->etaStart.min);
        printf("ETA End: %d:%d\n", temp->etaEnd.hrs, temp->etaEnd.min);
        FlightSchedule* temp2 = temp->flightSchedule;
        while(temp2){
            printf("Flight Id: %d\n", temp2->flightId);
            printf("Departure Time: %d:%d\n", temp2->departureTime.hrs, temp2->departureTime.min);
            printf("ETA: %d:%d\n", temp2->eta.hrs, temp2->eta.min);
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}
int main()
{

    Time etaS;
    etaS.hrs = 2 ;
    etaS.min = 00;
    Time etaE;
    etaE.hrs = 3 ;
    etaE.min = 00;
    Bucket *newB = createBucket(101, etaS, etaE);
    FlightSchedule *newS = createSchedule(101, etaS, etaE);
    newB->flightSchedule = newS;
    newS->next = createSchedule(102, etaS, etaE);

    Print(newB);


    return 0;
}