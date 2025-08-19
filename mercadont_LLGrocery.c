#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

//Used to create an instance of a grocery item
typedef struct groceryItems_tag {
    char item_name[30];
    float price;
    int stock;
    struct groceryItems_tag *nextItem;
} groceryItems;

//Used to create a booking linked list for each customer. This is so that you can access the details of the specific event in the event linked list
struct cart {
    struct groceryItems_tag *item_bought;
    struct cart *nextItem;
};

//Used to create an instance of a buyer
typedef struct buyer_tag {
    char name[30];
    int noofgroceries;
    struct buyer_tag *nextBuyer;
    struct cart *groceryItemsBought;
    float total_cost;
} buyer;

void stringlowercase(char a[30]){ //turns the string lowercase
    int i;
    for(i=0; i<strlen(a); i++){
        a[i]= tolower(a[i]);
    }
}

int stringchecker(char a[30], char b[30]){ //compares strings regardless of uppercase/lowercase
    int i, j;
    char temp1[30], temp2[30];
    strcpy(temp1, a);
    strcpy(temp2, b);
    stringlowercase(temp1);
    stringlowercase(temp2);
    
    return(strcmp(temp1, temp2));
    
    
}

int menu(){     //printing the menu
    int ans, i , j;
    printf("==================== MENU ====================\n");
    printf("[1] Add Grocery Item\n");
    printf("[2] Buy Grocery Item\n");
    printf("[3] Edit Grocery Item\n");
    printf("[4] Delete Grocery Item\n");
    printf("[5] View All Grocery Items\n");
    printf("[6] View All Buyers\n");
    printf("[7] Exit\n\n");
    printf("Enter choice: ");
    scanf("%d", &ans);
    return ans;
}

