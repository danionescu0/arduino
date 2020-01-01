from flask import Flask, request, Response
import numpy as np
import cv2

app = Flask(__name__)


# route http posts to this method
@app.route('/api/test', methods=['POST'])
def test():
    r = request
    # convert string of image data to uint8
    nparr = np.fromstring(r.data, np.uint8)
    # decode image
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    cv2.imwrite("file_location.jpg", img)

    # build a response dict to send back to client
    response = {'message': 'image received. size={}x{}'.format(img.shape[1], img.shape[0])}
    print(response)

    return Response(response="ok", status=200, mimetype="application/json")


app.run(host="0.0.0.0", port=5000)