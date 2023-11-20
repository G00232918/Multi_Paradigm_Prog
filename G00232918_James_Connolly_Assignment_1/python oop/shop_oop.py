import csv

# class for product with a name and optional price
class Product:

    def __init__(self, name, price=0):
        self.name = name
        self.price = price
    #  return a string representation of product and price
    def __repr__(self):
        return f'NAME: {self.name} PRICE: {self.price}'

class ProductStock:
    # product associated with stock and quantity
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity
    
    def name(self):
        return self.product.name
    
    def unit_price(self):
        return self.product.price
        
    def cost(self):
        return self.unit_price() * self.quantity
        
    def __repr__(self):
        return f"{self.product} QUANTITY: {self.quantity}"

class Customer:

    def __init__(self, name, budget, shopping_list):
        self.name = name
        self.budget = budget
        self.shopping_list = shopping_list
                
    def calculate_costs(self, price_list):
        for shop_item in price_list:
            for list_item in self.shopping_list:
                if list_item.name() == shop_item.name():
                    list_item.product.price = shop_item.unit_price()
    
    def order_cost(self):
        cost = 0
        
        for list_item in self.shopping_list:
            cost += list_item.cost()
        
        return cost
    
    def __repr__(self):
        
        str_ = f"{self.name} wants to buy"
        for item in self.shopping_list:
            cost = item.cost()
            str_ += f"\n{item}"
            if cost == 0:
                str_ += f" {self.name} doesn't know how much that costs :("
            else:
                str_ += f" COST: {cost}"
                
        str_ += f"\nThe cost would be: {self.order_cost()}, he would have {self.budget - self.order_cost()} left"
        return str_
        

class Shop:
    
    def __init__(self, cash, stock):
        self.cash = cash
        self.stock = stock
    
    def __repr__(self):
        str_ = ""
        str_ += f'Shop has {self.cash} in cash\n'
        for item in self.stock:
            str_ += f"{item}\n"
        return str_

def create_shop_from_csv(file_path):
    stock = []
    cash = 0
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        cash = float(first_row[0])
        for row in csv_reader:
            p = Product(row[0], float(row[1]))
            ps = ProductStock(p, float(row[2]))
            stock.append(ps)
    return Shop(cash, stock)

def create_customer_from_csv(file_path):
    shopping_list = []
    name = ""
    budget = 0
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        name = first_row[0]
        budget = float(first_row[1])
        for row in csv_reader:
            name = row[0]
            quantity = float(row[1])
            p = Product(name)
            ps = ProductStock(p, quantity)
            shopping_list.append(ps)
    return Customer(name, budget, shopping_list)

s = create_shop_from_csv("../stock.csv")
c = create_customer_from_csv("../customer.csv")
c.calculate_costs(s.stock)
print(c)

