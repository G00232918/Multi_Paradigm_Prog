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
    if (fp == NULL) {
        perror("Error opening stock file");
        exit(EXIT_FAILURE);
    }

    // Read the cash value for the shop
    read = getline(&line, &len, fp);
    float cash = atof(line);

    // Initialize the shop with the cash
    struct Shop shop = { cash };
    // Parsing each line of the stock file
    while ((read = getline(&line, &len, fp)) != -1) {
        char* n = strtok(line, ",");
        char* p = strtok(NULL, ",");
        char* q = strtok(NULL, ",");
        int quantity = atoi(q);
        double price = atof(p);
        char* name = malloc(sizeof(char) * 50);
        if (name == NULL) {
            perror("Error allocating memory for product name");
            exit(EXIT_FAILURE);
        }
        strcpy(name, n);
        struct Product product = { name, price };
        struct ProductStock stockItem = { product, quantity };
        shop.stock[shop.index++] = stockItem;
    }
    // Close the file and return the shop
    fclose(fp);
    return shop;
}

// List of items in stock with quantities
void printShop(struct Shop s) {
    // Print info about the shop's stock
    printf("In stock list\n");
    printf("Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++)
    {
        printProduct(s.stock[i].product);
        printf("The shop has %d of the above\n", s.stock[i].quantity);
    }
}

// Check if the product is in stock
int checkProductStock(char* n, int order, struct Shop *s)
{
    for (int i = 0; i < s->index; i++) {
        struct ProductStock product = s->stock[i];
        char* name = product.product.name;
        if (strcmp(name, n) == 0) {
            if (s->stock[i].quantity >= order) {
                s->stock[i].quantity = s->stock[i].quantity - order;
                return order;
            } else if (order > s->stock[i].quantity) {
                order = s->stock[i].quantity;
                printf("---------------\n");
                printf("** We don't have that much %s in stock. We only have %d **\n", product.product.name, s->stock[i].quantity);
                s->stock[i].quantity = 0;
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
    printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
    printf("-------------\n");
    for (int i = 0; i < c.index; i++)
    {
        printProduct(c.shoppingList[i].product);
        printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
        double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
        printf("The cost to %s will be €%.2f\n", c.name, cost);
    }
}

// finds price of product
double findProductPrice(char *n, struct Shop *s) {
    for (int i = 0; i < s->index; i++) {
        struct Product product = s->stock[i].product;
        char *name = product.name;
        if (strcmp(name, n) == 0) {
            return product.price;
        }
    }
    return -1;
}

// Reads in customer.csv file
struct Customer createCustomer(char* path_to_customer, struct Shop *s)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    struct Customer c;
    c.index = 0;
    // Insert customer csv files
    fp = fopen(path_to_customer, "r");
    if (fp == NULL) {
        perror("Error opening customer file");
        exit(EXIT_FAILURE);
    }
    //Reads the customer's name and budget
    read = getline(&line, &len, fp);
    char* n = strtok(line, ",");
    char* b = strtok(NULL, ",");
    char* custName = malloc(sizeof(char) * 50);
    if (custName == NULL) {
        perror("Error allocating memory for customer name");
        exit(EXIT_FAILURE);
    }
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
        if (name == NULL) {
            perror("Error allocating memory for product name");
            exit(EXIT_FAILURE);
        }
        strcpy(name, n);
        int quantity = atoi(q);
        custShopList.product.name = name;
        custShopList.product.price = findProductPrice(name, s);
        custShopList.quantity = quantity;
        c.shoppingList[c.index++] = custShopList;
    }

    fclose(fp);
    return c;
}

// *** LIVE MODE ***
void liveMode(struct Shop *s) {
    double myBudget, totalCost = 0, totalBill = 0;
    int qr, qb, select;
    printf("What is your budget?\n");
    scanf("%lf", &myBudget);
    do {
        printf("\n/////////////////////////////////////////\n");
        printf("Welcome to the C shop LIVE SHOPPING MODE!\n");
        printf("/////////////////////////////////////////\n\n");
        printf("Your current budget is €%.2f.\n\nPlease select what you would like to buy from the list below:\n\n", myBudget);
        for(int i=0; i < s->index; i++) {
            printf("%d - %s @ €%.2f each.\n", i + 1, s->stock[i].product.name, findProductPrice(s->stock[i].product.name, s));
        }
        printf("*98* - Finish shopping and print total bill\n");
        printf("*99* - Exit Live Mode\n");
        printf("\nPlease make your selection: ");
        scanf("%d", &select);
        switch(select) {
            case 99: {
                printf("\n--------------------\n");
                printf("Come again soon and have a nice day!\n");
                printf("--------------------\n");
                break;
            }
            case 98: {
                printf("\n--------------------\n");
                printf("Your total bill is €%.2f.\n", totalBill);
                printf("Thank you for your custom. Please come again soon!\n");
                printf("--------------------\n");
                break;
            }
            default: {
                if (select > s->index+1 || select <= 0) {
                    printf("\n** Invalid entry - please try again! **\n");
                    break;
                }
                printf ("How many %s would you like to purchase? ", s->stock[select-1].product.name);
                scanf("%d", &qr);
                qb = qr;

                totalCost = qb * findProductPrice(s->stock[select-1].product.name, s);

                if (myBudget < totalCost) {
                    qb = myBudget / findProductPrice(s->stock[select-1].product.name, s);
                }
                qb = checkProductStock(s->stock[select-1].product.name, qb, s);

                totalCost = qb * findProductPrice(s->stock[select-1].product.name, s);

                myBudget -= totalCost;
                totalBill += totalCost;
                s->cash += totalCost;
                printProduct(s->stock[select-1].product);
                printf("QUANTITY REQUIRED: %d\n", qr);
                printf("QUANTITY PURCHASED: %d\n", qb);
                printf("TOTAL ITEM COST: €%.2f\n", totalCost);
                printf("ADJUSTED BUDGET: €%.2f\n", myBudget);
                printf("(ADJUSTED SHOP FLOAT: €%.2f)\n", s->cash);
                printf("- - - - - - - - - - - - - -\n");
                printf("TOTAL BILL SO FAR: €%.2f\n", totalBill);
                if (qr != qb) {
                    printf("\n** You don't have enough money for %s! **\n", s->stock[select-1].product.name);
                }
            }
        }  
        // exit if 0 or 99 is entered
    } while (select != 98 && select != 99);
}

// does the shopping via the shopping list contained in customer.csv
void printCustomer(struct Customer c, struct Shop *s, bool upd) {
    printf("\n////////////////////////\n");
    printf("%s's shopping list\n", c.name);
    printf("////////////////////////\n\n");
    printf("BUDGET: €%.2f\n", c.budget);
    int qb, QB; 
    double cost, totalBill = 0;
    for(int i = 0; i < c.index; i++){
        qb = c.shoppingList[i].quantity;
        if (c.shoppingList[i].product.price > -1){
            cost = qb * c.shoppingList[i].product.price; 
            if (upd){
                QB = checkProductStock(c.shoppingList[i].product.name, qb, s);
                cost = QB * c.shoppingList[i].product.price;
                if(cost > c.budget){
                    printf("** Sorry! You don't have enough money left for %s! **\n", c.shoppingList[i].product.name);
                    printf("---------------\n");
                    continue;
                }							
                c.budget -= cost;				
                s->cash += cost;
                totalBill += cost;
                printProduct(c.shoppingList[i].product);
                printf("QUANTITY REQUIRED: %d\n", c.shoppingList[i].quantity);
                printf("- - - - - - - - \n");
                printf("QUANTITY PURCHASED: %d\n", QB);
                printf("TOTAL ITEM COST: €%.2f\n", cost);
                printf("- - - - - - - - \n");
                printf("ADJUSTED BUDGET: €%.2f\n", c.budget);
                printf("(ADJUSTED SHOP FLOAT: €%.2f)\n", s->cash);
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
void mainMenu(struct Shop s) {
    int menuSelect;
    do {
        printf("Welcome to the c shop\n");
        printf("\nPlease select from the following:\n");
        printf("1 - Show shop's current stock and float\n"); 
        printf("2 - John's shopping list\n"); 
        printf("3 - Tony's shopping list\n"); 
        printf("4 - Shop with JimBob's shopping list\n"); 
        printf("4 - Bob's shopping list\n"); 
        printf("5 - Enter Live Mode\n");
        printf("0 - Exit\n");
        printf("\nPlease make a selection: ");
        scanf("%d", &menuSelect);
        switch (menuSelect){

            case 1:{
                printShop(s);
                break;
            }
            case 2:{
                struct Customer customer1 = createCustomer("../customer1.csv", &s);
                printCustomer(customer1, &s, true);
                break;
            }
            case 3:{
                struct Customer customer2 = createCustomer("../customer2.csv", &s);
                printCustomer(customer2, &s, true);
                break;
            }
            case 4:{
                struct Customer customer3 = createCustomer("../customer3.csv", &s);
                printCustomer(customer3, &s, true);
                break;
            }
            case 5:{
                printf("\n////////////////////////////////////////////");
                printf("\nYou are now entering our LIVE SHOPPING MODE!\n");
                printf("////////////////////////////////////////////\n");
                liveMode(&s);
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
    } while (menuSelect != 0);
}

// main method
int main()
{
    // create shop
    struct Shop newShop = createAndStockShop();

    mainMenu(newShop);

    return 0;
}
