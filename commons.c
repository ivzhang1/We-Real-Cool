#include "commons.h"

void print_table(struct table *t){
    printf("Table: [%s]\n", t->name);
    printf("| ");
    for (int i = 0; i < t->num_columns; i ++)
        printf("[%s]\t| ", t->names[i]);
    printf("\n");

    for (int i = 0; i < t->al.num_rows; i ++){
        printf("| ");
        for (int j = 0; j < t->num_columns; j ++)
            if (t->types[j] == INT)
                printf("%d\t| ", t->al.rows[i].values[j].integer);
            else if (t->types[j] == DOUBLE)
                printf("%lf\t| ", t->al.rows[i].values[j].decimal);
            else if (t->types[j] == STRING)
                printf("%s\t| ", t->al.rows[i].values[j].string);
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

void create_table(char *str){
    struct table *t = malloc(sizeof(struct table));
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

    print_table(t);
}

void read_table(char *str){

}

void insert(char *str){

}

void delete(char *str){

}

void drop(char *str){
    str = rs(str);
}

void execute(char *str){
    str = rs(str);
    if (!strncmp(str, "create ", 7))
        create_table(str + 7);
    else if (!strncmp(str, "read ", 5))
        read_table(str + 5);
    else if (!strncmp(str, "insert ", 7))
        insert(str + 7);
    else if (!strncmp(str, "delete ", 7))
        delete(str + 7);
    else if (!strncmp(str, "drop ", 5))
        drop(str + 5);
    else
        printf("Invalid Command\n");
}

int main(){
    char str[] = "   create  oof  {  int   i  ,  double    d   ,  string   s  } ";
    execute(str);

    return 0;
}
