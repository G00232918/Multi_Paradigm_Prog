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
    printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
    printf("-------------\n");
}

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

// Reads in the stock list
struct Shop createAndStockShop()
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    read = getline(&line, &len, fp);
    float cash = atof(line);

    struct Shop shop = { cash };

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

    return shop;
}

// List of items in stock with quantities
void printShop(struct Shop s)
{
    printf("In stock list\n");
    printf("Shop has %.2f in cash\n", s.cash);
    for (int i = 0; i < s.index; i++)
    {
        printProduct(s.stock[i].product);
        printf("The shop has %d of the above\n", s.stock[i].quantity);
    }
}

// Check if the product is in stock
int checkProductStock(char* n, int order)
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

    c.index = 0;
    // Insert customer csv files
    fp = fopen(path_to_customer, "r");
    if (fp == NULL) {
        printf("Can't access customer file. Please try again!");
        exit(EXIT_FAILURE);
    }
    read = getline(&line, &len, fp);
    char* n = strtok(line, ",");
    char* b = strtok(NULL, ",");
    char* custName = malloc(sizeof(char) * 50);
    strcpy(custName, n);
    double custBudget = atof(b);
    c.name = custName;
    c.budget = custBudget;
    struct ProductStock custShopList;

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

// Main function
int main(void)
{
    struct Shop shop = createAndStockShop();
    printShop(shop);

    return 0;
}

// References
// Read in contents of a csv file - https://stackoverflow.com/questions/60937894/c-print-contents-of-a-csv-file
