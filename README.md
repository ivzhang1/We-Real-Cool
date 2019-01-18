# We-Real-Cool

Ivan Zhang, Simon Berens, William Lu (Period 4)

Simple CRUD Database System in C

### Query Syntax
- Create

```
create <tablename> {
    <type> <colname> [<tags>],
    [<type> <colname> [<tags>],]
    ...
};
```
types: `int`, `double`, `string`

tags: `-primarykey`, `-autoinc`, `default(<value>)`

- Read

```
read <tablename> [all] [where <expr>];
```

expressions: `col_name =/</> val
              val =/</> val
              col_name =/</> col_name`

- Insert

```
insert <tablename> {
    (<colname>:<value>, [<colname>:<value>,] ...),
    [(<colname>:<value>, [<colname>:<value>,] ...),]
    ...
};
```

- Update

`update <tablename>.<colname> to <val> where <expr>;`

- Delete

`delete <tablename> where <expr>;`

`drop <tablename>;`

- Load From File

`source <filename>;`
