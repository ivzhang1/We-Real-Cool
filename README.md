# We-Real-Cool

Ivan Zhang, Simon Berens, William Lu (Period 4)

Simple CRUD Database System in C

### Query Syntax
* Things inside `[]` are optional.
* Things inside `<>` are variables.

#### Create

```
create <table_name> {
    <type> <column_name> [<tag>],
  [ <type> <column_name> [<tag>],
    ...
    <type> <column_name> [<tag>], ]
}
```
types: `int`, `double`, `string`

tags: `-primarykey`, `-autoinc`, `-default(<value>)`

#### Read

```
read <table_name> * [where <expression0> [<boolean_operator> <expression0> ... ] ]
read <table_name>.<column_name> [where <expression0> [<boolean_operator> <expression0> ... ] ]
```

expressions:
```
column_name <operation> value
value <operation> value
column_name <operation> column_name
```

operations: `=`, `>`, `<`

boolean operators: `&`, `|`
 - We only implemented and use single character because that's much easier to implement by `strsep()`.

#### Insert

```
insert <table_name> {
    (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>])
  [ (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>])
    ...
    (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>]) ]
}
```
#### Update

`update <table_name>.<column_name> to <value> [where <expression0> [<boolean_operator> <expression0> ... ] ]`

#### Delete

`delete <table_name> [where <expression0> [<boolean_operator> <expression0> ... ] ]`

`drop <table_name>;`

#### Sort

`sort <table_name> by <column_name>`

#### Load Commands From File
`source <file_name>`

#### How to Run
Open >= 2 terminals

In one terminal:
```
$ make
$ ./database <port>
```
In other terminals:
```
& ./account_manager <ip> <port>
```
- Create and Delete accounts that can access the database
```
& ./client <ip> <port>
```
 - Client's query commands must be separated by semicolons or newline.

 - If two or more clients try to access the server at the same time, the one that connects late will be blocked by the semaphore. That client can still type commands but they won't be processed until the the other client exits.

#### Bugs
 - We tried to catch as many reasonable bad commands possible, such as creating a table that already exists, inserting an incomplete row, and creating a table with multiple columns with the same name. However, if the user insists to enter commands that do not follow the syntax, segmentation faults may occur and the server will crash.
 - User can access the usernames table (and DROP it).
