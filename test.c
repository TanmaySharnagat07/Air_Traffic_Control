#include <stdio.h>
#include <stdlib.h>
static int BucketIDStart = 500 ;
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
void insertFlightPlan(Bucket **bucketList, int flightId, Time departureTime, Time ETA) ;
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
    {   printf("\n");
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

// void insertFlightPlan(Bucket **bucketList, int flightId, Time departureTime, Time ETA)
// {
//     FlightSchedule *newSchedule = createFlightSchedule(flightId, departureTime, ETA);
//     Bucket *temp = *bucketList;
//     if (temp == NULL)
//     {
//         Time ETAS, ETAE;
//         ETAS.hrs = ETA.hrs;
//         ETAS.min = 0;
//         ETAE.hrs = ETA.hrs + 1;
//         ETAE.min = 0;
//         *bucketList = createBucket(BucketIDStart, ETAS, ETAE);
//         (*bucketList)->flightSchedule = newSchedule;
//     }
//     else
//     {
//         Bucket *prev = *bucketList;
//         int flag = 1;
//         while (temp && flag == 1)
//         {
//             if (liesBetween(temp->etaStart, temp->etaEnd, newSchedule->eta) == 1)
//             {
//                 flag = 0;
//             }
//             else
//             {
//                 prev = temp;
//                 temp = temp->next;
//             }
//         }

//         // Add new bucket to the list
//         if (prev != NULL && temp == NULL && flag == 1)
//         {
//             Time bETAstart ;
//             bETAstart.hrs = (newSchedule->eta.hrs) % 24;
//             bETAstart.min = 0;
//             Time bETAend = prev->etaEnd;
//             bETAend.hrs = (newSchedule->eta.hrs + 1) % 24;
//             bETAend.min = 0;
//             Bucket *newBucket = createBucket(prev->bucketId + 1, bETAstart, bETAend);
//             newBucket->flightSchedule = newSchedule;
//             *bucketList = insertBucket(*bucketList, newBucket);
//             printf("New bucket is created with bucketId %d\n", prev->bucketId + 1);
//         }

//         else
//         {
//             if (temp->flightSchedule == NULL)
//             {
//                 temp->flightSchedule = newSchedule;
//             }
//             else if (maxTime(newSchedule->departureTime, temp->flightSchedule->departureTime) < 0)
//             {
//                 newSchedule->next = temp->flightSchedule;
//                 temp->flightSchedule = newSchedule;
//             }
//             else
//             {
//                 FlightSchedule *prev = temp->flightSchedule;
//                 FlightSchedule *curr = temp->flightSchedule;
//                 while (curr && maxTime(newSchedule->departureTime, curr->departureTime) >= 0)
//                 {
//                     prev = curr;
//                     curr = curr->next;
//                 }
//                 prev->next = newSchedule;
//                 newSchedule->next = curr;
//             }
//         }
//     }
//     return;
// }

// Bucket *insertBucket(Bucket *bucketList, Bucket *newBucket)
// {
//     if (bucketList == NULL)
//     {
//         bucketList = newBucket;
//     }
//     else if (timeDiff(bucketList->etaStart, newBucket->etaEnd) >= 0)
//     {
//         newBucket->next = bucketList;
//         bucketList = newBucket;
//     }
//     else
//     {
//         Bucket *temp = bucketList;
//         Bucket *prev = bucketList;
//         int flag = 1;
//         while (flag && temp)
//         {
//             if (timeDiff(newBucket->etaStart, temp->etaStart) >= 0 && timeDiff(newBucket->etaStart, temp->etaStart) < 60 && timeDiff(temp->etaEnd, newBucket->etaEnd) >= 0 && timeDiff(temp->etaEnd, newBucket->etaEnd) < 60)
//             {
//                 flag = 0;
//             }
//             else
//             {
//                 prev = temp;
//                 temp = temp->next;
//             }
//         }
//         prev->next = newBucket;
//         newBucket->next = temp;
//     }
//     return bucketList;
// }

Bucket* insertBucket(Bucket *bucketList, Bucket* newBucket){
    
    if(bucketList == NULL){
        bucketList = newBucket ;
    }
    else if(bucketList->etaStart.hrs > newBucket->etaEnd.hrs){
        newBucket->next = bucketList ;
        bucketList = newBucket ;
    }
    else{
        Bucket* temp = bucketList ;
        Bucket* prev ;
        int flag = 1 ;
        while(temp && flag){
            if(temp->etaEnd.hrs < newBucket->etaStart.hrs){
                prev = temp ;
                temp = temp->next ;
            }
            else{
                flag = 0 ;
            }
        }
        if(flag && !temp){
            newBucket->next = temp ;
            prev->next = newBucket ;
        }
        else if(temp){
            prev->next = newBucket ;
            newBucket->next = temp ;
        }
    }
    return bucketList ;
}

void insertFlightPlan(Bucket** bucketList, int flightId, Time departTime, Time ETA){
    FlightSchedule* newSchedule = createFlightSchedule(flightId, departTime, ETA);
    if(*bucketList == NULL){
        Time etaS, etaE;
        etaS.hrs = ETA.hrs ;
        etaS.min = 0 ;
        etaE.hrs = ETA.hrs ;
        etaE.min = 59 ;
        Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
        *bucketList = insertBucket(*bucketList, newBucket);
        (*bucketList)->flightSchedule = newSchedule ;
    }
    else{
        Bucket* temp = *bucketList ;
        Bucket* prev ;
        int flag = 1 ;
        while(temp && flag){
            if(liesBetween(temp->etaStart, temp->etaEnd, ETA)) flag = 0 ;
            else{
                prev = temp ;
                temp = temp->next ;
            }
        }

        if(flag == 1){
            printf("New bucket added with bucketID %d\n",BucketIDStart);
            Time etaS, etaE ;
            etaS.hrs = ETA.hrs ;
            etaS.min = 0 ;
            etaE.hrs = ETA.hrs ;
            etaE.min = 59 ;
            Bucket *newBucket = createBucket(BucketIDStart++, etaS, etaE);
            newBucket->flightSchedule = newSchedule ;
            *bucketList = insertBucket(*bucketList, newBucket);
        }

        else{
            if(temp->flightSchedule == NULL){
                temp->flightSchedule = newSchedule ;
            }
            else if (maxTime(newSchedule->departureTime, temp->flightSchedule->departureTime) < 0)
            {
                newSchedule->next = temp->flightSchedule;
                temp->flightSchedule = newSchedule;
            }
            else{
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
    return ;
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

void reArrangeBucket(Bucket **bucket)
{
    Time t;
    int hr, min;
    printf("Enter current time hours and minutes\n");
    scanf("%d %d", &hr, &min);
    t.hrs = hr;
    t.min = min;
    Bucket *temp = *bucket;
}
int main()
{

    // Time etaS;
    // etaS.hrs = 2;
    // etaS.min = 00;
    // Time etaE;
    // etaE.hrs = 2;
    // etaE.min = 59;
    // Bucket *newB = createBucket(90, etaS, etaE);
    // etaS.hrs = 1;
    // etaS.min = 00;
    // etaE.hrs = 1;
    // etaE.min = 59;
    // newB = insertBucket(newB, 91, etaS, etaE);
    // etaS.hrs = 3;
    // etaS.min = 00;
    // etaE.hrs = 3;
    // etaE.min = 59;
    // newB = insertBucket(newB, 92, etaS, etaE);
    // Time dT;
    // dT.hrs = 1;
    // dT.min = 30;
    // insertFlightPlan(newB, 110, dT, dT);
    // dT.hrs = 2;
    // dT.min = 30;
    // insertFlightPlan(newB, 111, dT, dT);
    // dT.hrs = 1;
    // dT.min = 36;
    // insertFlightPlan(newB, 112, dT, dT);
    // dT.hrs = 2;
    // dT.min = 24;
    // insertFlightPlan(newB, 113, dT, dT);
    // dT.hrs = 4;
    // dT.min = 24;
    // insertFlightPlan(newB, 114, dT, dT);
    // dT.hrs = 4;
    // dT.min = 23;
    // insertFlightPlan(newB, 115, dT, dT);
    // Print(newB);
    // // showStatus(newB, 110);
    // // showStatus(newB, 114);
    // deleteFlightPlan(&newB, 110);
    // Print(newB);

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

    return 0;
}