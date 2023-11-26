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
    struct ProductStock stock[20];
    int index;
};

struct Customer {
    char* name;
    double budget;
    struct ProductStock shoppingList[20];
    int index;
};

// create global variables
struct Customer c;
struct Shop s;

// Printing product info
void printProduct(struct Product p)
{
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

// Reads in the stock list
struct Shop createAndStockShop()
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    // Opens the stock list from the stock csv
    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        perror("Cannot open the stock file");
        exit(EXIT_FAILURE);

    // Read the cash value for the shop
    read = getline(&line, &len, fp);
    float cash = atof(line);

    // Intialise the shop with the cash
    struct Shop shop = { cash };
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
        shop.stock[shop.index++] = stockItem;
    }
    // Close the file and returns the shop
    fclose(fp);
    return s;
}

// List of items in stock with quantities
void printShop(struct Shop s) {
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
int checkProductStock(char* n, int order)
{
    for (int i = 0; i < s.index; i++) {
        // Checking
        struct ProductStock product = s.stock[i];
        char* name = product.product.name;
        if (strcmp(name, n) == 0) {
            if (s.stock[i].quantity >= order) {
                // If quantity exists, update the stock and returns the order
                s.stock[i].quantity = s.stock[i].quantity - order;
                return order;
            } else if (order > s.stock[i].quantity) {
                // If there is not enough stock provide the update
                order = s.stock[i].quantity;
                printf("---------------\n");
                printf("** We don't have that much %s in stock. We only have %d **\n", product.product.name, s.stock[i].quantity);
                // This is to set the stock to 0, if the customer tries to purchase more than stock
                s.stock[i].quantity = 0;
            }
            return order;
        }
    }
    // Return an error if the stock is not found
    return -1;
}


// Print info about each customer and their shopping list

void printCustomer(struct Customer c)
{
    // Print the details of customer 
    printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
    printf("-------------\n");
    // For loop for the Shopping list
    for (int i = 0; i < c.index; i++)
    {
        printProduct(c.shoppingList[i].product);
        printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
        double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
        printf("The cost to %s will be €%.2f\n", c.name, cost);
    }
}

// Finds price of product
double findProductPrice(char *n) {
	for (int i = 0; i < s.index; i++) {
        // Find the product by product name
		struct Product product = s.stock[i].product;
		char *name = product.name;
		if (strcmp(name, n) == 0) {
			return product.price;
		}
	}
	return -1;
}

// Reads in customer.csv file
struct Customer createCustomer(char* path_to_customer)
{
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
    //Reads the customer's name and budget
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
    double myBudget, totalCost, totalBill;
    int quantity_requested, quantity_chosen, select;
    printf("What is your budget?\n");
    scanf("%lf", &myBudget);
    do {
    // Prompt the user to make a selection
    printf("\nPlease make your selection: ");
    scanf("%d", &select);

    // Switch statement to handle different user selections
    switch (select) {
        // Option 9 represents finishing shopping and printing the total bill
        case 9: {
            printf("Come again soon and have a nice day!\n");
            break;
        }
        // Option 10 represents exiting live mode
        case 10: {
            printf("Your total bill is €%.2f.\n", totalBill);
            printf("Thank you for your custom. Please come again soon!\n");
            break;
        }
        // Default case handles product selection
        default: {
            // Check if the selected product index is out of bounds
            if (select > s.index + 1 || select <= 0) {
                printf("\n** Invalid entry - please try again! **\n");
                break;
            }

            // Prompt the user to input the quantity of the selected product
            printf("How many %s would you like to purchase? ", s.stock[select - 1].product.name);
            scanf("%d", &quantity_requested);
            quantity_chosen = quantity_requested;

            // Calculate the total cost based on the selected quantity
            totalCost = quantity_chosen * findProductPrice(s.stock[select - 1].product.name);

            // Check if the budget is insufficient for the chosen quantity
            if (myBudget < totalCost) {
                quantity_chosen = myBudget / findProductPrice(s.stock[select - 1].product.name);
            }

            // Check and update the actual quantity based on the available stock
            quantity_chosen = checkProductStock(s.stock[select - 1].product.name, quantity_chosen);

            // Update budget, total bill, and shop's cash based on the purchase
            myBudget -= totalCost;
            totalBill += totalCost;
            s.cash += totalCost;

            // Print details of the purchased product and transaction
            printProduct(s.stock[select - 1].product);
            printf("QUANTITY REQUIRED: %d\n", quantity_requested);
            printf("QUANTITY PURCHASED: %d\n", quantity_chosen);
            printf("TOTAL ITEM COST: €%.2f\n", totalCost);
            printf("ADJUSTED BUDGET: €%.2f\n", myBudget);
            printf("(ADJUSTED SHOP FLOAT: €%.2f)\n", s.cash);
            printf("- - - - - - - - - - - - - -\n");
            printf("TOTAL BILL SO FAR: €%.2f\n", totalBill);

            // Notify if the purchased quantity is less than requested due to budget constraints
            if (quantity_requested != quantity_chosen) {
                printf("\n** You don't have enough money for %s! **\n", s.stock[select - 1].product.name);
            }
        }
    }
    // Repeat the loop until the user selects 9 or 10
} while (select != 9 && select != 10);



// does the shopping via the shopping list contained in customer.csv
void printCustomer(bool upd) {
    printf("\n////////////////////////\n");
	printf("%s's shopping list\n", c.name);
    printf("////////////////////////\n\n");
	printf("BUDGET: €%.2f\n", c.budget);
	int qc, QC; 
    double cost, totalBill;
	for(int i = 0; i < c.index; i++){
		qc = c.shoppingList[i].quantity;
		if (c.shoppingList[i].product.price > -1){
			cost = qc * c.shoppingList[i].product.price; 
			if (upd){
				QC = checkProductStock(c.shoppingList[i].product.name, qb);
                cost = QC * c.shoppingList[i].product.price;
                if(cost > c.budget){
				    printf("** Sorry! You don't have enough money left for %s! **\n", c.shoppingList[i].product.name);
                    printf("---------------\n");
				    continue;
                    };							
				c.budget -= cost;				
				s.cash += cost;
                totalBill += cost;
                printProduct(c.shoppingList[i].product);
                printf("QUANTITY REQUIRED: %d\n", c.shoppingList[i].quantity);
                printf("- - - - - - - - \n");
				printf("QUANTITY PURCHASED: %d\n", QC);
				printf("TOTAL ITEM COST: €%.2f\n", cost);
                printf("- - - - - - - - \n");
				printf("ADJUSTED BUDGET: €%.2f\n", c.budget);
				printf("(ADJUSTED SHOP FLOAT: €%.2f)\n", s.cash);
                printf("---------------\n");
                printf("TOTAL BILL SO FAR: €%.2f\n", totalBill);
                printf("---------------\n");			
			    }	
            }
            }
            printf("\nTOTAL BILL: €%.2f\n", totalBill);
            printf("BUDGET REMAINING: €%.2f\n", c.budget);
            printf("\n** Thank you for your custom **\n\n");
        }


// Main Menu with the options to select from
// main program with menu, etc.
void mainMenu(struct Shop s) {
	int menuSelect;
	do {
        printf("\n/////////////////////\n");
		printf("WELCOME TO THE C SHOP\n");
        printf("/////////////////////\n");
		printf("\nPlease select from the following:\n\n");
		printf("1 - Show shop's current stock and float\n"); 
		printf("2 - Shop with Caoimhin's shopping list\n"); 
        printf("3 - Shop with PJs's shopping list\n"); 
        printf("4 - Shop with JimBob's shopping list\n"); 
		printf("5 - Shop in Live Mode\n"); 
		printf("0 - Exit\n");
		printf("\nPlease make a selection: ");
		scanf("%d", &menuSelect);
		switch (menuSelect){

			case 1:{
				printShop();
				break;
			}
			case 2:{
                struct Customer customer1 = createCustomer("../customer1.csv");
				printCustomer(true);
				break;
			}
            case 3:{
                struct Customer customer2 = createCustomer("../customer2.csv");
				printCustomer(true);
				break;
			}
            case 4:{
                struct Customer customer3 = createCustomer("../customer3.csv");
				printCustomer(true);
				break;
			}
			case 5:{
                printf("\n////////////////////////////////////////////");
				printf("\nYou are now entering our LIVE SHOPPING MODE!\n");
                printf("////////////////////////////////////////////\n");
				liveMode();
				break;
			}
			case 0:{
				printf("\n** Bye! Thanks for your custom! **\n** Come again soon! **\n\n");
				break;
			}
			default:{
				printf("** Invalid entry - please enter a number between 0 and 5! **\n");
				break;
			}
		}
	}
	    while (menuSelect != 0);
}

// main method
int main() {
    // create shop
    struct Shop newShop = createAndStockShop();
    mainMenu(newShop);
    return 0;
}



// References
// Read in contents of a csv file - https://stackoverflow.com/questions/60937894/c-print-contents-of-a-csv-file
// Menu in C - https://stackoverflow.com/questions/33929671/simple-option-menu-c
// https://www.geeksforgeeks.org/global-variables-in-c/
// https://www.geeksforgeeks.org/error-handling-in-c/