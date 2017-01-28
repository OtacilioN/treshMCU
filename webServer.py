from flask import Flask
from flask_restful import reqparse
app = Flask(__name__)

ID = 0
S1 = 0
S2 = 0



@app.route("/init")
def hello():
    
    parser = reqparse.RequestParser()
    parser.add_argument('ID')
    parser.add_argument('S1', type=int,help='Ola')
    args = parser.parse_args()
    ID = args.ID
    S1 = args.S1
    
    
    print "your ID is: " +  str(ID)
    
#    print "Sensor One number: ".join(S1)

    return "Hello World!"

if __name__ == "__main__":
    app.run()