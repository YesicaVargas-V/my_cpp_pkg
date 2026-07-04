#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

using namespace std::placeholders;

class NumberCounterNode : public rclcpp::Node 
{
public:
    NumberCounterNode() : Node("number_counter") , counter (0)
    {
        counter_publisher_ = this-> create_publisher<example_interfaces::msg::Int64>("number_count", 10);
        number_susbcriber_ = this-> create_subscription<example_interfaces::msg::Int64>("number", 10,
                        std::bind(&NumberCounterNode::callbackNumberCounter, this, std::placeholders::_1));
        server_ = this->create_service<example_interfaces::srv::SetBool>("reset_counter", 
                        std::bind(&NumberCounterNode::callbackResetCounter, this, _1 ,_2));

        RCLCPP_INFO(this->get_logger(), "Number counter hass been started");
    }

private:
    void callbackNumberCounter(const example_interfaces::msg::Int64::SharedPtr msg)
    {
        counter = msg->data + counter;
        auto newMsg = example_interfaces::msg::Int64();
        newMsg.data = counter;
        counter_publisher_->publish(newMsg);
    }
    void callbackResetCounter( const example_interfaces::srv::SetBool::Request::SharedPtr request,
                               const example_interfaces::srv::SetBool::Response::SharedPtr response)
    {
        if (request->data ==true)
        {
            counter = 0;
            response->success = true;
            response->message = "The counter is restarted to 0";

        }
        else
        {
            counter = counter;
            response->success = false;
            response->message = "The counter stay the same";

        }

    }


    int counter; 
    rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr number_susbcriber_;
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr counter_publisher_; 
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr server_;

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberCounterNode>(); 
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
} 

