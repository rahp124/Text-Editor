# Text-Editor
ll_text *append_text(ll_text *list, char *text): Use malloc to allocate memory for a new node. Then allocate memory for the string and copy it. Set the next pointer of the new node to null. If the list is empty, the new node becomes the front of the list. Traverse the list to find the last node. Append the new node to the end of the list. Return the front of the list.

ll_text *insert_text(ll_text *list, char *text, int position): Allocate memory for a new node. Allocate memory for the string and copy it. Set the next of the new node to null. If position is 0, insert at the front of the list. Traverse the list to find the position to insert. Insert the new node into the list. Return the front of the list.

ll_text *delete_text(ll_text *list, int position): If position is 0, free the first node and return the next node. Traverse the list to find the node before the position to delete. Copy the list, Delete the node using a pointer and free. Return the front of the list.

ll_text *replace_text(ll_text *list, char *text, int position): Traverse to the node to replace. Free the existing text. Allocate memory for the new string and copy it. Return the front of the list.

ll_text *duplicate_ll_text(ll_text *list): While original content isn’t null, Allocate memory for a new node. Allocate memory for the string and copy it. Link the new node to the duplicated list. Move to the next node in the original list. Return front of the list.

int ll_text_length(ll_text *list): Int length is 0, have a pointer to the current node, traverse the node till current is null while adding 1 to length each time. Return front of the list.

ll_text_stack *push_duplicate(ll_text_stack *stack): Create a new stack. Copy the linked list from the existing top of the stack. Link the new entry to the existing stack. Return the top of the stack.

ll_text_stack *push_empty(ll_text_stack *stack): Create a new stack. Initialize an empty linked list for the new entry. Link the new entry to the existing stack. Return the top of the stack.

ll_text_stack *pop_stack(ll_text_stack *stack); Check if the stack is empty. Store a pointer to the next entry in the stack. Free memory associated with the linked list in the top entry. Free memory associated with the top entry. Return the top of the stack.
