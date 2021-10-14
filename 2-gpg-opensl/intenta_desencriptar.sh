 #!/bin/bash


echo $algoritmos

for i in `openssl enc -list|grep -v Supported`
do
    cypher=`echo $i | sed 's/^-//g'`
    if echo '_-_practica-gpg_-_' | openssl "$cypher" -d -in p1secret.gpg -out ./descifrar/p1secret."$cypher" -pass stdin 2> /dev/null
    then
        echo "funciona"
    fi
    
done
