# Database
Custom, dynamic and mutable in-memory relational database with its own query language.

# Features
* Multi-table.
* Mutable database. supports insertion, update, deletion and selection of table and records.
* Cache storage for recent queries.
* Supports write/read from disk and backup.
* Custom-made query language.
* Direct API usage.

# Overview
## Structure

## DSL VS API
It is possible to use both a query language or directly API functions. The query language is more convinient and simple to use while the API is more reliable, flexible and saves compiling time. Any query modifications in the code (such as adding condition extensions) would also require the query language to be modified. Also, the parser currently doens't perform input validity check as the main purpose of the language is merely to simplify the usage of the database in order to try out and experience it.
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
## Setup
Include database library:
```cpp
#include "dblib.h"
using namespace database;
```
Insert databsae file path:
```cpp
Database database([path]);
```
If file path isn't provided databse would be volatile.

In case file exists, load databse into main memory:
```cpp
database.load();
```
When the use of database is no longer needed, clear it from memory;
```cpp
database.clear();
```
To use the query language
```cpp
Parser parser;
std::shared_ptr<Query> query = parser.parser([query as String]);
```
## Operations
### Create
```
table_name: create {column1, column2...} {data_type1....}
```
```
flights: create {plane, passangers, destination, time} {str, int, str, str}
```
```Cpp
std::shared_ptr<CreateTableQuery> createTable = 
std::shared_ptr<CreateTableQuery>(new CreateTableQuery("flights"));
createTable->addColumn("plane", STRING);
createTable->addColumn("passangers", INT);
createTable->addColumn("destination", STRING);
createTable->addColumn("time", STRING);
std::shared_ptr<QueryResult> result = database.query(createTable);
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
### Insert
```
table_name: insert {column1=value1 column2=value2...}
```
```
flights: insert {plane='Airbus 380', passangers=170, destination='Los Angeles', time='0700'}
```
```cpp
std::shared_ptr<InsertionQuery> creationQuery = 
  std::shared_ptr<InsertionQuery>(new InsertionQuery("flights"));
creationQuery->addField("plane", "Airbus 380");
creationQuery->addField("passangers", "170");
creationQuery->addField("destination", "Los Angeles");
creationQuery->addField("time", "0700");
result = database.query(creationQuery);
```
```
flights: insert {plane='Boeing 787', passangers=240, destination='Miami', time='1325'}
flights: insert {plane='Boeing 767', passangers=216, destination='New York', time='2245'}
flights: insert {plane='Airbus 320', passangers=164, destination='Dallas', time='0109'}
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
Boeing 787 | 240 | Miami | 1325
Boeing 767 | 216 | New York | 2245
Airbus 320 | 164 | Dallas | 0109
### Update
```
table_name: update {column1=new_value1, column2=new_value2...} {condition1 & (condition2 | condition3)}
```
```
flights: update {destination='Chicago', time='0030'} {passangers >= 210 & passangers < 240}
```
```cpp
// create conditions
std::shared_ptr<ComparisonCondition> con = 
std::shared_ptr<ComparisonCondition>(new ComparisonCondition("passangers", "210", GREATER_EQUAL));
std::shared_ptr<ComparisonCondition> con2 = 
std::shared_ptr<ComparisonCondition>(new ComparisonCondition("passangers", "240", LESSER));

// create conditions query
ConditionsQuery query;
query.condition(con);
query.setOperator(AND);
query.condition(con2);
query.done();
std::shared_ptr<ConditionsPack> conditions = query.get();

// update query
std::shared_ptr<UpdateQuery> update = std::shared_ptr<UpdateQuery>(new UpdateQuery("flights", conditions));
update->addField("destination", "Chicago");
update->addField("time", "0030");
result = database.query(update);
````
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
Boeing 787 | 240 | Miami | 1325
Boeing 767 | 216 | Chicago | 0030
Airbus 320 | 164 | Dallas | 0109
### Delete
```
table_name: delete {condition}
```
```
flights: delete {passangers = 240 | (destination='Dallas' | (time <'0100' & time >'0000'))}
```
```cpp
std::shared_ptr<DeletionQuery> deletion = 
	sstd::hared_ptr<DeletionQuery>(new DeletionQuery("flights", conditions));
result = database.query(deletion);
```
plane | passangers | destination | time
------------ | ------------- |------------ | -------------
Airbus 380 | 170 | Los Angeles | 0700
### Select
```
table_name: select {column1, column2...} {conditions}
```
```
flights: select {*} {*}
* - means all columns or no conditions
```
```cpp
conditions = shared_ptr<ConditionsPack>(new ConditionsPack());
std::shared_ptr<SelectQuery> select = 
	std::shared_ptr<SelectQuery>(new SelectQuery("flights", conditions));
resut = database.query(select);
```
Fetch the results:
```cpp
std::shared_ptr<Selections> selections = result->getSelections();
std::shared_ptr<std::vector<std::string>> row;
for (int i = 0; i < selections->size(); i++)
{
	row = (*selections)[0];
	for (string str : *row)
		cout << str << endl;
}
```
### Other
Delete entire table.
```
table_name: drop
```
Save table to disk.
```
table_name: save
```
### Errors
If an error is detected, the database will not execute the query and include an error data (error code and a detailed message) in the 'QueryResult' object.
```
flights: insert {plane='Airbus 380', passangers=1as5, destination='Los Angeles', time='0700'}
```
```cpp
std::shared_ptr<QueryResult> result = database.query([Query]);
if (result->isError())
	cout << res->getError().getMessage();
```
Output:
```
"Value '1as5' not compatible with Column's type";
```
Error types include incorrect actions, names, columns, incompatibility of values with types and not existed tables.
