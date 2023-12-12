#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


// Structs to be used for the shop simulation
struct Product {
    char* name;
    double price;
};

struct ProductStock {
    struct Product product;
    int quantity;
};

struct Shop {
    double cash;
    struct ProductStock stock[25];
    int index;
};

struct Customer {
    char* name;
    double budget;
    struct ProductStock shoppingList[30];
    int index;
};

// Create global variables
// (GeeksforGeeks, 2022)
struct Customer c;
struct Shop s;

// Printing product info
void printProduct(struct Product p) {
    printf("-------------\n");
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

// Reads in the stock list
struct Shop createAndStockShop() {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    // Opens the stock list from the stock csv
    fp = fopen("C:\\Users\\james\\College\\Multi_Paradigm_Prog\\G00232918_James_Connolly_Assignment_1\\stock.csv", "r");
    
    if (fp == NULL) {
        // (GeeksforGeeks, 2017a)
        perror("Cannot open the stock file");
        exit(EXIT_FAILURE);
    }

    // Read the cash value for the shop
    //read = getline(&line, &len, fp);
    //float cash = atof(line);

    read = getline(&line, &len, fp);
    double shopFloat = atof(line);
    s.cash = shopFloat;


    // Intialise the shop with the cash
    //struct Shop shop = { cash };
    // Parsing each line of the stock file
    while ((read = getline(&line, &len, fp)) != -1) {
        char* n = strtok(line, ",");
        char* p = strtok(NULL, ",");
        char* q = strtok(NULL, ",");
        int quantity = atoi(q);
        double price = atof(p);
        char* name = malloc(sizeof(char) * 50);
        strcpy(name, n);
        struct Product product = { name, price };
        struct ProductStock stockItem = { product, quantity };
        s.stock[s.index++] = stockItem;

    }


    // Close the file and return the shop
    //return shop;
    return s;
}

// List of items in stock with quantities
void printShop() {
    // Print info about the shop's stock
    printf("In stock list\n");
    printf("Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++)
    {
        // Print the details of each product
        printProduct(s.stock[i].product);
        printf("The shop has %d of the above\n", s.stock[i].quantity);
    }
}

// Check if the product is in stock
int checkProductStock(char* n, int order) {
    for (int i = 0; i < s.index; i++) {
        // Checking
        struct ProductStock product = s.stock[i];
        char* name = product.product.name;
        if (strcmp(name, n) == 0) {
            if (s.stock[i].quantity >= order) {
                // If quantity exists, update the stock and return the order
                s.stock[i].quantity = s.stock[i].quantity - order;
                return order;
            } else if (order > s.stock[i].quantity) {
                // If there is not enough stock provide the update
                order = s.stock[i].quantity;
                printf("---------------\n");
                printf("** We don't have that much %s in stock. We only have %d **\n", product.product.name, s.stock[i].quantity);
                // This is to set the stock to 0 if the customer tries to purchase more than stock
                s.stock[i].quantity = 0;
                }
            return order;
        }
    }
    // Return an error if the stock is not found
    return -1;
}

// finds price of product
double findProductPrice(char *n) {
	for (int i = 0; i < s.index; i++) {
		struct Product product = s.stock[i].product;
		char *name = product.name;
		if (strcmp(name, n) == 0) {
			return product.price;
		}
	}
	return -1;
}

// Reads in customer.csv file
// (Stack Overflow, 2020)
struct Customer createCustomer(char* path_to_customer) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;
    c.index = 0;
    // Insert customer csv files
    fp = fopen(path_to_customer, "r");
    if (fp == NULL) {
        printf("Can't access customer file. Please try again!");
        exit(EXIT_FAILURE);
    }
    // Reads the customer's name and budget
    read = getline(&line, &len, fp);
    char* n = strtok(line, ",");
    char* b = strtok(NULL, ",");
    char* custName = malloc(sizeof(char) * 50);
    strcpy(custName, n);
    double custBudget = atof(b);
    c.name = custName;
    c.budget = custBudget;
    struct ProductStock custShopList;

    // Read each line to populate the customer list
    while ((read = getline(&line, &len, fp)) != -1) {
        char* n = strtok(line, ",");
        char* q = strtok(NULL, ",");
        char* name = malloc(sizeof(char) * 50);
        strcpy(name, n);
        int quantity = atoi(q);
        custShopList.product.name = name;
        custShopList.product.price = findProductPrice(name);
        custShopList.quantity = quantity;
        c.shoppingList[c.index++] = custShopList;
    }

    return c;
}

// *** LIVE MODE ***
void liveMode() {
    // Total bill had to be set to 0.0 as the program was going into
    // continous loop when selecting option 5
    double myBudget, totalCost, totalBill = 0.0;
    int quantity_requested, quantity_chosen, select = 0; // Initialize 'select' variable
    printf("What is your budget?\n");
    scanf("%lf", &myBudget);
    do {
        printf("\n***********************\n");
        printf("Welcome to the C shop LIVE SHOPPING MODE!\n");
        printf("***********************\n\n");
        printf("Your current budget is €%.2f.\n\nPlease select what you would like to buy from the list below:\n\n", myBudget);
        for(int i=0; i < s.index; i++) {
            printf("%d - %s @ €%.2f each.\n", i + 1, s.stock[i].product.name, findProductPrice(s.stock[i].product.name));
        }
        printf("Enter your choice (23 to print total bill, 24 to exit): ");
        scanf("%d", &select); // Read the user's selection

        switch(select) {
            case 23: {
                printf("Your total bill is €%.2f.\n", totalBill);
                printf("Thank you for your custom. Please come again soon!\n");
                break;
            }
            case 24: {
                printf("Come again soon and have a nice day!\n");
                return;
                break;
            }
            default: {
                if (select > s.index + 1 || select <= 0) {
                    printf("\n** Invalid entry - please try again! **\n");
                    break;
                }

                printf("How many %s would you like to purchase? ", s.stock[select - 1].product.name);
                scanf("%d", &quantity_requested);
                quantity_chosen = quantity_requested;

                totalCost = quantity_chosen * findProductPrice(s.stock[select - 1].product.name);

                if (myBudget < totalCost) {
                    quantity_chosen = myBudget / findProductPrice(s.stock[select - 1].product.name);
                }

                quantity_chosen = checkProductStock(s.stock[select - 1].product.name, quantity_chosen);

                myBudget -= totalCost;
                totalBill += totalCost;
                s.cash += totalCost;

                printProduct(s.stock[select - 1].product);
                printf("---------------\n");
                printf("QUANTITY REQUIRED: %d\n", quantity_requested);
                printf("QUANTITY PURCHASED: %d\n", quantity_chosen);
                printf("TOTAL ITEM COST: €%.2f\n", totalCost);
                printf("ADJUSTED BUDGET: €%.2f\n", myBudget);
                printf("(ADJUSTED SHOP FLOAT: €%.2f\n", s.cash);
                printf("TOTAL BILL SO FAR: €%.2f\n", totalBill);
                printf("---------------\n");

                if (quantity_requested != quantity_chosen) {
                    printf("\n** You don't have enough money for %s! **\n", s.stock[select - 1].product.name);
                }
            }
        }
    // This is to allow the code to continue executing once these cases
    // are not selected    
    } while (select != 23 && select != 24);
}


// Print info about each customer and their shopping list
// Bool used for the customer shopping, if its false it skips the function
// (Deshmukh, 2021)
void printCustomer(bool upd) {
    printf("%s's shopping list\n", c.name);
    printf("BUDGET: €%.2f\n", c.budget);
    int QC, quantity_chosen;
    double cost, totalBill = 0;
    // Loop through each item in the shopping list
    for (int i = 0; i < c.index; i++) {
        // Get the quantity chosen by the customer
        quantity_chosen = c.shoppingList[i].quantity;
        if (c.shoppingList[i].product.price > -1) {
            // Calculate the cost of product based on the quantity chosen
            cost = quantity_chosen * c.shoppingList[i].product.price;
            if (upd) {
                QC = checkProductStock(c.shoppingList[i].product.name, quantity_chosen);
                // Adjust the quantity chosen based on the available stock
                cost = QC * c.shoppingList[i].product.price;
                if (cost > c.budget) {
                    printf("** Sorry! You don't have enough money left for %s! **\n", c.shoppingList[i].product.name);
                    printf("---------------\n");
                    continue;
                }
                // Update each
                c.budget -= cost;
                s.cash += cost;
                totalBill += cost;
                printf("---------------");
                printProduct(c.shoppingList[i].product);
                printf("QUANTITY REQUIRED: %d\n", c.shoppingList[i].quantity);
                printf("QUANTITY PURCHASED: %d\n", QC);
                printf("TOTAL ITEM COST: €%.2f\n", cost);
                printf("ADJUSTED BUDGET: €%.2f\n", c.budget);
                printf("(ADJUSTED SHOP FLOAT: €%.2f)\n", s.cash);
                printf("TOTAL BILL SO FAR: €%.2f\n", totalBill);
                printf("---------------");
            }
        }
    }
    printf("\nTOTAL BILL: €%.2f\n", totalBill);
    printf("BUDGET REMAINING: €%.2f\n", c.budget);
    printf("\n** Thank you for your custom **\n\n");
}


// Main Menu with the options to select from
//(Stack Overflow, 2015)
void mainMenu(struct Shop s) {
    int menuSelect;
    do {
        printf("\n***********************\n");
        printf("WELCOME TO THE C SHOP\n");
        printf("***********************\n");
        printf("\nPlease select from the following:\n\n");
        printf("1 - Show shop's current stock and float\n");
        printf("2 - John's shopping list\n");
        printf("3 - Tony's shopping list\n");
        printf("4 - Bob's shopping list\n");
        printf("5 - Shop in Live Mode\n");
        printf("0 - Exit\n");
        printf("\nPlease make a selection: ");
        scanf("%d", &menuSelect);
        // Switch allows to select each case and execute it
        switch (menuSelect) {
            case 1: {
                printShop(s);
                break;
            }
            case 2: {
                struct Customer customer1 = createCustomer("C:\\Users\\james\\College\\Multi_Paradigm_Prog\\G00232918_James_Connolly_Assignment_1\\customer1.csv");
                printCustomer(true);
                break;
            }
            case 3: {
                struct Customer customer2 = createCustomer("C:\\Users\\james\\College\\Multi_Paradigm_Prog\\G00232918_James_Connolly_Assignment_1\\customer2.csv");
                printCustomer(true);
                break;
            }
            case 4: {
                struct Customer customer3 = createCustomer("C:\\Users\\james\\College\\Multi_Paradigm_Prog\\G00232918_James_Connolly_Assignment_1\\customer3.csv");
                printCustomer(true);
                break;
            }
            case 5: {
                printf("\n////////////////////////////////////////////");
                printf("\nYou are now entering our LIVE SHOPPING MODE!\n");
                printf("\n////////////////////////////////////////////");
                liveMode();
                break;
            }
            case 0: {
                printf("\n** Bye! Thanks for your custom! **\n** Come again soon! **\n\n");
                break;
            }
            default: {
                printf("** Invalid entry - please enter a number between 0 and 5! **\n");
                break;
            }
        }
    // if the selection is not 0, it is true    
    } while (menuSelect != 0);
}

// main method
int main() 
{
    
    // create shop
    struct Shop newShop = createAndStockShop();
    // newShop is the representation if each option chosen
    mainMenu(newShop);
    return 0;
}


// References
// GeeksforGeeks. (2022). Global Variables in C. [online] Available at: 
// https://www.geeksforgeeks.org/global-variables-in-c [Accessed 15 Nov. 2023].
// GeeksforGeeks. (2017a). Error Handling in C. [online] Available at: 
// https://www.geeksforgeeks.org/error-handling-in-c/ [Accessed 18 Nov. 2023].
// Stack Overflow. (2020). C -Print contents of a csv file. [online] Available at: https://stackoverflow.com
// questions/60937894/c-print-contents-of-a-csv-file [Accessed 16 Nov. 2023].
// Deshmukh, A. (2021). Boolean in C with Examples. [online] Scaler Topics. 
// Available at: https://www.scaler.com/topics/boolean-in-c/ [Accessed 22 Nov. 2022].
// Stack Overflow. (2015). Simple Option Menu (C). [online] Available at: https://stackoverflow.com/questions/
// 33929671/simple-option-menu-c [Accessed 21 Nov. 2023].