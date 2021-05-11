#include<iostream>
#include<map>

using namespace std;

int main()
{
    map<int,char> m;
    m[8] = 'a';
    m[6] = 'b';
    m[11] = 'c';
    m[5] = 'd';
    m[7] = 'e';
    m[10] = 'f';
    m[13] = 'g';
    m[12] = 'h';
    m[15] = 'i';
    map<int,char>::const_iterator it = m.begin();
    while( it!=m.end() )
    {
        cout<<it->first<<" "<<it->second<<endl;
        it++;
    }

    it = m.begin();//相当于获取了平衡树最左下面（most left）的结点的迭代器
    cout<<"min "<<it->first<<" "<<it->second<<endl;//最小的key值

    it = m.end();//相当于获取了平衡树最右下面（most right）的结点的迭代器
    it--;
    cout<<"max "<<it->first<<" "<<it->second<<endl; //最大的key值

    system("pause");
    return 0;
}