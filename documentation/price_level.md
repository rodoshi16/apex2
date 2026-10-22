***Price Level***

All the price levels are stored as as Balanced BST in the map. Each price level contains a doubly linked list of orders. 

std::list<Orders> order - doubly linked list 

Then, you have a hashmap which maps the order id to the Node in the linked list 

When you know the price level you want, you can:

- add an order
- cancel an order
- fill an order (execute a trade with that order)