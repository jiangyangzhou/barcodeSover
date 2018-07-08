#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <zbar.h>
#include <string>  
#include <list>  
#include <vector>  
#include <map>  
#include <stack>  
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <io.h>
#include <fstream>
#include "barcode_locate.h"


using namespace std;
using namespace cv;
using namespace zbar;


#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;



void main() {
	string dirname = "./1D/new.png";
	barcode_locate(dirname);
}
	