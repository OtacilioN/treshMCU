from flask import Flask, request, render_template
from flask_restful import reqparse
from tinydb import TinyDB, where
from tinydb.operations import delete
from tinydb import Query
from flask_table import Table, Col
import datetime
import numpy as np


User = Query()
app = Flask(__name__)
ID = 0
S1 = 0
S2 = 0
db = TinyDB('clint.json')

class ItemTable(Table):
    ID = Col('ID')
    acess = Col('Dinheiro')
    data = Col('Data')
    classes = ['table', 'table-bordered', 'table-hover']


class Item(object):
    def __init__(self, ID, acess, data):
        self.ID = ID
        self.acess = acess
        self.data = data


def insertData(myId,myS1,data):
    if (db.search(User.ID == myId) != [] ):
        db.update({'acess': myS1 , 'data' :data}, User.ID == myId)
        return
    db.insert({'ID': myId, 'acess': myS1, 'data' : data})
    return



@app.route("/init")
def hello():
    
    parser = reqparse.RequestParser()
    parser.add_argument('ID')
    parser.add_argument('acess')
    args = parser.parse_args()
    ID = args.ID
    ACESS = args.acess
    nowTime = str(datetime.datetime.now())
    nowTime = nowTime[:-7]
    
    print "your ID is: " +  str(ID)
    print "Sensor One number: " + str(ACESS)
    print "Data : " + nowTime

    insertData(ID,ACESS,nowTime)
    
    return "Request Seed to server! " + str(ID) + str(ACESS) + nowTime


@app.route("/view")
def view():

    items = db.all()
    table = ItemTable(items)
    return render_template('index.html', table=table) 

@app.route("/purge")
def purge():
    db.purge()
    return "Purge!"
@app.route("/")
def barra():
    return "WellCome!"

if __name__ == "__main__":
    app.run(host='0.0.0.0' )