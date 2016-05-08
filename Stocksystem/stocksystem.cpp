#pragma once 
#include "stocksystem.h"


// default constructor;
// begin with a balance of $100,000.00
StockSystem::StockSystem() {
	balance = 100000.00;
}

// returns the balance member
double StockSystem::GetBalance() {
	return balance;
}
// Add a new SKU to the system. Do not allow insertion of duplicate sku
bool StockSystem::StockNewItem(StockItem item) {

	return records.Insert(item);
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found.
bool StockSystem::EditStockItemDescription(int itemsku, string desc) {
	//update
	StockItem item(itemsku, "", 0.00);
	StockItem* newitem = records.Retrieve(item);
	if (newitem != NULL) {
		return newitem->SetDescription(desc);
	}
	return false;
}

// Locate the item with key itemsku and update its description field.
// Return false if itemsku is not found or retailprice is negative.
//newitem means current item
bool StockSystem::EditStockItemPrice(int itemsku, double retailprice) {
	StockItem item(itemsku, "", 0.00);
	StockItem* newitem = records.Retrieve(item);
	if (newitem != NULL) {
		return newitem->SetPrice(retailprice);
	}
	return false;

}

// Purchase quantity of item at unitprice each, to reach a maximum (post-purchase) on-hand stock quantity of 1000.
// Return false if balance is not sufficient to make the purchase,
//   or if SKU does not exist, or if quantity or unitprice are negative.
// Otherwise, return true and increase the item's on-hand stock by quantity,
//   and reduce balance by quantity*unitprice.
bool StockSystem::Restock(int itemsku, int quantity, double unitprice) {
	StockItem item(itemsku, "", 0.00);
	StockItem* newitem = records.Retrieve(item);
	//insufficient funds
	if (newitem == NULL || quantity*unitprice >balance || unitprice <0 || quantity<0)
	{
		return false;
	}

	int currentquantity = newitem->GetStock();
	//if over limit then make currentquantity + quantity = 1000(limit) which is quantity = 1000-currentquantity
	if (currentquantity + quantity>1000)
	{
		quantity = 1000 - currentquantity;
	}
	// Otherwise, return true and increase the item's on-hand stock by quantity,
	//   and reduce balance by quantity*unitprice. SetStock is bool
	balance = balance - quantity*unitprice;
	return newitem->SetStock(currentquantity + quantity);

}

// Sell an item to a customer, if quantity of stock is available and SKU exists.
// Reduce stock by quantity, increase balance by quantity*price, and return true if stock available.
// If partial stock (less than quantity) available, sell the available stock and return true.
// If no stock, sku does not exist, or quantity is negative, return false.
bool StockSystem::Sell(int itemsku, int quantity) {
	StockItem item(itemsku, "", 0.00);
	StockItem* newitem = records.Retrieve(item);

	if (newitem == NULL) {
		return false;
	}
	int currentquantity = newitem->GetStock();
	//no available stock
	if (currentquantity == 0) {
		return false;
	}

	if (quantity>currentquantity) {
		//quantity to be sold is over available quantity, then sell whatever left
		quantity = currentquantity;
	}
	//update balance		
	balance = balance + quantity * newitem->GetPrice();
	//update stock quantity 
	return newitem->SetStock(currentquantity - quantity);

}