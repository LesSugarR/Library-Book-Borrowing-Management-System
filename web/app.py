from flask import Flask, render_template, request, jsonify, Response
import subprocess, os, time, random, io, csv

app = Flask(__name__)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
EXE_PATH = os.path.join(BASE_DIR, 'main.exe')
def gen_book_id():
    return f"{int(time.time()*1000)}{random.randint(100, 999)}"
def gen_reader_id():
    return f"{random.randint(100000, 999999)}"
def run_cpp_command(args):
    if not os.path.exists(EXE_PATH):
        return {"success": False, "message": "找不到 main.exe"}

    try:
        result = subprocess.run(
            [EXE_PATH] + args,
            capture_output=True,
            text=True,
            encoding='gbk',          # Windows 中文控制台常用编码
            errors='ignore',
            cwd=BASE_DIR,
            timeout=10
        )
    except subprocess.TimeoutExpired:
        return {"success": False, "message": "C++ 执行超时（可能卡在文件读写）"}

    if result.returncode != 0:
        return {"success": False, "message": (result.stdout + result.stderr).strip()}

    return {"success": True, "output": result.stdout, "stderr": result.stderr}

def ok_or_err(res):
    """
    统一判断：C++ 成功时 stdout 输出 'OK'
    返回 (ok: bool, msg: str)
    ok=True  -> msg 固定 'OK'
    ok=False -> msg 为错误码/提示
    """
    if not res.get('success'):
        return False, res.get('message') or 'C++ 执行失败'
    out = (res.get('output') or '').strip()
    if out == 'OK':
        return True, 'OK'
    return False, out or '未知错误'

def parse_books_output(raw_text):
    lines = [line.strip() for line in raw_text.splitlines() if line.strip()]
    if not lines or not lines[0].isdigit():
        return []
    idx = 1
    n = int(lines[0])
    books = []
    for _ in range(n):
        if idx + 6 >= len(lines):
            break
        bid, name, author, press, total, borrowed, cat = lines[idx:idx+7]
        idx += 7
        books.append({
            "id": bid,
            "name": name,
            "author": author,
            "press": press,
            "total": total,
            "borrowed": borrowed,
            "category": cat,
            "status": "可借" if int(total) > int(borrowed) else "已借完"
        })
    return books

def parse_history_output(raw_text):
    if raw_text is None:
        return []
    lines = [line.rstrip('\r') for line in raw_text.splitlines()]
    if not lines:
        return []
    if not lines[0].isdigit():
        return []
    total = int(lines[0].strip())
    res = []
    i = 1
    for _ in range(total):
        if i + 10 >= len(lines):
            break
        res.append({
            "hid": lines[i].strip(),
            "bid": lines[i+1].strip(),
            "rid": lines[i+2].strip(),
            "bname": lines[i+3].strip(),
            "rname": lines[i+4].strip(),
            "note": lines[i+5].strip(),
            "brtime": lines[i+6].strip(),
            "btime": lines[i+7].strip(),
            "rrtime": lines[i+8].strip(),
            "rtime": lines[i+9].strip(),
            "status": lines[i+10].strip(),
        })
        i += 11
    return res
#读者列表解析
def parse_readers_output(raw_text):
    lines = [line.strip() for line in (raw_text or '').splitlines() if line.strip() != '']
    if not lines or not lines[0].isdigit():
        return []
    n = int(lines[0])
    i = 1
    out = []
    for _ in range(n):
        if i + 2 >= len(lines):
            break
        rid = lines[i]; rname = lines[i+1]; active = lines[i+2]
        i += 3
        try:
            active = int(active)
        except:
            pass
        out.append({"rid": rid, "rname": rname, "active": active})
    return out
@app.get('/api/readers')
def api_readers():
    res = run_cpp_command(['-api_show_readers'])
    if not res['success']:
        return jsonify({"status": "error", "message": res['message']})
    data = parse_readers_output(res['output'])
    return jsonify({"status": "success", "data": data})
@app.get('/api/book/history')
def api_book_history():
    bid = request.args.get('id', '').strip()
    if not bid:
        return jsonify({"status": "error", "message": "缺少图书ID"})
    res = run_cpp_command(['-api_book_history', bid])
    if not res['success']:
        return jsonify({"status": "error", "message": res['message']})
    data = parse_history_output(res['output'])
    return jsonify({"status": "success", "data": data})
@app.route('/')
def index():
    return render_template('index.html')

# 获取图书列表
@app.get('/api/get_books')
def api_get_books():
    result = run_cpp_command(['-api_show_books'])
    if not result['success']:
        return jsonify({"status": "error", "message": result['message']})
    book_list = parse_books_output(result['output'])
    return jsonify({"status": "success", "data": book_list})

# 修改图书
@app.post('/api/update_book')
def api_update_book():
    data = request.get_json(force=True)
    req = ['-api_update_book',
           data.get('id',''),
           data.get('name',''),
           data.get('author',''),
           data.get('press',''),
           str(data.get('total',0)),
           data.get('category','')]
    res = run_cpp_command(req)
    ok, msg = ok_or_err(res)
    if ok:
        return jsonify(status='success')
    return jsonify(status='fail', message=msg)

