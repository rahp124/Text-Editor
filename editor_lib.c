#include "editor_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Functions about ll_text...
// append a line: add to the end of the linked list
ll_text *append_text(ll_text *list, char *text) {
    ll_text *new_node = (ll_text *)malloc(sizeof(ll_text));
    if (new_node == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_node->text = strdup(text);
    if (new_node->text == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_node->next = NULL;
    if (list == NULL) {
        return new_node;
    }
    ll_text *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return list;
}

void save_to_file(ll_text *list, const char *filename) {
  FILE *outfile = fopen(filename, "w");
  for (ll_text *here = list; here; here = here->next) {
    fprintf(outfile, "%s\n", here->text);
  }
  fclose(outfile);
}

int ll_text_length(ll_text *list) {
  int length = 0;
  ll_text *current = list;
  while (current != NULL) {
      length++;
      current = current->next;
  }
  return length;
}

// insert a specific line: add to specific place in the linked list; this is the
// new entry at the specified line
ll_text *insert_text(ll_text *list, char *text, int position) {
    ll_text *new_node = (ll_text *)malloc(sizeof(ll_text));
    if (new_node == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_node->text = strdup(text);
    if (new_node->text == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_node->next = NULL;
    if (position == 0) {
        new_node->next = list;
        return new_node;
    }
    ll_text *current = list;
    for (int i = 0; i < position - 1; i++) {
        if (current == NULL) {
            fprintf(stderr, "Invalid position\n");
            exit(1);
        }
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    return list;
}

// delete a specific line: delete the ith entry in the linked list and return
// the new front of the linked list.
ll_text *delete_text(ll_text *list, int position) {
    if (position == 0) {
        ll_text *next_node = list->next;
        free(list->text);
        free(list);
        return next_node;
    }
    ll_text *current = list;
    for (int i = 0; i < position - 1; i++) {
        if (current == NULL || current->next == NULL) {
            fprintf(stderr, "Invalid position");
            exit(1);
        }
        current = current->next;
    }
    ll_text *deleted_node = current->next;
    current->next = deleted_node->next;
    free(deleted_node->text);
    free(deleted_node);
    return list;
}

// replace a line: go to the specific entry, free the text that's there, replace
// it with a copy of the specified text.
ll_text *replace_text(ll_text *list, char *text, int position) {
    ll_text *current = list;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }
    free(current->text);
    current->text = strdup(text);
    return list;
}

// duplicate a list. Returns a copy of the list, including newly copied versions
// of all of the strings.
ll_text *duplicate_ll_text(ll_text *list) {
  ll_text *original_current = list;
    ll_text *new_list = NULL;
    ll_text *new_current = NULL;
    while (original_current != NULL) {
        ll_text *new_node = (ll_text *)malloc(sizeof(ll_text));
        if (new_node == NULL) {
            fprintf(stderr, "failed\n");
            exit(1);
        }
        new_node->text = strdup(original_current->text);
        if (new_node->text == NULL) {
            fprintf(stderr, "failed\n");
            exit(1);
        }
        new_node->next = NULL;
        if (new_list == NULL) {
            new_list = new_node;
            new_current = new_node;
        }
       	else {
            new_current->next = new_node;
            new_current = new_node;
        }
        original_current = original_current->next;
    }
    return new_list;
}

// functions about the state stack...
ll_text_stack *load_from_file(const char *filename) {
  ll_text *lines = NULL;
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  char buf[1024];
  FILE *infile = fopen(filename, "r");
  while (fgets(buf, 1024, infile)) {
    chomp(buf);
    lines = append_text(lines, buf);
  }
  stack->lines = lines;
  return stack;
}

ll_text_stack *new_stack_empty_text(void) {
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = NULL;
  return stack;
}

// Push a new state onto the stack that's an exact copy of the state currently
// at the top.
ll_text_stack *push_duplicate(ll_text_stack *stack) {
    ll_text_stack *new_entry = (ll_text_stack *)malloc(sizeof(ll_text_stack));
    if (new_entry == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_entry->lines = duplicate_ll_text(stack->lines);
    new_entry->next = stack;
    return new_entry;
}

// Push a new entry onto the stack that has an empty ll_text at the top.
ll_text_stack *push_empty(ll_text_stack *stack) {
    ll_text_stack *new_entry = (ll_text_stack *)malloc(sizeof(ll_text_stack));
    if (new_entry == NULL) {
        fprintf(stderr, "failed\n");
        exit(1);
    }
    new_entry->lines = NULL;
    new_entry->next = stack;
    return new_entry;
}

// undo, removing the most recent state
// this has to free all the memory associated with that state
ll_text_stack *pop_stack(ll_text_stack *stack) {
    if (stack == NULL) {
        return NULL;
    }
    ll_text_stack *new_top = stack->next;
    ll_text *next;
    while (stack->lines != NULL) {
        next = stack->lines->next;
        free(stack->lines->text);
        free(stack->lines);
        stack->lines = next;
    }
    free(stack);
    return new_top;
}

// Find the first newline in the input and replace it with \0
void chomp(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\n' || s[i] == '\r') {
      // Check for \r in case we're on Windows or a very old Mac??
      // Look, I won't stop you from doing CSE 13s on a Mac from 2001 or prior.
      s[i] = '\0';
      break;
    }
  }
}
