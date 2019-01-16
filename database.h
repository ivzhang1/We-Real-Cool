# define KEY 314159265

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int sem_setup();

int server_setup(char *port);

int get_client(int listening_sd);

void fulfill(int client_sd, int sem_id);

char * process(char *query, int sem_id);