from flask import Flask
from flask_restful import Api, Resource
from datetime import datetime

app = Flask(__name__)

api = Api(app)

class LogReadings(Resource):
    def get(self, StationID, Sensor1, Sensor2, Sensor3,Verbose):
        now = datetime.now()
        with open('IoT_Data.txt', 'a') as f:
            str1 = "%s\t %s\t %6.3f\t %6.3f\t %6.3f\n" %(StationID, now.strftime("%m/%d/%Y, %H:%M:%S"), Sensor1, Sensor2, Sensor3)
            f.write(str1)
        f.close()
        if(Verbose ==1):
            return{"StationID" : StationID, "Sensor1": Sensor1, "Sensor2": Sensor2, "Sensor3": Sensor3, "Date_Time": now.strftime("%m/%d/%Y, %H:%M:%S") }
        else:
            return{ "Logged @" : now.strftime("%m/%d/%Y, %H:%M:%S")}
        

api.add_resource(LogReadings, "/log/<string:StationID>/<float:Sensor1>/<float:Sensor2>/<float:Sensor3>/<int:Verbose>")

if __name__ == "__main__":

    app.run(host="192.168.2.101", port="5000", debug=True)

