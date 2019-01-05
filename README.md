# We-Real-Cool
Ivan Zhang, Simon Berens, Runmin Lu (Period 4)

Simple CRUD Database System in C

### Syntax
- Create

```
create <tablename> {
    <type> <colname> [tags],
    [<type> <colname> [tags],]
    ...
};
```
types: `int`, `string(<len>)`

tags: `-unique`, `-autoinc`

- Read

```
read {
    [$<operation>](<colname>) [as <name>],
    [[$<operation>](<colname>) [as <name>],]
    ...
} from <tablename>
[where <name> = <val>];
```

operations: `avg`, `max`, `min`

- Update

`update <colname> from <tablename> to <val> where <colname> = <val>;`

- Delete

`delete [from] <tablename> where <colname> = <val>;`
