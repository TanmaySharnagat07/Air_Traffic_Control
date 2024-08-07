# Air Traffic Control System

## Description

This project simulates an air traffic control (ATC) system with a digital dashboard to manage in-air flights. The system uses linked lists and AVL trees in C to handle and organize flight data efficiently. The dashboard maintains flights bucketed into 60-minute intervals based on their expected time of arrival (ETA). 

## Features

- **Flight Management**: Manage flight-plans within time buckets.
- **Efficient Data Retrieval**: Utilize AVL trees for sorting and searching flight-plans.
- **Dynamic Buckets**: Handle and rearrange buckets based on current time and flight ETAs.
- **File Handling**: Save and load flight data from/to text files.

## Data Structures

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/air-traffic-control.git
   cd air-traffic-control
2. **Run the Application**:
   ```bash
   gcc test.c
   ./a.out
   ./a.exe
