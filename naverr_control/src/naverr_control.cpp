#include <ros/ros.h>
#include <ros/console.h>
#include <iostream>
#include <gazebo_msgs/ApplyJointEffort.h>
#include <gazebo_msgs/ContactsState.h>
#include <vector>

// These variables are used to communicate with ROS
static ros::NodeHandle* node_joints; // Declared as a pointer because it must be initialized after ros::init()
static ros::ServiceClient client_joints;

// This is the effort "magnitude" you will need to move the bot forward
// We got this value by trial & error, but we can also get it by solving equations
static double forwardEffort = 2.5;

// This is the effort "magnitude" you will need to rotate the bot
// We got this value by trial & error, but we can also get it by solving equations
// IMPORTANT NOTE: Using this rotation magnitude, we can rotate the robot half a full rotation in 3 seconds
static double rotationEffort = 2;

// We will use this variable to store the last time we hit a wall
static ros::Time* last_bump_resolution;

/* This function is used to apply an effort on both wheels of our bot.
 * Parameters:
 * left_wheel_effort, right_wheel_effort: Effort values (can ne negative) to apply on left and right wheel respectively
 * duration: Time duration in seconds to apply these efforts on the joints starting from start_time
 * start_time: Specifies when to start applying the efforts on the joints. Note: This specifies absolute time.
 *
 * IMPORTANT NOTE: If you call this function multiple times with overlapping application time, it will "accumulate"
 * efforts on the joints and will NOT "reset" the efforts.
*/
void apply_efforts_on_bot_joints(double left_wheel_effort, double right_wheel_effort, double duration, ros::Time start_time){
    gazebo_msgs::ApplyJointEffort right_wheel_msg;

    // TODO-1: Fill the parameters of the message using the values in the parameter list
    // Hint: Check the code of last tutorial; the right wheel's name is "front_right_wheel_joint"

    // TODO-2: Send this message using ROS services.
    // Hint: The service object is called "client_joints"


    gazebo_msgs::ApplyJointEffort left_wheel_msg;

    // TODO-3: Fill the parameters of the message using the values in the parameter list
    // Hint: Check the code of last tutorial; the left wheel's name is "front_left_wheel_joint"

    // TODO-4: Send this message using ROS services.
    // Hint: The service object is called "client_joints"

    std::cout << "Moved bot for duration " << duration << " with efforts (L R) ";
    std::cout << left_wheel_effort << " " << right_wheel_effort << std::endl;
}

// This function is going to be called "every" time the bumper sensor sends data according to its update rate
void bumpedCallback(const gazebo_msgs::ContactsState& msg){
    /* TODO-5: Complete this function to make the robot perform the basic functionality of a Roomba robot on hitting
     * an object. More details on this functionality can be found in the requirement document
     *
     * Hints:
     * 1. We don't need to perform anything if the sensor sends empty data. That's why we check
     *    if the states list is empty or not
     *
     * 2. The steps needed are the simplest steps you can think of to evade the wall;
     *    we don't need a complicated algorithm. All what you need to think about is how to use
     *    the function apply_efforts_on_bot_joints to perform this functionality. Think carefully about the start_time
     *    and duration parameters of that function. Use the forwardEffort and rotationEffort values when apply efforts.
     *
     * 3. Notice that during evading a wall, the bumper sensor will still tell us that we are hitting a wall, constantly.
     *    So, we only need to start evading a wall if we aren't currently evading it which can be determined by the last
     *    time we started evading a wall. Use last_bump_resolution and ros::Time::now() to be able to handle this.
     *
     * */
    if(!msg.states.empty() && true){
        std::cout << "Wall detected, evading ...\n";
    }
}

// This function is called to start listening to the bumper sensor
void listen_to_bump(){
    last_bump_resolution = new ros::Time(ros::Time::now());

    /* TODO-6 Find out how to subscribe to a ROS topic from the "Writing the Subscriber Node" section in this link:
     * http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29
     * We want to call the function "bumpedCallback" whenever there is a new message on the "/bumper_vals" topic
     * with a queue of size 1000
    */
}


int main(int argc, char **argv) {
    // This performs various steps to be able to work under the ROS framework
    ros::init(argc, argv, "naverr_control");

    // This will allow us to see debug messages in the console if they occur.
    if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME,
                                       ros::console::levels::Debug)) {
        ros::console::notifyLoggerLevelsChanged();
    }

    // We acquire a serviceClient that can call the "/gazebo/apply_joint_effort" service
    node_joints = new ros::NodeHandle();
    client_joints = node_joints->serviceClient<gazebo_msgs::ApplyJointEffort>(
            "/gazebo/apply_joint_effort");

    // We start moving the bot in the forward direction for an infinite duration after a second from now
    apply_efforts_on_bot_joints(forwardEffort, forwardEffort, 100000, ros::Time::now() + ros::Duration(1.0));

    // We subscribe to our only sensor and set the callback of acquiring sensor data
    listen_to_bump();
}
