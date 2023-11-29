import csv

# Class for product with a name and optional price
class Product:

    def __init__(self, name, price=0):
        self.name = name
        self.price = price
    # Return a string representation of product and price
    def __repr__(self):
        # Format for each product in stock when printShop function runs
        return f'---------------\nPRODUCT NAME: {self.name}\nPRODUCT PRICE: €{self.price:.2f}\n--------------'

# productstock class
class ProductStock:
    # Product associated with stock and quantity
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity
    # Return the name of the product
    def name(self):
        return self.product.name
    # Return the unit price 
    def unit_price(self):
        return self.product.price
    # Calculates the cost of the product      
    def cost(self):
        return self.itemPrice() * self.quantity
    # String representation of the product stock and quantity    
   
    def __repr__(self):
        return f"{self.product}\nQUANTITY REQUIRED: {int(self.quantity)}\n"

# Defining customer class
class Customer:
    # Reads in customer csv file
    def __init__(self, path):
        self.shoppingList = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            # Finding the customer name on first row
            self.name = first_row[0]
            # Finding the customer budget
            self.budget = float(first_row[1]) 
            for row in csv_reader:
                # Finds the product name
                name = row[0]
                # Finds quantity 
                quantity = float(row[1])
                p = Product(name)
                ps = ProductStock(p, quantity)
                # Append to the shopping list
                self.shoppingList.append(ps) 
                
    def printCustomer(self):
        stock = shop.stock
        for i in stock:
            shop_item_name = i.product.name
            if shop_item_name == self.shoppingList: 
                return i
    
    # Procesing the customer shopping list
    def __repr__(self):
        print('')
        float_format = "{:.2f}".format(self.budget)
        str = f"\n{self.name}'s shopping list\n"
        str += f"BUDGET: €{float_format}"
        totalBill = 0
        # For loop each shopping item
        for item in self.shoppingList:
            # For loop for the stock
            for prod in shop.stock:
                # Checking for a match item in list with stock name
                if item.product.name == prod.name():
                    choice_name = item.product.name
                    item.product.price = prod.product.price
                    shopPrice = prod.product.price
                    item.quantity = int(item.quantity)
                    # Checking is the quantity in stock
                    quantity_chosen = shop.checkProductStock(choice_name, item.quantity) 
                    # Calculate the cost of the quantity chosen
                    cost1 = quantity_chosen * shopPrice 
                    if quantity_chosen < item.quantity:
                        str += f"\n---------------"
                        str += f"\n** We don't have that much {choice_name} in stock. We only have {quantity_chosen} **"                   
                    if self.budget < cost1:
                        str += f"\n** Sorry! You don't have enough money left for {choice_name}! **\n"
                        str += f"---------------"                      
                        continue                 
                    else:
                        # Updates the bill for each item
                        totalBill += cost1
                        # Updates the budget for each item
                        self.budget -= cost1
                        # Updates the float
                        shop.cash += cost1
                        # Prints the item
                        str += f"\n{item}" 
                        str += f"QUANTITY PURCHASED: {quantity_chosen}"
                        str += f"\nTOTAL ITEM COST: €{cost1:.2f}\n"
                        str += f"ADJUSTED BUDGET: €{self.budget:.2f}"
                        str += f"\n(ADJUSTED SHOP FLOAT: €{shop.cash:.2f})"
                        str += f"\nTOTAL BILL SO FAR: €{totalBill:.2f}"
                    prod.quantity -= quantity_chosen  
        str += f"\n\nTOTAL BILL: €{totalBill:.2f}"
        str += f"\nBUDGET REMAINING: €{self.budget:.2f}\n"
        str += f"\n** Thank you for your custom **\n"
            
        return str 

