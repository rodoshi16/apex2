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

}