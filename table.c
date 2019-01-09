#include "table.h"

void print_row(struct table *t, int index){
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        if (t->types[i] == INT)
            printf("[%d] | ", get(&(t->al), index).values[i].integer);
        else if (t->types[i] == DOUBLE)
            printf("[%lf] | ", get(&(t->al), index).values[i].decimal);
        else if (t->types[i] == STRING)
            printf("[%s] | ", get(&(t->al), index).values[i].string);
    printf("\n");
}

void print_table(struct table *t){
    printf("Table: [%s]\n", t->name);
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s] | ", t->names[i]);
    printf("\n");

    for (int i = 0; i < t->al.num_rows; i ++){
        print_row(t, i);
        // printf("\n");
    }
}



//removes begining and ending spaces
char *rs(char *str){
    while (!strncmp(str, " ", 1))
        str ++;
    char *dummy = str;
    while (*dummy)
        dummy ++;
    dummy --;
    while (!strncmp(dummy, " ", 1)){
        *dummy = 0;
        dummy --;
    }
    return str;
}

int hs(char *str){
    while (*str){
        if (!strncmp(str, " ", 1))
            return 1;
        str ++;
    }
    return 0;
}

struct table *get_table(char *tname, struct database *db){
  for (int i = 0; i < db->num_tables; i ++)
    if ( !strcmp(db->tables[i].name, tname) )
           return db->tables + i;
  return 0;
}

void create_table(char *str, struct database *db){
    struct table *t = db->tables + db->num_tables;
    db->num_tables ++;

    construct(&(t->al));
    str = rs(str);
    strcpy(t->name, rs( strsep(&str, "{") ));
    // printf("[%s]\n", t->name);
    char *data = strsep(&str, "}");
    int i;
    char **piece = calloc(DATA_SIZE, sizeof(char *));
    char **type = calloc(DATA_SIZE, sizeof(char *));

    for (i = 0; piece[i] = strsep(&data, ","); i ++){
        piece[i] = rs(piece[i]);
        type[i] = strsep(&piece[i], " ");
        piece[i] = rs(piece[i]);
    }

    t->num_columns = i;
    t->types = calloc(i, sizeof(int));
    t->names = calloc(i, sizeof(char *));

    for (int j = 0; j < i; j ++){
        if ( !strcmp(type[j], "int") )
            t->types[j] = INT;
        else if ( !strcmp(type[j], "double") )
            t->types[j] = DOUBLE;
        else if ( !strcmp(type[j], "string") )
            t->types[j] = STRING;
        else {
            printf("invalid type: %s\n", type[j]);
            return;
        }
        t->names[j] = piece[j];
    }
    // print_table(t);
    // return t;
    free(piece);
    free(type);
}

int index_of(struct table *t, char *col){
    col = rs(col);
    for (int i = 0; i < t->num_columns; i ++)
        if ( !strcmp(t->names[i], col))
            return i;
    return -1;
}

void read_spec(struct table *t, char *expr){
    char **each;
    int *types, ctr = 0;
    while (*expr)
        if ( !strncmp(expr, "&", 1)){
            each[ctr] = strsep(&expr, "&");
            types[ctr] = 1;
            ctr ++;
        }
        else if ( !strncmp(expr, "|", 1)){
            each[ctr] = strsep(&expr, "|");
            types[ctr] = 0;
            ctr ++;
        }
        else
            expr ++;
    char *col = rs(strsep(&expr, "="));
    expr = rs(expr);
    // union value val;
    int index = index_of(t, col);
    if (index == -1){
        printf("column does not exist\n");
        return;
    }
    if (t->types[index] == INT) {
        int val = atoi(expr);
        for (int j = 0; j < t->al.num_rows; j ++)
            if (val == get(&(t->al), j).values[index].integer)
                print_row(t, j);
    }
        // val.integer = atoi(expr);
    else if (t->types[index] == DOUBLE) {
        double val = atof(expr);
        for (int j = 0; j < t->al.num_rows; j ++)
            if (val == get(&(t->al), j).values[index].decimal)
                print_row(t, j);
    }
        // val.decimal = atof(expr);
    else if (t->types[index] == STRING)
        for (int j = 0; j < t->al.num_rows; j ++)
            if ( !strcmp(expr, get(&(t->al), j).values[index].string) )
                print_row(t, j);
}

