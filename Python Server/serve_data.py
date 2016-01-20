import os
import sys
from flask import Flask, render_template, request, redirect, url_for, send_from_directory, session

"""
    SSI rockets text based ground station. 
"""

app = Flask(__name__)
app.secret_key = 'xbf\xcb7\x0bv\xcf\xc0N\xe1\x86\x98g9\xfei\xdc\xab\xc6\x05\xff%\xd3\xdf'

"""
    Gets current log from XBee. 
"""
def get_log(): 
    return "hi"

"""
    Show homepage 
"""
@app.route('/getdata')
def index():
    return get_log()

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    app.debug = True
    app.run(host='0.0.0.0', port=port)
