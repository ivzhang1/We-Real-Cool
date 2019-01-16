#include "table.h"

char *str_row(struct table *t, int index){
    char *s = malloc(BUFFER_SIZE);
    char *d = s;
    d += sprintf(d, "| ");
    for (int i = 0; i < t->num_columns; i ++)
        if (t->types[i] == INT)
            d += sprintf(d, "[%d] | ", get(t->al, index)->values[i].integer);
        else if (t->types[i] == DOUBLE)
            d += sprintf(d, "[%lf] | ", get(t->al, index)->values[i].decimal);
        else if (t->types[i] == STRING)
            d += sprintf(d, "[%s] | ", get(t->al, index)->values[i].string);
    d += sprintf(d, "\n");
    return s;
}

char *str_table(struct table *t, int *read){
    char *s = malloc(BUFFER_SIZE);
    char *d = s;
    d += sprintf(d, "| ");
    for (int i = 0; i < t->num_columns; i ++)
        d += sprintf(d, "[%s] | ", t->col_names[i]);
    d += sprintf(d, "\n");

    for (int i = 0; i < t->al->num_rows; i ++)
        if (read[i]){
            d = str_row(t, i);
            strcat(s, d);
            free(d);
        }
        // printf("\n");
    return s;
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
    t->tags = calloc(i, sizeof(int));
    t->col_names = calloc(i, sizeof(char *));
    t->defaults = calloc(i, sizeof(union value));
    t->al = malloc(sizeof(struct array_list));
    construct(t->al);
}

char *set_tag(struct table *t, int j, char *tag){
    // printf("[%s]\n", tag);
    char *s = malloc(BUFFER_SIZE);
    if (!strcmp(tag, "-primarykey")){
        for (int i = 0; i < j; i ++)
            if (t->tags[i] == PRIMARY_KEY){
                strcpy(s, "each table has at most 1 -primarykey column\n");
                return s;
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
    free(s);
    return 0;
}

char *create_table(char *str, struct database *db){

    char *s = malloc(BUFFER_SIZE);
    char *tname = rs( strsep(&str, "{") );
    if (get_table(tname, db)){
      sprintf(s, "table: %s exists\n", tname);
      return s;
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
            sprintf(s, "invalid type: %s\n", type[j]);
            return s;
        }
        t->col_names[j] = name[j];
        if (tag = strsep(piece + j, " "))
            set_tag(t, j, tag);
    }
    // print_table(t);
    // return t;
    free(piece);
    free(type);
    free(s);
    return 0;
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
                    read[r] += get(t->al, r)->values[index].integer != atoi(val) ;
                else if (t->types[index] == DOUBLE)
                    read[r] += get(t->al, r)->values[index].decimal != atof(val);
                else if (t->types[index] == STRING){
                    if (!str){
                        // printf("syntax error: need quotation marks for text field\n");
                        return 0;
                    }
                    read[r] += strcmp(get(t->al, r)->values[index].string, str);
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
        if (!subread)
            return 0;
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

char *read_spec(struct table *t, char *expr){

    char *s = malloc(BUFFER_SIZE);
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
    if (!read){
        strcpy(s, "syntax error");
        return s;
    }
    free(piece);
    free(s);
    s = str_table(t, read);
    free(read);
    return s;
}

char *read_table(char *str, struct database *db){

    char *s = malloc(BUFFER_SIZE);
    str = rs(str);
    // printf("[%s]\n", str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    // printf("[%s]\n", str);
    char *tname = rs( strsep(&str, " {") );
    struct table *t = get_table(tname, db);
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
    }
    // printf("[%s]\n", str);
    str = rs(str);
    // printf("[%s]\n", str);
    if ( !strncmp(str, "all", 3) ){
        // printf("[%s]", str + 3);
        if ( !str[3] ){
            // printf("#");
            int read[t->al->num_rows];
            for (int i = 0; i < t->al->num_rows; i ++)
                read[i] = 1;
            free(s);
            return str_table(t, read);
        }
        strsep(&str, " ");
        str = rs(str);
        char *piece;
        while (piece = strsep(&str, ",")){
            piece = rs(piece);
            if ( !strncmp(piece, "where ", 6) ){
                free(s);
                return read_spec(t, piece + 6);
            }
        }
        // strsep(&str, " ");

    }
    free(s);
    return 0;

}

char *insert(char *str, struct database *db){
    char *s = malloc(BUFFER_SIZE);
    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    char *tname = rs( strsep(&str, "{") );
    struct table *t = get_table(tname, db);
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
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
            strcpy(s, "missing closing ')'\n");
            return s;
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
                strcpy(s, "column does not exist\n");
                return s;
            }
            if (t->types[i] == INT)
                r->values[i].integer = atoi(piece);
            else if (t->types[i] == DOUBLE)
                r->values[i].decimal = atof(piece);
            else if (t->types[i] == STRING){
                piece = rq(piece);
                if (!piece){
                    strcpy(s, "syntax error: need quotation marks for text field\n");
                    return s;
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
                            r->values[j].integer = get(t->al, t->al->num_rows - 1)->values[j].integer + 1;
                    }
                    else if (t->types[j] == DOUBLE){
                        if (t->al->num_rows == 0)
                            r->values[j].decimal = 0;
                        else
                            r->values[j].decimal = get(t->al, t->al->num_rows - 1)->values[j].decimal + 1;
                    }
                }
                else if (t->tags[j] == DEFAULT){
                    if (t->types[j] == INT)
                        r->values[j].integer = t->defaults[j].integer;
                    else if (t->types[j] == DOUBLE)
                        r->values[j].decimal = t->defaults[j].decimal;
                }
                else {
                    strcpy(s, "insert failed: incomplete row\n");
                    return s;
                }
            }
            else
                fill[j] = 0;
        }
        add(t->al, r);
        // printf("[%s]\n", data);
    }
    free(s);
    return 0;
}

