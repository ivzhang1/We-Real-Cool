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
    // printf("Table: [%s]\n", t->name);
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

    char *tname = rs( strsep(&str, "{") );
    if (get_table(tname, db)){
      printf("table: %s exists\n", tname);
      return;
    }

    struct table *t = db->tables + db->num_tables;
    db->num_tables ++;

    construct(&(t->al));
    str = rs(str);

    strcpy(t->name, tname);
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

int *bool_and(struct table *t, char **piece, int c){
    char *col, *val;
    int *read = calloc(t->al.num_rows, sizeof(int));
    for (int i = 0; i < c; i ++){
        col = rs( strsep(piece + i, "=") );
        val = rs( piece[i] );
        int index = index_of(t, col);
        if (index == -1){
            printf("column does not exist\n");
            return 0;
        }
        for (int r = 0; r < t->al.num_rows; r ++)
            if (t->types[index] == INT)
                read[r] += get(&(t->al), r).values[index].integer != atoi(val);
            else if (t->types[index] == DOUBLE)
                read[r] += get(&(t->al), r).values[index].decimal != atof(val);
            else if (t->types[index] == STRING)
                read[r] += strcmp(get(&(t->al), r).values[index].string, val);
            else
                return 0;
    }
    for (int r = 0; r < t->al.num_rows; r ++)
        read[r] = !read[r];
    return read;
}

int *bool_or(struct table *t, char **piece, int c){
    int *read = calloc(t->al.num_rows, sizeof(int));
    int *subread;
    int ctr;
    char **subpiece;
    for (int j = 0; j < c; j ++){
        ctr = 0;
        subpiece = calloc(strlen(piece[j]), sizeof(char *));
        while (subpiece[ctr] = strsep(piece + j, "&")){
            subpiece[ctr] = rs(subpiece[ctr]);
            // printf("[%s]\n", subpiece[ctr]);
            ctr ++;
        }
        subread = bool_and(t, subpiece, ctr);
        free(subpiece);
        for (int r = 0; r < t->al.num_rows; r ++)
            read[r] += subread[r];
        free(subread);
    }
    return read;
    // for (int i = 0; i < t->al.num_rows; i ++)
    //     if (read[i])
    //         print_row(t, i);
    // free(read);
}

void read_spec(struct table *t, char *expr){
    expr = rs(expr);
    char **piece = calloc(strlen(expr), sizeof(char *));
    // piece[0] = expr;

    int ctr = 0;
    // printf("[%s]\n", expr);
    while (piece[ctr] = strsep(&expr, "|")){
        piece[ctr] = rs(piece[ctr]);
        ctr ++;
    }
    // printf("%d\n", ctr);
    int *read = bool_or(t, piece, ctr);
    free(piece);

    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s] | ", t->names[i]);
    printf("\n");

    for (int i = 0; i < t->al.num_rows; i ++)
        if (read[i])
            print_row(t, i);
    free(read);
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
        // strsep(&str, " ");

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
    str = rs(str);
    if ( strncmp(str, "where ", 6) ){
        printf("invalid syntax\n");
        return;
    }
    str += 6;
    str = rs(str);
    char **piece = calloc(strlen(str), sizeof(char *));
    // piece[0] = expr;

    int ctr = 0;
    // printf("[%s]\n", expr);
    while (piece[ctr] = strsep(&str, "|")){
        piece[ctr] = rs(piece[ctr]);
        ctr ++;
    }
    // printf("%d\n", ctr);
    int *read = bool_or(t, piece, ctr);
    free(piece);
    int num_removed = 0;
    for (int i = 0; i < t->al.num_rows; i ++)
        if (read[i + num_removed]){
            remov(&(t->al), i);
            num_removed ++;
            i --;
        }
    free(read);
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
        printf("invalid command\n");
}

int main(){
    // int shm_id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    struct database *db = malloc( sizeof(struct database) );
    db->tables = calloc(10, sizeof(struct table));
    db->num_tables = 0;
    char str[] = "   create  oof  {  int   i  ,  double    d   ,  string   s  } ";
    execute(str, db);
    // print_table(&(db->tables[0]));

    char ins[] = "  insert    oof  { (   5, 91.7  , smd )(1024 ,  2.718 ,  xD )  (5, 6, oof)(  3 , 6.9 , lmao ) (   5, 3.14  , UwU )  }";
    execute(ins, db);
    // print_table(&(db->tables[0]));

    // char dr[] = " drop   oof   ";
    // execute(dr, db);
    // execute(ins, db);

    char rd[] = " read   oof   all where  i  = 5";
    execute(rd, db);

    char st[] = "  sort  oof by  d ";
    execute(st, db);
    // execute(rd, db);
    print_table(&(db->tables[0]));

    char rm[] = " delete   oof    where  i = 5 &  d = 3.14  | s = xD";
    execute(rm, db);
    print_table(&(db->tables[0]));

    return 0;
}