int main(){
    int choice, grocno, buyno, i, j, noofitem, duplicheck = 0, initialval = 0, ans2, found = 0; //variable declaration and initialization;
    char grocname[30], blank[50], temp1[30], temp2[30], tempname[30];
    int price, stock, alphacount = 0, exist = 0;
    
    groceryItems *headGI = NULL, *tempGI, *traverseGI, *prevGI = NULL; //creating head nodes for the linkedlists
    buyer *headB = NULL, *tempB, *traverseB;
    struct cart *headC = NULL, *tempC = NULL, *traverseC = NULL;


    FILE * F1 = fopen("groceryitem2.txt", "r"); //load data of groceries
    FILE * F2 = fopen("buyers2.txt", "r");   //load data of buyers
    if(F1 == NULL){ //Checks if groceryitem.txt exists
        if(F2 == NULL){ //checks if buyers.txt ALSO exists
            printf("The file for the list of Groceries AND Buyers does not exist! Please contact Mr. Mercado at ntmercado2@up.edu.ph for help!\n");
            return 0;
        }
        printf("The file for the list of Groceries does not exist! Please contact Mr. Mercado at ntmercado2@up.edu.ph for help!\n");
        return 0; //only groceryitem.txt exists
    }
    else if(F2 == NULL){ //only buyers.txt exists
        printf("The file for the list of Buyers does not exist! Please contact Mr. Mercado at ntmercado2@up.edu.ph for help!\n");
        return 0;
    }

    if (fscanf(F1, "%d", &grocno) != 1){ //checks if fscanf works
        grocno = 0; //lets grocery number = 0 if fscanf fails
    }

    for(i=0; i <grocno; i++){   //takes saved data from the grocery
        tempGI = (groceryItems *) malloc(sizeof(groceryItems));
        fscanf(F1, " %[^\n]s", tempGI->item_name); 
        fscanf(F1, "%f", &(tempGI->price));
        fscanf(F1, "%d", &(tempGI->stock));
        tempGI->nextItem = NULL;
        if(headGI == NULL){ //if first element
            headGI = tempGI;
            
        } else if(headGI != NULL){ //elements after first
            traverseGI = headGI;
            while(traverseGI->nextItem != NULL){
                traverseGI = traverseGI->nextItem;//moving to next node
            }
            traverseGI->nextItem = tempGI; //
        }
    }   
    if (fscanf(F2, "%d", &buyno) != 1){ //checks if fscanf works
        buyno = 0; //lets number of buyers = 0 if fscanf fails
    }
    fflush(stdout); //an error with buffers was occuring. learning how to use this code was taken from stack overflow
    for (i = 0; i < buyno; i++) {
        tempB = (buyer *)malloc(sizeof(buyer));
        fscanf(F2, " %[^\n]s", tempB->name);
        fscanf(F2, "%d", &(tempB->noofgroceries));
        tempB->total_cost = 0; //initializing the total cost to 0
        tempB->groceryItemsBought = NULL;  // Initialize to NULL
    
        for(j = 0; j < tempB->noofgroceries; j++) {
            tempC = (struct cart *)malloc(sizeof(struct cart));
            // Don't allocate new grocery item, we'll point to existing one
            char item_name[30];
            fscanf(F2, " %[^\n]s", item_name);
    
            // Find the grocery item in the main list
            groceryItems *foundItem = NULL; //initializing founditem to null after every buyer
            traverseGI = headGI;
            while(traverseGI != NULL) {
                if(stringchecker(traverseGI->item_name, item_name) == 0) { //finding item in grocerylist
                    foundItem = traverseGI;
                    break;
                }
                traverseGI = traverseGI->nextItem; //go to next node
            }
    
            if(foundItem != NULL) {
                tempC->item_bought = foundItem;  // adding found item to cart of buyer
                tempB->total_cost += foundItem->price;
                tempC->nextItem = NULL;
    
                // Add to cart (insert at end)
                if(tempB->groceryItemsBought == NULL) {
                    tempB->groceryItemsBought = tempC;
                } else {
                    struct cart *last = tempB->groceryItemsBought;
                    while(last->nextItem != NULL) {
                        last = last->nextItem;
                    }
                    last->nextItem = tempC;
                }
            } else {
                free(tempC);  // Item not found, free the cart node
                tempB->noofgroceries--;  // Adjust count
            }
        }
        
        tempB->nextBuyer = NULL; // Add buyer to list
        if(headB == NULL) {
            headB = tempB;
        } else {
            traverseB = headB;
            while(traverseB->nextBuyer != NULL) {
                traverseB = traverseB->nextBuyer;
            }
            traverseB->nextBuyer = tempB;
        }
    }
    fclose(F1); //closing the files
    printf("Successfully loaded Grocery data!\n");
    fclose(F2);
    printf("Successfully loaded Buyer data!\n");

    while(1){
        choice = menu();

        if(choice == 1){
            printf("\n\nEnter item name: ");
            scanf(" %[^\n]s", temp1);
            traverseGI = headGI;
        
            while(traverseGI != NULL){
                if(stringchecker(traverseGI->item_name, temp1)==0){ //check if item name exists
                    duplicheck ++;
                    break;
                }
                traverseGI = traverseGI->nextItem;
            }
        
            if(duplicheck!= 0){
                printf("That item already exists!\n");
                duplicheck = 0; 
                continue;
            }
        
            
            tempGI = (groceryItems *)malloc(sizeof(groceryItems)); // Create new node
            strcpy(tempGI->item_name, temp1); //user input
            printf("Enter price: ");
            scanf("%f", &(tempGI->price));
            printf("Enter stock: ");
            scanf("%d", &(tempGI->stock));
            tempGI->nextItem = NULL;
        
            stringlowercase(temp1); // Insert in alphabetical order
        
            if(headGI == NULL || stringchecker(temp1, headGI->item_name) < 0){ //insert at head
                tempGI->nextItem = headGI;
                headGI = tempGI;
            } else {
                traverseGI = headGI;
                while(traverseGI->nextItem != NULL && stringchecker(temp1, traverseGI->nextItem->item_name) > 0){ //insert at wherever the user input item name is alphabetical
                    traverseGI = traverseGI->nextItem; //next grocery
                }
                tempGI->nextItem = traverseGI->nextItem; //insert new item at the found position
                traverseGI->nextItem = tempGI; 
            }
        
            grocno++; //increase number of groceries
            printf("Item added successfully!\n");
            initialval++; //show that data was changed
            
        }else if(choice == 2){
            if(grocno == 0){ 
                printf("\nThere are no grocery items available! \n\n");
                continue;
            }
            
            printf("Enter name here: ");
            scanf(" %[^\n]s", tempname);
            
            printf("\n-------- GROCERY ITEMS AVAILABLE --------\n"); //prints all options on the grocery list
            traverseGI = headGI;
            int count = 0;
            while(traverseGI!= NULL){
                printf("[%d] %s - %.2f\n", count+1, traverseGI->item_name, traverseGI->price); //printing the grocery item
                traverseGI = traverseGI ->nextItem;
                count++;
            }
            printf("Enter grocery to buy: ");
            scanf("%d", &ans2); //user input
            if(ans2 > count + 1){
                printf("\nGrocery item not found!\n");
                continue;
            }

            tempGI = headGI;

            for(i=0; i < ans2-1; i++){  //checking which item the user chose
                tempGI = tempGI->nextItem;
            }
            if(tempGI->stock == 0){ //if item has no stock
                printf("%s is out of stock!\n", tempGI->item_name);
                continue;
            }

            traverseB = headB;
            int found = 0; //iintializing no buyer found
            while(traverseB != NULL){ //will repeat until end of string
                if(stringchecker(traverseB->name, tempname) == 0){
                    found = 1;  // Buyer found
                    if(traverseC == NULL) {
                        tempC = (struct cart *)malloc(sizeof(struct cart));
                        tempC->item_bought = tempGI;  // Add item to the cart
                        tempC->nextItem = traverseB->groceryItemsBought;
                        traverseB->groceryItemsBought = tempC;
                        traverseB->noofgroceries++;
                        traverseB->total_cost += tempGI->price; //increasing the total cost
                        printf("Item added to cart.\n");
                    }
                    break;
                }
            traverseB = traverseB->nextBuyer;
            }

            if(found != 1) { //if name wasnt found in list of buyers
                buyer *newBuyer = (buyer *)malloc(sizeof(buyer));
                strcpy(newBuyer->name, tempname);
                newBuyer->noofgroceries = 1;
                newBuyer->total_cost = tempGI->price;

                tempC = (struct cart *)malloc(sizeof(struct cart));
                tempC->item_bought = tempGI;  // Add item to the cart
                tempC->nextItem = NULL;
                newBuyer->groceryItemsBought = tempC;

                newBuyer->nextBuyer = headB;
                headB = newBuyer;
                buyno++;
            }
    
            tempGI->stock--;
            printf("successfully bought grocery item: %s\n", tempGI->item_name);
            initialval++;

        }else if(choice == 3){
            if(grocno == 0){ //if there are no grocery items
                printf("\nThere are no grocery items available! \n\n");
                continue;
            }

            printf("\n\nEnter grocery item name: ");
            scanf(" %[^\n]s", temp1);
            traverseGI = headGI;

            float tempprice, diff, currentprice;
            int tempstock;
            while(traverseGI != NULL){  //while the grocery list traverse is not at the end
                if(stringchecker(traverseGI->item_name, temp1)==0){ //checks if an item matche the input
                    currentprice = traverseGI->price;
                    printf("Enter new price: ");    //user input of editing new details
                    scanf("%f", &(traverseGI->price));
                    tempprice = traverseGI->price;
                    diff = currentprice - tempprice;
                    printf("Enter new stock: ");
                    scanf("%d", &(traverseGI->stock));
                    tempstock = traverseGI->stock;
                    duplicheck++; //counter for a found item
                    break;
                }
                traverseGI = traverseGI->nextItem; //moves to next node
            }
            if(duplicheck== 0){  //if no similar item was found
                printf("That item does not exist in the store!\n");
                duplicheck = 0; 
                continue; //break the option
            }
            duplicheck = 0; //reset duplicheck
            traverseB = headB; //traverse the linked list of buyers
            while(traverseB != NULL){
                traverseC = traverseB->groceryItemsBought; //traverse linked list of buyer's items
                while(traverseC != NULL){
                    if(stringchecker(traverseC->item_bought->item_name, temp1) == 0){//if entered item name has a similar entry in the cart
                        traverseC->item_bought->price = tempprice;
                        traverseC->item_bought->stock = tempstock;
                        traverseB->total_cost -= diff; //subtract from total
                    }
                    traverseC = traverseC->nextItem; //next item
                }
                traverseB = traverseB->nextBuyer; //next buyer
            }
            printf("Item details successfully update! Update will also reflect on buyers' accounts!\n");
            initialval++;

        }else if(choice == 4){
            if(grocno == 0){ //if there are no grocery items
                printf("\nThere are no grocery items available! \n\n");
                continue;
            }

            printf("\n\nEnter grocery item name: "); //user input
            scanf(" %[^\n]s", temp1);

            traverseB = headB;
            exist = 0; //initialize to item does not exist
            while(traverseB != NULL){
                traverseC = traverseB->groceryItemsBought;  // Use separate pointer
                while(traverseC != NULL){
                    if(stringchecker(traverseC->item_bought->item_name, temp1) == 0){
                        exist++; //if item found, then it exists and cannot be deleted(refer to line 366)
                        break;
                    }
                    traverseC = traverseC->nextItem; //next item
                }
                if(exist != 0){
                    break;  // No need to check other buyers if found
                } 
                traverseB = traverseB->nextBuyer;
}

            if(exist != 0){ //if an existing item was found, it cannot be deleted anymore
                printf("You cannot delete items that were already bought!\n");
                exist = 0;
                continue;
            }

            traverseGI = headGI;
            
            
            prevGI = NULL;  

            // Find the item
            while (traverseGI != NULL) {
                if (stringchecker(traverseGI->item_name, temp1) == 0){ //if item has similar name in grocerylist
                    grocno--; //decrease number of groceries
                    duplicheck++; //show that a copy was found
                    if (prevGI == NULL){ //if element is at the start of the linked list
                        headGI = traverseGI->nextItem;
                        free(traverseGI);
                        printf("Item Succesfully deleted!\n");
                    }else{ //if element is at the end of the linked list
                        prevGI->nextItem = traverseGI->nextItem;
                        free(traverseGI);
                        printf("Item Succesfully deleted!\n");
                    }
                    break;
                }
                prevGI = traverseGI; //the old pointer will be the current one
                traverseGI = traverseGI->nextItem; // the current one will become the new one

            }
            if(duplicheck== 0){ //checks if a duplicate was found
                printf("That item does not exist in the store!\n");
                duplicheck = 0; 
                continue;
            }
            duplicheck = 0; //resets if a duplicate was found
            initialval ++; //indicates that data was moved

        } else if(choice == 5){
            if(grocno == 0){ //if there are no grocery items
                printf("\nThere are no grocery items available! \n\n");
                continue;
            }

            tempGI = headGI;
            for(i=0; i < grocno; i++){
                printf("Grocery Item: %s\n", tempGI->item_name); //print grocery name
                printf("Item Price: %.2f\n", tempGI->price); //print price
                printf("Stock: %d\n\n", tempGI->stock); //print stock
                tempGI = tempGI->nextItem;
            }
        }else if(choice == 6){
            if(buyno == 0){ //if there are no grocery items
                printf("\nThere are no buyers yet! \n\n");
                continue;
            }
            traverseB = headB;
            for(i=0; i < buyno; i++){
                printf("Customer Name: %s\n", traverseB->name); //print buyer name
                printf("Grocery Item Bought:\n");
                traverseC = traverseB->groceryItemsBought; //creating a cart traverse pointer
                while(traverseC != NULL){ //print all grocery items of the buyer
                    printf("- ");
                    printf("Price: %.2f, Item: %s\n", traverseC->item_bought->price, traverseC->item_bought->item_name);
    
                    traverseC = traverseC->nextItem; //go to next item
                }
                printf("Total Cost: %.2f\n\n", traverseB->total_cost); //print total cost
                traverseB = traverseB->nextBuyer; //go to next buyer
            }
        } else if(choice == 7){
            FILE * F1 = fopen("groceryitem2.txt", "w");  //file opening for writing of groceryitem.txt
            fprintf(F1, "%d\n", grocno);    //prints how many groceries there are
            tempGI = headGI;
            while(tempGI != NULL){
                fprintf(F1, "%s\n", tempGI->item_name);
                fprintf(F1, "%f\n", tempGI->price);
                fprintf(F1, "%d\n", tempGI->stock);
                tempGI = tempGI->nextItem;
            }
            fclose(F1);
            FILE * F2 = fopen("buyers2.txt", "w"); //opening buyer list in write mode
            fprintf(F2, "%d\n", buyno); //printing how many groceries were bought
            tempB = headB;
            while(tempB != NULL){ //will traverse all nodes
                fprintf(F2, "%s\n", tempB->name); 
                fprintf(F2, "%d\n", tempB->noofgroceries);
                traverseC = tempB->groceryItemsBought;
                while(traverseC != NULL){
                    fprintf(F2, "%s\n", traverseC->item_bought->item_name);//prints the groceries the buyer bought
                    traverseC = traverseC->nextItem;
                }
                tempB = tempB->nextBuyer;//traverses to next buyer
            }
            fclose(F2);
            if(initialval != 0){    //if changes were made
                printf("\nYour changes were saved!\n");
            } else{ //if changes were not made
                printf("\nNo changes were made!\n");
            }
            printf("\nGoodbye!\n");
            break;
            
        }else{ //if input is not in range 1-7
            printf("That is an invalid choice!");
        }
    }
    tempGI = headGI; //freeing the grocery linked list
    while (tempGI != NULL) {
        groceryItems *currentGI = tempGI;
        tempGI = tempGI->nextItem;
        free(currentGI); 
    }


    traverseB = headB; //freeing the Buyer linked list
    while (traverseB != NULL) {
        buyer *currentB = traverseB;
        traverseB = traverseB->nextBuyer;


        traverseC = currentB->groceryItemsBought; //freeing the cart of the buyers
        while (traverseC != NULL) {
            struct cart *currentCart = traverseC;
            traverseC = traverseC->nextItem;

            free(currentCart);
        }
        free(currentB); 
    }
    return 0;
}