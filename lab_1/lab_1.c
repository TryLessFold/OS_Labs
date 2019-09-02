#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct data
{
        char name[16];
        char sec_name[16];
        int middle_mark;
};

struct list
{
        struct list *next;
        struct list *prev;
        struct data info;
};

int add_list(struct list **head, struct list **tail, struct data input)
{
        if (NULL == (*head)) {
                (*head) = malloc(sizeof(struct list));
                if (NULL == memcpy(&(*head)->info, &input, sizeof(struct data))) {
                        return -1;
                }
                (*head)->prev = NULL;
                (*head)->next = NULL;
                (*tail) = (*head);
        }
        else {
                struct list *tmp = malloc(sizeof(struct list));
                if (NULL == memcpy(&tmp->info, &input, sizeof(struct data))) {
                        return -1;
                }
                (*tail)->next = tmp;
                tmp->next = NULL;
                tmp->prev = (*tail);
                (*tail) = tmp;
        }
        return 0;
}

int show_list(struct list *head, int start_p, int n)
{
        struct list *tmp = head;
        printf("%18s%18s%4s\n", "Name", "Family", "N.");
        for(int i = 0; i < start_p; i++) {
                if (tmp == NULL)
                        return -1;
                tmp = tmp->next;
        }
        for(int i = 0; i < n; i++) {
                if (tmp == NULL)
                        return -1;
                printf("%18s%18s%4d\n", tmp->info.name, tmp->info.sec_name,
                       tmp->info.middle_mark);
                tmp = tmp->next;
        }
        return 0;
}
int file_to_list(FILE *filename, struct list *head)
{

}

int list_to_file(FILE *filename, struct list *head)
{

}

int main()
{
	struct list *head = NULL, *tail = NULL;
	int list_counter = 0;
	struct data tmp = {"Heha", "How", 1};
	if (!add_list(&head, &tail, tmp))
		list_counter++;
        show_list(head, 0, list_counter);
	return 0;
}
