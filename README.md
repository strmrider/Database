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
## Errors
