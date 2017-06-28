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
#include <fstream>

using namespace std;
void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{

  FILE* file;
  char str[10];
  MYSQL *con = mysql_init(NULL);
  file=fopen("~/catkin_ws/src/mysql/src/abc.txt","r");
  if(file){
  	while(fscanf(file,"%s",str)!=EOF)
  		ROS_INFO("%s",str);
  	 ROS_INFO("fuck");
  }
  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "140.113.149.61", "test", "0120","nova", 0, NULL, 0) == NULL)
  {
  finish_with_error(con);
  }
  if (mysql_query(con, "DROP TABLE IF EXISTS DATA621")) {
        finish_with_error(con);
    }
  if (mysql_query(con, "CREATE TABLE DATA621(X DOUBLE, Y DOUBLE, Z DOUBLE)")) {
      finish_with_error(con);
  }
  
  fclose(file);
  mysql_close(con);
  exit(0);
}
