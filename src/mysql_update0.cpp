#include <mysql/mysql.h>
#include <ros/ros.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "cardmsgs/CardIO.h"
using namespace std;
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
void UpLoad(string str)
{
    printf("%s",str.c_str());
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
    }
    if (mysql_real_connect(con, "140.113.149.61", "test", "0120","nova", 0, NULL, 0) == NULL)
    {
    finish_with_error(con);
    }
    if (mysql_query(con, "SELECT * FROM Patients"))
    {
    finish_with_error(con);
    }
  /*  if (mysql_query(con, "CREATE TABLE Patients(Id TEXT, DES TEXT)"))
    {
    finish_with_error(con);
    }
    if (mysql_query(con, "INSERT INTO Patients VALUES('000075946905','ABCD')"))
    {
    finish_with_error(con);
    }
    if (mysql_query(con, "INSERT INTO Patients VALUES('000075946900','Mercedes')"))
    {
    finish_with_error(con);
  }*/
    mysql_close(con);
}
bool update(cardmsgs::CardIO::Request &req,cardmsgs::CardIO::Response &res)
{
    string str,temp,str_up;
    char buffer[1];
//    ROS_INFO("get:%s",req.ReadCode.c_str());
    for(int i=0;i<24/*req.ReadCode.size()*/;i=i+2)
    {
    str=req.ReadCode.substr(i+1,1);
 //   printf("%s",str.c_str());
    sprintf(buffer,"%s", str.c_str());
    temp=buffer;
    str_up=str_up+temp;
    }
    UpLoad(str_up);
    ROS_INFO("Upload Card Number:%s",str_up.c_str());//show up the ic_card number
    res.ReadSuccess=true;
    return true;
}
int main(int argc, char **argv)
{
    ros::init(argc,argv,"mysql_update_node");
    ros::NodeHandle n;
    ros::ServiceServer service=n.advertiseService("ReadCard",update);
    ROS_INFO("MySQL client version: %s\n", mysql_get_client_info());
    ros::spin();
    return 0;
}
