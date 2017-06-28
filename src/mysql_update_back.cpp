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
using namespace std;
using namespace cv;
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
void UpLoad(string str)
{
    printf("%s",str.c_str());
    FILE *fp = fopen("/home/evansong/catkin_ws/src/mysql/src/mapback.png", "wb");

  if (fp == NULL)
  {
      fprintf(stderr, "cannot open image file\n");
      exit(1);
  }

    MYSQL *con = mysql_init(NULL);
    char *x;
    x=new char [100];
    if (con == NULL)
    {
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
  }
    if (mysql_real_connect(con, "140.113.149.61", "test", "0120","nova", 0, NULL, 0) == NULL)
    {
    finish_with_error(con);
    }
  /*  if (mysql_query(con, "SELECT Data FROM Images WHERE Id=1")) {
      finish_with_error(con);
  }*/

    sprintf(x,"SELECT * FROM CARDNUM%s",str.c_str());
    string a=x;
    if (mysql_query(con, a.c_str()))
    {
    finish_with_error(con);
  }
    MYSQL_RES *result = mysql_store_result(con);
      if (result == NULL)
      {
          finish_with_error(con);
      }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields-1; i++)
      {
          ROS_INFO("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  sprintf(x,"SELECT Data FROM CARDNUM%s",str.c_str());
  a=x;
  if (mysql_query(con, a.c_str()))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result1 = mysql_store_result(con);

    if (result1 == NULL)
    {
        finish_with_error(con);
    }

    MYSQL_ROW row1 = mysql_fetch_row(result1);
    unsigned long *lengths = mysql_fetch_lengths(result1);

    if (lengths == NULL) {
        finish_with_error(con);
    }

    fwrite(row1[0], lengths[0], 1, fp);
 /*fwrite(row[2], lengths[2], 1, fp);*/
    int r = fclose(fp);
    mysql_free_result(result);
    mysql_free_result(result1);
    mysql_close(con);
    Mat image;
    image = imread("/home/evansong/catkin_ws/src/mysql/src/mapback.png", CV_LOAD_IMAGE_COLOR);
    namedWindow( "Display window", WINDOW_NORMAL );// Create a window for display.
    resizeWindow("Display window",1920,1080);
    imshow( "Display window", image );                   // Show our image inside it.
    waitKey(1000);
    image = imread("/home/evansong/catkin_ws/src/mysql/src/maporigin.png", CV_LOAD_IMAGE_COLOR);
    namedWindow( "Display window", WINDOW_NORMAL );// Create a window for display.
    resizeWindow("Display window",1920,1080);                                 // Wait for a keystroke in the window
    imshow( "Display window", image );
    waitKey(800);
}
bool update(cardmsgs::CardIO::Request &req,cardmsgs::CardIO::Response &res)
{
    string str,temp,str_up;
    char buffer[1];
//    ROS_INFO("get:%s",req.ReadCode.c_str());
    for(int i=0;i<12/*req.ReadCode.size()*/;i=i+1)
    {
    str=req.ReadCode.substr(i,1);
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
