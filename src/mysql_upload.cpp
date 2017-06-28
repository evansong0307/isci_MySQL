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
string datalist[7];
string doctorlist[20];
int numberofdoctor;
bool DoctorListCallbackbool = false;

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}


void DoctorListCallback(const std_msgs::String::ConstPtr& msg)
{
  char* pch;
  int i = 0;
  string strlist[2];
  char s[100];
  char* token = ":";
  string str = msg->data;
  memset(s, '0', sizeof(str));
  strcpy(s, str.c_str());
  pch = strtok(s, token);
  while (pch != NULL)
  {
    strlist[i] = pch;
    pch = strtok(NULL, token);
    i++;
  }
  ROS_INFO("%s", strlist[0].c_str());
  ROS_INFO("%s", strlist[1].c_str());
  i = 0;
  token = ",";
  memset(s, '0', sizeof(strlist[0]));
  strcpy(s, strlist[0].c_str());
  pch = strtok(s, token);
  while (pch != NULL)
  {
    datalist[i] = pch;
    pch = strtok(NULL, token);
    i++;
  }
  i = 0;
  token = "'";
  memset(s, '0', sizeof(strlist[1]));
  strcpy(s, strlist[1].c_str());
  pch = strtok(s, token);
  while (pch != NULL)
  {
    if (i % 2 == 1)
      doctorlist[i / 2] = pch;
    pch = strtok(NULL, token);
    i++;
  }
  numberofdoctor = i / 2;
  ROS_INFO("%d", numberofdoctor);
  DoctorListCallbackbool = true;
}



int main(int argc, char **argv)
{


  ros::init(argc, argv, "mysql_upload");
  ros::NodeHandle n;
  ros::Subscriber subD = n.subscribe("DoctorList", 1000, DoctorListCallback);
  ros::Rate loop_rate(20);

  while (ros::ok())
  {
    string doctorsequence;
    if (DoctorListCallbackbool)
    {
      for (int i = 0; i < numberofdoctor; i++)
      {
        doctorsequence = doctorsequence + doctorlist[i];
      }
      ROS_INFO("%s", doctorsequence.c_str());
      MYSQL *con = mysql_init(NULL);
      if (con == NULL)
      {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
      }
      if (mysql_real_connect(con, "140.113.149.61", "test", "0120", "nova", 0, NULL, 0) == NULL)
      {
        finish_with_error(con);
      }
      if (int(datalist[0].c_str()[0]) >= 48 && int(datalist[0].c_str()[0]) <= 57)
      {
        string name = "DROP TABLE IF EXISTS CARDNUM" + datalist[0];
        if (mysql_query(con, name.c_str())) {
          finish_with_error(con);
        }
        name = "CREATE TABLE CARDNUM" + datalist[0] + "(Id TEXT,Birth TEXT,Name TEXT,Blood TEXT,Height TEXT,Weight TEXT,Health_Check TEXT)";
        if (mysql_query(con, name.c_str())) {
          finish_with_error(con);
        }
        char datastr[100];
        sprintf(datastr, "('%s','%s','%s','%s','%s','%s','%s')", datalist[1].c_str(), datalist[2].c_str(), datalist[3].c_str(), datalist[4].c_str(), datalist[5].c_str(), datalist[6].c_str(), doctorsequence.c_str());
        name = "INSERT INTO CARDNUM" + datalist[0] + "(Id,Birth,Name,Blood,Height,Weight,Health_Check) VALUES" + datastr;
        ROS_INFO("%s", name.c_str());
        if (mysql_query(con, name.c_str() ))
        {
          finish_with_error(con);
        }

      }
      else
        ROS_INFO("Not a Card");


      mysql_close(con);
      DoctorListCallbackbool = false;
    }
    ros::spinOnce();
    loop_rate.sleep();
  }

  exit(0);
}