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


    class MarketMaker{

    public:
        MarketMaker(
            double gamma, 
            double kappa, 
            double sigma, 
            int64_t half_spread_min_bps
        ) : gamma_(gamma), 
            kappa_(kappa),
            sigma_(sigma), 
            half_spread_min(half_spread_min_bps), 
            inventory_(0), 
            next_order_id_(1000000) {}
        

        std::pair<Order, Order> quote(int64_t mid_bps){
            double mid = static_cast<double>(mid_bps);
            double reservation = mid - inventory_ * gamma_ * sigma_ * sigma_;

            double spread = gamma_ * sigma_ * sigma_
                      + (2.0 / gamma_) * std::log(1.0 + gamma_ / kappa_);

            

        }
        

        

    private:
        double gamma_, kappa_, sigma_;
        int64_t half_spread_min;
        int64_t inventory_;
        uint64_t next_order_id_; 














    }







}