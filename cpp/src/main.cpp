#include "../include/Library.h"
#include "../include/Console.h"
#include "../include/Menu.h"
#include <bits/stdc++.h>
using namespace std;

Console con;

int main(int argc,char* argv[])
{
    //cout << unitbuf << "=== Program Start ===" << endl;
    BookList bookList;
    ifstream inFile("bookls");
    if (inFile) {
        inFile >> bookList;
        inFile.close();
    }
    HistoryList historyList;  // 名字按你项目里来
    {
        ifstream inHis("historyls");
        if (inHis) {
            inHis >> historyList;
            inHis.close();
        }
    }
    // 处理 Python 发来的指令
    if (argc > 1)
    {
        string cmd = argv[1];
        //展示图书
        if (cmd == "-api_show_books")
        {
            cout << bookList;
        }

        //新增图书 (对应 Python 的 /api/add_book) ---
        else if (cmd == "-api_add_book") {
            if (argc < 8) {
                cout << "Error: 参数不足! ..." << endl;
                return -1; // <--- 改成 -1 (告诉 Python 出错了)
            }

            try {
                // 1. 解析参数
                string newBid = argv[2];
                string newName = argv[3];
                string newAuthor = argv[4];
                string newPress = argv[5];
                int newTotal = atoi(argv[6]); // 只有数量需要转成 int
                string newCat = argv[7];

                // 2. 查重
                if (bookList.bidExist(newBid)) {
                    cout << "Error: 书籍编号 [" << newBid << "] 已存在！" << endl;
                    return -1; // <--- 改成 -1
                }

                // 3. 创建对象
                Book newBook(newBid, newName, newAuthor, newPress, newTotal, newCat);

                // 4. 添加到链表
                bookList.add(newBook);

                // 5. 保存回文件 (关键！)
                ofstream outFile("bookls");
                if (outFile.is_open()) {
                    outFile << bookList;
                    outFile.close();
                    cout << "Success: 书籍 [" << newName << "] 添加成功！" << endl;
                } else {
                    cout << "Error: 无法打开文件进行写入！" << endl;
                    return -1; // <--- 改成 -1
                }

            }catch (exception& e) {
                cout << "Error: C++ 内部错误 - " << e.what() << endl;
                return -1; // <--- 改成 -1
            }
        }
        //删除图书
        else if (cmd == "-api_delete_book") {
            if (argc < 3) {
                cout << "Error: 参数不足! 用法: -api_delete_book <id>\n";
                return -1;
            }
            string bid = argv[2];
            if (!bookList.bidExist(bid)) {
                cout << "Error: 未找到编号为 [" << bid << "] 的书籍\n";
                return -1;
            }

            bookList.del(bid);

            ofstream outFile("bookls");
            if (!outFile) {
                cout << "Error: 无法打开文件进行写入！\n";
                return -1;
            }
            outFile << bookList;
            outFile.close();

            cout << "OK\n";
            return 0;
        }
        //搜索图书
        else if (cmd == "-api_search_books") {
            if (argc < 3) {
                cout << "Error: 参数不足! 用法: -api_search_books <keyword>\n";
                return -1;
            }
            string keyword = argv[2];

            // 不用 STL 容器，直接遍历 BookList
            int total = bookList.size();
            // 我们先把匹配到的书输出到一个临时字符串中，并统计数量
            ostringstream buffer;
            int matchCount = 0;

            for (int i = 0; i < total; ++i) {
                // 你有 Book& getByIndex(const int &index);
                Book& b = bookList.getByIndex(i);

                // 这里的 getter 名字按你实际的改
                string all = b.getBid() + " " +
                             b.getBname() + " " +
                             b.getAuthor() + " " +
                             b.getPress() + " " +
                             b.getCategory();

                if (all.find(keyword) != string::npos) {
                    ++matchCount;
                    buffer << b;  // 用 Book 的 operator<< 输出格式
                }
            }

            // 先输出匹配数量
            cout << matchCount << "\n";
            // 然后输出所有匹配的书
            cout << buffer.str();
            return 0;
        }
        else if (cmd == "-api_show_history") {
            ifstream inHis("historyls");
            if (!inHis) {
                // 没有历史文件就输出 0
                cout << 0 << "\n";
                return 0;
            }
            cout << inHis.rdbuf();
            inHis.close();
            return 0;
        }
        else {
            cout << "Error: 未知命令 [" << cmd << "]\n";
            return -1;
        }
    } else {
        // 如果没有参数，说明是直接双击运行的，可以显示个简易菜单或者提示
        cout << "=== Library System Kernel ===" << endl;
        cout << "Running in Standalone Mode." << endl;
        cout << "Current Books: " << bookList.size() << endl;
         //bookList.show(); // 也可以在这里显示一下
    }

    return 0;
}
