#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct contact {
    char name[50];
    char phone[20];
    struct contact *next;
} Contact;

Contact *head = NULL;
char name[50];
char src[50];
char phone[20];

Contact *createContact(char name[], char phone[]) {
    Contact *newContact = (Contact *) malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = NULL;
    return newContact;
}

Contact *searchContact(Contact *head, char name[]) {
    Contact *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Contact *searchNumber(Contact *head, char phone[]) {
    Contact *current = head;
    while (current != NULL) {
        if (strcmp(current->phone, phone) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addContact(Contact **head, char name[], char phone[]) {
    Contact *newContact = createContact(name, phone);
    newContact->next = *head;
    *head = newContact;
}

void seeAll(Contact *head) {
    system("cls"); //clear console!!
    printf("All phonebook contacts:\n");
    Contact *current = head;
    int i = 1;
    if (current == NULL) {
        printf("Your phone book is empty! Returning to menu...\n");
    }
    while (current != NULL) {
        printf("%d. Name: %s; Phone: %s\n", i, current->name, current->phone);
        ++i;
        current = current->next;
    }
    printf("\nPress any key to return to the main menu.");
    _getch();
    system("cls");
}

void readFile() {
    FILE *rf;
    rf = fopen("C:\\Users\\mures\\Desktop\\phonebook\\agenda.txt", "r");
    int problem = 0;
    char line[50];
    while (fgets(line, 100, rf)) {
        char *nameStart = strstr(line, "Name: ");
        char *phoneStart = strstr(line, "Phone: ");
        if (nameStart != NULL && phoneStart != NULL) {
            // Extract contact name
            char *nameEnd = strstr(nameStart, ";");
            if (nameEnd == NULL) {
                printf("Problem regarding contact file content! Missing ';' after contact name in line: %s\n", line);
                problem = 1;
                continue;
            }
            *nameEnd = '\0';
            char *name = nameStart + 6;

            // Extract number
            char *phoneEnd = strstr(phoneStart, "\n");
            if (phoneEnd == NULL) {
                printf("Problem regarding contact file content! Missing '\\n' after phone number in line: %s\n", line);
                problem = 1;
                continue;
            }
            *phoneEnd = '\0';
            char *phone = phoneStart + 7;

            // Add contact to phonebook
            Contact *clone = searchContact(head, name);
            if (clone == NULL) {
                Contact *clone = searchNumber(head, phone);
                if (clone == NULL) {
                    addContact(&head, name, phone);
                } else {
                    printf("Problem regarding contact file content! The phone number %s is already being assigned to contact %s, therefore this contact was not added.\n",
                           phone, clone->name);
                    problem = 1;
                }
            } else {
                printf("Problem regarding contact file content! There already exists a contact named %s, therefore this contact was not added.\n",
                       name);
                problem = 1;
            }
        } else {
            printf("Problem regarding contact file content! Incorrect format in line: %s\n", line);
            problem = 1;
        }
    }
    if (problem) {
        printf("\nPress any key to continue to the main menu.");
        _getch();
        system("cls");
    }
    fclose(rf);
}

void writeInFileOption(int context)  //context == 0 - option from menu; context == 1 - option from exit choice;
{
    system("cls");
    FILE *rw;
    int counter = 1;
    rw = fopen("C:\\Users\\mures\\Desktop\\phonebook\\agenda.txt", "w");
    Contact *current = head;
    while (current) {
        fprintf(rw, "%d. Name: %s; Phone: %s\n", counter, &current->name, &current->phone);
        current = current->next;
        counter++;
    }
    if (context == 0) {
        printf("File successfully updated! Press any key to return to the main menu.");
        _getch();
        system("cls");
    } else if (context == 1) {
        printf("File successfully updated! Press any key to exit.");
        _getch();
    }

}

void addContactsOption() {
    system("cls");
    do {
        printf("Enter a name (or 'q' to quit): ");
        getchar(); // consume newline character from previous input
        fgets(name, 50, stdin); // read a line of input, including spaces
        name[strcspn(name, "\n")] = 0; // remove newline character at the end
        Contact *clone = searchContact(head, name);
        if (clone == NULL) {
            if (strcmp(name, "q") != 0) {
                printf("Enter a phone number: ");
                scanf("%s", phone);
                Contact *clone = searchNumber(head, phone);
                if (clone == NULL) {
                    addContact(&head, name, phone);
                    system("cls");
                    printf("Contact %s has been successfully added.\n", name);
                } else {
                    printf("The phone number %s is already being assigned to contact %s. Please try again.\n\n", phone,
                           clone->name);
                }
            }
        } else
            printf("There already exists a contact named %s. Please try again.\n", name);

    } while (strcmp(name, "q") != 0);
    system("cls");
}

void searchContactByName() {
    system("cls");
    do {
        printf("Enter a name to search for (or 'q' to return): ");
        fflush(stdin);
        fgets(src, sizeof(src), stdin);
        src[strcspn(src, "\n")] = 0; // remove the newline character at the end of the string
        if (strcmp(src, "q") != 0) {
            Contact *result = searchContact(head, src);
            if (result == NULL) {
                printf("Contact not found.\n");
            } else {
                printf("Name: %s; Phone: %s\n", result->name, result->phone);
            }
        }
    } while (strcmp(src, "q") != 0);
    system("cls");
}

void searchContactByNumber() {
    system("cls");
    do {
        printf("Enter a phone number to search for (or 'q' to return): ");
        scanf("%s", src);
        if (strcmp(src, "q") != 0) {
            Contact *result = searchNumber(head, src);
            if (result == NULL) {
                printf("Contact not found.\n");
            } else {
                printf("Name: %s; Phone: %s\n", result->name, result->phone);
            }
        }
    } while (strcmp(src, "q") != 0);
    system("cls");
}

void searchContactOption() {
    system("cls");
    char choice[50];
    do {
        printf("Do you want to search by name (n) or by phone number (p)? (or press \"q\" to return to the main menu) \n");
        scanf("%s", choice);
        if (strcmp(choice, "n")==0)
            searchContactByName();
        if (strcmp(choice, "p")==0)
            searchContactByNumber();
    } while (strcmp (choice, "q") != 0);
}

void deleteContactOption() {
    int check = -1;
    char input[50];
    do {
        system("cls");
        printf("Enter the name of the contact you want deleted (or 'q' to return to menu): \n");

        if (check == 0)
            printf("Contact %s has not been found.\n", input);
        if (check == 1)
            printf("Contact %s has been deleted.\n", input);
        printf("CHECK");
        fgets(input, sizeof(name), stdin);
        input[strcspn(input, "\n")] = 0;  // remove newline character
        check = -1;
        if (strcmp(input, "q") != 0) {
            if (head == NULL) {
                system("cls");
                printf("Your phone book is empty! Returning to menu...\n");
                break;
            }
            Contact *temp = head;
            if (strcmp(head->name, input) == 0) {
                check = 1;
                head = head->next;
                free(temp);
            } else {
                Contact *prev = head;
                temp = temp->next;
                while (temp->next != NULL) {
                    if (strcmp(temp->name, input) == 0) {
                        check = 1;
                        prev->next = temp->next;
                        free(temp);
                    }
                    prev = prev->next;
                    temp = temp->next;
                }
                if (strcmp(temp->name, input) == 0) {
                    check = 1;
                    temp = NULL;
                    prev->next = NULL;
                    free(temp);
                } else
                    check = 0;
            }
        }

    } while (strcmp(input, "q") != 0);
}

void updateContactOption() {
    system("cls");
    char nameToUpdate[50], newInfo[50], option;
    printf("Enter the name of the contact to update: ");
    scanf("%s", nameToUpdate);
    Contact *contactToUpdate = searchContact(head, nameToUpdate);
    if (contactToUpdate == NULL) {
        printf("Contact not found.\n");
    } else {
        printf("Do you want to update the name (n) or phone number (p)? ");
        scanf(" %c", &option);
        if (option == 'n') {
            printf("Enter the new name: ");
            scanf("%s", newInfo);
            Contact *duplicateName = searchContact(head, newInfo);
            if (duplicateName != NULL && strcmp(duplicateName->phone, contactToUpdate->phone) != 0) {
                printf("The name %s is already being assigned to contact %s. Please try again.\n", newInfo, duplicateName->phone);
            } else {
                strcpy(contactToUpdate->name, newInfo);
                printf("Contact %s has been successfully updated.\n", nameToUpdate);
            }
        } else if (option == 'p') {
            printf("Enter the new phone number: ");
            scanf("%s", newInfo);
            Contact *duplicatePhone = searchNumber(head, newInfo);
            if (duplicatePhone != NULL && strcmp(duplicatePhone->name, nameToUpdate) != 0) {
                printf("The phone number %s is already being assigned to contact %s. Please try again.\n", newInfo, duplicatePhone->name);
            } else {
                strcpy(contactToUpdate->phone, newInfo);
                printf("Contact %s has been successfully updated.\n", nameToUpdate);
            }
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }
    printf("Press any key to return to the main menu.\n");
    _getch();
    system("cls");
}

int main() {
    int choice;
    readFile();
    do {
        choice = 0;
        printf("Welcome to your Phone Book! \nChoose your action:\n1. See all contacts \n2. Add contacts \n3. Search contacts \n4. Delete contacts \n5. Update a contact \n6. Update contacts file using current list \n9. Quit\n");
        scanf("%d", &choice);

        if (choice == 1)
            seeAll(head);
        if (choice == 2)
            addContactsOption();
        if (choice == 3)
            searchContactOption();
        if (choice == 4)
            deleteContactOption();
        if (choice == 5)
            updateContactOption();
        if (choice == 6)
            writeInFileOption(0);
    } while (choice != 9);
    system("cls");
    char end;
    printf("Do you want to save your changes? (y - yes, n - no)\n");
    do {
        scanf("%c", &end);
        if (end == 'y') {
            writeInFileOption(1);
            break;
        }
    } while (end != 'n');
    return 0;
}