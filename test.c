#include<stdio.h>
#include<stdlib.h>

 typedef struct  Time
 {
    int hrs;
    int min;
    int sec;
   
 }Time;

 typedef struct FlightSchedule
 {
    int flightId;
    Time departureTime;
    Time  eta;
    struct FlightSchedule *next;

 }FlightSchedule;

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
    diff = (a.hrs * 3600 + a.min * 60 + a.sec) - (b.hrs * 3600 + b.min * 60 + b.sec);
    return diff;
}

int maxTime(Time a, Time b)
{
    int diff = timeDiff(a, b);
    if(diff == 0)
    {
        return 0;
    }
    else if(diff>0)
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

FlightSchedule *createSchedule(int flightId, Time departureTime, Time eta )
{
    FlightSchedule *newSchedule = (FlightSchedule *)malloc(sizeof(FlightSchedule));
    newSchedule->flightId = flightId;
    newSchedule->departureTime = departureTime;
    newSchedule->eta = eta;
    newSchedule->next = NULL;

}