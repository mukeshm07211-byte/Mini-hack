#include <stdio.h>
#include <stdlib.h>

struct data
{
    int number;
    char name[20];
    float balance;
};

int main()
{
    FILE *file;
    struct data person, empty = {0, "", 0};
    int choice, accNo;
    float amount;

    // Open file or create if not exists
    file = fopen("bank.dat", "rb+");

    if (file == NULL)
    {
        file = fopen("bank.dat", "wb+");

        // create 100 empty records
        for (int i = 0; i < 100; i++)
        {
            fwrite(&empty, sizeof(struct data), 1, file);
        }
    }

    do
    {
        printf("\n1. Create text file");
        printf("\n2. Update account");
        printf("\n3. Add account");
        printf("\n4. Delete account");
        printf("\n5. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        // -------- ADD ACCOUNT --------
        if (choice == 3)
        {
            printf("Enter account number (1-100): ");
            scanf("%d", &accNo);

            fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
            fread(&person, sizeof(struct data), 1, file);

            if (person.number != 0)
            {
                printf("Account already exists!\n");
            }
            else
            {
                printf("Enter name and balance: ");
                scanf("%s %f", person.name, &person.balance);

                person.number = accNo;

                fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
                fwrite(&person, sizeof(struct data), 1, file);
            }
        }

        // -------- UPDATE ACCOUNT --------
        else if (choice == 2)
        {
            printf("Enter account number: ");
            scanf("%d", &accNo);

            fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
            fread(&person, sizeof(struct data), 1, file);

            if (person.number == 0)
            {
                printf("Account not found!\n");
            }
            else
            {
                printf("Current Balance: %.2f\n", person.balance);
                printf("Enter amount (+deposit / -withdraw): ");
                scanf("%f", &amount);

                person.balance += amount;

                // 🔴 Minimum Balance Warning
                if (person.balance < 500)
                {
                    printf("Warning: Minimum balance not maintained!\n");
                }

                fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
                fwrite(&person, sizeof(struct data), 1, file);
            }
        }

        // -------- DELETE ACCOUNT --------
        else if (choice == 4)
        {
            printf("Enter account number to delete: ");
            scanf("%d", &accNo);

            fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
            fread(&person, sizeof(struct data), 1, file);

            if (person.number == 0)
            {
                printf("Account does not exist!\n");
            }
            else
            {
                fseek(file, (accNo - 1) * sizeof(struct data), SEEK_SET);
                fwrite(&empty, sizeof(struct data), 1, file);
                printf("Account deleted.\n");
            }
        }

        // -------- CREATE TEXT FILE --------
        else if (choice == 1)
        {
            FILE *text;
            text = fopen("accounts.txt", "w");

            rewind(file);

            fprintf(text, "AccNo   Name      Balance\n");

            while (fread(&person, sizeof(struct data), 1, file))
            {
                if (person.number != 0)
                {
                    fprintf(text, "%-7d %-8s %.2f\n",
                            person.number, person.name, person.balance);
                }
            }

            fclose(text);
            printf("Text file created!\n");
        }

    } while (choice != 5);

    fclose(file);
    return 0;
}
