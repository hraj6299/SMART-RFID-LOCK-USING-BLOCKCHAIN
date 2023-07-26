#imports
import datetime, hashlib, json

class Blockchain:

    #initialization with genesis block
    def __init__(self):
        self.chain = []
        self.create_block(data = None)
 
    #function for creating new blocks
    def create_block(self, data):
        if(len(self.chain) == 0):
            proof = 1
            previous_hash = '0'
        else:
            proof = self.proof_of_work()
            previous_hash = self.hash(self.latest_block())

        block = {'index': len(self.chain) + 1,
                 'timestamp': str(datetime.datetime.now()),
                 'data': data,
                 'proof': proof,
                 'previous_hash': previous_hash}
        self.chain.append(block)
        return block
 
    # This function is created
    # to display the previous block
    def latest_block(self):
        return self.chain[-1]
 
    # This is the function for proof of work
    # and used to successfully mine the block
    def proof_of_work(self):
        new_proof = 1
        check_proof = False
        previous_proof = self.latest_block()["proof"]

        while check_proof is False:
            hash_operation = hashlib.sha256(str(new_proof**2 - previous_proof**2).encode()).hexdigest()
            if hash_operation[:5] == '00000': #adjust speed here
                check_proof = True
            else:
                new_proof += 1
 
        return new_proof
 
    def hash(self, block):
        encoded_block = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(encoded_block).hexdigest()
 
    def chain_valid(self):
        if(len(self.chain) == 1):
            return True
        previous_block = self.chain[0]
        block_index = 1

        while block_index < len(self.chain):
            block = self.chain[block_index]
            if block['previous_hash'] != self.hash(previous_block):
                return False
 
            previous_proof = previous_block['proof']
            proof = block['proof']
            hash_operation = hashlib.sha256(
                str(proof**2 - previous_proof**2).encode()).hexdigest()
 
            if hash_operation[:5] != '00000': #also adjust speed here
                return False
            previous_block = block
            block_index += 1
 
        return True
    
    def find_data(self, num):
        for i in self.chain:
            if(num == i["data"]):
                return True
        return False
