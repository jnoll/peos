

#define EQ 1
#define NE 2
#define GT 3
#define LT 4
#define GE 5
#define LE 6

#define REPOSITORY_MASK 	0666

typedef struct {
	char *name;
	char *id;
	void *info;
	int type;
} repository;

typedef struct repository_entry repository_entry;

typedef struct {
	int type;
	char *id;
	void *object;
} repository_object;


typedef struct repository_queryresult repository_queryresult;

struct repository_queryresult {
	repository_object *entry;
	repository_queryresult *next;
};

struct repository_entry {
	char *id;
	int	nr;
	repository *(*open)(int,int);
	int (*close)(repository *);
	int (*assert)(repository *, char *, int operand, char *);
	int (*query)(repository_queryresult *, repository *, char *, int operand, char *);
	int (*queryclose)(repository_queryresult *);
	int (*read)(repository_object *, char *, char *);
	repository_entry *next;
};

repository * repository_open(char *);

int repository_close(repository *);

int repository_query(repository_queryresult *, repository *, char *, int, char *);

int repository_assert(repository *, char *, int, char *);

int repository_initexternal(char *id,
					repository *(*open)(int,int),
					int (*close)(repository *),
					int (*assert)(repository *, char *, int, char *),
					int (*query)(repository_queryresult *, repository *, char *, int, char *),
					int (*read)(repository_object *, char *, char *));

int repository_init();

