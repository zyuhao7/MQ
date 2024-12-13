#include "sqlite.hpp"
#include <cassert>
int select_stu_callback(void *arg, int col_count, char **result, char **fields_name)
{
    std::vector<std::string> *arry = (std::vector<std::string> *)arg;
    arry->push_back(result[1]);
    return 0;
}

int main()
{
    SqliteHelper helper("./test.db");
    assert(helper.open());

    const char *ct = "create table if not exists student(sn int primary key, name varchar(32),age int);";
    assert(helper.exec(ct, nullptr, nullptr));
    const char *insert_sql = "insert into student values(1, '小明', 18), (2, '小黑', 20),(3, '小美', 21) ;";
    // 1|小明|18
    // 2|小黑|20
    // 3|小美|21
    // assert(helper.exec(insert_sql, nullptr, nullptr));

    // const char *update_sql = "update student set name='李明' where sn=1;";
    // assert(helper.exec(update_sql, nullptr, nullptr));

    std::vector<std::string> name_buf;
    const char *select_sql = "select sn, name, age from student";
    assert(helper.exec(select_sql, select_stu_callback, &name_buf));
    for (auto &name : name_buf)
    {
        std::cout << name << std::endl;
        // 李明
        // 小黑
        // 小美
    }

    helper.close();
    return 0;
}