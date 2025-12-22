from flask import Flask, render_template, request, jsonify
import subprocess
import os

app = Flask(__name__)

EXE_PATH = './main.exe'

def run_cpp_command(args):
    if not os.path.exists(EXE_PATH):
        return {"success": False, "message": "找不到 main.exe"}
    try:
        # 保持 gbk
        result = subprocess.run(
            [EXE_PATH] + args,
            capture_output=True,
            text=True,
            encoding='gbk',
            errors='ignore'
        )
        return {"success": True, "output": result.stdout}
    except Exception as e:
        return {"success": False, "message": str(e)}

def parse_books_output(raw_text):
    lines = [line.strip() for line in raw_text.split('\n') if line.strip()]
    books = []

    for i in range(1, len(lines), 8):
        # 确保剩余行数够解析一本书
        if i + 7 < len(lines):
            books.append({
                "id":       lines[i],     # 索引1是ID
                "name":     lines[i+1],   # 索引2是书名
                "author":   lines[i+2],
                "press":    lines[i+3],
                "total":    lines[i+4],
                "borrowed": lines[i+5],
                "category": lines[i+6],
                "status":   lines[i+7]
            })
    return books
def parse_history_output(raw_text):
    lines = [line.strip() for line in raw_text.split('\n') if line.strip()]
    historys = []
    if not lines:
        return historys

    for i in range(1, len(lines), 10):
        if i + 9 < len(lines):
            historys.append({
                "hid":      lines[i],     # 历史记录的id
                "bid":      lines[i+1],   # 图书的id
                "rid":      lines[i+2],   #读者的id
                "bname":    lines[i+3],  #图书名称
                "rname":    lines[i+4],  #读者姓名
                "note":     lines[i+5],     #管理员备注信息
                "brtime":   lines[i+6],    #借阅时间
                "btime":    lines[i+7],   #管理员同意借出时间
                "rrtime":   lines[i+8], # 申请归还时间
                "rtime":    lines[i+9]  #实际归还时间
            })
    return historys


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/get_books', methods=['GET'])
def api_get_books():
    result = run_cpp_command(['-api_show_books'])
    if not result['success']:
        return jsonify({"status": "error", "message": result['message']})

    book_list = parse_books_output(result['output'])
    return jsonify({"status": "success", "data": book_list})

@app.route('/api/add_book', methods=['POST'])
def api_add_book():
    data = request.get_json() or {}
    # 参数顺序和 C++ 对应
    args = ['-api_add_book',
            str(data.get('id')),
            data.get('name'),
            data.get('author'),
            data.get('press'),
            str(data.get('total')),
            data.get('category')]

    result = run_cpp_command(args)
    if result['success']:
        return jsonify({"status": "success", "message": "添加成功"})
    else:
        return jsonify({"status": "error", "message": result['message']})
# 删除图书
@app.route('/api/books/<book_id>', methods=['DELETE'])
def api_delete_book(book_id):
    result = run_cpp_command(['-api_delete_book', book_id])
    if result['success']:
        return jsonify({"status": "success"})
    else:
        return jsonify({"status": "error", "message": result['message']})
#搜索图书
@app.route('/api/books/search', methods=['GET'])
def api_search_books():
    keyword = request.args.get('q', '')
    result = run_cpp_command(['-api_search_books', keyword])
    if result['success']:
        book_list = parse_books_output(result['output'])
        return jsonify({"status": "success", "data": book_list})
    else:
        return jsonify({"status": "error", "message": result['message']})

@app.route('/api/history', methods=['GET'])
def api_get_history():
    result = run_cpp_command(['-api_show_history'])
    if result['success']:
        historys = parse_history_output(result['output'])
        return jsonify({"status": "success", "data": historys})
    else:
        return jsonify({"status": "error", "message": result['message']})



if __name__ == '__main__':
    app.run(debug=True, port=5000)