import sys
import base58
import hashlib
import binascii
from blockchain.reader import BlockchainFileReader
block_reader=BlockchainFileReader('/var/data/bitcoin-data/blocks/blk00325.dat')
satochi_convert=1e8
def sha256(x):
    h=hashlib.new('sha256')
    h.update(x)
    return h.digest()


def hashStr(buffer):
    return binascii.hexlify(buffer)



for block in block_reader:
    #block has .header, .transactions
    for iter,tx in enumerate(block.transactions):
        print('  Transaction (txn_hash):{} {}'.format(type(tx.txn_hash),tx.txn_hash))
        for x in tx.outputs:
            script_pub_key_str=hashStr(x.script_pub_key)
            if script_pub_key_str[0:4]==b'76a9':
                #This is a pubkeyhash
                bytes=x.script_pub_key[2]  #number of bytes in the pub_key
                assert bytes==20
                public_key = x.script_pub_key[3:3+bytes] #20 bytes
                z=b'\00'+public_key
                #checksum=sha256(sha256(z))[:4]
                #address1=base58.b58encode(z + checksum)
                address2=base58.b58encode_check(z)   # adds checksum for you
                print('  output value {:<20} address {}'.format(float(x.value)/satochi_convert,address2))
            elif script_pub_key_str[0:2]==b'a9':
                #this is a scripthash (pay-to-script address)
                bytes=x.script_pub_key[1]  #number of bytes in the pub_key
                assert bytes==20
                public_key = x.script_pub_key[2:2+bytes] #20 bytes
                z=b'\05'+public_key  #used for mainnet
                address2=base58.b58encode_check(z)
                print('  output value {:<20} address {}'.format(float(x.value)/satochi_convert,address2))
            else:
                print('  output value {:<20} other {}'.format(float(x.value)/satochi_convert,script_pub_key_str))
        print()