# Define a class for the shop
class Shop:
    def __init__(self, path): 
        self.stock = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            # Find the cash float from the file
            self.cash = float(first_row[0])
            for row in csv_reader:
                # Find the product name
                p = Product(row[0], float(row[1])) 
                # Find the product quantity
                ps = ProductStock(p, float(row[2])) 
                # From the stock it creates the list
                self.stock.append(ps) 
    
    # Prints current float contents of shop
    def printShop(self):
        print("Stock list")
        print(f"Shop has a float of €{shop.cash:.2f}")
        # For loop to check the each item
        for item in self.stock:
            curr_stock = int(item.quantity)
            print(item.product)
            print(f"The shop has {curr_stock} in stock")
    
    # Checks availability of product
    def checkProductStock(self, n,q):
       for item in shop.stock:
           if n == item.product.name:
               if q <= item.quantity:
                   # Stock quantity updated
                   item.quantity = item.quantity - q 
                   if item.quantity < 1:
                       # To set the quantity to zero
                       item.quantity = 0 
                   return q
               # If the quantity is more than what is in stock
               elif q > item.quantity:
                   # If all the stock is taken
                   q = int(item.quantity) 
                   item.quantity = 0
                   if q < 1:
                       q = 0 
                       print("---------------")
                       print(f"** We don't have that many {n} in stock. We can only give you {q} **")
                   return q

    # Live mode
    def liveMode(self):
        budget = float(input("What is your budget? \n"))
        totalBill = 0
        print("Welcome to the OOP Python shop LIVE SHOPPING MODE!")
        while True:
            print(f"Your current budget is €{budget:.2f}.\n\nPlease select what you would like to buy from the list below:\n")
            # For loop for products in stock
            for prod in self.stock: 
                print(f"{shop.stock.index(prod) + 1} - {prod.product.name} @ €{prod.product.price:.2f} each")
            print("*23* - Finish shopping and print total bill")
            print("*24* - Exit Live Mode\n")
            choice = int(input("Please make your selection: "))
            if choice == 23:
                print("\n--------------------\n")
                print("** Come again soon and have a nice day! **\n")
                print("--------------------\n")
                break
            elif choice <= 0:
                print("\n** Invalid entry - please try again! **\n")
            elif choice <= len(shop.stock) and budget > 0:
                choice = choice - 1
                choice_name = shop.stock[choice].product.name
                choice_price = shop.stock[choice].product.price
                choice_details = shop.stock[choice].product
                quant = int(input(f"How many {choice_name} would you like to purchase? "))
                # Prints the choice details
                Product(choice_details) 
                purch_quan = self.checkProductStock(choice_name, quant) # checks availability
                totalCost = purch_quan * choice_price # calculates cost
                if budget < totalCost:
                    print(f"\n** You don't have enough money for {choice_name}! **\n")
                    continue
                # Updates the budget
                budget -= totalCost 
                # Updates the total bill
                totalBill += totalCost 
                # Updates the cash float
                shop.cash += totalBill # updates shop cash float
                print(f"QUANTITY REQUIRED: {quant}")
                print(f"QUANTITY PURCHASED: {purch_quan}")
                print(f"TOTAL ITEM COST: €{totalCost:.2f}")
                print(f"ADJUSTED BUDGET: €{budget:.2f}")
                print(f"(ADJUSTED SHOP FLOAT: €{shop.cash:.2f})")
                print(f"----------------------\n")
                print(f"TOTAL BILL SO FAR: €{totalBill:.2f}") 
            elif choice == 24:
                print("\n------------------\n")
                print(f"Your total bill is €{totalBill:.2f}\n")
                print("** Thank you for your custom. Please come again soon! **\n")
                print("--------------------\n")
                break
            else:
                print("** Invalid entry - please try again! **")
        # Updates shop float
        shop.cash += totalBill 
        mainMenu()

# main menu on arrival
def mainMenu():
    print("Welcome to the python proc shop\n")
    print("Please select from the following: \n")
    print("1 - Show shop's current stock and float")
    print("2 - John's shopping list")
    print("3 - Tony's shopping list")
    print("4 - Bob's shopping list")
    print("5 - Shop in Live Mode")
    print("0 - Exit\n")

# Creates the variable for stock csv file
shop = Shop("G00232918_James_Connolly_Assignment_1\stock.csv")

def main():
    # (www.javatpoint.com, n.d.)
    mainMenu()
    while True:
        choice = input("Please make a selection: ")
        if (choice == "1"):
            shop.printShop()
            mainMenu()
        elif (choice == "2"):
            customer1 = Customer("G00232918_James_Connolly_Assignment_1\customer1.csv")
            customer1.printCustomer()
            print(customer1)
            mainMenu()
        elif (choice == "3"):
            customer2 = Customer("G00232918_James_Connolly_Assignment_1\customer2.csv")
            customer2.printCustomer()
            print(customer2)
            mainMenu()
        elif (choice == "4"):
            customer3 = Customer("G00232918_James_Connolly_Assignment_1\customer3.csv")
            customer3.printCustomer()
            print(customer3)
            mainMenu()
        elif (choice == "5"):
            print("\n////////////////////////////////////////////")
            print("You are now entering our LIVE SHOPPING MODE!")
            print("////////////////////////////////////////////")
            shop.liveMode()
        elif (choice == "0"):
            print("\n** Bye! Thanks for your custom! **\n** Come again soon! **\n")
            break
        else:
            print("** Invalid entry - please enter a number between 0 and 5! **")
            mainMenu()

if __name__ == "__main__":
	main()


# References 
# www.javatpoint.com. (n.d.). Menu-Driven Programs in Python - Javatpoint. [online] Available at: 
# https://www.javatpoint.com/menu-driven-programs-in-python [Accessed 23 Nov. 2023].