char *delete(char *str, struct database *db){
    char *s = malloc(BUFFER_SIZE);
    str = rs(str);
    char *tname = strsep(&str, " ");
    struct table *t = get_table(tname, db);
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
    }
    if (!str){
        while(t->al->num_rows)
            remov(t->al, t->al->num_rows - 1);
        free(s);
        return 0;
    }
    if ( strncmp(str, "where ", 6) ){
        strcpy(s, "invalid syntax\n");
        return s;
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
    free(s);
    return 0;
}

char *drop(char *str, struct database *db){
    char *s = malloc(BUFFER_SIZE);
    struct table *t = 0;
    // strcpy(s, "%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    str = rs(str);
    int i;
    for (i = 0; i < db->num_tables; i ++)
        if ( !strcmp(db->tables[i].name, str) )
            t = db->tables + i;
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
    }

    for (int j = i; j < db->num_tables - 1; j ++)
        db->tables[i] = db->tables[i + 1];
    db->num_tables --;
    free(s);
    return 0;
}

char *sort(char *str, struct database *db){
    char *s = malloc(BUFFER_SIZE);
    str = rs(str);
    char *tname = strsep(&str, " ");
    struct table *t = get_table(tname, db);
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
    }
    str = rs(str);
    if ( strncmp(str, "by ", 3) ){
        strcpy(s, "invalid syntax\n");
        return s;
    }
    char *col = rs(str + 3);
    // printf("%s\n", t->col_names[3]);
    for (int i = 0; i < t->num_columns; i ++){
        // printf("[%s]\n", t->col_names[i]);
        if (!strcmp(col, t->col_names[i])){
            // printf("%d", t->types[i]);
            quick_sort(t->al, i, t->types[i]);
            free(s);
            return 0;
        }
    }
    // printf("[%s]\n", col);
    strcpy(s, "column does not exist\n");
    return s;
}

char *update(char *str, struct database *db){
    char *s = malloc(BUFFER_SIZE);
    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    char *tname = rs( strsep(&str, "{") );
    struct table *t = get_table(tname, db);
    if (!t){
        strcpy(s, "table does not exist\n");
        return s;
    }
    return 0;
}

char *execute(char *str, struct database *db){
    char *s;
    str = rs(str);
    if (!strncmp(str, "create ", 7))
        s = create_table(str + 7, db);
    else if (!strncmp(str, "read ", 5))
        s = read_table(str + 5, db);
    else if (!strncmp(str, "insert ", 7))
        s = insert(str + 7, db);
    else if (!strncmp(str, "update ", 7))
        s = update(str + 7, db);
    else if (!strncmp(str, "delete ", 7))
        s = delete(str + 7, db);
    else if (!strncmp(str, "drop ", 5))
        s = drop(str + 5, db);
    else if (!strncmp(str, "sort ", 5))
        s = sort(str + 5, db);
    else{
        s = malloc(BUFFER_SIZE);
        strcpy(s, "invalid command\n");
    }
    if (s)
        return s;
    s = malloc(1);
    strcpy(s, "");
    return s;
}

int main(){
    // int shm_id = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    struct database *db = malloc( sizeof(struct database) );
    db->tables = calloc(10, sizeof(struct table));
    db->num_tables = 0;
    char a[] = "   create  oof  {  int   i  ,  double    d   ,  string   s  } ";
    printf("%s", execute(a, db));

    char b[] = "  insert    oof  { (   i:5, d:91.7  , s:\"smd\" )(d:2.718 , i:1024 ,   s:\"xD\" )  (d:5, s:\"oof\", i:6 )(  i:3 ,d: 6.9 ,s : \"lmao\" ) (   i:5, d:3.14  ,s  : \"UwU\" )  }";
    printf("%s", execute(b, db));

    //
    char c[] = " read   oof   all where i = 5";
    printf("%s", execute(c, db));

    char d[] = "  sort  oof by  d ";
    printf("%s", execute(d, db));

    char e[] = "create  foo  { int n  -primarykey, int  ctr -autoinc,  double x  -default(3.14), string txt }";
    printf("%s", execute(e, db));

    char f[] = "insert foo { (txt: \"this is fun\") (txt: \"boba\", x:5.56, ctr: 8) (txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\")(txt:\"asdf\") }";
    printf("%s", execute(f, db));

    char g[] = "read foo all";
    printf("%s", execute(g, db));

    return 0;
}
