#include<iostream>
#include<bits/stdc++.h>
#include<queue>
#include<map>
using namespace std;


//will define a transfer function later


struct Order {
	//so just use isbid? oppo is false anyway,
	private:
		bool isbid_;
		long long volume_;
		//minecraft item ids,
		int itemID_;
		long long value_;
		long long UserID_;

	public:
		//add logic for expceptions when things like price/ volume not > 0
		//also add the item safeguard
		Order(long long iprice,int iitemID, long long ivolume,bool iisBid, long long UID)
			: isbid_(iisBid), volume_(ivolume), itemID_(iitemID), value_(iprice), UserID_(UID) {}

		//helper functions
		bool isBid() const{
			return isbid_;
		}

		long long volume() const{
			return volume_;
		}

		int itemID() const{
			return itemID_;
		}
			
		long long userID() const {
			return UserID_;
		}

		long long value() const{
			return value_;
		}

		void reduceVolume(long long qty) {
			volume_-=qty;
		}
		//add a helpfunction for total cost of an order

};

/*
use two maps an order one and a buy map bascially, 
so first we want too handle insertion logic map will almost never be empty,
so dont really have too worry about this case tbh,
so an order is inserted into the  if not empty map we search from the back too see
what happens,


so first indetify order type,

for the precision issue, since we are using longs  just multiply
//ill think about precision later tbh not really a problem atm just get a working system up and running,

using longs here is better too avoid float precision errors

also will be ignoreing the log for the time being since i just want working order matching first pretty much

*/


void Transfer(long long buyerID, long long sellerID, long long amount,
              long long quantity, int itemID) {
    std::cout << "TRADE: buyer=" << buyerID << " seller=" << sellerID
              << " item=" << itemID << " qty=" << quantity
              << " amount=" << amount << "\n";
}

int main() {
	//so differentiate via item bascially 


	//eventually change too an ordered map for clean iteration
	std::unordered_map<int, std::map<long long, std::queue<Order>>> AskMapPerItem;
	std::unordered_map<int, std::map<long long, std::queue<Order>>> BidMapPerItem;

	//std::vector<Order> inputOrders;
	//for testing purposes


std::vector<Order> inputOrders;
//testing orders
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	//matching logic
	//match the itemID too the required map pretty much
	for (Order order : inputOrders) {

	    if (order.isBid()) {
	        //BID ORDERS LOGIC
	        ///////////////////////////////////////////
	        //add the item safeguard in the order object
	        //so gets polluted with defunct entry so unirered map[] creates an empty entry so fix it bruh
	        auto asksIt = AskMapPerItem.find(order.itemID());
	        if(asksIt != AskMapPerItem.end()){
	        	//create a refrence too item map
	        	auto& asksForItem = asksIt->second;
		        

		        while (!asksForItem.empty() && order.volume() > 0) {
		            auto it = asksForItem.begin();     // lowest ask
		            long long bestPrice = it->first;

		            // Crossing check bid is willing to pay >= the ask?
		            //does the opposite logic
		            if (bestPrice > order.value()) {
		                break;   // chepeast ask is too expensive so break the loop
		            }

		            // Drain this price level
		            auto& pricelevelQueue = it->second;
		            while (!pricelevelQueue.empty() && order.volume() > 0) {
		                Order& oldestOrder = pricelevelQueue.front();

		                long long tradeQty   = std::min(order.volume(), oldestOrder.volume());
		                long long tradePrice = oldestOrder.value();
		                long long amount     = tradeQty * tradePrice;


		               	//for now just prints a statement for testing here the seller is the oldest order
		                Transfer(order.userID(), oldestOrder.userID(), amount, tradeQty, order.itemID());


		                order.reduceVolume(tradeQty);
		                oldestOrder.reduceVolume(tradeQty);

		                if (oldestOrder.volume() == 0) {
		                    pricelevelQueue.pop();
		                }
		            }

		            if (pricelevelQueue.empty()) {
		                asksForItem.erase(it);
		            }
		        }

		        //erase if the map is empty
				if (asksForItem.empty()) {
				    AskMapPerItem.erase(asksIt);
				}
	        }

	       	// Anything left over rests as a new bid
	        if (order.volume() > 0) {
	            BidMapPerItem[order.itemID()][order.value()].push(order);
	        }

	    } else {
	        //ASK ORDERS LOGIC
	        ///////////////////////////////////////////////
	       	auto BidIt = BidMapPerItem.find(order.itemID());
	        if(BidIt != BidMapPerItem.end()) {
	        	//refrence to item map
				auto& BidsForItem = BidIt->second;
		        

		        while (!BidsForItem.empty() && order.volume() > 0) {
		            auto it = std::prev(BidsForItem.end());     // highest bid
		            long long bestPrice = it->first;

		            // Crossing check bid is willing to pay >= the ask?
		            //does the opposite logic
		            if (bestPrice < order.value()) {
		                break;   // best bid is too low; stop
		            }

		            // Drain this price level
		            auto& pricelevelQueue = it->second;
		            while (!pricelevelQueue.empty() && order.volume() > 0) {
		                Order& oldestOrder = pricelevelQueue.front();

		                long long tradeQty   = std::min(order.volume(), oldestOrder.volume());
		                long long tradePrice = oldestOrder.value();
		                long long amount     = tradeQty * tradePrice;

		                Transfer(oldestOrder.userID(), order.userID(), amount, tradeQty, order.itemID());

		                order.reduceVolume(tradeQty);
		                oldestOrder.reduceVolume(tradeQty);

		                if (oldestOrder.volume() == 0) {
		                    pricelevelQueue.pop();
		                }
		            }

		            if (pricelevelQueue.empty()) {
		                BidsForItem.erase(it);
		            }
		        }
		        //erase if the map is empty
				if (BidsForItem.empty()) {
					BidMapPerItem.erase(BidIt);
				}
	        }

	        // Anything left over rests as a new ask
	        if (order.volume() > 0) {
	            AskMapPerItem[order.itemID()][order.value()].push(order);
	        }
	    }

	}

}   