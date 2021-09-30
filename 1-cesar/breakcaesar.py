import sys

char_frequency = {
    "a":0.08167,
    "b":0.01492,
    "c":0.02782,
    "d":0.04253,
    "e":0.12702,
    "f":0.02228,
    "g":0.02015,
    "h":0.06094,
    "i":0.06966,
    "j":0.00153,
    "k":0.00772,
    "l":0.04025,
    "m":0.02406,
    "n":0.06749,
    "o":0.07507,
    "p":0.01929,
    "q":0.00095,
    "r":0.05987,
    "s":0.06327,
    "t":0.09056,
    "u":0.02758,
    "v":0.00978,
    "w":0.02360,
    "x":0.00150,
    "y":0.01974,
    "z":0.00074
}

char_freq=[0.08167,0.01492,0.02782,0.04253,0.12702,0.02228,0.02015,0.06094,0.06966,0.00153,0.00772,0.04025,0.02406,0.06749,0.07507,0.01929,0.00095,0.05987,0.06327,0.09056,0.02758,0.00978,0.02360,0.00150,0.01974,0.00074]
bigrams=["th", "he", "in", "en", "nt", "re", "er", "an", "ti", "es", "on", "at", "se", "nd", "or", "ar", "al", "te", "co", "de", "to", "ra", "et", "ed", "it", "sa", "em", "ro"]
trigrams=["the", "and", "tha", "ent", "ing", "ion", "tio", "for", "nde", "has", "nce", "edt", "tis", "oft", "sth", "men"]



def caesar_encrypt(text, key):
    text=text.upper()
    result=""
    for c in text:
        if(c.isupper()):
            c=chr((ord(c)+key-ord("A"))%26+ord("A"))
        result+=c
    return result

def calculate_relative_freq(text):
    frequencies={}#guarda las frecuencias absolutas de cada letra que aparece en el texto
    text=text.upper()
    for c in text:
        if(c.isupper()):
            if c in frequencies:
                frequencies[c]+=1
            else:
                frequencies[c]= 1

    
    print(frequencies)



def caesar_decrypt(text):
    result=[]
    for i in range(26):
        attempt=(caesar_encrypt(text,-i),1,1,1)
        result.append(attempt)
    return result


if(len(sys.argv)!=1):
    print("argc error", file=sys.stderr)
    exit(-1)

for line in sys.stdin:
    sys.stdout.write(line)
    #print(caesar_decrypt(line))
    calculate_relative_freq(line)