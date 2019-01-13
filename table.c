#include "table.h"

void print_row(struct table *t, int index){
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        if (t->types[i] == INT)
            printf("[%d] | ", get(t->al, index).values[i].integer);
        else if (t->types[i] == DOUBLE)
            printf("[%lf] | ", get(t->al, index).values[i].decimal);
        else if (t->types[i] == STRING)
            printf("[%s] | ", get(t->al, index).values[i].string);
    printf("\n");
}

void print_table(struct table *t){
    // printf("Table: [%s]\n", t->name);
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s] | ", t->col_names[i]);
    printf("\n");

    for (int i = 0; i < t->al->num_rows; i ++){
        print_row(t, i);
        // printf("\n");
    }
}



//removes begining and ending spaces
char *rs(char *str){
    if (!str) return 0;
    while (*str && !strncmp(str, " ", 1))
        str ++;
    char *dummy = str + strlen(str) - 1;
    while (!strncmp(dummy, " ", 1))
        dummy --;
    *(dummy + 1) = 0;
    return str;
}

//removes quotation marks
char *rq(char *str){
    if (strncmp(str, "\"", 1))
        return 0;
    str ++;
    char *dummy = str;
    while (*dummy){
        if (!strncmp(dummy, "\"", 1)){
            *dummy = 0;
            return str;
        }
        dummy ++;
    }
    return 0;
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

void initialize(struct table *t, int i){
    t->num_columns = i;
    t->types = calloc(i, sizeof(int));
    t->tags = calloc(i * 3, sizeof(int));
    t->col_names = calloc(i, sizeof(char *));
    t->defaults = calloc(i, sizeof(union value));
    t->al = malloc(sizeof(struct array_list));
    construct(t->al);
}

void set_tag(struct table *t, int j, char *tag){
    // printf("[%s]\n", tag);
    if (!strcmp(tag, "-primarykey")){
        // if (t->tags[pos(j, DEFAULT)]){
        //     printf("-primarykey cannot coexist with -default\n");
        //     return;
        // }
        for (int i = 0; i < j; i ++)
            if (t->tags[i] == PRIMARY_KEY){
                printf("each table has at most 1 -primarykey column\n");
                return;
            }
        t->tags[j] = PRIMARY_KEY;
    }
    else if (!strcmp(tag, "-autoinc")){
        t->tags[j] = AUTO_INC;
    }
    else if (!strncmp(tag, "-defaults", 8)){
        t->tags[j] = DEFAULT;
        strsep(&tag, "(");
        char *val = rs( strsep(&tag, ")") );
        if (t->types[j] == INT)
            t->defaults[j].integer = atoi(val);
        else if (t->types[j] == DOUBLE)
            t->defaults[j].decimal = atof(val);
        else if (t->types[j] == STRING)
            t->defaults[j].string = val;
    }
}

void create_table(char *str, struct database *db){

    char *tname = rs( strsep(&str, "{") );
    if (get_table(tname, db)){
      printf("table: %s exists\n", tname);
      return;
    }

    struct table *t = db->tables + db->num_tables;
    db->num_tables ++;

    str = rs(str);

    strcpy(t->name, tname);
    // printf("[%s]\n", t->name);
    char *data = strsep(&str, "}");
    int i;
    char **piece = calloc(DATA_SIZE, sizeof(char *));
    char **type = calloc(DATA_SIZE, sizeof(char *));
    char **name = calloc(DATA_SIZE, sizeof(char *));

    for (i = 0; piece[i] = strsep(&data, ","); i ++){
        piece[i] = rs(piece[i]);
        type[i] = strsep(piece + i, " ");
        piece[i] = rs(piece[i]);
        name[i] = strsep(piece + i, " ");
        // printf("[%s]\n", piece[i]);
        piece[i] = rs(piece[i]);
    }

    initialize(t, i);

    char *tag;

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
        t->col_names[j] = name[j];
        if (tag = strsep(piece + j, " "))
            set_tag(t, j, tag);
    }
    // print_table(t);
    // return t;
    free(piece);
    free(type);
}

int index_of(struct table *t, char *col){
    col = rs(col);
    for (int i = 0; i < t->num_columns; i ++)
        if ( !strcmp(t->col_names[i], col))
            return i;
    return -1;
}

int *bool_and(struct table *t, char **piece, int c){
    char *col, *val, *str;
    int val_int;
    double val_double;
    char *val_str;
    int col_exists;
    int *read = calloc(t->al->num_rows, sizeof(int));
    for (int i = 0; i < c; i ++){
        col = rs( strsep(piece + i, "=") );
        val = rs( piece[i] );
        str = rq(val);
        int index = index_of(t, col);
        if (index == -1){
            val_int = atoi(col);
            val_double = atof(col);
            val_str = rq(col);
            col_exists = 0;
        }
        else
            col_exists = 1;
        for (int r = 0; r < t->al->num_rows; r ++)
            if (col_exists)
                if (t->types[index] == INT)
                    read[r] += get(t->al, r).values[index].integer != atoi(val) ;
                else if (t->types[index] == DOUBLE)
                    read[r] += get(t->al, r).values[index].decimal != atof(val);
                else if (t->types[index] == STRING){
                    if (!str){
                        printf("syntax error: need quotation marks for text field\n");
                        return 0;
                    }
                    read[r] += strcmp(get(t->al, r).values[index].string, str);
                }
                else
                    return 0;
            else
                read[r] += !(val_int && val_int == atoi(val) ||
                             val_double && val_double == atof(val) ||
                             val_str && !strcmp(val_str, str) );
    }
    for (int r = 0; r < t->al->num_rows; r ++)
        read[r] = !read[r];
    return read;
}

