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

string RecieveCardNumber="none";

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void CardNumberCallback(const std_msgs::String::ConstPtr& msg)
{
  RecieveCardNumber=msg->data;
ROS_INFO("%s",RecieveCardNumber.c_str());
ROS_INFO("%s","here");
}

int main(int argc, char **argv)
{


  ros::init(argc,argv,"mysql_update");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("CardNumber", 1000, CardNumberCallback);
  ros::Rate loop_rate(20);
 /* FILE *fp = fopen("/home/evansong/catkin_ws/src/mysql/src/mapB.png", "rb");

  if (fp == NULL)
  {
      fprintf(stderr, "cannot open image file\n");
      exit(1);
  }

  fseek(fp, 0, SEEK_END);

  if (ferror(fp)) {

      fprintf(stderr, "fseek() failed\n");
      int r = fclose(fp);

      if (r == EOF) {
          fprintf(stderr, "cannot close file handler\n");
      }

      exit(1);
  }

  int flen = ftell(fp);

  if (flen == -1) {

      perror("error occurred");
      int r = fclose(fp);

      if (r == EOF) {
          fprintf(stderr, "cannot close file handler\n");
      }

      exit(1);
  }

  fseek(fp, 0, SEEK_SET);

  if (ferror(fp)) {

      fprintf(stderr, "fseek() failed\n");
      int r = fclose(fp);

      if (r == EOF) {
          fprintf(stderr, "cannot close file handler\n");
      }

      exit(1);
  }

  char *data;
  data=new char [flen+1];

  int size = fread(data, 1, flen, fp);

  if (ferror(fp)) {

      fprintf(stderr, "fread() failed\n");
      int r = fclose(fp);

      if (r == EOF) {
          fprintf(stderr, "cannot close file handler\n");
      }

      exit(1);
  }

  int r = fclose(fp);

  if (r == EOF) {
      fprintf(stderr, "cannot close file handler\n");
  }
*/
  while (ros::ok())
  {

    ros::spinOnce();
    loop_rate.sleep();
  }
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "140.113.149.61", "test", "0120","nova", 0, NULL, 0) == NULL)
  {
  finish_with_error(con);
  }
  
  /*********************For Doctor*************************/
/*  if (mysql_query(con, "DROP TABLE IF EXISTS DOCTORS")) {
        finish_with_error(con);
    }

  if (mysql_query(con, "CREATE TABLE DOCTORS(Name TEXT,Room TEXT)")) {
      finish_with_error(con);
  }
 
 if (mysql_query(con, "INSERT INTO DOCTORS(Name,Room) VALUES('Chen','125')"))
  {
      finish_with_error(con);
  }
  if (mysql_query(con, "INSERT INTO DOCTORS(Name,Room) VALUES('Lin','413')"))
  {
      finish_with_error(con);
  }
if (mysql_query(con, "INSERT INTO DOCTORS(Name,Room) VALUES('Xie','112')"))
  {
      finish_with_error(con);
  }
if (mysql_query(con, "INSERT INTO DOCTORS(Name,Room) VALUES('Gao','513')"))
  {
      finish_with_error(con);
  }
*/
  /*********************For Patient*************************/
  if (mysql_query(con, "DROP TABLE IF EXISTS CARDNUM000075946905")) {
        finish_with_error(con);
    }

  if (mysql_query(con, "CREATE TABLE CARDNUM000075946905(Id TEXT,Birth TEXT,Name TEXT,Health_Check TEXT)")) {
      finish_with_error(con);
  }
 
 
  if (mysql_query(con, "INSERT INTO CARDNUM000075946905(Id,Birth,Name,Health_Check) VALUES('J122107196','0830307','Shao_Huang_Song','B')"))
  {
      finish_with_error(con);
  }

  mysql_close(con);
  exit(0);
}
