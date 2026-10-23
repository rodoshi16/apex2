#pragma once
#include <atomic>
#include <random>
#include <thread>
#include <vector>
#include <cmath>
#include <utility>
#include "apex/order.hpp"
#include "apex/disruptor.hpp"

namespace apex{

    class MidPriceProcess{
    public:
        MidPriceProcess(
            double initial_price, 
            double theta, 
            double mu, 
            double sigma, 
            double dt
        ) : price_(initial_price), 
            theta_(theta), 
            mu_(mu), 
            sigma_(sigma),
            dt_(dt), 
            rng_(std::random_device{}()),
            normal_(0.0, 1.0) {}
        
        int64_t step(){
            double z = normal_(rng_);
            price_ += theta_ * (mu_ - price_) * dt_ + sigma_ * std::sqrt(dt_) * z;
            price_ = std::max(price_, 1.0); 
            return static_cast<int64_t>(price_ * 100);
        }


        int64_t current_bps() const{
            return static_cast<int64_t>(price_ * 100);
        }

        private:
        double price_;
        double theta_, mu_, sigma_, dt_;
        std::mt19937_64 rng_;
        std::normal_distribution<double> normal_;
    };


    //Avellaneda - Stoikov model
    // decides what price to quote when buying and selling
    // adjust your quotes around inventory then set a spread around that 
    // reservation = mid - q * gamma * sigma^2
   //spread = gamma * sigma^2 + (2/gamma)*ln(1 + gamma/kappa)

    class MarketMaker {
    public:
        MarketMaker(
            double gamma,   
            double kappa,   
            double sigma,  
            int64_t half_spread_min_bps  
        ) : gamma_(gamma),
            kappa_(kappa),
            sigma_(sigma),
            half_spread_min_(half_spread_min_bps),
            inventory_(0),
            next_order_id_(1000000) {}

    
    std::pair<Order, Order> quote(int64_t mid_bps) {
        double mid = static_cast<double>(mid_bps);
        double reservation = mid - inventory_ * gamma_ * sigma_ * sigma_;

       
        double spread = gamma_ * sigma_ * sigma_
                      + (2.0 / gamma_) * std::log(1.0 + gamma_ / kappa_);

        int64_t half_spread = std::max(
            half_spread_min_,
            static_cast<int64_t>(spread * 50)  
        );

        int64_t bid_price = static_cast<int64_t>(reservation) - half_spread;
        int64_t ask_price = static_cast<int64_t>(reservation) + half_spread;

    
        if (bid_price >= ask_price) {
            bid_price = static_cast<int64_t>(reservation) - half_spread_min_;
            ask_price = static_cast<int64_t>(reservation) + half_spread_min_;
        }

        Order bid{
            next_order_id_++, 0,
            bid_price, 100, 0,
            Side::Bid, OrderType::Limit, OrderStatus::New
        };

        Order ask{
            next_order_id_++, 0,
            ask_price, 100, 0,
            Side::Ask, OrderType::Limit, OrderStatus::New
        };

        return {bid, ask};
    }

    void update_inventory(int64_t delta) { inventory_ += delta; }
    int64_t inventory() const { return inventory_; }

    private:
        double  gamma_, kappa_, sigma_;
        int64_t half_spread_min_;
        int64_t inventory_;
        uint64_t next_order_id_;
};

    //Arrives via Poisson process
    // Trades it in the direction it thinks the price will move

    class InformedTrader{
        InformedTrader(
        double arrival_rate,       
        double information_edge,  
        uint64_t start_id
    ) :
        information_edge_(information_edge),
        next_order_id_(start_id),
        rng_(std::random_device{}()),
        exponential_(arrival_rate),
        uniform_(0.0, 1.0) {}

    
        bool arrives() {
            ticks_until_arrival_--;
            if (ticks_until_arrival_ <= 0) {
                ticks_until_arrival_ = static_cast<int>(exponential_(rng_));
                return true;
            }
            return false;
        }


        Order generate_order(int64_t current_price, int64_t true_price) {
            bool price_going_up = true_price > current_price;
            bool trade_correct  = uniform_(rng_) < information_edge_;

            Side side;
            if (trade_correct) {
                side = price_going_up ? Side::Bid : Side::Ask;
            } else {
                side = price_going_up ? Side::Ask : Side::Bid;
            }

            return Order{
                next_order_id_++, 0,
                0, 50, 0, 
                side, OrderType::Market, OrderStatus::New
            };
        }

    private:
        double  information_edge_;
        uint64_t next_order_id_;
        int      ticks_until_arrival_ = 1;
        std::mt19937_64 rng_;
        std::exponential_distribution<double> exponential_;
        std::uniform_real_distribution<double> uniform_;
    };

}     