//// test_borrow_full.cpp
//#include <iostream>
//#include "../include/Book.h"
//#include "../include/Reader.h"
//#include "../include/History.h"
//#include "../include/Console.h"
//#include "../include/Library.h"
//#include <filesystem>
//#include <cassert>
//
//using namespace std;
//
////// 定义全局Console对象（确保在main.cpp中也有）
//extern Console con;
//
//// 创建测试数据目录
//void setupTestEnvironment() {
//    cout << "\n=== 设置测试环境 ===" << endl;
//
//    // 设置data目录到当前目录
//    filesystem::path currentDir = filesystem::current_path();
//    string dataPath = currentDir.string() + "/test_data";
//    con.setDataDir(dataPath);
//
//    cout << "测试数据目录: " << con.getDataDir() << endl;
//
//    // 确保目录存在
//    try {
//        filesystem::create_directories(dataPath + "/reader");
//        filesystem::create_directories(dataPath + "/book");
//        filesystem::create_directories(dataPath + "/history");
//        cout << "✓ 测试目录创建成功" << endl;
//    } catch (const exception& e) {
//        cerr << "✗ 创建目录失败: " << e.what() << endl;
//        throw;
//    }
//}
//
//// 清理测试数据
//void cleanupTestEnvironment() {
//    cout << "\n=== 清理测试环境 ===" << endl;
//
//    string dataPath = con.getDataDir();
//
//    try {
//        if (filesystem::exists(dataPath)) {
//            filesystem::remove_all(dataPath);
//            cout << "✓ 测试数据清理完成" << endl;
//        }
//    } catch (const exception& e) {
//        cerr << "✗ 清理失败: " << e.what() << endl;
//    }
//}
//
//// 测试文件读写
//void testFileOperations() {
//    cout << "\n=== 测试文件读写 ===" << endl;
//
//    try {
//        // 1. 测试写入和读取Book对象
//        cout << "1. 测试Book文件操作..." << endl;
//        {
//            Book bookToSave("FB001", "文件测试书", "文件作者", "文件出版社", 7, 2);
//            string bookPath = con.bdir("FB001") + "FB001";
//
//            // 写入文件
//            con.outf(bookPath, bookToSave);
//            cout << "   ✓ Book写入成功: " << bookPath << endl;
//
//            // 读取文件
//            Book bookToLoad;
//            con.inf(bookPath, bookToLoad);
//            cout << "   ✓ Book读取成功: " << bookToLoad.getBname() << endl;
//
//            // 验证数据
//            assert(bookToLoad.getBid() == "FB001");
//            assert(bookToLoad.getBname() == "文件测试书");
//            cout << "   ✓ Book数据验证通过" << endl;
//        }
//
//        // 2. 测试写入和读取Reader对象
//        cout << "\n2. 测试Reader文件操作..." << endl;
//        {
//            Reader readerToSave("FR001", "文件测试读者", "filepass", 3);
//            string readerPath = con.rdir("FR001") + "FR001";
//
//            // 写入文件
//            con.outf(readerPath, readerToSave);
//            cout << "   ✓ Reader写入成功: " << readerPath << endl;
//
//            // 读取文件
//            Reader readerToLoad;
//            con.inf(readerPath, readerToLoad);
//            cout << "   ✓ Reader读取成功: " << readerToLoad.getRname() << endl;
//
//            // 验证数据
//            assert(readerToLoad.getRid() == "FR001");
//            assert(readerToLoad.getRname() == "文件测试读者");
//            cout << "   ✓ Reader数据验证通过" << endl;
//        }
//
//        // 3. 测试写入和读取History对象
//        cout << "\n3. 测试History文件操作..." << endl;
//        {
//            History historyToSave("FH001", "FB001", "FR001", "文件测试书", "文件测试读者");
//            string historyPath = con.hdir("FH001") + "FH001";
//
//            // 写入文件
//            con.outf(historyPath, historyToSave);
//            cout << "   ✓ History写入成功: " << historyPath << endl;
//
//            // 读取文件
//            History historyToLoad;
//            con.inf(historyPath, historyToLoad);
//            cout << "   ✓ History读取成功" << endl;
//
//            // 验证数据
//            assert(historyToLoad.getHid() == "FH001");
//            assert(historyToLoad.getBid() == "FB001");
//            cout << "   ✓ History数据验证通过" << endl;
//        }
//
//        cout << "=== 文件读写测试完成 ===" << endl;
//
//    } catch (const exception& e) {
//        cerr << "文件操作测试失败: " << e.what() << endl;
//        throw;
//    }
//}
//
//// 测试完整的借还书流程（包含文件操作）
//void testFullBorrowReturnWorkflow() {
//    cout << "\n=== 测试完整借还书流程 ===" << endl;
//
//    try {
//        // 1. 创建测试图书并保存到文件
//        cout << "1. 创建测试图书..." << endl;
//        Book testBook("TB001", "借还测试书", "测试作者", "测试出版社", 3, 0);
//        string bookPath = con.bdir("TB001") + "TB001";
//        con.outf(bookPath, testBook);
//        cout << "   ✓ 测试图书保存: " << testBook.getBname() << endl;
//
//        // 2. 创建测试读者并保存到文件
//        cout << "2. 创建测试读者..." << endl;
//        Reader testReader("TR001", "借还测试读者", "testpass", 0);
//        string readerPath = con.rdir("TR001") + "TR001";
//        con.outf(readerPath, testReader);
//        cout << "   ✓ 测试读者保存: " << testReader.getRname() << endl;
//
//        // 3. 加载图书和读者数据
//        cout << "3. 加载数据..." << endl;
//        Book loadedBook;
//        Reader loadedReader;
//        con.inf(bookPath, loadedBook);
//        con.inf(readerPath, loadedReader);
//        cout << "   ✓ 数据加载完成" << endl;
//
//        // 4. 测试借书请求
//        cout << "4. 测试借书请求..." << endl;
//        {
//            // 创建借书记录
//            BaseHistory borrowHistory("BH001", "TB001", "TR001", "借还测试书", "借还测试读者");
//
//            // 图书借书请求
//            loadedBook.reqBorrow(borrowHistory);
//            cout << "   ✓ 图书借书请求成功" << endl;
//
//            // 保存更新后的图书
//            con.outf(bookPath, loadedBook);
//
//            // 读者借书请求
//            loadedReader.reqBorrow(borrowHistory);
//            cout << "   ✓ 读者借书请求成功" << endl;
//
//            // 保存更新后的读者
//            con.outf(readerPath, loadedReader);
//
//            // 显示借书请求
//            int borrowReqCount = loadedReader.showBorrowReq();
//            cout << "   当前借书请求数量: " << borrowReqCount << endl;
//
//            if (borrowReqCount > 0) {
//                string hid = loadedReader.showBorrowReq(0);
//                cout << "   借书记录ID: " << hid << endl;
//            }
//        }
//
//        // 5. 测试还书请求
//        cout << "\n5. 测试还书请求..." << endl;
//        {
//            // 重新加载数据
//            con.inf(bookPath, loadedBook);
//            con.inf(readerPath, loadedReader);
//
//            // 创建还书记录
//            BaseHistory returnHistory("RH001", "TB001", "TR001", "借还测试书", "借还测试读者");
//
//            // 图书还书请求
//            loadedBook.reqReturn(returnHistory);
//            cout << "   ✓ 图书还书请求成功" << endl;
//
//            // 保存更新后的图书
//            con.outf(bookPath, loadedBook);
//
//            // 读者还书请求
//            loadedReader.reqReturn(returnHistory);
//            cout << "   ✓ 读者还书请求成功" << endl;
//
//            // 保存更新后的读者
//            con.outf(readerPath, loadedReader);
//
//            // 显示还书请求
//            int returnReqCount = loadedReader.showReturnReq();
//            cout << "   当前还书请求数量: " << returnReqCount << endl;
//        }
//
//        // 6. 测试借阅历史和归还历史
//        cout << "\n6. 测试历史记录..." << endl;
//        {
//            // 重新加载数据
//            con.inf(readerPath, loadedReader);
//
//            int borrowHisCount = loadedReader.showBorrowHis();
//            int returnHisCount = loadedReader.showReturnHis();
//
//            cout << "   借阅历史数量: " << borrowHisCount << endl;
//            cout << "   归还历史数量: " << returnHisCount << endl;
//        }
//
//        // 7. 测试修改密码
//        cout << "\n7. 测试修改密码..." << endl;
//        {
//            // 重新加载数据
//            con.inf(readerPath, loadedReader);
//
//            // 修改密码
//            loadedReader.changeRpasswd("newpassword123");
//
//            // 保存
//            con.outf(readerPath, loadedReader);
//
//            // 验证新密码
//            bool passCorrect = loadedReader.rpasswdRight("newpassword123");
//            bool passWrong = loadedReader.rpasswdRight("wrongpass");
//
//            cout << "   ✓ 密码修改成功" << endl;
//            cout << "   新密码验证: " << (passCorrect ? "通过" : "失败") << endl;
//            cout << "   错误密码验证: " << (passWrong ? "通过" : "失败") << endl;
//        }
//
//        cout << "\n=== 完整借还书流程测试完成 ===" << endl;
//
//    } catch (const exception& e) {
//        cerr << "完整流程测试失败: " << e.what() << endl;
//        throw;
//    }
//}
//
//// 测试Library系统的借还书功能
//void testLibrarySystemBorrow() {
//    cout << "\n=== 测试Library系统借还书 ===" << endl;
//
//    try {
//        // 注意：这里需要Library::ReaderOperation类的完整实现
//        // 由于代码复杂，这里只提供测试框架
//
//        cout << "Library系统借还书测试需要:" << endl;
//        cout << "1. Library::ReaderOperation类的实例化" << endl;
//        cout << "2. Library::Data的初始化" << endl;
//        cout << "3. 图书和读者数据的加载" << endl;
//        cout << "4. 历史记录系统的支持" << endl;
//
//        // 以下是一个框架示例
//        /*
//        // 初始化Library数据
//        Library::Data libData;
//
//        // 添加测试图书
//        Book libBook("LIB001", "Library测试书", "作者", "出版社", 5, 0);
//        string libBookPath = con.bdir("LIB001") + "LIB001";
//        con.outf(libBookPath, libBook);
//
//        // 添加测试读者
//        Reader libReader("LIBR001", "Library测试读者", "libpass", 0);
//        string libReaderPath = con.rdir("LIBR001") + "LIBR001";
//        con.outf(libReaderPath, libReader);
//
//        // 创建ReaderOperation对象
//        Library::ReaderOperation lrOp;
//        lrOp.setReader("LIBR001");
//
//        // 测试借书
//        lrOp.reqBorrow("LIB001");
//
//        // 测试还书
//        // 需要先获取历史记录ID
//        // lrOp.reqReturn("历史记录ID");
//        */
//
//        cout << "\n=== Library系统测试框架完成 ===" << endl;
//
//    } catch (const exception& e) {
//        cerr << "Library系统测试失败: " << e.what() << endl;
//    }
//}
//Console con;
//// 主测试函数
//int main() {
//    cout << "=== 图书管理系统完整借还书测试 ===" << endl;
//
//    try {
//        // 设置测试环境
//        setupTestEnvironment();
//
//        // 运行文件操作测试
//        testFileOperations();
//
//        // 运行完整借还书流程测试
//        testFullBorrowReturnWorkflow();
//
//        // 运行Library系统测试
//        testLibrarySystemBorrow();
//
//        // 清理测试环境
//        cleanupTestEnvironment();
//
//        cout << "\n=== 所有测试成功完成 ===" << endl;
//        cout << "测试覆盖:" << endl;
//        cout << "1. ✓ 文件读写操作" << endl;
//        cout << "2. ✓ 完整借还书流程" << endl;
//        cout << "3. ✓ Library系统框架" << endl;
//
//    } catch (const exception& e) {
//        cerr << "\n!!! 主测试程序发生错误: " << e.what() << endl;
//
//        // 尝试清理
//        try {
//            cleanupTestEnvironment();
//        } catch (...) {
//            // 忽略清理错误
//        }
//
//        return 1;
//    }
//
//    return 0;
//}
//using namespace std;
//
//Console con;
//void initData() {
//    cout << "正在初始化数据..." << endl;
//
//    // 创建必要的目录
//    con.mkdir("data");
//    con.mkdir("data/book");
//    con.mkdir("data/reader");
//    con.mkdir("data/history");
//
//    // 1. 初始化图书列表文件
//    {
//        ofstream bookls("data/bookls");
//        if (!bookls) {
//            cerr << "无法创建 data/bookls" << endl;
//            return;
//        }
//        bookls << "0\n";  // 初始时没有图书
//        bookls.close();
//        cout << "✓ 创建图书列表文件" << endl;
//    }
//
//    // 2. 初始化读者列表文件
//    {
//        ofstream readerls("data/readerls");
//        if (!readerls) {
//            cerr << "无法创建 data/readerls" << endl;
//            return;
//        }
//        readerls << "0\n";  // 初始时没有读者
//        readerls.close();
//        cout << "✓ 创建读者列表文件" << endl;
//    }
//
//    // 3. 初始化历史记录列表文件
//    {
//        ofstream historyls("data/historyls");
//        if (!historyls) {
//            cerr << "无法创建 data/historyls" << endl;
//            return;
//        }
//        historyls << "0\n";  // 初始时没有历史记录
//        historyls.close();
//        cout << "✓ 创建历史记录列表文件" << endl;
//    }
//
//    // 4. 初始化借阅请求文件
//    {
//        ofstream borrowReq("data/borrowReq");
//        if (!borrowReq) {
//            cerr << "无法创建 data/borrowReq" << endl;
//            return;
//        }
//        borrowReq << "0\n";
//        borrowReq.close();
//        cout << "✓ 创建借阅请求文件" << endl;
//    }
//
//    // 5. 初始化归还请求文件
//    {
//        ofstream returnReq("data/returnReq");
//        if (!returnReq) {
//            cerr << "无法创建 data/returnReq" << endl;
//            return;
//        }
//        returnReq << "0\n";
//        returnReq.close();
//        cout << "✓ 创建归还请求文件" << endl;
//    }
//
//    // 6. 初始化管理员密码文件
//    {
//        ofstream managerPasswd("data/managerPasswd");
//        if (!managerPasswd) {
//            cerr << "无法创建 data/managerPasswd" << endl;
//            return;
//        }
//        managerPasswd << "admin\n";  // 默认密码
//        managerPasswd.close();
//        cout << "✓ 创建管理员密码文件 (默认密码: admin)" << endl;
//    }
//
//    // 7. 创建一些示例图书目录结构
//    con.mkdir("data/book/B0/00");
//    con.mkdir("data/book/B0/01");
//    con.mkdir("data/book/B1/00");
//
//    // 8. 创建一些示例读者目录结构
//    con.mkdir("data/reader/R0/00");
//    con.mkdir("data/reader/R0/01");
//    con.mkdir("data/reader/R1/00");
//
//    // 9. 创建一些示例历史记录目录结构
//    con.mkdir("data/history/00/00");
//    con.mkdir("data/history/00/01");
//
//    cout << "\n✅ 数据初始化完成！" << endl;
//    cout << "现在可以运行主程序了。" << endl;
//}

