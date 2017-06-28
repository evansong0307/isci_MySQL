#include <mysql/mysql.h>
#include <ros/ros.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include "cardmsgs/CardIO.h"
#include "std_msgs/String.h"
using namespace std;

string download_data;
bool datarecievebool=false;

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void CardNumberCallback(const std_msgs::String::ConstPtr& msg)
{
    string str;
    bool no_data=false;
 //   memset(str, '0', sizeof(msg->data));
   // strcpy(str, msg->data.c_str());
str=msg->data.substr(0,1);

if((int(str.c_str()[0]))>=48 && (int(str.c_str()[0])<=57)){
MYSQL *con = mysql_init(NULL);
if (mysql_real_connect(con, "140.113.149.61", "test", "0120","nova", 0, NULL, 0) == NULL)
  {
  finish_with_error(con);
  }
string name = "SELECT * FROM CARDNUM" + msg->data;
ROS_INFO("%s",name.c_str());
if (mysql_query(con, name.c_str())) {
        ROS_INFO("No DataBase Upload First");
        no_data=true;
    }
    if(!no_data){
MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        finish_with_error(con);
    }
int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    string buffersend;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields ; i++) {          
            buffersend=buffersend+":"+row[i];
            ROS_INFO("%s ", row[i] ? row[i] : "NULL");
        }
        ROS_INFO("--------%s--------",buffersend.c_str());
    }
download_data=buffersend;
datarecievebool=true;
mysql_free_result(result);
}
mysql_close(con);

}
else
    ROS_INFO("No CardNumber available");
}

int main(int argc, char **argv)
{


  ros::init(argc,argv,"mysql_download");
  ros::NodeHandle n;
  ros::Publisher pub=n.advertise<std_msgs::String>("DataRecieve", 1000);
  ros::Subscriber sub = n.subscribe("CardNumber", 1000, CardNumberCallback);
  ros::Rate loop_rate(20);
 
  while (ros::ok())
  {
    std_msgs::String msg;
    if(datarecievebool)
    {
        msg.data=download_data;
        pub.publish(msg);
        datarecievebool=false;
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
 
  exit(0);
}
