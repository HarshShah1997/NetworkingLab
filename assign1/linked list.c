#include <stdio.h>
#include <malloc.h>
#define ISEMPTY printf("\nEMPTY LIST:");
struct node{
    int value;
    struct node *next;
};
typedef struct node snode;
snode* create_node(int);
void insert_node_first();
void insert_node_last();
void insert_node_pos();
void sorted_ascend();
void delete_pos();
void search();
void update_val();
void display();
void rev_display(snode *);
snode *newnode, *ptr, *prev, *temp;
snode *first = NULL, *last = NULL;
int main(){
    int ch;
    char ans = 'Y';
    while (ans == 'Y'||ans == 'y'){
        printf("\nOperations on singly linked list\n");
        printf("\n1.Insert node at first");
        printf("\n2.Insert node at last");
        printf("\n3.Insert node at position");
        printf("\n4.Sorted Linked List in Ascending Order");
        printf("\n5.Delete Node from any Position");
        printf("\n6.Update Node Value");
        printf("\n7.Search Element in the linked list");
        printf("\n8.Display List from Beginning to end");
        printf("\n9.Display List from end using Recursion");
        printf("\n10.Exit\n");
        printf("\nEnter your choice");
        scanf("%d", &ch);
	switch (ch){
        case 1: 
            printf("\nInsert node at start\n");
            insert_node_first();
            break;
        case 2: 
            printf("\nInsert node at last\n");
            insert_node_last();
            break;
        case 3: 
            printf("\nInsert node at position\n");
            insert_node_pos();
            break;
        case 4: 
            printf("\nSort Linked List in Ascending\n");
            sorted_ascend();
            break;
        case 5: 
            printf("\nDelete Node from any Position\n");
            delete_pos();
            break;
        case 6: 
            printf("\nUpdate Node Value\n");
            update_val();
            break;
        case 7: 
            printf("\nSearch Element List\n");
            search();
            break;
        case 8: 
            printf("\nDisplay List From Top\n");
            display();
            break;
        case 9: 
            printf("\nDisplaying List From End\n");
            rev_display(first);
            break;
        case 10: 
            printf("\nExit\n");
            return 0;
            break;

        default: 
            printf("\nInvalid Choice\n");
            break;
        }
        printf("\nCONTINUE (Y/N)");
        scanf(" %c", &ans);
    }
    return 0;
 }
snode* create_node(int val){
    newnode = (snode *)malloc(sizeof(snode));
    if (newnode == NULL){
        printf("\nMemory not allocated");
        return 0;
    }
    else{
        newnode->value = val;
        newnode->next = NULL;
        return newnode;
    }
}
void insert_node_first(){
    int val;
    printf("\nEnter the value:");
    scanf("%d", &val);
    newnode = create_node(val);
    if (first == last && first == NULL){
        first = last = newnode;
        first->next = NULL;
        last->next = NULL;
    }
    else{
        temp = first;
        first = newnode;
        first->next = temp;
    }
    printf("\nINSERTED");    
}
void insert_node_last(){
    int val;
    printf("\nEnter the value:");    
    scanf("%d", &val);
    newnode = create_node(val);
    if (first == last && last == NULL){
        first = last = newnode;
        first->next = NULL;
        last->next = NULL;
    }
    else{
        last->next = newnode;
        last = newnode;
        last->next = NULL;
    }
    printf("\nINSERTED");
}    
void insert_node_pos(){
    int pos, val, cnt = 0, i;
    printf("\nEnter the value:");
    scanf("%d", &val);
    newnode = create_node(val);
    printf("\nEnter the position ");
    scanf("%d", &pos);
    ptr = first;
    while (ptr != NULL){
        ptr = ptr->next;
        cnt++;
    }
    if (pos == 1){
        if (first == last && first == NULL){
            first = last = newnode;
            first->next = NULL;
            last->next = NULL;
        }
        else{
            temp = first;
            first = newnode;
            first->next = temp;
        }
        printf("\nInserted");
    }
    else if (pos>1 && pos<=cnt){
        ptr = first;
        for (i = 1;i < pos;i++){
            prev = ptr;
            ptr = ptr->next;
        }
        prev->next = newnode;
        newnode->next = ptr;
        printf("\nINSERTED");
    }
    else{
        printf("Position is out of range");
    }
}
void sorted_ascend(){
    snode *nxt;
    int t;
    if (first == NULL){
        ISEMPTY;
        printf(":No elements to sort\n");
    }
    else{
        for (ptr = first;ptr != NULL;ptr = ptr->next){
            for (nxt = ptr->next;nxt != NULL;nxt = nxt->next){
                if (ptr->value > nxt->value){    
                    t = ptr->value;
                    ptr->value = nxt->value;
                    nxt->value = t;
                }
            }
        }
        printf("\n---Sorted List---");
        for (ptr = first;ptr != NULL;ptr = ptr->next){
            printf("%d\t", ptr->value);
        }
    }
}
void delete_pos(){
    int pos, cnt = 0, i;
    if (first == NULL){
        ISEMPTY;
        printf(":No node to delete\n");
    }
    else{
        printf("\nEnter the position of value to be deleted:");
        scanf(" %d", &pos);
        ptr = first;
        if (pos == 1){
            first = ptr->next;
            printf("\nElement deleted");    
        }
        else{
            while (ptr != NULL){
                ptr = ptr->next;
                cnt = cnt + 1;
            }
            if (pos > 0 && pos <= cnt){
                ptr = first;
                for (i = 1;i < pos;i++){
                    prev = ptr;
                    ptr = ptr->next;
                }
                prev->next = ptr->next;
            }
            else{
                printf("Position is out of range");
            }
        free(ptr);
        printf("\nElement deleted");
        }
    }
}
void update_val(){
    int oldval, newval, flag = 0;
    if (first == NULL){
        ISEMPTY;
        printf(":No nodes in the list to update\n");
    }
    else{
        printf("\nEnter the value to be updated:");
        scanf("%d", &oldval);
        printf("\nEnter the newvalue:");    
        scanf("%d", &newval);
        for (ptr = first;ptr != NULL;ptr = ptr->next){
            if (ptr->value == oldval){
                ptr->value = newval;
                flag = 1;
                break;
            }
        }
        if (flag == 1){
            printf("\nUpdated Successfully");
        }
        else{
            printf("\nValue not found in List");
        }
    }    
}
void search(){
    int flag = 0, key, pos = 0;
    if (first == NULL){
        ISEMPTY;
        printf(":No nodes in the list\n");
    }
    else{
        printf("\nEnter the value to search");
        scanf("%d", &key);
        for (ptr = first;ptr != NULL;ptr = ptr->next){
            pos = pos + 1;
            if (ptr->value == key){
                flag = 1;
                break;
            }
        }
        if (flag == 1){
            printf("\nElement %d found at %d position\n", key, pos);
        }
        else{
            printf("\nElement %d not found in list\n", key);
        }
    }    
}
void display(){
    if (first == NULL){
        ISEMPTY;
        printf(":No nodes in the list to display\n");
    }
    else{
        for (ptr = first;ptr != NULL;ptr = ptr->next){    
            printf("%d\t", ptr->value);
        }
    }
}
void rev_display(snode *ptr){
    int val;
    if (ptr == NULL){
        ISEMPTY;
        printf(":No nodes to display\n");
    }
    else{
        if (ptr != NULL){
            val = ptr->value;
            rev_display(ptr->next);
            printf("%d\t", val);        
        }
    }
}
