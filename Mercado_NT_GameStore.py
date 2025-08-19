#Nicolai T. Mercado
#2024-03243
#CMSC 56 Y-2L
#exer6

gamelib = {}
cart = []
balance = 10000
print("Welcome to the stream store!")
def mainmenu():
    print()
    print('Your current balance is', balance)
    print('What would you like to do?')
    print("    [1] Add game to store")
    print("    [2] Add game to cart")
    print("    [3] View current cart")
    print("    [4] View game list")
    print("    [5] Remove game from cart")
    print("    [6] Remove game from library")
    print("    [7] Clear cart")
    print("    [8] Purchase games")
    print("    [9] Check balance")
    print("   [10] Save Library")
    print("   [11] Load Library")
    print("    [0] Exit")
    choice=int(input("Select Option: "))
    return choice

#For function 1
def add_game_store():
    ID = (input('Enter ID: '))
    characteristics = []
    if ID not in gamelib:
        Title = (input('Enter Game Title: '))
        Desc = (input('Enter Game Description: '))
        Copies = int(input('Enter Number of copies the game has: '))
        Price = float(input('Enter the price of each copy: '))
        characteristics.append(Title)
        characteristics.append(Desc)
        characteristics.append(Copies)
        characteristics.append(Price)
        gamelib[ID] = characteristics
        print('Game successfully added!')

    else:
        print('The game ID is already in use!')

#For Function 2
def add_to_cart():
    IDInput = (input('Choose the ID of the game you want to add to your cart: '))
    if IDInput in gamelib:
        cart.append(IDInput)
        print('Game added to cart successfully!')
    else:
        print('ID does not exist in library')

#For Function 3
def view_cart():
    print('Here is your cart:')
    print('|   ID | Name' + ' '*33 + '| Description' + ' '*55 + '| Stock  | Price  \t')
    for i in cart:
        number1 = gamelib[i][2]
        count1 = 0
        while number1 % 10 == 0 and number1 != 0:
            count1 += 1
            number1 //= 10
        print('|  ', i, ' |', gamelib[i][0],' '*(35 - len(gamelib[i][0])),'|', gamelib[i][1], ' '*(64 - len(gamelib[i][1])), '|', gamelib[i][2], ' '*(4 - count1),'|', gamelib[i][3], '\t|')  

#For Function 4
def view_games():
    print('|   ID | Name' + ' '*33 + '| Description' + ' '*55 + '| Stock  | Price  \t |')
    for i in gamelib.keys():
        game_name = gamelib[i][0]
        game_numofcopies = gamelib[i][2]
        game_description = gamelib[i][1]
        game_price = gamelib[i][3]
        number1 = game_numofcopies
        count1 = 0
        while number1 % 10 == 0 and number1 != 0:
            count1 += 1
            number1 //= 10
        print('|  ', i, ' |', game_name,' '*(35 - len(game_name)),'|', game_description, ' '*(64 - len(game_description)), '|', game_numofcopies,' '*(4 - count1), '|', game_price, '\t', '|')     

#For Function 5
def remove_game_from_cart():
    IDInput2 = (input('Choose the ID of the game you want to remove from your cart: '))
    if IDInput2 in cart:
        cart.remove(IDInput2)
        print('Game removed from cart successfully!')
    else:
        print('ID Does not exist in cart!')

#For Function 6
def remove_game():
    IDInput3 = (input('Choose the ID of the game you want remove from your library: '))
    if IDInput3 in gamelib:
            del gamelib[IDInput3]
            if IDInput3 in cart:
                cart.remove(IDInput3)
                print('Game succesfully deleted!')
    else:
        print('ID Does not exist in Library!')

#For Function 7
def clear_cart():
    cart = []
    return cart
    

#For Function 8
def buy_games(balance, cart):
    if cart == []:
            print('There are currently no games in your cart!')
    else:
        total = 0
        for i in cart:
            total += gamelib[i][3]
        printtotal = total
        balance -= total
        if balance < 0:
            print(printtotal)
            print('Sorry, you do not have enough balance to make this transaction!')
            balance += total
            return balance, cart
    
        elif balance >= 0:
            for i in cart:
                out_of_stock = 0
                gamelib[i][2] = (gamelib[i][2]) - 1
                if gamelib[i][2] < 0:
                    out_of_stock += 1
            
            if out_of_stock >= 1:
                for i in cart:    
                    gamelib[i][2] += 1    
                balance += total
                print('A game in your cart is currently is out of stock or has limited stock. Sorry!')
            
            if out_of_stock == 0:
                cart = []
                print('The total of the games was', printtotal, 'Thank you for your purchase! Your cart is now empty')
    return balance, cart
                   
#For Function 9
def check_balance():
    print('your current balance is', balance, 'credits')

while True:
    option=mainmenu()
    if option==1:
        add_game_store()

    elif option==2:
        add_to_cart()

    elif option==3:
        view_cart()

    elif option==4:
        view_games()

    elif option==5:
        remove_game_from_cart()

    elif option==6:
        remove_game()
        
    elif option==7:
        cart = clear_cart()

    elif option==8:
        balance, cart = buy_games(balance, cart)
        

    elif option==9:
        check_balance()

    if option==0:
        print('Bye! Thanks for shopping!')
        break