#include <iostream>
#include "contacts.pb.h" // 引⼊编译⽣成的头⽂件
using namespace std;

int main()
{
    // -L /usr/local/lib
    string people_str;
    // 序列化
    {
        // .proto⽂件声明的package，通过protoc编译后，会为编译⽣成的C++代码声明同名的命名空间
        // 其范围是在.proto ⽂件中定义的内容

        contacts::PeopleInfo people;
        people.set_age(1);
        people.set_name("徐颖馨");
        // 调⽤序列化⽅法，将序列化后的⼆进制序列存⼊string中
        if (!people.SerializeToString(&people_str))
        {
            cout << "序列化联系⼈失败." << endl;
        }
        // 打印序列化结果
        cout << "序列化后的 people_str: " << people_str.size() << endl;
    }
    // 反序列化
    {
        contacts::PeopleInfo people;
        // 调⽤反序列化⽅法，读取string中存放的⼆进制序列，并反序列化出对象
        if (!people.ParseFromString(people_str))
        {
            cout << "反序列化出联系⼈失败." << endl;
        }
        // 打印结果
        cout << "Parse age: " << people.age() << endl;
        cout << "Parse name: " << people.name() << endl;
    }
    return 0;
}