void read_table(char *str, struct database *db){

    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    // printf("[%s]\n", str);
    char *tname = rs( strsep(&str, " {") );
    struct table *t = get_table(tname, db);
    if (!t){
        printf("table does not exist\n");
        return;
    }
    // printf("[%s]\n", str);
    str = rs(str);
    // printf("[%s]\n", str);
    if ( !strncmp(str, "all", 3) ){
        if ( !*(str + 4) ){
            print_table(t);
            return;
        }
        strsep(&str, " ");
        str = rs(str);
        char *piece;
        while (piece = strsep(&str, ",")){
            piece = rs(piece);
            if ( !strncmp(piece, "where ", 6) ){
                read_spec(t, piece + 6);
            }
        }
        strsep(&str, " ");

    }

}

void insert(char *str, struct database *db){

    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    char *tname = rs( strsep(&str, "{") );
    struct table *t = get_table(tname, db);
    if (!t){
        printf("table does not exist\n");
        return;
    }
    char *data = strsep(&str, "}");
    char *row, *piece;
    struct row *r;
    while (strsep(&data, "(") && data){
        row = strsep(&data, ")");
        // printf("[%s]\n", row);
        if (!row){
            printf("missing closing '('\n");
            return;
        }
        r = malloc(sizeof(struct row));
        r->values = calloc(t->num_columns, sizeof(union value));
        // r = malloc(sizeof(struct row));
        for (int i = 0; i < t->num_columns; i ++){
            piece = rs( strsep(&row, ",") );
            // printf("[%s]\n", piece);
            if (!piece){
                printf("insufficient number of parameters\n");
                return;
            }
            if (t->types[i] == INT)
                r->values[i].integer = atoi(piece);
            else if (t->types[i] == DOUBLE)
                r->values[i].decimal = atof(piece);
            else if (t->types[i] == STRING)
                r->values[i].string = piece;
        }
        add_last(&(t->al), *r);
        free(r);
        // printf("[%s]\n", data);
    }
}

void delete(char *str, struct database *db){
    str = rs(str);
    char *tname = rs( strsep(&str, " ") );
    struct table *t = get_table(tname, db);
    if (!t){
        printf("table does not exist\n");
        return;
    }

}

void drop(char *str, struct database *db){
    struct table *t = 0;
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    str = rs(str);
    int i;
    for (i = 0; i < db->num_tables; i ++)
        if ( !strcmp(db->tables[i].name, str) )
            t = db->tables + i;
    if (!t){
        printf("table does not exist\n");
        return;
    }

    for (int j = i; j < db->num_tables - 1; j ++)
        db->tables[i] = db->tables[i + 1];
    db->num_tables --;
}

void sort(char *str, struct database *db){
    str = rs(str);
    char *tname = strsep(&str, " ");
    struct table *t = get_table(tname, db);
    if (!t){
        printf("table does not exist\n");
        return;
    }
    str = rs(str);
    if ( strncmp(str, "by ", 3) ){
        printf("invalid syntax\n");
        return;
    }
    char *col = rs(str + 3);
    // printf("%s\n", t->names[3]);
    for (int i = 0; i < t->num_columns; i ++){
        // printf("[%s]\n", t->names[i]);
        if (!strcmp(col, t->names[i])){
            // printf("%d", t->types[i]);
            quick_sort(&(t->al), i, t->types[i]);
            return;
        }
    }
    // printf("[%s]\n", col);
    printf("column does not exist\n");
}

void execute(char *str, struct database *db){
    str = rs(str);
    if (!strncmp(str, "create ", 7))
        create_table(str + 7, db);
    else if (!strncmp(str, "read ", 5))
        read_table(str + 5, db);
    else if (!strncmp(str, "insert ", 7))
        insert(str + 7, db);
    else if (!strncmp(str, "delete ", 7))
        delete(str + 7, db);
    else if (!strncmp(str, "drop ", 5))
        drop(str + 5, db);
    else if (!strncmp(str, "sort ", 5))
        sort(str + 5, db);
    else
        printf("Invalid Command\n");
}

int main(){
    // int shm_id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    struct database *db = malloc( sizeof(struct database) );
    db->tables = calloc(10, sizeof(struct table));
    db->num_tables = 0;
    char str[] = "   create  oof  {  int   i  ,  double    d   ,  string   s  } ";
    execute(str, db);
    // print_table(&(db->tables[0]));

    char ins[] = "  insert    oof  {  (1024 ,  2.718 ,  xD ) (   5, 91.7  , smd ) (  3 , 6.9 , lmao ) (   5, 3.14  , UwU )  }";
    execute(ins, db);
    // print_table(&(db->tables[0]));

    // char dr[] = " drop   oof   ";
    // execute(dr, db);
    // execute(ins, db);

    char rd[] = " read   oof   all where  d  =  91.7 ";
    execute(rd, db);

    char st[] = "  sort  oof by  d ";
    execute(st, db);
    // execute(rd, db);
    print_table(&(db->tables[0]));


    return 0;
}
