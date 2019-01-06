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
types: `int`, `double`, `string(<len>)`

tags: `-primarykey`, `-autoinc`, `-foreignkey(<tablename>.<colname>)`, `default(<value>)`

- Read

```
read from <tablename> [{
    <jointype> join <tablename> [on <expr>],
    [<jointype> join <tablename> [on <expr>],]
    ...
}] [all]OR[{
    [$<operation>](<tablename>.<colname>) [as <name>],
    [[$<operation>](<tablename>.<colname>) [as <name>],]
    ...
}] 
[where <expr>]
[group by <name>]
[order by <name> [<order>]]
[limit <num>];
```

operations: `avg`, `max`, `min`

expressions: `<name/val> == <name/val>`

orders: `asc`, `dsc`

- Insert

```
insert into <tablename> {
    (<colname>:<value>, [<colname>:<value>,] ...),
    [(<colname>:<value>, [<colname>:<value>,] ...),]
    ...
};
```

- Update

`update <tablename>.<colname> to <val> where <expr>;`

- Delete

`delete <tablename>.<colname> where <expr>;`

`drop <tablename>;`

- Load From File

`source <filename>;`
