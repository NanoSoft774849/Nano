// openCvx.cpp : Defines the entry point for the console application.
//


#include<conio.h>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
using namespace cv;
using namespace std;



bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
int match_method;
int max_Trackbar = 5;
void MatchingMethod(int, void*);
Point ns_find_template(const char*, const char*);

double ns_find_distance(Point p1, Point p2);

double xdiff(Point p1, Point p2);

double ydiff(Point p1, Point p2);

struct ns_mat
{
	Mat image;
	Point maxLoc;
};


#define ns_max(x,y) ( x>=y?x:y)
#define ns_min(x,y) (x<=y ?x:y)

void jsonfmt(const char * key, int value, bool comma)
{
	///const char* com=
	cout << "\"" << key << "\":" << value << (comma ? "," : " ") << endl;
}

void jsonfmt(const char * key, double value, bool comma)
{
	///const char* com=
	cout << "\"" << key << "\":" << value << (comma ? "," : " ") << endl;
}
void jsonfmt(const char * key, const char * value, bool comma)
{
	///const char* com=
	cout << "\"" << key << "\":" << "\"" << value << "\"" << (comma ? "," : " ") << endl;
}

int main(int argc, char**argv)
{
	/**Arg descriptions**/
	/*

	argv[0]--appname
	argv[1] -ref image
	argv[2] -template image
	argv[3]--image that you want to match


	*/
	if (argc < 4)
	{
		return -1;
	}

	try
	{
		const char * ref_path = argv[1];
		const char * temp_path = argv[2];
		const char * img_path = argv[3];//image
		//template 

		Point ref_point = ns_find_template(ref_path, temp_path);


		Point img_point = ns_find_template(img_path, temp_path);

		cout << "{" << endl;
		jsonfmt("ref_x", ref_point.x, true);
		jsonfmt("ref_y", ref_point.y, true);
		jsonfmt("img_x", img_point.x, true);
		jsonfmt("img_y", img_point.y, true);
		jsonfmt("xdiff", xdiff(ref_point, img_point), true);
		jsonfmt("ydiff", ydiff(ref_point, img_point), true);
		jsonfmt("distance", ns_find_distance(ref_point, img_point), false);

		cout << "}" << endl;
	}
	catch (Exception ex)
	{
		return -1;
	}



	//cout << "ref_Max_loc(x,y):(" << ref_point.x << "," << ref_point.y << ")" << endl;
	//
	//
	//cout << "image_Max_loc(x,y):(" << img_point.x << "," << img_point.y << ")" << endl;
	//
	//
	//cout << "distance(mm):" << ns_find_distance(ref_point, img_point) << endl;



	//cout << "argcount:" << argc << endl;
	//cout << argv[0] << argv[1] << endl;
	//match_method = TM_CCORR_NORMED;
	//use_mask = false;
	// img = imread(img_path);
	// templ = imread(temp_path);
	//
	//if (!img.empty())
	//{
	//	//imshow("mywindow", img);
	//}
	//MatchingMethod(0, 0);
	//waitKey(0);
	return 0;
}
double xdiff(Point p1, Point p2)
{
	double xmax = ns_max(p1.x, p2.x);
	double xmin = ns_min(p1.x, p2.x);
	return (xmax - xmin) * 100 / 294;
}
double ydiff(Point p1, Point p2)
{
	double ymax = ns_max(p1.y, p2.y);
	double ymin = ns_min(p1.y, p2.y);
	return (ymax - ymin) * 100 / 294;
}
double ns_find_distance(Point p1, Point p2)
{

	double x = p1.x - p2.x;
	double y = p1.y - p2.y;

	double dist = (x*x) + (y*y);


	return sqrt(dist) * 100 / 294;// distance in mm

}
Point ns_find_template(const char * img_path, const char * temp_path)
{

	Mat img_display;

	Mat im = imread(img_path);
	Mat temp = imread(temp_path);

	im.copyTo(img_display);
	Mat result;
	int result_cols = im.cols - temp.cols + 1;
	int result_rows = im.rows - temp.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	int match_method = TM_CCORR_NORMED;


	matchTemplate(im, temp, result, match_method);

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());


	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	return maxLoc;
}
void MatchingMethod(int, void*)
{
	Mat img_display;
	img.copyTo(img_display);
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;
	result.create(result_rows, result_cols, CV_32FC1);
	bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
	if (use_mask && method_accepts_mask)
	{
		matchTemplate(img, templ, result, match_method);
	}
	else
	{
		matchTemplate(img, templ, result, match_method);
	}
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, img_display);
	//imshow(result_window, result);
	cout << "MaxLoc:(x,y):(" << maxLoc.x << "," << matchLoc.y << ")" << endl;
	return;
}

