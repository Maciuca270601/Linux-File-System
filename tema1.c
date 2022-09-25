#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
    void* data;
    ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

typedef struct stack_t stack_t;
struct stack_t
{
	struct linked_list_t *list;
};

#define MIN(x, y) ((x) < (y) ? (x) : (y))


//-------------------LinkedList Functions--------------------------------
linked_list_t* ll_create(unsigned int data_size) {
    linked_list_t* ll = calloc(1, sizeof(*ll));
    
	ll->data_size = data_size;

	return ll;
}

static ll_node_t* get_nth_node(linked_list_t* list, unsigned int n) {
    unsigned int len = list->size - 1;
    unsigned int i;
    ll_node_t* node = list->head;

    n = MIN(n, len);

    for (i = 0; i < n; ++i)
        node = node->next;
	
	return node;
}

static ll_node_t* create_node(const void* new_data, unsigned int data_size) {
    ll_node_t* node = calloc(1, sizeof(*node));

    node->data = malloc(data_size);

    memcpy(node->data, new_data, data_size);

    return node;
}

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data) {
    ll_node_t *new_node, *prev_node;

    if (!list)
        return;

    new_node = create_node(new_data, list->data_size);

    if (!n || !list->size) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        prev_node = get_nth_node(list, n - 1);
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }

    ++list->size;
}

ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n) {
    ll_node_t *prev_node, *removed_node;

    if (!list || !list->size)
        return NULL;

    if (!n) {
        removed_node = list->head;
        list->head = removed_node->next;
        removed_node->next = NULL;
    } else {
        prev_node = get_nth_node(list, n - 1);
        removed_node = prev_node->next;
        prev_node->next = removed_node->next;
        removed_node->next = NULL;
    }

    --list->size;

    return removed_node;
}

void ll_free(linked_list_t** pp_list) {
    ll_node_t *node;

    if (!pp_list || !*pp_list)
        return;

    while ((*pp_list)->size) {
        node = ll_remove_nth_node(*pp_list, 0);
        free(node->data);
        free(node);
    }

    free (*pp_list);
    *pp_list = NULL;
}


//-------------------Stack Functions--------------------------------
stack_t* st_create(unsigned int data_size) {
	stack_t *stack = calloc(1, sizeof(stack_t));

	stack->list = ll_create(data_size);

	return stack;
}

unsigned int st_is_empty(stack_t *st) {
	if (st->list->size == 0) {
		return 1;
	}
	return 0;
}

void* st_peek(stack_t *st) {
	return st->list->head->data;
}

void st_pop(stack_t *st) {
	linked_list_t *list = st->list;
	ll_node_t *node;
	node = ll_remove_nth_node(list, 0);

	free(node->data);
	free(node);
}

void st_push(stack_t *st, void *new_data) {
	linked_list_t *list = st->list;
	ll_add_nth_node(list, 0, new_data);
}

void st_clear(stack_t *st) {
	int cnt = st->list->size;
	for (int i = 0; i < cnt; i++) {
		st_pop(st);
	}
}

void st_free(stack_t *st) {
	st_clear(st);
	free(st->list);
	free(st);
}

//-------------------TOUCH--------------------------------
void touch (Dir* parent, char* name)  {
	File *node = calloc(1, sizeof(*node));
	if (node == NULL) {
		fprintf(stderr, "File calloc failed!\n");
		return;
	}

	node->name = malloc(MAX_INPUT_LINE_SIZE);
	if (node->name == NULL) {
		fprintf(stderr, "File->name malloc failed!\n");
		return;
	}
	strcpy(node->name, name);

	node->parent = parent;
	node->next = NULL;

	//Check if the file is the first one to be created
	if (parent->head_children_files == NULL) {
		parent->head_children_files = node;
		return;
	}
	
	//In this case the file must be positioned at the end
	//Before adding the file I have to check if it does not already exist
	//Since I check pos->next, first file might have the same name.
	File *pos = parent->head_children_files;
	if (strcmp(pos->name, name) == 0) {
		printf("File already exists\n");
		free(node->name);
		free(node);
		return;
	}
	while (pos->next != NULL) {
		if (strcmp(pos->name, name) == 0) {
			printf("File already exists\n");
			free(node->name);
			free(node);
			return;
		}
		pos = pos->next;
	}
	pos->next = node;
}

