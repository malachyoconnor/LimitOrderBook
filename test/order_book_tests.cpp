#include <gtest/gtest.h>
#include "OrderBook.h"
#include <gtest/gtest.h>
#include "OrderBook.h"
#include "Order.h"
#include "Price.h"
#include "Quantity.h"

// Senior Dev Note:
// We use a Test Fixture to ensure a clean state for every test.
class OrderBookTest : public ::testing::Test {
protected:
    OrderBook book;

    // Helper to create orders quickly to reduce boilerplate in tests
    Order create_dummy_order(int64_t price, int64_t quantity, uint64_t id, Side side) {
        return Order(Price(price), Quantity(quantity), id, side);
    }
};

// 1. Basic Add - Testing the "Happy Path"
TEST_F(OrderBookTest, AddSingleOrderReturnsFalseWhenNoMatch) {
    // We add a BID. Since the book is empty, it should just sit there.
    // Your API returns 'false' when no trade happens.
    Order order = create_dummy_order(100, 10, 1, BID);

    // EXPECT_FALSE because no execution should occur
    bool execution_occurred = book.add_order(order);

    EXPECT_FALSE(execution_occurred) << "Order should be queued, not executed";
}

// 2. Execution Logic - The Core Functionality
TEST_F(OrderBookTest, FullMatchExecution) {
    // 1. Add a Passive BID (Liquidity Provider)
    Order bid = create_dummy_order(100, 10, 1, BID);
    book.add_order(bid);

    // 2. Add an Aggressive ASK (Liquidity Taker)
    // Same price, same quantity. Should match fully.
    Order ask = create_dummy_order(100, 10, 2, ASK);

    bool execution_occurred = book.add_order(ask);

    EXPECT_TRUE(execution_occurred) << "Orders should have matched";
}

// 3. Partial Fills
TEST_F(OrderBookTest, PartialFillLogic) {
    // 1. Big BID: Buy 20 @ 100
    Order bid = create_dummy_order(100, 20, 1, BID);
    book.add_order(bid);

    // 2. Small ASK: Sell 5 @ 100
    Order ask = create_dummy_order(100, 5, 2, ASK);

    // This should execute (fill the ask, partially fill the bid)
    bool execution_occurred = book.add_order(ask);
    EXPECT_TRUE(execution_occurred);

    // Senior Dev Note:
    // To truly test this, we would need to inspect the book state to ensure
    // the BID has 15 remaining. Since your API doesn't expose getters (Encapsulation is good,
    // but makes testing hard), we verify by trying to cross the remaining amount.

    // 3. Try to sell 15 more. If the previous logic worked, this should also match.
    Order ask_cleanup = create_dummy_order(100, 15, 3, ASK);
    EXPECT_TRUE(book.add_order(ask_cleanup)) << "Remaining liquidity was not found in book";
}

// 4. Price Time Priority (The complexity test)
TEST_F(OrderBookTest, PricePriorityEnforcement) {
    // Add two BIDS.
    // Bid 1: 100 (Lower price)
    // Bid 2: 110 (Higher price - Should be filled first)
    Order bid_low = create_dummy_order(100, 10, 1, BID);
    Order bid_high = create_dummy_order(110, 10, 2, BID);

    book.add_order(bid_low);
    book.add_order(bid_high);

    // Incoming ASK at 100. It is willing to sell at 100.
    // It should match with the 110 BID because that is the "Best Bid".
    Order ask = create_dummy_order(100, 10, 3, ASK);

    EXPECT_TRUE(book.add_order(ask));

    // The book should now contain the LOW bid (100) but NOT the HIGH bid (110).
    // We verify this by deleting the LOW bid. If it returns true, it was there.
    EXPECT_TRUE(book.delete_order(1)) << "Low bid should still be in book";
    EXPECT_FALSE(book.delete_order(2)) << "High bid should have been consumed";
}

// 5. Deletion
TEST_F(OrderBookTest, DeleteOrderRemovesLiquidity) {
    Order bid = create_dummy_order(100, 10, 1, BID);
    book.add_order(bid);

    // Delete it
    bool deleted = book.delete_order(1);
    EXPECT_TRUE(deleted) << "Should return true on successful deletion";

    // Verify it's gone by trying to delete again
    bool deleted_again = book.delete_order(1);
    EXPECT_FALSE(deleted_again) << "Should return false when deleting non-existent order";

    // Verify it's gone by trying to match against it
    Order ask = create_dummy_order(100, 10, 2, ASK);
    EXPECT_FALSE(book.add_order(ask)) << "Should not match against deleted order";
}

// 6. Self-Match Prevention (Optional, but good practice)
// Your current logic actually ALLOWS self-matching if IDs are different but logic is same.
// Usually we prevent this, but for your API we just test basic matching.
TEST_F(OrderBookTest, NoMatchOnPriceGap) {
    // Bid @ 100
    Order bid = create_dummy_order(100, 10, 1, BID);
    book.add_order(bid);

    // Ask @ 110 (Too expensive for the buyer)
    Order ask = create_dummy_order(110, 10, 2, ASK);

    EXPECT_FALSE(book.add_order(ask)) << "Orders should not match due to price spread";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}