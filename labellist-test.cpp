#include <iostream>
#include "labellist.h"
using namespace std;


typedef std::vector<LabelList::MsgType> MsgVec;
int main()
{
    LabelList l1;
    LabelList::MsgToken newToken1 = l1.add("!!!"); 
    LabelList::MsgToken newToken3 = l1.add("Hello"); 
    l1.label(newToken3, "Greet");
    LabelList::MsgToken newToken4 = l1.add("Bye"); 
    LabelList::MsgToken newToken2 = l1.add("???");
    LabelList::MsgToken newToken5 = l1.add("?!!");
    l1.label(newToken1, "Symbols");
    l1.label(newToken2, "Symbols"); 
    l1.label(newToken5, "Symbols"); // symobls is labels_[2] and has 3 msgs in it
    LabelList::MsgToken foundMsg=l1.find(0, "Greet");
    LabelList::MsgToken foundMsg2=newToken5.prev("Symbols");
    cout<< foundMsg2<<endl;
    l1.remove(newToken4);
    l1.remove(newToken1);
    l1.remove(newToken5);
    cout<<l1.size()<<endl;
    l1.print(std::cout, "all", ' ');
    l1.clear();
    cout<<l1.size()<<endl;
     return 0;  
}