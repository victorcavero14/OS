#Víctor Manuel Cavero Gracia 45355080T
#Iván Fernández Sánchez 52902115E


#!/bin/bash    
#Creamos temp. Si ya está creado lo eliminamos

if [ -d temp ]; then
        rm -rf temp
    fi
mkdir temp

#Creamos mount-point. Si ya estaba se borra y se crea otra vez

if [ -d mount-point ]; then
	rm -rf mount-point
	fi
mkdir mount-point
	

#copiamos fuseLib y myFs.h
 cp ./src/fuseLib.c ./temp/
 cp ./src/myFS.h ./temp

 cp ./src/fuseLib.c ./mount-point/
 cp ./src/myFS.h ./mount-point


#auditamos el disco

./my-fsck-static-64 virtual-disk

#hacemos diferencias
diff -q ./src/fuseLib.c ./mount-point/fuseLib.c
diff -q ./src/myFS.h ./mount-point/myFS.h

#truncamos 

truncate -o --size -1 ./temp/fuseLib.c
truncate -o --size -1 ./mount-point/fuseLib.c

#auditamos el disco

./my-fsck-static-64 virtual-disk

#hacemos diferencias
diff -q ./src/fuseLib.c ./temp/fuseLib.c
diff -q ./src/fuseLib.c ./mount-point/fuseLib.c

#Copiar un tercer archivo

cp ./src/MyFileSystem.c ./mount-point/

#auditamos el disco

./my-fsck-static-64 virtual-disk

#diferencias fichero original y el del SF

diff -q ./src/MyFileSystem.c ./mount-point/MyFileSystem.c

#truncamos 

truncate -o --size +3 ./temp/myFS.h
truncate -o --size +3 ./mount-point/myFS.h

#auditamos el disco

./my-fsck-static-64 virtual-disk

#hacemos diferencias
diff -q ./src/fuseLib.c ./temp/fuseLib.c
diff -q ./src/fuseLib.c ./mount-point/fuseLib.c

#hacemos diferencias
diff -q ./src/myFS.h ./temp/myFS.h
diff -q ./src/myFS.h ./mount-point/myFS.h







