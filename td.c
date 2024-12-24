#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 100
#define NAME_LENGTH 50
#define PHONE_LENGTH 11 // 10 digits + null terminator
#define EMAIL_LENGTH 50
#define DATA_FILE "contacts.dat"

// Contact structure
struct Contact {
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
};

struct Contact directory[MAX_CONTACTS];
int contactCount = 0;

// Function to validate name
int isValidName(const char *name) {
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

// Function to validate phone number
int isValidPhone(const char *phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Function to validate email
int isValidEmail(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && at != email && *(dot + 1) != '\0';
}

// Function to save contacts to file
void saveContactsToFile() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        printf("Error: Unable to save contacts to file.\n");
        return;
    }
    fwrite(directory, sizeof(struct Contact), contactCount, file);
    fclose(file);
}

// Function to load contacts from file
void loadContactsFromFile() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) return;
    contactCount = fread(directory, sizeof(struct Contact), MAX_CONTACTS, file);
    fclose(file);
}

// Function to check if the contact already exists (by name, phone, or email)
int isDuplicateContact(const struct Contact *newContact) {
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(directory[i].name, newContact->name) == 0) {
            return 1; // Duplicate name found
        }
        if (strcmp(directory[i].phone, newContact->phone) == 0) {
            return 1; // Duplicate phone found
        }
        if (strcmp(directory[i].email, newContact->email) == 0) {
            return 1; // Duplicate email found
        }
    }
    return 0; // No duplicates found
}

// Function to add a contact
void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Directory is full. Cannot add more contacts.\n");
        return;
    }

    struct Contact newContact;

    do {
        printf("Enter name (only characters and spaces): ");
        scanf(" %[^\n]%*c", newContact.name);
        if (!isValidName(newContact.name)) {
            printf("Invalid name. Please use only alphabets and spaces.\n");
        }
    } while (!isValidName(newContact.name));

    do {
        printf("Enter phone number (exactly 10 digits): ");
        scanf("%s", newContact.phone);
        if (!isValidPhone(newContact.phone)) {
            printf("Invalid phone number. Please enter a 10-digit number.\n");
        }
    } while (!isValidPhone(newContact.phone));

    do {
        printf("Enter email: ");
        scanf("%s", newContact.email);
        if (!isValidEmail(newContact.email)) {
            printf("Invalid email. Please enter a valid email address.\n");
        }
    } while (!isValidEmail(newContact.email));

    // Check if the new contact is a duplicate
    if (isDuplicateContact(&newContact)) {
        printf("Error: This contact already exists in the directory (name, phone, or email).\n");
        return; // Don't save the contact if it's a duplicate
    }

    directory[contactCount++] = newContact;
    saveContactsToFile();
    printf("Contact added successfully.\n");
}

// Function to display all contacts
void viewContacts() {
    if (contactCount == 0) {
        printf("No contacts in the directory.\n");
        return;
    }

    printf("\nTelephone Directory:\n");
    for (int i = 0; i < contactCount; i++) {
        printf("%d. Name: %s, Phone: %s, Email: %s\n",
               i + 1, directory[i].name, directory[i].phone, directory[i].email);
    }
}

// Function to delete a contact
void deleteContact() {
    if (contactCount == 0) {
        printf("No contacts to delete.\n");
        return;
    }

    int index;
    viewContacts();
    printf("\nEnter the contact number to delete: ");
    scanf("%d", &index);

    if (index < 1 || index > contactCount) {
        printf("Invalid contact number.\n");
        return;
    }

    for (int i = index - 1; i < contactCount - 1; i++) {
        directory[i] = directory[i + 1];
    }
    contactCount--;
    saveContactsToFile();
    printf("Contact deleted successfully.\n");
}

// Function to update a contact
void updateContact() {
    if (contactCount == 0) {
        printf("No contacts to update.\n");
        return;
    }

    int index;
    viewContacts();
    printf("\nEnter the contact number to update: ");
    scanf("%d", &index);

    if (index < 1 || index > contactCount) {
        printf("Invalid contact number.\n");
        return;
    }

    struct Contact *contact = &directory[index - 1];

    printf("Updating contact: %s\n", contact->name);

    do {
        printf("Enter new name (only characters and spaces): ");
        scanf(" %[^\n]%*c", contact->name);
        if (!isValidName(contact->name)) {
            printf("Invalid name. Please use only alphabets and spaces.\n");
        }
    } while (!isValidName(contact->name));

    do {
        printf("Enter new phone number (exactly 10 digits): ");
        scanf("%s", contact->phone);
        if (!isValidPhone(contact->phone)) {
            printf("Invalid phone number. Please enter a 10-digit number.\n");
        }
    } while (!isValidPhone(contact->phone));

    do {
        printf("Enter new email: ");
        scanf("%s", contact->email);
        if (!isValidEmail(contact->email)) {
            printf("Invalid email. Please enter a valid email address.\n");
        }
    } while (!isValidEmail(contact->email));

    saveContactsToFile();
    printf("Contact updated successfully.\n");
}

// Menu function
void menu() {
    printf("\n--- Telephone Directory ---\n");
    printf("1. Add Contact\n");
    printf("2. Delete Contact\n");
    printf("3. Display Contacts\n");
    printf("4. Update Contact\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

// Main function
int main() {
    loadContactsFromFile();

    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                deleteContact();
                break;
            case 3:
                viewContacts();
                break;
            case 4:
                updateContact();
                break;
            case 5:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
