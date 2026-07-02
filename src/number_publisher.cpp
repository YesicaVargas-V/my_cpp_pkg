#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"

using namespace std::chrono_literals;

class NumberPublisherNode : public rclcpp::Node 
{
public:
    NumberPublisherNode() : Node("number_publisher"), number (42) 
    {
       publisher_ = this-> create_publisher<example_interfaces::msg::Int64>("number", 10);
       timer_ = this-> create_wall_timer(1s, std::bind(&NumberPublisherNode::publishNews, this));
       RCLCPP_INFO(this->get_logger(),"Number publisher has been started");
    }

private:
    void publishNews()
    {
        auto msg = example_interfaces::msg::Int64();
        msg.data = number;
        publisher_ -> publish(msg);
    }
    std::int64_t number;
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr publisher_; 
    rclcpp::TimerBase::SharedPtr timer_;  
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberPublisherNode>(); 
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}