//int main() {
//    initData();
//    return 0;
//}
// 测试函数
//void testLibraryManager() {
//    cout << "=== 测试图书馆管理系统 ===" << endl;
//
//    // 创建图书馆实例
//    Library library;
//
//    try {
//        // 测试1: 添加图书
//        cout << "\n1. 测试添加图书..." << endl;
//        Book book1("B001", "C++ Primer", "Stanley Lippman", "Addison-Wesley", 10, 0);
//        library.mop.addBook(book1);
//        cout << "   添加图书成功: " << book1.getBname() << endl;
//
//        Book book2("B002", "Effective C++", "Scott Meyers", "Addison-Wesley", 5, 0);
//        library.mop.addBook(book2);
//        cout << "   添加图书成功: " << book2.getBname() << endl;
//
//        // 测试2: 增加库存
//        cout << "\n2. 测试增加库存..." << endl;
//        library.mop.addBook("B001", 5);
//        cout << "   增加库存成功: B001 +5" << endl;
//
//        // 测试3: 检查是否可以删除
//        cout << "\n3. 测试检查是否可以删除..." << endl;
//        bool canDelB1 = library.mop.canDelb("B001");
//        cout << "   图书B001是否可以删除: " << (canDelB1 ? "是" : "否") << endl;
//
//        bool canDelB2 = library.mop.canDelb(1);
//        cout << "   第二本图书是否可以删除: " << (canDelB2 ? "是" : "否") << endl;
//
//        // 测试4: 添加读者
//        cout << "\n4. 测试添加读者..." << endl;
//        Reader reader1("R001", "张三", "123456", 0);
//        library.mop.addReader(reader1);
//        cout << "   添加读者成功: " << reader1.getRname() << endl;
//
//        Reader reader2("R002", "李四", "654321", 0);
//        library.mop.addReader(reader2);
//        cout << "   添加读者成功: " << reader2.getRname() << endl;
//
//        // 测试5: 显示图书
//        cout << "\n5. 测试显示所有图书..." << endl;
//        int bookCount = library.op.showBook();
//        cout << "   图书总数: " << bookCount << endl;
//
//        // 测试6: 显示读者
//        cout << "\n6. 测试显示所有读者..." << endl;
//        int readerCount = library.op.showReader();
//        cout << "   读者总数: " << readerCount << endl;
//
////        // 测试7: 搜索图书
////        cout << "\n7. 测试搜索图书..." << endl;
////        library.mop.sch("C++");
////        cout << "   搜索'C++'完成" << endl;
//
//        // 测试8: 搜索读者
//        cout << "\n8. 测试搜索读者..." << endl;
//        library.mop.schReader("张");
//        cout << "   搜索'张'完成" << endl;
//
//        // 测试9: 删除图书（如果可删除）
//        cout << "\n9. 测试删除图书..." << endl;
//        if (canDelB2) {
//            library.mop.delBook("B002");
//            cout << "   删除图书B002成功" << endl;
//        } else {
//            cout << "   图书B002不可删除（可能有借阅记录）" << endl;
//        }
//
//        // 测试10: 显示请求列表（应为空，因为没有借阅请求）
//        cout << "\n10. 测试显示借阅请求..." << endl;
//        int borrowReqCount = library.mop.showBorrowReq();
//        cout << "   借阅请求数量: " << borrowReqCount << endl;
//
//        // 测试异常处理
//        cout << "\n11. 测试异常处理..." << endl;
//        try {
//            library.mop.addBook("B999", 10);  // 不存在的图书
//            cout << "   ❌ 未抛出异常 - 测试失败" << endl;
//        } catch (const runtime_error& e) {
//            cout << "   ✅ 异常捕获成功: " << e.what() << endl;
//        }
//
//        // 测试12: 密码管理
//        cout << "\n12. 测试密码管理..." << endl;
//        library.mop.changeMpasswd("admin123");
//        bool passwordCorrect = library.mop.mpasswdRight("admin123");
//        cout << "   密码验证结果: " << (passwordCorrect ? "正确" : "错误") << endl;
//
//        cout << "\n=== 所有测试完成 ===" << endl;
//
//    } catch (const exception& e) {
//        cerr << "\n❌ 测试过程中发生错误: " << e.what() << endl;
//    }
//}
//
//// 测试借阅流程
//void testBorrowProcess() {
//    cout << "\n=== 测试借阅流程 ===" << endl;
//
//    Library library;
//
//    try {
//        // 准备数据
//        Book book("B100", "Test Book", "Test Author", "Test Press", 3, 0);
//        library.mop.addBook(book);
//
//        Reader reader("R100", "Test Reader", "password", 0);
//        library.mop.addReader(reader);
//
//        cout << "1. 图书和读者已添加" << endl;
//
//        // 显示当前状态
//        cout << "2. 当前图书列表:" << endl;
//        library.op.showBook();
//
//        cout << "\n3. 当前读者列表:" << endl;
//        library.op.showReader();
//
//        // 检查是否可以借阅
//        cout << "\n4. 图书可借阅数量: " << endl;
//        library.op.showBook("B100");
//
//    } catch (const exception& e) {
//        cerr << "❌ 测试过程中发生错误: " << e.what() << endl;
//    }
//}
//
//int main() {
//    cout << "开始测试图书馆管理系统..." << endl;
//
//    // 设置数据目录
//    con.setDataDir("test_data");
//
//    // 运行测试
//    testLibraryManager();
//
//    // 测试借阅流程
//    testBorrowProcess();
//
//    cout << "\n测试完成！" << endl;
//    cout << "请检查 test_data 目录下的文件是否正确生成" << endl;
//
//    return 0;
//}