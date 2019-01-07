#include "commons.h"

void print_table(struct table *t){
    printf("Table: [%s]\n", t->name);
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s] | ", t->names[i]);
    printf("\n");

    for (int i = 0; i < t->al.num_rows; i ++){
        printf("| ");
        for (int j = 0; j < t->num_columns; j ++)
            if (t->types[j] == INT)
                printf("[%d] | ", get(&(t->al), i)->values[j].integer);
            else if (t->types[j] == DOUBLE)
                printf("[%lf] | ", get(&(t->al), i)->values[j].decimal);
            else if (t->types[j] == STRING)
                printf("[%s] | ", get(&(t->al), i)->values[j].string);
        printf("\n");
    }
}

void prompt(char display[], char input[]) {
    printf("%s", display);
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0'; // strip newline
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

void read_table(char *str, struct database *db){
    struct table *t = 0;
    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));

    if ( !strncmp(str, "all ", 4) ){
        str += 4;
        if ( strncmp(str, "from ", 5) ){
            printf("invalid syntax\n");
            return;
        }

        str += 5;
        str = rs(str);

        char *tname;
        if (hs(str))
            tname = strsep(&str, " ");
        else
            tname = str;

        for (int i = 0; i < db->num_tables; i ++)
            if ( !strcmp(db->tables[i].name, tname) )
                t = db->tables + i;

        if (!t){
            printf("table does not exist\n");
            return;
        }

        if (!hs(str))
            print_table(t);
    }

}

void insert(char *str, struct database *db){
    struct table *t = 0;
    str = rs(str);
    // printf("%s\n", db->tables[0].name);
    // print_table(&(db->tables[0]));
    char *tname = rs( strsep(&str, "{") );
    for (int i = 0; i < db->num_tables; i ++)
        if ( !strcmp(db->tables[i].name, tname) )
            t = db->tables + i;
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
        add_last(&(t->al), r);
        // printf("[%s]\n", data);
    }
}

void delete(char *str, struct database *db){

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
    print_table(&(db->tables[0]));

    char ins[] = "  insert    oof  {  (  3 , 6.9 , lmao ) (   5, 3.14  , UwU )  (1024 ,  2.718 ,  xD ) }";
    execute(ins, db);
    print_table(&(db->tables[0]));

    // char dr[] = " drop   oof   ";
    // execute(dr, db);
    // execute(ins, db);

    char rd[] = " read   all from  oof  ";
    execute(rd, db);

    return 0;
}
