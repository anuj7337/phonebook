#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a contact
typedef struct contact {
    char name[4];
    char num[10];
    struct contact* next; // Linked list pointer for contacts
} Contact;

// Structure to represent a node in the binary search tree
typedef struct treenode {
    struct treenode* left;
    struct treenode* right;
    Contact* details; // Pointer to the contact information
} Treenode;

typedef Treenode* (*DeleteContactFunction)(Treenode* root, char name[4]);

// Global variable representing the root of the binary search tree
Treenode* root = NULL;

// Function to create a new contact node
Treenode* createcontact(char name[4], char num[10]) {
    Treenode* newleaf = (Treenode*)malloc(sizeof(Treenode));
    Contact* new = (Contact*)malloc(sizeof(Contact));
    strcpy(new->name, name);
    strcpy(new->num, num);
    new->next = NULL; // Initialize the linked list pointer to NULL
    newleaf->details = new;
    newleaf->left = NULL;
    newleaf->right = NULL;
    return newleaf;
}

// Function to add a new contact to the phonebook
void addcontact(char name[4], char num[10]) {
    Treenode* current;
    Treenode* newcontact = createcontact(name, num);

    if (root == NULL) {
        // If the tree is empty, set the new contact as the root
        root = newcontact;
        return;
    }

    current = root;
    while (current != NULL) {
        if (strcmp(name, current->details->name) == 0) {
            // Add to the linked list of contacts with the same name
            newcontact->details->next = current->details->next;
            current->details->next = newcontact->details;
            return;
        } else if (strcmp(name, current->details->name) < 0) {
            if (current->left == NULL) {
                current->left = newcontact;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = newcontact;
                return;
            }
            current = current->right;
        }
    }
}

// Function to check if a contact with the given name exists in the phonebook
int exists(char name[4]) {
    Treenode* currentleaf = root;
    while (currentleaf != NULL) {
        if (strcmp(name, currentleaf->details->name) == 0) {
            return 1; // Contact with the given name exists
        }
        if (strcmp(name, currentleaf->details->name) < 0) {
            currentleaf = currentleaf->left;
        } else {
            currentleaf = currentleaf->right;
        }
    }
    return 0; // Contact with the given name does not exist
}

// Function to retrieve the phone number for a given name
char* printnumber(char name[4]) {
    Treenode* currentleaf = root;
    while (currentleaf != NULL) {
        if (strcmp(name, currentleaf->details->name) == 0) {
            return currentleaf->details->num;
        }
        if (strcmp(name, currentleaf->details->name) < 0) {
            currentleaf = currentleaf->left;
        } else {
            currentleaf = currentleaf->right;
        }
    }
    return NULL; // Contact not found
}

// Function to retrieve the name for a given phone number
char* printname(char num[10]) {
    Treenode* currentleaf = root;
    while (currentleaf != NULL) {
        if (strcmp(num, currentleaf->details->num) == 0) {
            return currentleaf->details->name;
        }
        if (strcmp(num, currentleaf->details->num) < 0) {
            currentleaf = currentleaf->left;
        } else {
            currentleaf = currentleaf->right;
        }
    }
    return NULL; // Contact not found
}

// Function to print all contacts in the phonebook (in-order traversal)
void printall(Treenode* node) {
    if (node != NULL) {
        printall(node->left);
        Contact* current = node->details;
        while (current != NULL) {
            printf("%s : %s\n", current->name, current->num);
            current = current->next;
        }
        printall(node->right);
    }
}

// Function to update the phone number for a given name and return the old phone number
char* updatenum(char name[4], char num1[10]) {
    Treenode* currentleaf = root;
    while (currentleaf != NULL) {
        if (strcmp(name, currentleaf->details->name) == 0) {
            // Allocate memory for the old phone number and copy it
            char* oldnum = (char*)malloc(strlen(currentleaf->details->num) + 1);
            strcpy(oldnum, currentleaf->details->num);

            // Update the phone number
            strcpy(currentleaf->details->num, num1);

            return oldnum; // Return the old phone number
        }

        if (strcmp(name, currentleaf->details->name) < 0) {
            currentleaf = currentleaf->left;
        } else {
            currentleaf = currentleaf->right;
        }
    }

    return NULL; // Contact not found
}

// Function to delete the entire phonebook
void deletephonebook(Treenode* root) {
    if (root != NULL) {
        // Recursively delete the left and right subtrees
        deletephonebook(root->left);
        deletephonebook(root->right);

        // Free the linked list of contacts within the node
        Contact* current = root->details;
        while (current != NULL) {
            Contact* temp = current; // Store the current contact pointer
            current = current->next; // Move to the next contact
            free(temp); // Free the current contact
        }

        free(root); // Free the node itself
    }
}

// Recursive function for contact deletion
Treenode* deletecontact_recursive(Treenode* root, char name[4]) {
    if (root == NULL) {
        return root;
    }

    // Check if the contact to be deleted is in the linked list of the current node
    Contact* current = root->details;
    Contact* prev = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }

    // If the contact is found in the linked list, update the linked list
    if (current != NULL) {
        if (prev != NULL) {
            prev->next = current->next;
        } else {
            root->details = current->next;
        }

        free(current); // Free the memory for the contact
    }

    // Recursively update the left and right subtrees
    root->left = deletecontact_recursive(root->left, name);
    root->right = deletecontact_recursive(root->right, name);

    return root;
}

// Function to delete a specific contact
void deletecontact(char name[4]) {
    DeleteContactFunction deleteFunction = deletecontact_recursive;
    root = deleteFunction(root, name);
}

// Function to return all contacts who have a specific substring in the name
void searchcontacts(Treenode* root, const char* substring) {
    if (root != NULL) {
        // Traverse the left subtree
        searchcontacts(root->left, substring);

        // Check if the substring is present in the contact names
        Contact* current = root->details;
        while (current != NULL) {
            if (strstr(current->name, substring) != NULL) {
                printf("%s : %s\n", current->name, current->num);
            }
            current = current->next;
        }

        // Traverse the right subtree
        searchcontacts(root->right, substring);
    }
}

int main() {
    // Example usage of the implemented functions
    addcontact("uABC", "1234567890");
    addcontact("uDEF", "6942012345");
    addcontact("uGHI", "0987654321");

    printf("\nAll contacts:\n");
    printall(root);

    char* oldnum = updatenum("uABC", "9999999999"); // Update ABC's number
    if (oldnum != NULL) {
        free(oldnum);
    } else {
        printf("Contact not found.\n");
    }

    printf("\nAll contacts:\n");
    printall(root); // Printing all contacts after update

    printf("\nSearch results for 'u':\n");
    searchcontacts(root, "u"); // Searching for all contacts with 'A' in the name

    deletecontact("uDEF"); // Deleting DEF's contact

    printf("\nAll contacts: after deletion:\n");
    printall(root);

    // Free the memory used by the phonebook
    deletephonebook(root);

    return 0;
}
