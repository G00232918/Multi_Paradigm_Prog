from dataclasses import dataclass, field
from typing import List
import csv

# setting the data classes for shop elements
@dataclass
class Product:
    name: str
    price: float = 0.0

@dataclass 
class ProductStock:
    product: Product
    quantity: int

@dataclass 
class Shop:
    cash: float = 0.0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str = ""
    budget: float = 0.0
    shopping_list: List[ProductStock] = field(default_factory=list)


# function to create and stock the shop from the stock csv
def create_and_stock_shop():
    s = Shop()
    with open('G00232918_James_Connolly_Assignment_1\stock.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        s.cash = float(first_row[0])
        for row in csv_reader:
            p = Product(row[0], float(row[1]))
            ps = ProductStock(p, float(row[2]))
            s.stock.append(ps)
    return s

# print the product info, name and quantity
def print_product(p):
    print(f'\nPRODUCT NAME: {p.name} \nPRODUCT PRICE: {p.price}')

# print the shop cash and quantities of stock
def print_shop(s):
    print(f'Shop has {s.cash} in cash')
    for item in s.stock:
        print_product(item.product)
        print(f'The Shop has {item.quantity} of the above')

# check shop stock
def check_prod_stock(n, quantity, shop):
    for item in shop.stock:
        if n == item.product.name:
            if quantity <= item.quantity:
                item.quantity -= quantity
                return quantity
            elif quantity > item.quantity:
                # adjust the quantity available
                quantity = int(item.quantity)
                item.quantity = 0
                # error message if not in stock
                raise ValueError(f"We don't have that many {n} in stock. We can only give you {quantity}")
    # error if the product cannot be found
    raise ValueError(f"We don't have {n} in stock.")

# reads in the customer lists created
def read_customer(file_path):
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        c = Customer(first_row[0], float(first_row[1]))
        for row in csv_reader:
            name = row[0]
            quantity = float(row[1])
            p = Product(name)
            ps = ProductStock(p, quantity)
            c.shopping_list.append(ps)
        return c

# function to to print customer information
def print_customer(customer, shop):
    print(f"{customer.name}'s shopping list")
    print(f"BUDGET: €{customer.budget:.2f}")
    total_bill = 0

    for item in customer.shopping_list:
        found_match = False
        for prod in shop.stock:
            # remove any unwanted white spaces 
            if item.product.name.strip().lower() == prod.product.name.strip().lower():
                # if the match is found
                found_match = True
                # record the name of match
                choice_name = item.product.name
                # record the price
                shop_price = prod.product.price
                # convert the quantity to an integer
                item.quantity = int(item.quantity)

                try:
                    quantity_purchased = check_prod_stock(prod.product.name, item.quantity, shop)
                    cost = quantity_purchased * shop_price 
                    prod.quantity -= quantity_purchased
                    # if the customer budget is less than the cost
                    if customer.budget < cost:
                        print(f"** Sorry! You don't have enough money left for {choice_name}! **")
                        continue
                    else:
                        # updating each cost        
                        total_bill += cost
                        customer.budget -= cost
                        shop.cash += cost

                    print(f"PRODUCT NAME: {item.product.name}\n")
                    print(f"PRODUCT PRICE: €{prod.product.price:.2f}\n")
                    print(f"QUANTITY REQUIRED: {item.quantity}\n")
                    print(f"QUANTITY PURCHASED: {quantity_purchased}\n")
                    print(f"TOTAL ITEM COST: €{cost:.2f}\n")
                    print(f"ADJUSTED BUDGET: €{customer.budget:.2f}\n")
                    print(f"(ADJUSTED SHOP FLOAT: €{shop.cash:.2f})\n")
                    
                # I put this in because I was getting duplicates when selecting customer csvs
                except ValueError as e:
                    print(f"** Warning: {e} **")
        # if no match            
        if not found_match:
            print(f"** Warning: Product {item.product.name} not found in the shop's stock! **")

    print(f"\nTOTAL BILL: €{total_bill:.2f}")
    print(f"BUDGET REMAINING: €{customer.budget:.2f}")
    print("\n** Thank you for your custom **\n")

    return customer


def liveMode(shop):
    budget = float(input("What is your budget? \n"))
    totalBill = 0
    print("Welcome to Procedural Python Shopping Program\n")
    while True:
        print(f"Your current budget is €{budget:.2f}.\n\nPlease select what you would like to buy from the list below:\n")
        for index, prod in enumerate(shop.stock, start=1):
            print(f"{index} - {prod.product.name} @ €{prod.product.price:.2f} each")

        print("*9* - Finish shopping and print total bill")
        print("*10* - Exit Live Mode\n")

        choice = int(input("Please make your selection: "))

        if choice == 9:
            print("\n--------------------\n")
            print("Come again soon and have a nice day!\n")
            print("--------------------\n")
            break

        elif 1 <= choice <= len(shop.stock):
            choice -= 1
            choice_name = shop.stock[choice].product.name
            choice_price = shop.stock[choice].product.price
            choice_detail = shop.stock[choice].product
            quant = int(input(f"How many {choice_name} would you like to purchase? "))
            purch_quan = check_prod_stock(choice_name, quant, shop)
            totalCost = purch_quan * choice_price

            if budget < totalCost:
                print(f"\n** You don't have enough money for {choice_name}! **\n")
                continue

            budget -= totalCost
            totalBill += totalCost
            Shop.cash += totalBill

            print_product(choice_detail)

            print(f"QUANTITY REQUIRED: {quant}")
            print(f"QUANTITY PURCHASED: {purch_quan}")
            print(f"TOTAL ITEM COST: €{totalCost:.2f}")
            print(f"ADJUSTED BUDGET: €{budget:.2f}")
            print(f"(ADJUSTED SHOP FLOAT: €{shop.cash:.2f})")
            print(f"TOTAL BILL SO FAR: €{totalBill:.2f}")

        elif choice == 10:
            print("\n--------------------\n")
            print(f"Your total bill is €{totalBill:.2f}\n")
            print("Thank you for your custom. Please come again soon!\n")
            print("--------------------\n")
            break

        else:
            print("\n** Invalid entry - please try again! **\n")

    shop.cash += totalBill

def mainMenu():
    print("Welcome to the python proc shop\n")
    print("Please select from the following: \n")
    print("1 - Show shop's current stock and float")
    print("2 - John's shopping list")
    print("3 - Tony's shopping list")
    print("4 - Bob's shopping list")
    print("5 - Shop in Live Mode")
    print("0 - Exit\n")

def shopMenu(shop):
    mainMenu()

    while True:
        choice = input("Please make a selection: ")

        if choice == "1":
            print_shop(shop)
            mainMenu()

        elif choice == "2":
            customer1 = read_customer("G00232918_James_Connolly_Assignment_1\customer1.csv")
            print_customer(customer1, shop)
            mainMenu()

        elif choice == "3":
            customer2 = read_customer("G00232918_James_Connolly_Assignment_1\customer2.csv")
            print_customer(customer2, shop)
            mainMenu()

        elif choice == "4":
            customer3 = read_customer("G00232918_James_Connolly_Assignment_1\customer3.csv")
            print_customer(customer3, shop)
            mainMenu()

        elif choice == "5":
            print("You are now entering our LIVE SHOPPING MODE!\n")
            liveMode(shop)

        elif choice == "0":
            print("\n** Bye! Thanks for your custom! **\n** Come again soon! **\n")
            exit()

        else:
            print("** Invalid entry - please enter a number between 0 and 5! **")
            mainMenu()

def main():
    newShop = create_and_stock_shop()
    shopMenu(newShop)

if __name__ == "__main__":
    main()
