#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::cout;
using std::endl;
using std::vector;


void CheckMakeArrayPixelXList (int i, vector<int> & GlobalArray, binary_type * MyGrpArray, string str);
//void CheckMakeArrayPixelYList (int i, vector<int> & GlobalArray, binary_type * MyGrpArray);






void CheckMakeArrayPixelXList (int i, vector<int> & GlobalArray, binary_type * MyGrpArray, string str)
{

    if (str == "PIXELX")
    {
	  cout << endl << "****RETRIEVER_MAKE_ARRAY_PIXELX_LIST****************************************"<<endl;
	  cout << endl << "Check if MyGrpArray["<<i<<"] after MakeArray_pixelX in CalculateArray is correct: " << endl;
    }
  else if (str == "PIXELY")
    {
          cout << endl << "****RETRIEVER_MAKE_ARRAY_PIXELY_LIST****************************************"<<endl;
	  cout << endl << "Check if MyGrpArray["<<i<<"] after MakeArray_pixelY in CalculateArray is correct: " << endl;
    }
    else if (str == "PIXELYbefore")
    {
          cout << endl << "****RETRIEVER_MAKE_ARRAY_PIXELY_LIST****************************************"<<endl;
	  cout << endl << "Check if MyGrpArray["<<i<<"] before MakeArray_pixelY in CalculateArray is correct: " << endl;
    }
    else if (str == "TBIN")
    {
	  cout << endl << "****RETRIEVER_MAKE_ARRAY_TBIN_LIST****************************************"<<endl;
	  cout << endl << "Check if MyGrpArray["<<i<<"] after MakeArray_Tbin in CalculateArray is correct: " << endl;
    }
    else if (str == "EVERYTHING")
    {
	  cout << endl << "****RETRIEVER_EVERYTHING****************************************";
	  cout << endl << "Check if MyGrpArray["<<i<<"] in CalculateArray is correct: " << endl<<endl;
    }

	  int ll = 0;
	  int mm = 0;
          cout << "   ";

	  for (int j=0; j<GlobalArray[1]*GlobalArray[2]*GlobalArray[0]; ++j)
	    {
	      cout << MyGrpArray[j] << " ";            //listing of MyGrpArray    
      
	      if (ll == (GlobalArray[2]-1))
		{
		  cout << endl << "   ";
		  ll = 0;
		  ++mm;
		}
	      else
		{
		  ++ll;
		}
	      if (mm == (GlobalArray[1]))
		{
		  cout << endl;
		  cout << "   ";
		  mm = 0;
		}
	    } 

  return;
}
