#imports
from flask import Flask, jsonify, request, render_template
from blockchainContract import Blockchain
import hashlib
 
#initialisations
app = Flask(__name__)
blockchain = Blockchain()

@app.route('/')
def main():
    return render_template('index.html')

# Check validity of blockchain
 
@app.route('/valid', methods=['GET'])
def valid():
    valid = blockchain.chain_valid(blockchain.chain)

    if valid:
        response = {'message': 'The Blockchain is valid.'}
    else:
        response = {'message': 'The Blockchain is not valid.'}
    return jsonify(response), 200

@app.route('/add_card', methods=['GET'])
def addCard():
    num = hashlib.sha256(request.args.get('cardNum').encode()).hexdigest()
    a = blockchain.create_block(num)
    return str(True), 200

@app.route('/verify_card', methods=['GET'])
def verifyCard():
    num = hashlib.sha256(request.args.get('cardNum').encode()).hexdigest() 
    return str(blockchain.find_data(num)), 200

@app.route('/display')
def display():
    blockchain.chain_valid()
    return jsonify(blockchain.chain), 200

 
# Run the flask server locally
app.run(host='192.168.137.1', port=5000, debug=True)