//-------------------MKDIR--------------------------------
void mkdir (Dir* parent, char* name) {
	Dir *node = calloc(1, sizeof(*node));
	if (node == NULL) {
		fprintf(stderr, "Director calloc failed!\n");
		return;
	}

	node->name = malloc(MAX_INPUT_LINE_SIZE);
	if (node->name == NULL) {
		fprintf(stderr, "Director->name malloc failed!\n");
		return;
	}
	strcpy(node->name, name);

	node->head_children_dirs = NULL;
	node->head_children_files = NULL;
	node->parent = parent;
	node->next = NULL;

	//Check if root directory already exists
	if (parent == NULL) {
		parent = node;
		return;		
	}

	//Check if the directory is the first one to be created
	if (parent->head_children_dirs == NULL) {
		parent->head_children_dirs = node;
		return;
	}

	//Same rules apply here as in touch
	Dir *pos = parent->head_children_dirs;
	if (strcmp(pos->name, name) == 0) {
		printf("Directory already exists\n");
		free(node->name);
		free(node);
		return;
	}
	while (pos->next != NULL) {
		if (strcmp(pos->name, name) == 0) {
			printf("Directory already exists\n");
			free(node->name);
			free(node);
			return;
		}
		pos = pos->next;
	}
	pos->next = node;
}

//-------------------LS--------------------------------
void ls (Dir* parent) {
	Dir *dir = parent->head_children_dirs;
	File *file = parent->head_children_files;

	//Showing all directories
	while (dir != NULL) {
		printf("%s\n", dir->name);
		dir = dir->next;
	}

	//Showing all files
	while (file != NULL) {
		printf("%s\n", file->name);
		file = file->next;
	}
}

//-------------------RM--------------------------------
void rm (Dir* parent, char* name) {
	File *node = parent->head_children_files;
	if (node == NULL) {
		printf("Could not find the file\n");
		return;
	}
	//Since I check node->next, I have to check also the first node,
	//before deleting it and remaking the connections.
	if (strcmp(node->name, name) == 0) {
		File *aux = node->next;
		free(node->name);
		free(node);
		parent->head_children_files = aux;
		return;
	}
	while (node->next != NULL) {
		if (strcmp(node->next->name, name) == 0) {
			File *aux = node->next->next;
			free(node->next->name);
			free(node->next);
			node->next = aux;
			return;
		}
		node = node->next;
	}
	printf("Could not find the file\n");
}

//-------------------FREE--------------------------------
void free_system(Dir *parent) {
	Dir *curr_dir;
	File *curr_file;

	//Frees all the files that a certain dir can have
	while (parent->head_children_files) {
		File *to_free = parent->head_children_files;
		parent->head_children_files = parent->head_children_files->next;
		free(to_free->name);
		free(to_free);
	}
	
	//Recursively frees all the directories
	while (parent->head_children_dirs) {
		free_system(parent->head_children_dirs);
		Dir *to_free = parent->head_children_dirs;
		parent->head_children_dirs = parent->head_children_dirs->next;
		free(to_free->name);
		free(to_free);
	}
}

