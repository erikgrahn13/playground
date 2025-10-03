#include <iostream> 

class Transport {
    public:
    virtual ~Transport(){};
    virtual void deliver() = 0;
};

class Truck : public Transport {
    public:
    void deliver() override {
        std::cout << "Deliver by Truck" << std::endl;
    }
};

class Ship : public Transport {
    public:
    void deliver() override {
        std::cout << "Deliver by Ship" << std::endl;
    }
};

class Logistics {
    public:
    virtual ~Logistics(){}
    void planDelivery() {
        auto transport = createTransport();
        transport->deliver();
    }

    protected:
    virtual std::unique_ptr<Transport> createTransport() = 0;
};

class RoadLogistics : public Logistics {
    protected:
    std::unique_ptr<Transport> createTransport() override {
        return std::make_unique<Truck>();
    }
};

class SeaLogistics : public Logistics {
    protected:
    std::unique_ptr<Transport> createTransport() override {
        return std::make_unique<Ship>();
    }
};


int main(int argc, char* argv[]) {
    std::unique_ptr<Logistics> logistics;
    
    if(argc > 1 && std::string(argv[1]) == "road") {
        logistics = std::make_unique<RoadLogistics>();
    } else if(argc > 1 && std::string(argv[1]) == "sea") {
        logistics = std::make_unique<SeaLogistics>();
    } else {
        std::cout << "Usage: " << argv[0] << " [road|sea]" << std::endl;
        return 1;
    }
    
    logistics->planDelivery();

    return 0;
}