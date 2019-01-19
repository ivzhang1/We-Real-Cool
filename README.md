# We-Real-Cool

Ivan Zhang, Simon Berens, William Lu (Period 4)

Simple CRUD Database System in C

### Query Syntax
* Things inside `[]` are optional.
* Things inside `<>` are variables.

####Create

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

####Read

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

####Insert

```
insert <table_name> {
    (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>])
  [ (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>])
    ...
    (<column_name>:<value>, [<column_name>:<value>, ..., <column_name>:<value>]) ]
}
```
####Update

`update <table_name>.<column_name> to <value> [where <expression0> [<boolean_operator> <expression0> ... ] ]`

####Delete

`delete <table_name> [where <expression0> [<boolean_operator> <expression0> ... ] ]`

`drop <table_name>;`

####Sort

`sort <table_name> by <column_name>`

####Load Commands From File
  - Commands must be separated by semicolons or newline.
`source <file_name>`