//-------------------RMDIR--------------------------------
void rmdir (Dir* parent, char* name) {
	Dir *node = parent->head_children_dirs;
	if (node == NULL) {
		printf("Could not find the dir\n");
		return;
	}
	//Since I check node->next, I have to check also the first node,
	//before deleting it and remaking the connections.
	//In order to free a directory that might contain others directories
	//I am using the free_system function that works recursively.
	if (strcmp(node->name, name) == 0) {
		Dir *aux = node->next;
		free_system(node);
		free(node->name);
		free(node);
		parent->head_children_dirs = aux;
		return;
	}
	while (node->next != NULL) {
		if (strcmp(node->next->name, name) == 0) {

			Dir *aux = node->next->next;
			free_system(node->next);
			free(node->next->name);
			free(node->next);
			node->next = aux;
			return;
		}
		node = node->next;
	}
	printf("Could not find the dir\n");
}

//-------------------CD--------------------------------
void cd(Dir** target, char *name) {
	//If target->parent is NULL I can not apply ..
	//Otherwise I return target->parent.
	if (strcmp(name, "..") == 0) {
		if ((*target)->parent == NULL) {
			return;
		}
		*target = (*target)->parent;
		return;
	}

	//The main idea is using a pointer to determine the current position
	//This function is the only one capable of changing its value, and because
	//the value must change also in main, target is a double pointer.
	Dir *pos = (*target)->head_children_dirs;
	while (pos != NULL) {
		if (strcmp(pos->name, name) == 0) {
			*target = pos;
			return;
		}
		pos = pos->next;
	}
	printf("No directories found!\n");
}

//-------------------PWD--------------------------------
char *pwd (Dir* target) {
	//In order to show that path I need a stack so a recursive function should
	//do the trick but since the functions returns a string that I have to alloc,
	//I have decided to use a stack to not realloc the string in every call
	//The stack implementation is using simple linked lists.
	char *path = calloc(1, MAX_INPUT_LINE_SIZE);
	stack_t *stack = st_create(MAX_INPUT_LINE_SIZE);
	
	while (target != NULL) {
		st_push(stack, target->name);
		target = target->parent;
	}

	while (st_is_empty(stack) != 1) {
		strcat(path, "/");
		strcat(path, (char *)st_peek(stack));
		st_pop(stack);
	}

	st_free(stack);
	return path;
}

void stop (Dir* target) {}

//-------------------TREE--------------------------------
void tree (Dir* target, int level) {
	if (target == NULL) {
	 	return;
	}

	File *curr_file = target->head_children_files;
	Dir *curr_dir = target->head_children_dirs;
	//Iterates through all directories
	for (; curr_dir; curr_dir = curr_dir->next) {
		//Indents with the specific number of spaces
		for (int i = 0; i < 4 * level; i++) {
			printf(" ");
		}
		printf("%s\n", curr_dir->name);
		//Recursively calls tree in order to get to the next level
		tree(curr_dir, level + 1);
	}
	//Iterates through all files
	for (; curr_file; curr_file = curr_file->next) {
		//Indents with the specific number of spaces
		for (int i = 0; i < 4 * level; i++) {
			printf(" ");
		}
		printf("%s\n", curr_file->name);
	}
}

