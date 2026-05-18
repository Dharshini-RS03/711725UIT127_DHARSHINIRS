// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.

#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

// extra functions
void displayTitle(void);
void searchRecord(FILE *fPtr);
void totalAccounts(FILE *fPtr);
void line(void);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // display title
    displayTitle();

    // enable user to specify action
    while ((choice = enterChoice()) != 8)
    {
        switch (choice)
        {
        // create text file from record file
        case 1:
            textFile(cfPtr);
            break;

        // update record
        case 2:
            updateRecord(cfPtr);
            break;

        // create record
        case 3:
            newRecord(cfPtr);
            break;

        // delete existing record
        case 4:
            deleteRecord(cfPtr);
            break;

        // search account
        case 5:
            searchRecord(cfPtr);
            break;

        // display total accounts
        case 6:
            totalAccounts(cfPtr);
            break;

        // display title again
        case 7:
            displayTitle();
            break;

        default:
            puts("Incorrect choice");
            break;
        }
    }

    fclose(cfPtr); // fclose closes the file

    printf("\nProgram terminated successfully.\n");

    return 0;
}

// line function
void line(void)
{
    printf("====================================================\n");
}

// display title function
void displayTitle(void)
{
    line();

    printf("\tBANK ACCOUNT MANAGEMENT SYSTEM\n");

    line();

    printf("Features Included:\n");
    printf("1. Store accounts into text file\n");
    printf("2. Update account balance\n");
    printf("3. Add new account\n");
    printf("4. Delete account\n");
    printf("5. Search account\n");
    printf("6. Display total active accounts\n");
    printf("7. Display title again\n");
    printf("8. Exit program\n");

    line();
}

// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; // accounts.txt file pointer
    int result;

    // create clientData with default information
    struct clientData client = {0, "", "", 0.0};

    // fopen opens the file
    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        // copy all records from random-access file into text file
        while (!feof(readPtr))
        {
            result = fread(&client,
                           sizeof(struct clientData),
                           1,
                           readPtr);

            // write single record to text file
            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt file created successfully.\n");
    }
}

// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    // create clientData with no information
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%d", &account);

    // move file pointer to correct record in file
    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record from file
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // display error if account does not exist
    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    {
        line();

        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        // request transaction amount from user
        printf("Enter charge (+) or payment (-): ");
        scanf("%lf", &transaction);

        // update balance
        client.balance += transaction;

        printf("\nUpdated Record:\n");

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        // move back by one record
        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        // write updated record
        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account updated successfully.\n");

        line();
    }
}

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;

    // blank client
    struct clientData blankClient = {0, "", "", 0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%d", &accountNum);

    // move file pointer
    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // display error if record does not exist
    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    }
    else
    {
        // move file pointer again
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        // replace existing record with blank record
        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// create and insert record
void newRecord(FILE *fPtr)
{
    // create clientData with default information
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%d", &accountNum);

    // move file pointer
    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // display error if account already exists
    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n",
               client.acctNum);
    }
    else
    {
        // user enters data
        printf("Enter lastname firstname balance\n? ");

        scanf("%14s%9s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        // move file pointer
        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        // insert record
        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("New account added successfully.\n");
    }
}

// search record function
void searchRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    // move file pointer
    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    line();

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Account Found Successfully\n\n");

        printf("Account Number : %u\n", client.acctNum);
        printf("Last Name      : %s\n", client.lastName);
        printf("First Name     : %s\n", client.firstName);
        printf("Balance        : %.2f\n", client.balance);
    }

    line();
}

// display total active accounts
void totalAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    int count = 0;

    rewind(fPtr);

    // read all records
    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            count++;
        }
    }

    line();

    printf("Total Active Accounts = %d\n", count);

    line();
}

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\nEnter your choice\n");

    printf("1 - Store formatted text file\n");
    printf("2 - Update an account\n");
    printf("3 - Add a new account\n");
    printf("4 - Delete an account\n");
    printf("5 - Search account\n");
    printf("6 - Display total accounts\n");
    printf("7 - Display title again\n");
    printf("8 - End program\n");

    printf("? ");

    scanf("%u", &menuChoice);

    return menuChoice;
}