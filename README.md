# We-Real-Cool

Ivan Zhang, Simon Berens, William Lu (Period 4)

Simple CRUD Database System in C

### Query Syntax
- Create

```
create <tablename> {
    <type> <colname> [<tags>][,
    <type> <colname> [<tags>]]
    ...
};
```
types: `int`, `double`, `string`

tags: `-primarykey`, `-autoinc`, `-foreignkey(<tablename>.<colname>)`, `default(<value>)`

- Insert

```
insert {
    (<colname>:<value>, [<colname>:<value>,] ...)[,
    (<colname>:<value>, [<colname>:<value>,] ...)]
     ...
} into <tablename>;
```

- Read

```
read [all]OR[{
     [$<operation>](<tablename>.<colname>) [as <name>][,
     [$<operation>](<tablename>.<colname>) [as <name>]]
     ...
}] from <tablename> [{
    <jointype> join <tablename> [on <expr>][,
    <jointype> join <tablename> [on <expr>]]
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

- Update

`update <tablename>.<colname> to <val> where <expr>;`

- Delete

`delete <tablename> where <expr>;`

`drop <tablename>;`

- Load From File

`source <filename>;`
