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
#include <sstream>
using namespace std;
using namespace cv;

string route;
bool Recieve=false;

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}

void UpLoad(string str)
{
	FILE *fp = fopen("/home/evansong/catkin_ws/src/mysql/src/mapback.png", "wb");

	if (fp == NULL) {
		fprintf(stderr, "cannot open image file\n");
		exit(1);
	}

	MYSQL *con = mysql_init(NULL);
	if (con == NULL)	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "140.113.149.61", "admin", "123", "nova", 0, NULL, 0) == NULL) {
		finish_with_error(con);
	}
	/*if (mysql_query(con, "SELECT Data FROM Images WHERE Id=1")) {
		finish_with_error(con);
	}*/

	string name = "SELECT * FROM CARDNUM" + str;
	if (mysql_query(con, name.c_str())) {
		finish_with_error(con);
	}

	MYSQL_RES *result = mysql_store_result(con);
	if (result == NULL) {
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	char *buffer=new char [10];

	while ((row = mysql_fetch_row(result))) {
		for (int i = 0; i < num_fields ; i++) {
			if(i==(num_fields-1))
			buffer=row[i];
			ROS_INFO("%s ", row[i] ? row[i] : "NULL");
		}
		ROS_INFO("---------------------");
	}
	ROS_INFO("%s",buffer);
	ROS_INFO("%c",buffer[0]);

//num_fields - 1
	if (buffer[0] == 'A')
	{
		ROS_INFO("---------1--------");
		route="01";
	}
	else if (buffer[0] == 'B')
	{
		ROS_INFO("----------2----------");
		route="012";
	}
	else{
		ROS_INFO("----------3----------");
		route="0123";
	}
	Recieve=true;
/*
	name = "";
	name = "SELECT H FROM CARDNUM" + str;
	if (mysql_query(con, name.c_str())) {
		finish_with_error(con);
	}

	MYSQL_RES *result1 = mysql_store_result(con);

	if (result1 == NULL) {
		finish_with_error(con);
	}

	MYSQL_ROW row1 = mysql_fetch_row(result1);
	unsigned long *lengths = mysql_fetch_lengths(result1);

	if (lengths == NULL) {
		finish_with_error(con);
	}

	fwrite(row1[0], lengths[0], 1, fp);
	//fwrite(row[2], lengths[2], 1, fp);

	int r = fclose(fp);
	*/
	mysql_free_result(result);
	/*mysql_free_result(result1);*/
	mysql_close(con);


	/*Mat image;
	image = imread("/home/evansong/catkin_ws/src/mysql/src/mapback.png", CV_LOAD_IMAGE_COLOR);
	namedWindow( "Display window", WINDOW_NORMAL );// Create a window for display.
	resizeWindow("Display window", 800, 600);
	imshow( "Display window", image );                   // Show our image inside it.
	waitKey(1000);
	image = imread("/home/evansong/catkin_ws/src/mysql/src/maporigin.png", CV_LOAD_IMAGE_COLOR);
	namedWindow( "Display window", WINDOW_NORMAL );	// Create a window for display.
	resizeWindow("Display window", 800, 600);		// Wait for a keystroke in the window
	imshow( "Display window", image );
	waitKey(800);*/
}

bool update(cardmsgs::CardIO::Request &req, cardmsgs::CardIO::Response &res)
{
	string str;
	//ROS_INFO_STREAM("get: " << req.ReadCode.c_str());
	str = req.ReadCode.substr(0, 12);
	ROS_INFO_STREAM("Upload Card Number: " << str);	//show up the ic_card number
	UpLoad(str);

	res.ReadSuccess = true;
	return true;
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "mysql_update_node");
	ros::NodeHandle n;
	ros::ServiceServer service = n.advertiseService("ReadCard", update);
	ros::Publisher route_pub = n.advertise<std_msgs::String>("Doctorlist", 1000);
	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		std_msgs::String msg;
		msg.data=route;
		if(Recieve)
		route_pub.publish(msg);
	Recieve=false;
		ros::spinOnce();
		loop_rate.sleep();
	}
	ROS_INFO("MySQL client version: %s\n", mysql_get_client_info());
	ros::spin();
	return 0;
}