# 删除图书（保留唯一一份，避免重复注册）
@app.route('/api/books/<book_id>', methods=['DELETE'])
def api_delete_book(book_id):
    res = run_cpp_command(['-api_delete_book', book_id])
    ok, msg = ok_or_err(res)
    if ok:
        return jsonify(status='success')
    return jsonify(status='fail', message=msg)

# 添加图书
@app.post('/api/add_book')
def api_add_book():
    data = request.get_json() or {}
    name = data.get('name', '').strip()
    author = data.get('author', '').strip()
    press = data.get('press', '').strip()
    # 校验 total 为非负整数
    try:
        total = int(data.get('total', 0))
        if total < 0:
            total = 0
    except Exception:
        return jsonify({"status": "error", "message": "库存必须是整数"})
    category = data.get('category', '').strip()
    for _ in range(5):
        book_id = gen_book_id()
        ok, msg = ok_or_err(run_cpp_command(
            ['-api_add_book', book_id, name, author, press, str(total), category]
        ))
        if ok:
            return jsonify({"status": "success", "message": "添加成功"})
        # 非 ID 冲突，直接报错
        if msg != 'ERR_BOOK_ID_EXISTS':
            return jsonify({"status": "error", "message": msg})
    # 5 次都因为冲突失败
    return jsonify({"status": "error", "message": "生成唯一图书ID失败，请稍后再试"})

# 添加读者
@app.post('/api/add_reader')
def api_add_reader():
    data = request.get_json() or {}
    name = data.get('name', '').strip()
    if not name:
        return jsonify({"status": "error", "message": "姓名不能为空"})
    for _ in range(5):
        reader_id = gen_reader_id()
        ok, msg = ok_or_err(run_cpp_command(['-api_add_reader', reader_id, name]))
        if ok:
            return jsonify({"status": "success", "message": f"添加成功！ID是: {reader_id} (默认密码123456)"})
        if msg != 'ERR_READER_EXISTS':
            return jsonify({"status": "error", "message": msg})
    return jsonify({"status": "error", "message": "生成唯一读者ID失败，请稍后再试"})

# 借书
@app.post('/api/borrow')
def api_borrow():
    data = request.get_json() or {}
    reader_id = data.get('reader_id')
    book_id = data.get('book_id')
    if not reader_id or not book_id:
        return jsonify({"status": "error", "message": "缺少读者ID或书籍ID"})
    res = run_cpp_command(['-api_reader_borrow_req', reader_id, book_id])
    ok, msg = ok_or_err(res)
    if ok:
        return jsonify({"status": "success", "message": "借阅成功"})
    else:
        return jsonify({"status": "error", "message": msg})

# 还书
@app.post('/api/return')
def api_return():
    data = request.get_json() or {}
    reader_id = data.get('reader_id')
    book_id = data.get('book_id')
    if not reader_id or not book_id:
        return jsonify({"status": "error", "message": "缺少参数"})
    res = run_cpp_command(['-api_return_book', reader_id, book_id])
    ok, msg = ok_or_err(res)
    if ok:
        return jsonify({"status": "success", "message": "归还成功"})
    else:
        return jsonify({"status": "error", "message": msg})

# 搜索图书
@app.get('/api/books/search')
def api_search_books():
    keyword = request.args.get('q', '')
    res = run_cpp_command(['-api_search_books', keyword])
    if not res['success']:
        return jsonify({"status": "error", "message": res['message']})
    book_list = parse_books_output(res['output'])
    return jsonify({"status": "success", "data": book_list})

# 全部历史
@app.get('/api/history')
def api_get_history():
    res = run_cpp_command(['-api_show_history'])
    if not res['success']:
        return jsonify({"status": "error", "message": res['message']})
    data = parse_history_output(res['output'])
    return jsonify({"status": "success", "data": data})

# 某读者历史
@app.get('/api/reader/history')
def api_get_reader_history():
    reader_id = request.args.get('id')
    if not reader_id:
        return jsonify({"status": "error", "message": "请输入读者ID"})
    res = run_cpp_command(['-api_reader_history', reader_id])
    if not res['success']:
        return jsonify({"status": "error", "message": res.get('message') or "查询失败"})
    data = parse_history_output(res['output'])
    return jsonify({"status": "success", "data": data})
#随机生成数据
@app.post('/api/gen_demo')
def api_gen_demo():
    data = request.get_json(silent=True) or {}
    n = int(data.get('n', 20))
    cats = ['计算机','文学','历史','教育','艺术','科学','经济']
    words = ['算法','设计','系统','原理','工程','实践','导论','进阶','基础','指南','手册','分析']
    ok_cnt = 0
    for _ in range(n):
        name = f"随机{random.choice(words)}{random.randint(1,999)}"
        author = f"作者{random.randint(100,999)}"
        press = f"出版社{random.randint(1,50)}"
        total = str(random.randint(3,20))
        category = random.choice(cats)
        for _try in range(5):
            book_id = gen_book_id()
            ok, msg = ok_or_err(run_cpp_command(
                ['-api_add_book', book_id, name, author, press, total, category]
            ))
            if ok:
                ok_cnt += 1
                break
            if msg != 'ERR_BOOK_ID_EXISTS':
                break
    return jsonify(status='success', made=ok_cnt)