int *bool_or(struct table *t, char **piece, int c){
    int *read = calloc(t->al->num_rows, sizeof(int));
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
        if (!subread) return 0;
        free(subpiece);
        for (int r = 0; r < t->al->num_rows; r ++)
            read[r] += subread[r];
        free(subread);
    }
    return read;
    // for (int i = 0; i < t->al->num_rows; i ++)
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
    if (!read) return;
    free(piece);

    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s] | ", t->col_names[i]);
    printf("\n");

    for (int i = 0; i < t->al->num_rows; i ++)
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
    char *row, *piece, *col;
    int i;
    int *fill = calloc(t->num_columns, sizeof(int));
    struct row *r;
    while (strsep(&data, "(") && data){
        row = strsep(&data, ")");
        // printf("[%s]\n", row);
        if (!data){
            printf("missing closing ')'\n");
            return;
        }
        r = malloc(sizeof(struct row));
        r->values = calloc(t->num_columns, sizeof(union value));
        // r = malloc(sizeof(struct row));

        while(piece = strsep(&row, ",") ){
        // printf("[%s]\n", piece);
            col = rs(strsep(&piece, ":"));
            // printf("[%s]\n", col);
            piece = rs(piece);
            i = index_of(t, col);
            if (i == -1){
                printf("column does not exist\n");
                return;
            }
            if (t->types[i] == INT)
                r->values[i].integer = atoi(piece);
            else if (t->types[i] == DOUBLE)
                r->values[i].decimal = atof(piece);
            else if (t->types[i] == STRING){
                piece = rq(piece);
                if (!piece){
                    printf("syntax error: need quotation marks for text field\n");
                    return;
                }
                r->values[i].string = piece;
            }
            fill[i] = 1;
        }
        for (int j = 0; j < t->num_columns; j ++){
            // printf("%d\n", fill[j]);
            if (!fill[j]){
                // printf("%d\n", j);
                if (t->tags[j] == PRIMARY_KEY || t->tags[j] == AUTO_INC){
                    // printf("%d\n", j);
                    if (t->types[j] == INT){
                        if (t->al->num_rows == 0)
                            r->values[j].integer = 0;
                        else
                            r->values[j].integer = get(t->al, t->al->num_rows - 1).values[j].integer + 1;
                    }
                    else if (t->types[j] == DOUBLE){
                        if (t->al->num_rows == 0)
                            r->values[j].decimal = 0;
                        else
                            r->values[j].decimal = get(t->al, t->al->num_rows - 1).values[j].decimal + 1;
                    }
                }
                else if (t->tags[j] == DEFAULT){
                    if (t->types[j] == INT)
                        r->values[j].integer = t->defaults[j].integer;
                    else if (t->types[j] == DOUBLE)
                        r->values[j].decimal = t->defaults[j].decimal;
                }
                else {
                    printf("insert failed: incomplete row\n");
                    return;
                }
            }
            else
                fill[j] = 0;
        }
        add_last(t->al, *r);
        free(r);
        // printf("[%s]\n", data);
    }
}

void delete(char *str, struct database *db){
    str = rs(str);
    char *tname = strsep(&str, " ");
    struct table *t = get_table(tname, db);
    if (!t){
        printf("table does not exist\n");
        return;
    }
    if (!str){
        while(t->al->num_rows)
            remov(t->al, t->al->num_rows - 1);
        return;
    }
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
    for (int i = 0; i < t->al->num_rows; i ++)
        if (read[i + num_removed]){
            remov(t->al, i);
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
    // printf("%s\n", t->col_names[3]);
    for (int i = 0; i < t->num_columns; i ++){
        // printf("[%s]\n", t->col_names[i]);
        if (!strcmp(col, t->col_names[i])){
            // printf("%d", t->types[i]);
            quick_sort(t->al, i, t->types[i]);
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
    else if (!strncmp(str, "update ", 7))
        update(str + 7, db);
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
    char a[] = "   create  oof  {  int   i  ,  double    d   ,  string   s  } ";
    execute(a, db);

    char b[] = "  insert    oof  { (   i:5, d:91.7  , s:\"smd\" )(d:2.718 , i:1024 ,   s:\"xD\" )  (d:5, s:\"oof\", i:6 )(  i:3 ,d: 6.9 ,s : \"lmao\" ) (   i:5, d:3.14  ,s  : \"UwU\" )  }";
    execute(b, db);
    // print_table(&(db->tables[0]));


    char c[] = " read   oof   all where  3  = 3";
    execute(c, db);

    char d[] = "  sort  oof by  d ";
    execute(d, db);

    char e[] = "create  foo  { int n  -primarykey, int  ctr -autoinc,  double x  -default(3.14), string txt }";
    execute(e, db);

    char f[] = "insert foo { (txt: \"this is fun\") (x: 5.56) }";
    execute(f, db);
    // printf("%d\n", db->tables[1].tags[pos(2, 3)]);
    print_table(&(db->tables[1]));

    return 0;
}
