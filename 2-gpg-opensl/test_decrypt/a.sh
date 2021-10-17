#!/bin/bash

echo "Some important content" > p1secret
[ -f p1secret.gpg ] && rm p1secret.gpg
[ -f p1secret_decrypted ] && rm p1secret_decrypted

echo "_-_practica-gpg_-_" | gpg --batch --passphrase-fd 0 --output p1secret.gpg --symmetric p1secret
echo "_-_practica-gpg_-_" | gpg --batch --passphrase-fd 0 --output p1secret_decrypted --decrypt p1secret.gpg

diff p1secret p1secret_decrypted && echo "funciona"
