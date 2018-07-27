from flask import Flask
from flask import render_template
from flask import request
from flask import session, redirect, url_for, escape
from flaskext.mysql import MySQL
import hashlib
from charts     import charts_bp
from forms      import forms_bp
from examples   import examples_bp
from layout     import layout_bp
from mailbox    import mailbox_bp
from tables     import tables_bp
from ui         import ui_bp
import base64

app = Flask(__name__)
app.register_blueprint(charts_bp, url_prefix='/charts')
app.register_blueprint(forms_bp, url_prefix='/forms')
app.register_blueprint(examples_bp, url_prefix='/examples')
app.register_blueprint(layout_bp, url_prefix='/layout')
app.register_blueprint(mailbox_bp, url_prefix='/mailbox')
app.register_blueprint(tables_bp, url_prefix='/tables')
app.register_blueprint(ui_bp, url_prefix='/ui')
mysql = MySQL()
app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'haiyao'
app.config['MYSQL_DATABASE_DB'] = 'kq'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(app)


# set the secret key.  keep this really secret:
app.secret_key = '\xe5\xcc\xc5\xd9+\xfbC\xc6\xdbD\x1af\xe0\xa6*\xeb$\xa7\xe4\xf6p~\x01\xaf'

@app.route('/')
def index():
    if 'username' in session:
        return render_template('index.html')
    else:
        return redirect(url_for('login'))

@app.route('/index2')
def index2():
    return render_template('index2.html')

def valid_login(username, password):
    md5=hashlib.md5(password.encode('utf-8')).hexdigest()
    app.logger.debug('md5:%s' %(md5))
    cursor = mysql.connect().cursor()
    cursor.execute("SELECT * from kq_users where username='" + username + "' and password='" + md5 + "'")
    data = cursor.fetchone()
    if data is None:
        app.logger.debug("Username or Password is wrong")
        return False
    else:
        app.logger.debug("Logged in successfully")
        return True


@app.route('/login', methods=['GET', 'POST'])
def login():
    error = None
    if request.method == 'POST':
        if valid_login(request.form['username'],
                       request.form['password']):
            session['username'] = request.form['username']
            return redirect(url_for('index'))
        else:
            error = 'Invalid username/password'
    else:
        return render_template('login.html', error=error)

@app.route('/log', methods=['GET', 'POST', 'PUT'])
def log():
    error = None
    if request.method == 'POST':
            print request.headers
            print request.values
            print request.data
            print request.args
            #str = base64.b64decode(request.data)
            #print str
            return 'welcome'
    if request.method == 'PUT':
            print request.headers
            print request.values
            print request.data
            print request.args
            #str = base64.b64decode(request.data)
            #print str
            return 'put'
    else:
        return render_template('login.html', error=error)

@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    session.pop('username', None)
    return redirect(url_for('login'))

@app.errorhandler(404)
def page_not_found(error):
    return render_template('pages/examples/404.html'), 404


@app.route("/widgets")
def widgets():
    return render_template('pages/widgets.html')  

@app.route("/calendar")
def calendar():
    return render_template('pages/calendar.html')                    

@app.route('/test/<username>')
def test(username):
    return redirect(url_for(username))


@app.route('/db/<username>')
def db():
    cursor = mysql.get_db().cursor()
    cursor.execute('select * from kq_person_info')
    data = cursor.fetchall()
    for d in data:
        print(d[3], SEX[d[4]])
    #return jsonify({'code': 200})   
    return 'db test'

if __name__ == "__main__":
    app.run(debug=True,host='0.0.0.0')