//-------------------MV--------------------------------
void mv(Dir* parent, char *oldname, char *newname) {
	Dir *dir = parent->head_children_dirs;
	File *file = parent->head_children_files;
    File *pos_file = file;
	Dir *pos_dir = dir;
	int dir_oldname = 0; // oldname dir does not exists
	int dir_newname = 0; // newname dir does not exists
	int file_oldname = 0; // oldname file does not exists
	int file_newname = 0; // newname file does not exists
	
	//Checks if oldname directory exists
	while (dir != NULL) {
		if (strcmp(dir->name, oldname) == 0) {
			dir_oldname = 1; // oldname dir does exists
			break;
		}
		dir = dir->next;
	}

	//If oldname dir does not exist check if oldname file exists
	while (file != NULL && dir_oldname == 0) {
		if (strcmp(file->name, oldname) == 0) {
			file_oldname = 1; // oldname file exists
			break;
		}
		file = file->next;
	}

	if (dir_oldname == 0 && file_oldname == 0) {
		printf("File/Director not found\n");
		return;
	}

	//Checks if newname file does exists
	while (pos_file != NULL) {
		if (strcmp(pos_file->name, newname) == 0) {
			file_newname = 1; // newname file exists
			break;
		}
		pos_file = pos_file->next;
	}

	//If newname file does not exist check if newname dir exists
	if (file_newname == 0) {
		while (pos_dir != NULL) {
			if (strcmp(pos_dir->name, newname) == 0) {
				dir_newname = 1; // newname dir exists
				break;
			}
			pos_dir = pos_dir->next;
		}
	}

	if (dir_newname == 1 || file_newname == 1) {
		printf("File/Director already exists\n");
		return;
	}

	if (file_oldname == 1) {
		//Since I check aux->next, I have to do a double check
		//Firstly is the first file is the file I am looking for
		//Secondly to get to the certain file I am looking for if not found yet
		File *aux = parent->head_children_files;
		if (strcmp(aux->name, file->name) == 0) {
			if (aux->next == NULL) {
				strcpy(dir->name, newname);
				return;
			}
			parent->head_children_files = aux->next;
			while (aux->next != NULL) {
				aux = aux->next;
			}
			aux->next = file;
			file->next = NULL;
			strcpy(file->name, newname);
			return;
		}
		while (strcmp(aux->next->name, file->name) != 0) {
			aux = aux->next;
		}
		aux->next = file->next;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = file;
		file->next = NULL;
		strcpy(file->name, newname);
	}

	if (dir_oldname == 1) {
		//Same logic as previous but the directories.
		Dir *aux = parent->head_children_dirs;
		if (strcmp(aux->name, dir->name) == 0) {
			if (aux->next == NULL) {
				strcpy(dir->name, newname);
				return;
			}
			parent->head_children_dirs = aux->next;
			while (aux->next != NULL) {
				aux = aux->next;
			}
			aux->next = dir;
			dir->next = NULL;
			strcpy(dir->name, newname);
			return;
		}
		while (strcmp(aux->next->name, dir->name) != 0) {
			aux = aux->next;
		}
		aux->next = dir->next;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = dir;
		dir->next = NULL;
		strcpy(dir->name, newname);
	}
}

//-------------------MAIN--------------------------------
int main () {
	//Creating root directory known also as home
	Dir *home = calloc(1, sizeof(*home));
	home->name = calloc(1, MAX_INPUT_LINE_SIZE);
	strcpy(home->name, "home");
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->next = NULL;
	home->parent = NULL;
	//Preparing the parser
	Dir *curr = home;
	//Initially is 1 but after stop was given as input it becomes 0
	int running = 1;
	char *sentence = malloc(MAX_INPUT_LINE_SIZE);
	char *command = malloc(20);
	char *name = malloc(50);
	char *name2 = malloc(50);

	do {
		fgets(sentence, MAX_INPUT_LINE_SIZE, stdin);
		sscanf(sentence, "%s %s %s", command, name, name2);

		if(strcmp(command, "touch") == 0) {
			touch(curr, name);
		}
		if(strcmp(command, "mkdir") == 0) {
			mkdir(curr, name);
		}
		if(strcmp(command, "ls") == 0) {
			ls(curr);
		}
		if(strcmp(command, "rm") == 0) {
			rm(curr, name);
		}
		if(strcmp(command, "rmdir") == 0) {
			rmdir(curr, name);
		}
		if(strcmp(command, "cd") == 0) {
			cd(&curr, name);
		}
		if(strcmp(command, "tree") == 0) {
			tree(curr, 0);
		}
		if(strcmp(command, "pwd") == 0) {
			char *ans = pwd(curr);
			printf("%s\n", ans);
			free(ans);
		}
		if(strcmp(command, "mv") == 0) {
			mv(curr, name, name2);
		}
		if(strcmp(command, "stop") == 0) {
			running = 0;
			free(sentence);
			free(command);
			free(name);
			free(name2);
		}
	} while (running == 1);

	free_system(home);
	free(home->name);
	free(home);
	
	return 0;
}
