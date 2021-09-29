import sys

def caesar_encript(text, key):
    text=text.upper()
    result=""
    for c in text:
        if(c.isupper()):
            c=chr((ord(c)+key-ord("A"))%25+ord("A"))
        result+=c
    print(result)


if(len(sys.argv)!=2):
    print("argc error", file=sys.stderr)
    exit(-1)

key=int(sys.argv[1])
print("key: ",key)

for line in sys.stdin:
    sys.stdout.write(line)
    caesar_encript(line, key)
    