@app.post('/api/import_books')
def api_import_books():
    f = request.files.get('file')
    if not f:
        return jsonify(status='fail', message='请上传CSV文件')
    try:
        text = f.stream.read().decode('utf-8-sig', errors='ignore')
        reader = csv.DictReader(io.StringIO(text))
        ok_cnt, fail_cnt = 0, 0
        for row in reader:
            name = (row.get('name') or '').strip()
            author = (row.get('author') or '').strip()
            press = (row.get('press') or '').strip()
            category = (row.get('category') or '').strip()
            try:
                total = str(max(0, int(row.get('total') or 0)))
            except:
                total = '0'
            bid = (row.get('id') or '').strip() or gen_book_id()
            ok, msg = ok_or_err(run_cpp_command(['-api_add_book', bid, name, author, press, total, category]))
            if ok:
                ok_cnt += 1
            else:
                # 如果是 ID 冲突，换个ID再试一次（兼容重复ID导入）
                if msg == 'ERR_BOOK_ID_EXISTS':
                    bid2 = gen_book_id()
                    ok2, _ = ok_or_err(run_cpp_command(['-api_add_book', bid2, name, author, press, total, category]))
                    ok_cnt += 1 if ok2 else 0
                    fail_cnt += 0 if ok2 else 1
                else:
                    fail_cnt += 1
        return jsonify(status='success', ok=ok_cnt, fail=fail_cnt)
    except Exception as e:
        return jsonify(status='fail', message=f'解析失败: {e}')
@app.get('/api/export_books')
def api_export_books():
    res = run_cpp_command(['-api_show_books'])
    if not res['success']:
        return jsonify(status='fail', message=res['message'])
    books = parse_books_output(res['output'])
    sio = io.StringIO()
    w = csv.writer(sio)
    w.writerow(['id','name','author','press','total','borrowed','category','available'])
    for b in books:
        total = int(b.get('total',0) or 0); borrowed = int(b.get('borrowed',0) or 0)
        w.writerow([b['id'], b['name'], b['author'], b['press'], total, borrowed, b['category'], max(total-borrowed,0)])
    csv_text = sio.getvalue()
    body = '\ufeff' + csv_text
    return Response(body,
                    mimetype='text/csv; charset=utf-8',
                    headers={'Content-Disposition':'attachment; filename=books.csv'})
@app.get('/api/export_history')
def api_export_history():
    res = run_cpp_command(['-api_show_history'])
    if not res['success']:
        return jsonify(status='fail', message=res['message'])
    his = parse_history_output(res['output'])

    sio = io.StringIO(newline='')
    w = csv.writer(sio)
    w.writerow(['hid','bid','rid','bname','rname','note','brtime','btime','rrtime','rtime','status'])
    for h in his:
        w.writerow([h['hid'],h['bid'],h['rid'],h['bname'],h['rname'],h['note'],
                h['brtime'],h['btime'],h['rrtime'],h['rtime'],h['status']])

    csv_text = sio.getvalue()
    body = '\ufeff' + csv_text
    return Response(body,
                mimetype='text/csv; charset=utf-8',
                headers={'Content-Disposition':'attachment; filename=history.csv'})
def _get_books():
    r = run_cpp_command(['-api_show_books'])
    if not r['success']: return []
    return parse_books_output(r['output'])
@app.get('/api/stats/basic')
def api_stats_basic():
    books = _get_books()
    total_books = len(books)
    total_copies = sum(int(b['total']) for b in books)
    borrowed = sum(int(b['borrowed']) for b in books)
    return jsonify(status='success', data={
        'total_books': total_books,
        'total_copies': total_copies,
        'borrowed_copies': borrowed,
        'available_copies': max(total_copies-borrowed,0)
    })
@app.get('/api/stats/by_category')
def api_stats_by_category():
    books = _get_books()
    m = {}
    for b in books:
        c = (b.get('category') or '未分类').strip()
        m.setdefault(c, {'books':0,'copies':0})
        m[c]['books'] += 1
        m[c]['copies'] += int(b['total'])
    arr = [{'category':k, **v} for k,v in m.items()]
    arr.sort(key=lambda x: x['copies'], reverse=True)
    return jsonify(status='success', data=arr)
@app.get('/api/stats/top')
def api_stats_top():
    n = int(request.args.get('n',5))
    books = _get_books()
    arr = sorted(books, key=lambda b:int(b['borrowed']), reverse=True)[:n]
    return jsonify(status='success', data=arr)
if __name__ == '__main__':
    app.run(debug=True, port=5000)
