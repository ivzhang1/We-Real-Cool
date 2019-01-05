# We-Real-Cool

Ivan Zhang, Simon Berens, Runmin Lu (Period 4)

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

### Database Management Syntax
- Create Database

```
create <portnum> {
    <username> <password> [<permissions>],
    [<username> <password> [<permissions>],]
    ...
};
```

permissions: in the format `crud`

For example, if a user is to have only read and update permissions, the permissions would look like `-ru-`

- Update Password

`update <portnum>:<username> from <curpassword> to <newpassword>;`

- Update Port

`move <portnum> to <portnum>;`

- Delete Database

`delete <portnum>;`

- Delete User

`remove <portnum>:<username>;`