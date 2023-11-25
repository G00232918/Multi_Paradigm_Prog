#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    // Print information about a product
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

void printCustomer(struct Customer c)
{
    // Print information about a customer and their shopping list
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

// Reads in the stock list from a file
struct Shop createAndStockShop()
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    // Open the file containing stock information
    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Read the cash value for the shop
    read = getline(&line, &len, fp);
    float cash = atof(line);

    // Initialize the shop with cash
    struct Shop shop = { cash };

    // Read each line in the file to populate the shop's stock
    while ((read = getline(&line, &len, fp)) != -1) {
        // Parsing each line of the stock file
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

    // Close the file and return the shop
    fclose(fp);
    return shop;
}

// List of items in stock with quantities
void printShop(struct Shop s)
{
    // Print information about the shop's stock
    printf("In stock list\n");
    printf("Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++)
    {
        printProduct(s.stock[i].product);
        printf("The shop has %d of the above\n", s.stock[i].quantity);
    }
}

// Check if the product is in stock and update the stock
int checkProductStock(char* n, int order, struct Shop s)
{
    for (int i = 0; i < s.index; i++) {
        struct ProductStock product = s.stock[i];
        char* name = product.product.name;
        if (strcmp(name, n) == 0) {
            if (s.stock[i].quantity >= order) {
                s.stock[i].quantity = s.stock[i].quantity - order;
                return order;
            } else if (order > s.stock[i].quantity) {
                order = s.stock[i].quantity;
                printf("---------------\n");
                printf("** We don't have that much %s in stock. We only have %d **\n", product.product.name, s.stock[i].quantity);
                s.stock[i].quantity = 0;
            }
            return order;
        }
    }
    // Return an error if the stock is not found
    return -1;
}

// Reads in customer.csv file
struct Customer createCustomer(char* path_to_customer)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    struct Customer c;
    c.index = 0;

    // Open the file containing customer information
    fp = fopen(path_to_customer, "r");
    if (fp == NULL) {
        printf("Can't access customer file. Please try again!");
        exit(EXIT_FAILURE);
    }

    // Read the customer's name and budget
    read = getline(&line, &len, fp);
    char* n = strtok(line, ",");
    char* b = strtok(NULL, ",");
    char* custName = malloc(sizeof(char) * 50);
    strcpy(custName, n);
    double custBudget = atof(b);
    c.name = custName;
    c.budget = custBudget;
    struct ProductStock custShopList;

    // Read each line in the file to populate the customer's shopping list
    while ((read = getline(&line, &len, fp)) != -1) {
        char* n = strtok(line, ",");
        char* q = strtok(NULL, ",");
        char* name = malloc(sizeof(char) * 50);
        strcpy(name, n);
        int quantity = atoi(q);
        custShopList.product.name = name;
        // The function findProductPrice is referenced but not provided in the code
        custShopList.product.price = findProductPrice(name);
        custShopList.quantity = quantity;
        c.shoppingList[c.index++] = custShopList;
    }

    // Close the file and return the customer
    fclose(fp);
    return c;
}

// Main function
int main(void)
{
    // Create and stock the shop
    struct Shop shop = createAndStockShop();
    // Print information about the shop
    printShop(shop);

    return 0;
}
