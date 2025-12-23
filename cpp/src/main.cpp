#include "../include/Library.h"
#include "../include/Console.h"
#include "../include/Menu.h"
#include <bits/stdc++.h>
using namespace std;

Console con;

// 统一失败输出：只打印 ASCII 错误码，避免 Python 解码乱码
static int fail(const string &code)
{
    cout << code << "\n";
    return -1;
}

// 统一对外导出状态码，兼容旧数据（根据 rrtime 推断是否已归还）
static string exportStatus(const BH &h)
{
    string s = h.getStatus();
    if (s == "BORROWED" || s == "RETURNED" || s == "REJECTED")
        return s;
    // 旧数据可能是中文或乱码，rrtime 非空视为已归还
    if (!h.getRrtime().empty())
        return "RETURNED";
    return "BORROWED";
}

int main(int argc, char *argv[])
{
    HistoryList historyList;
    ifstream inHis("historyls");
    if (inHis)
    {
        inHis >> historyList;
        inHis.close();
    }

    if (argc > 1)
    {
        string cmd = argv[1];

        // 展示图书（列表）
        if (cmd == "-api_show_books")
        {
            BookList bookList;
            ifstream inFile("bookls");
            if (inFile)
            {
                inFile >> bookList;
                inFile.close();
            }
            cout << bookList.size() << "\n";
            for (int i = 0; i < bookList.size(); ++i)
            {
                Book &b = bookList.getByIndex(i);
                cout << b.getBid() << "\n"
                     << b.getBname() << "\n"
                     << b.getAuthor() << "\n"
                     << b.getPress() << "\n"
                     << b.getTotalNum() << "\n"
                     << b.getBorrowNum() << "\n"
                     << b.getCategory() << "\n";
            }
            return 0;
        }

        // 新增图书
        else if (cmd == "-api_add_book")
        {
            if (argc < 8)
                return fail("ERR_PARAM");

            BookList bookList;
            ifstream inFile("bookls");
            if (inFile)
            {
                inFile >> bookList;
                inFile.close();
            }

            try
            {
                string newBid = argv[2];
                string newName = argv[3];
                string newAuthor = argv[4];
                string newPress = argv[5];
                int newTotal = atoi(argv[6]);
                string newCat = argv[7];

                if (bookList.bidExist(newBid))
                {
                    return fail("ERR_BOOK_ID_EXISTS");
                }

                Book newBook(newBid, newName, newAuthor, newPress, newTotal, newCat);
                bookList.add(newBook);

                ofstream outFile("bookls");
                if (!outFile.is_open())
                    return fail("ERR_BOOKLS_WRITE_FAIL");
                outFile << bookList;
                outFile.close();

                cout << "OK\n";
                return 0;
            }
            catch (exception &e)
            {
                return fail("ERR_CPP_INTERNAL");
            }
        }
        else if (cmd == "-api_update_book") {
            if (argc < 8) return fail("ERR_PARAM");
            string bid     = argv[2];
            string name    = argv[3];
            string author  = argv[4];
            string press   = argv[5];
            int    total   = atoi(argv[6]);
            string cat     = argv[7];
            BookList bookList;
            ifstream inFile("bookls");
            if (inFile) { inFile >> bookList; inFile.close(); }
            if (!bookList.bidExist(bid)) return fail("ERR_BOOK_NOT_FOUND");
            Book &b = bookList.getByBid(bid);
            int borrowed = b.getBorrowNum();
            if (total < borrowed) return fail("ERR_TOTAL_LT_BORROWED");
            // 假设有这些 setter，如无请告诉我类定义
            b.setBname(name);
            b.setAuthor(author);
            b.setPress(press);
            b.setTotalNum(total);
            b.setCategory(cat);
            ofstream outFile("bookls");
            if (!outFile) return fail("ERR_BOOKLS_WRITE_FAIL");
            outFile << bookList;
            outFile.close();
            cout << "OK\n";
            return 0;
        }
        // 新增读者
        else if (cmd == "-api_add_reader")
        {
            if (argc < 4)
                return fail("ERR_PARAM");

            string rid = argv[2];
            string rname = argv[3];

            ReaderList readerList;
            ifstream inReader("readerls");
            if (inReader)
            {
                inReader >> readerList;
                inReader.close();
            }

            if (readerList.ridExist(rid))
                return fail("ERR_READER_EXISTS");

            Reader newR(rid, rname, "123456");
            readerList.add(newR);

            ofstream outReader("readerls");
            if (!outReader)
                return fail("ERR_READERLS_WRITE_FAIL");
            outReader << readerList;
            outReader.close();

            cout << "OK\n";
            return 0;
        }

        // 归还图书
        else if (cmd == "-api_return_book")
        {
            if (argc < 4)
                return fail("ERR_PARAM");

            string rid = argv[2];
            string bid = argv[3];

            // 读 readerls
            ReaderList readerList;
            ifstream inReader("readerls");
            if (inReader)
            {
                inReader >> readerList;
                inReader.close();
            }
            if (!readerList.ridExist(rid))
                return fail("ERR_READER_NOT_FOUND");

            // 读 bookls
            BookList bookList;
            ifstream inBook("bookls");
            if (inBook)
            {
                inBook >> bookList;
                inBook.close();
            }
            if (!bookList.bidExist(bid))
                return fail("ERR_BOOK_NOT_FOUND");

            // 读 historyls
            HistoryList historyList;
            ifstream inHis2("historyls");
            if (inHis2)
            {
                inHis2 >> historyList;
                inHis2.close();
            }

            time_t now = time(nullptr);
            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d_%H:%M:%S", localtime(&now));
            string rrtime = timebuf;

            Reader &r = readerList.getByRid(rid);
            HL &his = r.getBorrowHis();
            bool foundReaderRec = false;
            for (int i = 0; i < his.size(); i++)
            {
                BH &record = his.getByIndex(i);
                // 兼容旧值：只要不是 RETURNED 就当作可归还
                if (record.getBid() == bid && record.getStatus() != "RETURNED")
                {
                    record.setStatus("RETURNED");
                    record.setRrtime(rrtime);
                    foundReaderRec = true;
                    break;
                }
            }
            if (!foundReaderRec)
                return fail("ERR_NOT_BORROWED_OR_ALREADY_RETURNED");

            // 库存 -1
            Book &b = bookList.getByBid(bid);
            if (b.getBorrowNum() > 0)
                b.setBorrowNum(b.getBorrowNum() - 1);

            // 全馆历史更新
            bool foundGlobalRec = false;
            for (int i = historyList.size() - 1; i >= 0; --i)
            {
                BH &rec = historyList.getByIndex(i);
                if (rec.getRid() == rid && rec.getBid() == bid && rec.getStatus() != "RETURNED")
                {
                    rec.setStatus("RETURNED");
                    rec.setRrtime(rrtime);
                    foundGlobalRec = true;
                    break;
                }
            }
            // 即使没找到，也继续写文件，保证 readerls/bookls 已更新

            ofstream outReader("readerls");
            if (!outReader)
                return fail("ERR_READERLS_WRITE_FAIL");
            outReader << readerList;
            outReader.close();

            ofstream outBook2("bookls");
            if (!outBook2)
                return fail("ERR_BOOKLS_WRITE_FAIL");
            outBook2 << bookList;
            outBook2.close();

            ofstream outHis3("historyls");
            if (!outHis3)
                return fail("ERR_HISTORYLS_WRITE_FAIL");
            outHis3 << historyList;
            outHis3.close();

            cout << "OK\n";
            return 0;
        }

        // 删除图书
         else if (cmd == "-api_delete_book")
        {
            if (argc < 3)
                return fail("ERR_PARAM");

            string bid = argv[2];

            BookList bookList;
            ifstream inFile("bookls");
            if (inFile)
            {
                inFile >> bookList;
                inFile.close();
            }

            if (!bookList.bidExist(bid))
                return fail("ERR_BOOK_NOT_FOUND");
            Book &b = bookList.getByBid(bid);
            if (b.getBorrowNum() > 0)
                return fail("ERR_BOOK_IN_USE");
            bookList.del(bid);

            ofstream outFile("bookls");
            if (!outFile)
                return fail("ERR_BOOKLS_WRITE_FAIL");
            outFile << bookList;
            outFile.close();

            cout << "OK\n";
            return 0;
        }

        // 搜索图书
        else if (cmd == "-api_search_books")
        {
            if (argc < 3)
                return fail("ERR_PARAM");

            string keyword = argv[2];
            int matchCount = 0;

            BookList bookList;
            ifstream inFile("bookls");
            if (inFile)
            {
                inFile >> bookList;
                inFile.close();
            }

            for (int i = 0; i < bookList.size(); ++i)
            {
                Book &b = bookList.getByIndex(i);
                string all = b.getBid() + " " + b.getBname() + " " + b.getAuthor() + " " + b.getPress() + " " + b.getCategory();
                if (all.find(keyword) != string::npos)
                    matchCount++;
            }

            cout << matchCount << "\n";
            for (int i = 0; i < bookList.size(); ++i)
            {
                Book &b = bookList.getByIndex(i);
                string all = b.getBid() + " " + b.getBname() + " " + b.getAuthor() + " " + b.getPress() + " " + b.getCategory();
                if (all.find(keyword) != string::npos)
                {
                    cout << b.getBid() << "\n"
                         << b.getBname() << "\n"
                         << b.getAuthor() << "\n"
                         << b.getPress() << "\n"
                         << b.getTotalNum() << "\n"
                         << b.getBorrowNum() << "\n"
                         << b.getCategory() << "\n";
                }
            }
            return 0;
        }

        // 展示全馆借阅历史
        else if (cmd == "-api_show_history")
        {
            HistoryList historyList;
            ifstream inFile("historyls");
            if (inFile)
            {
                inFile >> historyList;
                inFile.close();
            }
            cout << historyList.size() << "\n";
            for (int i = 0; i < historyList.size(); ++i)
            {
                BH &one = historyList.getByIndex(i);
                cout << one.getHid() << "\n"
                     << one.getBid() << "\n"
                     << one.getRid() << "\n"
                     << one.getBname() << "\n"
                     << one.getRname() << "\n"
                     << one.getNote() << "\n"
                     << one.getBrtime() << "\n"
                     << one.getBtime() << "\n"
                     << one.getRrtime() << "\n"
                     << one.getRtime() << "\n"
                     << exportStatus(one) << "\n"; // 用 ASCII 状态码
            }
            return 0;
        }
        else if (cmd == "-api_show_readers")
        {
            ReaderList readerList;
            ifstream inReader("readerls");
            if (inReader) { inReader >> readerList; inReader.close(); }

            cout << readerList.size() << "\n";
            for (int i = 0; i < readerList.size(); ++i)
            {
                Reader &r = readerList.getByIndex(i);
                HL &his = r.getBorrowHis();
                int active = 0;
                for (int j = 0; j < his.size(); ++j) {
                    const BH &rec = his.getByIndex(j);
                    if (exportStatus(rec) == "BORROWED") active++;
                }
                cout << r.getRid() << "\n"
                << r.getRname() << "\n"
                << active << "\n";
            }
            return 0;
        }
        else if (cmd == "-api_book_history")
        {
            if (argc < 3) return fail("ERR_PARAM");
            string bid = argv[2];
            HistoryList historyList;
            ifstream inFile("historyls");
            if (inFile) { inFile >> historyList; inFile.close(); }
            // 第一遍统计
            int cnt = 0;
            for (int i = 0; i < historyList.size(); ++i) {
                BH &one = historyList.getByIndex(i);
                if (one.getBid() == bid) cnt++;
            }
            // 必须先输出数量（供 Python 解析）
            cout << cnt << "\n";
            // 第二遍输出明细
            for (int i = 0; i < historyList.size(); ++i) {
                BH &one = historyList.getByIndex(i);
                if (one.getBid() != bid) continue;
                cout << one.getHid()   << "\n"
                     << one.getBid()   << "\n"
                     << one.getRid()   << "\n"
                     << one.getBname() << "\n"
                     << one.getRname() << "\n"
                     << one.getNote()  << "\n"
                     << one.getBrtime()<< "\n"
                     << one.getBtime() << "\n"
                     << one.getRrtime()<< "\n"
                     << one.getRtime() << "\n"
                     << exportStatus(one) << "\n";
            }
            return 0;
        }
        // 借书
        else if (cmd == "-api_reader_borrow_req")
        {

            if (argc < 4)
                return fail("ERR_PARAM");

            string rid = argv[2];
            string bid = argv[3];

            ReaderList readerList;
            ifstream inReader("readerls");
            if (inReader)
            {
                inReader >> readerList;
                inReader.close();
            }

            BookList bookList;
            ifstream inFile("bookls");
            if (inFile)
            {
                inFile >> bookList;
                inFile.close();
            }

            if (!readerList.ridExist(rid))
                return fail("ERR_READER_NOT_FOUND");
            if (!bookList.bidExist(bid))
                return fail("ERR_BOOK_NOT_FOUND");

            Reader &r = readerList.getByRid(rid);
            Book &b = bookList.getByBid(bid);
            const int MAX_BORROW = 5; // 每人最多同时在借 N 本
            int active = 0;
            HL &rh = r.getBorrowHis();
            for (int i = 0; i < rh.size(); ++i) {
                const BH &rec = rh.getByIndex(i);
                if (exportStatus(rec) == "BORROWED") active++;
            }
            if (active >= MAX_BORROW) return fail("ERR_REACH_LIMIT");
            if (b.getBorrowNum() >= b.getTotalNum())
                return fail("ERR_NO_STOCK");

            b.setBorrowNum(b.getBorrowNum() + 1);

            string bname = b.getBname();
            string rname = r.getRname();

            time_t now = time(nullptr);
            stringstream ssHid;
            ssHid << "H" << rid << "" << bid << "" << now;
            string hid = ssHid.str();

            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d_%H:%M:%S", localtime(&now));
            string nowStr = timebuf;

            // 统一 note/status 使用英文码
            BH bh(hid, bid, rid, bname, rname, "BORROW_REQ", nowStr, "", "", "");
            bh.setStatus("BORROWED");

            r.agrBorrow(bh);
            historyList.add(bh);

            ofstream outReader("readerls");
            if (!outReader)
                return fail("ERR_READERLS_WRITE_FAIL");
            outReader << readerList;
            outReader.close();

            ofstream outHis("historyls");
            if (!outHis)
                return fail("ERR_HISTORYLS_WRITE_FAIL");
            outHis << historyList;
            outHis.close();

            ofstream outBook("bookls");
            if (!outBook)
                return fail("ERR_BOOKLS_WRITE_FAIL");
            outBook << bookList;
            outBook.close();

            cout << "OK\n";
            return 0;
        }

        // 读者历史
        else if (cmd == "-api_reader_history")
        {
            if (argc < 3)
                return fail("ERR_PARAM");
            string rid = argv[2];

            ReaderList readerList;
            ifstream inReader("readerls");
            if (inReader)
            {
                inReader >> readerList;
                inReader.close();
            }

            if (!readerList.ridExist(rid))
                return fail("ERR_READER_NOT_FOUND");

            Reader &r = readerList.getByRid(rid);
            HL &bhis = r.getBorrowHis();

            int n = bhis.size();
            cout << n << "\n";
            for (int i = 0; i < n; ++i)
            {
                const BH &one = bhis.getByIndex(i);
                cout << one.getHid() << "\n";
                cout << one.getBid() << "\n";
                cout << one.getRid() << "\n";
                cout << one.getBname() << "\n";
                cout << one.getRname() << "\n";
                cout << one.getNote() << "\n";
                cout << one.getBrtime() << "\n";
                cout << one.getBtime() << "\n";
                cout << one.getRrtime() << "\n";
                cout << one.getRtime() << "\n";
                cout << exportStatus(one) << "\n"; // 用 ASCII 状态码
            }
            return 0;
        }

        else
        {
            return fail("ERR_UNKNOWN_CMD");
        }
    }
    else
    {
        // 无参运行时，打印一些信息（纯调试，ASCII 字符）
        cout << "=== Library System Kernel ===\n";
        cout << "Running in Standalone Mode.\n";
        BookList bookList;
        ifstream inFile("bookls");
        if (inFile)
        {
            inFile >> bookList;
            inFile.close();
        }
        cout << "Current Books: " << bookList.size() << "\n";
    }

    return 0;
}