# Database
Custom dynamic, mutable, in-memory relational database with its own DSL query language.

# Features
* Multi-table.
* Mutable database. support insertion, update, deletion and selection of table and records.
* Cache for last queries.
* Support write/read from disk and backup.
* Custom-made query language.
* Direct API usage.

# Overview
## Structure

## DSL VS API
It is possible to use both a query language or directly API functions. The query language is more convinient and simple to use while the API is more reliable, flexbile and saves compiling time. Any query modifications in the code (such as adding condition extensions) would also require the query language to be modified. Also, the parser doens't perform input validity check as the main purpose of the language is merely to simplify the usage of the database.
## Data types
| Type     | Size (bytes) |
| ---      | ---       |
| string   | unlimited         |
| int     | 4        |
| long     | 8        |
| float     | 4        |
## Conditions
The database built in conditions include string or numeric comparisons such as the operators =, !=, >, <, >=, <=.
It is possible to write extensions and create more desiarble conditions (string manipulation, math etc) and attach them to a query directly throught the API.
## Operations
#### Create
```
table_name: create {column1, column2...} {data_type1....}

flights: create {plane, passangers, destination, time} {str, int, str, str}
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
#### Insert
```
table_name: insert {column1=value1 column2=value2...}
```
```
flights: insert {plane='Airbus 380', passangers=170, destination='Los Angeles', time='0700'}
flights: insert {plane='Boeing 787', passangers=240, destination='Miami', time='1325'}
flights: insert {plane='Boeing 767', passangers=216, destination='New York', time='2245'}
flights: insert {plane='Airbus 320', passangers=164, destination='Dalas', time='0109'}
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
Boeing 787 | 240 | Miami | 1325
Boeing 767 | 216 | New York | 2245
Airbus 320 | 164 | Dalas | 0109
#### Update
```
table_name: update {column1=new_value1, column2=new_value2...} {condition1 & (condition2 | condition3)}
```
```
flights: update {destination='Chicago', time='0030'} {passangers >= 210 & passangers < 240}
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
Boeing 787 | 240 | Miami | 1325
Boeing 767 | 216 | Chicago | 0030
Airbus 320 | 164 | Dalas | 0109
#### Delete
```
table_name: delete {condition}
```
```
flights: delete {passangers = 240 | (destination='Dalas' | (time <'0100' & time >'0000')}
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
Boeing 787 | 240 | Miami | 1325
#### Select
#### Other
Delete entire table.
```
table_name: drop
```
Save table to disk.
```
table_name: save
```
#### Errors

