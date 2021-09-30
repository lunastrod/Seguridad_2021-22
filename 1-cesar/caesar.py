import sys

def caesar_encrypt(text, key):
    text=text.upper()
    result=""
    for c in text:
        if(c.isupper()):
            c=chr((ord(c)+key-ord("A"))%26+ord("A"))
        result+=c
    return result


if(len(sys.argv)!=2):
    print("argc error", file=sys.stderr)
    exit(-1)

key=int(sys.argv[1])
print("key: ",key)

for line in sys.stdin:
    sys.stdout.write(line)
    print(caesar_encrypt(line, key))