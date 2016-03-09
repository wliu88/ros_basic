#include "ros/ros.h"
// string msg
#include "std_msgs/String.h"
// customized srv
#include "beginner_tutorials/AddTwoInts.h"

#include <sstream>

// callback for subscriber
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

// service node service function
bool add(beginner_tutorials::AddTwoInts::Request  &req,
         beginner_tutorials::AddTwoInts::Response &res)
{
  res.sum = req.a + req.b;
  return true;
}


int main(int argc, char **argv)
{
  /*
   The ros::init() function needs to see argc and argv so that it can perform
   any ROS arguments and name remapping that were provided at the command line.
  */
  ros::init(argc, argv, "talker");

  /*
   NodeHandle is the main access point to communications with the ROS system.
   The first NodeHandle constructed will fully initialize this node, and the last
   NodeHandle destructed will close down the node.
  */
  ros::NodeHandle n;
  
  // publisher & subscriber
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  

  // service node & client node
  ros::ServiceServer service = n.advertiseService("add_two_ints", add);
  ros::ServiceClient client = n.serviceClient<beginner_tutorials::AddTwoInts>("add_two_ints");

  ros::Rate loop_rate(10);

  /*
   Since service calls are blocking, it will return once the call is done. 
   If the service call succeeded, call() will return true and the value in srv.
   response will be valid. If the call did not succeed, call() will return false 
   and the value in srv.response will be invalid.
  */ 
  beginner_tutorials::AddTwoInts srv;
  srv.request.a = 1;
  srv.request.b = 2;
  if (client.call(srv))
  {
    ROS_INFO("Sum: %ld", (long int)srv.response.sum);
  }
  

  while (ros::ok())
  {
    std_msgs::String msg;

    chatter_pub.publish(msg);
    
    /*
     ros::spin() will enter a loop, pumping callbacks.  With this version, all
     callbacks will be called from within this thread (the main one).  ros::spin()
     will exit when Ctrl-C is pressed, or the node is shutdown by the master.
    */
    ros::spin();

    loop_rate.sleep();
  }

  return 0;
}
