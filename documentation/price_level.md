***Price Level***

All the price levels are stored as as Balanced BST in the map. Each price level contains a doubly linked list of orders. 

std::list<Orders> order - doubly linked list 

Then, you have a hashmap which maps the order id to the Node in the linked list 

When you know the price level you want, you can:

- add an order
- cancel an order
- fill an order (execute a trade with that order)

*** How to implement them?***

- when you add an order:

1. append to the doubly linked list 
2. increase total quant
3. update the hashmap so id -> node

- to cancel

1. find the order using the id
2. decrease quant, delete from ll
3. remove from order map

- to fill

1. track the quantity you need
2. get orders from front of linked list (time priority)
3. increase filled quant but decrease total quant and remaining
4. for each order, take the quantity and see if that order is fully or partially filled
5. if its fully filled, remove it and record the event 


Ex: I want 10 shares at $100

$100: A(5 shares), B(2 shares), C(7 shares)

- so i can wipe out 5, 2 from A and B so i need to remove them