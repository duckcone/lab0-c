#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/**
 * create_new_element() - Create an element
 * @s: string would be inserted
 *
 * Argument s points to the string to be stored.
 *
 * Return: The pointer to the element.
 */
element_t *create_new_element(char *s)
{
    if (!s || !*s)
        return NULL;

    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        free(new_element);
        return NULL;
    }

    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return NULL;
    }
    return new_element;
}

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *current = NULL, *safe = NULL;
    list_for_each_entry_safe (current, safe, head, list) {
        q_release_element(current);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s || !*s)
        return false;

    element_t *new_element = create_new_element(s);
    if (!new_element)
        return false;
    list_add(&new_element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s || !*s)
        return false;

    element_t *new_element = create_new_element(s);
    if (!new_element)
        return false;
    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *head_element = list_first_entry(head, element_t, list);
    if (!head_element)
        return NULL;

    if (!sp) {
        return NULL;
    }

    memset(sp, '\0', bufsize);
    strncpy(sp, head_element->value, bufsize - 1);
    list_del(&head_element->list);
    return head_element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *tail_element = list_last_entry(head, element_t, list);
    if (!tail_element)
        return NULL;

    if (!sp)
        return tail_element;

    memset(sp, '\0', bufsize);
    strncpy(sp, tail_element->value, bufsize - 1);
    list_del(&tail_element->list);
    return tail_element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;

    struct list_head *tmp_node;
    int len = 0;
    list_for_each (tmp_node, head) {
        len++;
    }
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *head_ptr = head->next;
    struct list_head *tail_ptr = head->prev;

    while ((head_ptr != tail_ptr) && (head_ptr->next != tail_ptr)) {
        head_ptr = head_ptr->next;
        tail_ptr = tail_ptr->prev;
    }

    element_t *mid_element = list_entry(head_ptr, element_t, list);
    if (!mid_element)
        return false;

    list_del(head_ptr);
    q_release_element(mid_element);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head) || list_is_singular(head))
        return false;

    struct list_head *current, *next;
    list_for_each_safe (current, next, head) {
        const element_t *current_element = list_entry(current, element_t, list);
        struct list_head *dup_head = current;

        while (next != head) {
            const element_t *next_element = list_entry(next, element_t, list);

            if (strcmp(current_element->value, next_element->value) == 0) {
                next = next->next;
            } else {
                break;
            }
        }

        if (dup_head->next != next) {
            dup_head->prev->next = next;
            next->prev = dup_head->prev;

            while (dup_head != next) {
                struct list_head *temp = dup_head->next;
                element_t *delete_element =
                    list_entry(dup_head, element_t, list);
                list_del(dup_head);
                q_release_element(delete_element);
                dup_head = temp;
            }
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *current, *next;

    list_for_each (current, head) {
        next = current->next;
        list_move(current, next);
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *current, *safe;

    list_for_each_safe (current, safe, head) {
        current->next = current->prev;
        current->prev = safe;
    }
    safe = head->next;
    current->next = current->prev;
    current->prev = safe;

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
